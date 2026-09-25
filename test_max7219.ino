/*
 * MAX7219 屏幕测试 - 全部点亮
 * 用途: 唤醒屏幕，确认硬件接线和上传正常
 */

#define PIN_DIN  13
#define PIN_CLK  14
#define PIN_CS   15
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
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);

  max7219_sendAll(0x09, 0x00);   // 不译码
  max7219_sendAll(0x0B, 0x07);   // 扫描8行
  max7219_sendAll(0x0A, 0x0F);   // 最大亮度
  max7219_sendAll(0x0F, 0x00);   // 关闭测试
  max7219_sendAll(0x0C, 0x01);   // 唤醒，正常工作

  // 全部点亮
  for (uint8_t row = 1; row <= 8; row++) {
    max7219_sendAll(row, 0xFF);
  }
}

void loop() {
}
