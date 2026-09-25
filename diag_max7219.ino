/*
 * MAX7219 诊断测试
 * - 板载 LED 闪烁: 证明 ESP8266 在运行
 * - 串口输出: 115200，打开串口监视器看状态
 * - CS 改到 D2(GPIO4): 避免 GPIO15 启动脚问题
 * - MAX7219 全亮测试
 */

#define PIN_DIN  13   // D7
#define PIN_CLK  14   // D5
#define PIN_CS   4    // D2  ← 改到这里，原来接 D8 的线移到 D2
#define MAX7219_CHAIN  9

void max7219_send(uint16_t word) {
  digitalWrite(PIN_CS, LOW);
  for (int i = 15; i >= 0; i--) {
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_DIN, (word >> i) & 1);
    digitalWrite(PIN_CLK, HIGH);
  }
  digitalWrite(PIN_CS, HIGH);
}

void max7219_sendAll(uint8_t reg, uint8_t val) {
  for (int i = 0; i < MAX7219_CHAIN; i++) max7219_send((reg << 8) | val);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\n=== MAX7219 诊断开始 ===");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);

  Serial.println("初始化 MAX7219...");
  max7219_sendAll(0x09, 0x00);   // 不译码
  max7219_sendAll(0x0B, 0x07);   // 扫描8行
  max7219_sendAll(0x0A, 0x0F);   // 最大亮度
  max7219_sendAll(0x0F, 0x00);   // 关闭测试
  max7219_sendAll(0x0C, 0x01);   // 唤醒

  Serial.println("全部点亮...");
  for (uint8_t row = 1; row <= 8; row++) {
    max7219_sendAll(row, 0xFF);
  }
  Serial.println("完成，LED 开始闪烁");
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);   // 板载 LED 亮
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);  // 板载 LED 灭
  delay(500);
}
