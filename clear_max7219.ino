/*
 * MAX7219 点阵屏 - 彻底清屏
 * 作用: 熄灭所有 LED，并让芯片进入低功耗关机模式
 * 使用: 单独上传本文件即可，不需要字库
 */

// ===== 引脚（与主程序保持一致）=====
#define PIN_DIN  13   // D7
#define PIN_CLK  14   // D5
#define PIN_CS   15   // D8
#define MAX7219_CHAIN  9   // 级联模块数: 3x3 = 9

// ===== 底层发送 =====
void max7219_send(uint16_t word) {
  digitalWrite(PIN_CS, LOW);
  for (int i = 15; i >= 0; i--) {
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_DIN, (word >> i) & 1);
    digitalWrite(PIN_CLK, HIGH);
  }
  digitalWrite(PIN_CS, HIGH);
}

// 给链路上每一块都发同一条命令
void max7219_sendAll(uint8_t reg, uint8_t val) {
  for (int i = 0; i < MAX7219_CHAIN; i++) max7219_send((reg << 8) | val);
}

void setup() {
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);

  // 1. 把每一行的数据都清零
  for (uint8_t row = 1; row <= 8; row++) {
    max7219_sendAll(row, 0x00);
  }

  // 2. 关闭显示测试
  max7219_sendAll(0x0F, 0x00);

  // 3. 进入关机模式（彻底熄灭，功耗最低）
  max7219_sendAll(0x0C, 0x00);

  // 串口提示
  Serial.begin(115200);
  Serial.println("MAX7219 已清屏并关机");
}

void loop() {
  // 什么都不做，保持熄灭
}
