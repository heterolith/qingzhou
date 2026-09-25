/*
 * 中秋节祝福显示
 * 平台: ESP8266 (Arduino Core)
 * 字模: 24x24 纵向取模 Bit0在上 阴码  (由字模管理器导出 font_zh24.h)
 *
 * 显示序列（循环）:
 *   祝大家  (2秒) -> 中秋节 (2秒) -> 快乐! (2秒)
 *
 * 用法:
 *   1. 把导出的 font_zh24.h 放到本文件同目录
 *   2. 按你的屏幕修改下方「硬件配置」和「MAX7219 驱动」里的引脚/级联数
 *   3. 编译烧录
 */

#include "font_zh24.h"

// ===================== 硬件配置（按你的屏幕改）=====================
#define SCREEN_W   72      // 屏幕宽(像素): 3个字 x 24
#define SCREEN_H   24      // 屏幕高(像素)
#define CHAR_W     24
#define CHAR_H     24
#define CHAR_BYTES (CHAR_W * CHAR_H / 8)   // 72

// MAX7219 引脚（若不用 MAX7219，忽略下面驱动区）
#define PIN_DIN  D7
#define PIN_CLK  D5
#define PIN_CS   D8
#define MAX7219_CHAIN  9   // 级联模块数: 24x24 = 3x3 = 9 块 8x8

// ===================== 帧缓冲 =====================
// fb[y][x] = 1 表示该点亮。1字节1像素，简单直观。
static uint8_t fb[SCREEN_H][SCREEN_W];

static void clearBuffer() {
  memset(fb, 0, sizeof(fb));
}

// ===================== 绘制单个 24x24 字 =====================
// 字模存储格式: data[col*3 + page], 每个字节 bit0 在最上
void drawChar(const uint8_t *data, int x0, int y0) {
  for (int col = 0; col < CHAR_W; col++) {
    for (int page = 0; page < CHAR_H / 8; page++) {      // 3 页
      uint8_t b = data[col * (CHAR_H / 8) + page];
      for (int bit = 0; bit < 8; bit++) {
        if (b & (1 << bit)) {
          int x = x0 + col;
          int y = y0 + page * 8 + bit;
          if (x >= 0 && x < SCREEN_W && y >= 0 && y < SCREEN_H)
            fb[y][x] = 1;
        }
      }
    }
  }
}

// ===================== 内置「!」字模（字库没有时兜底）=====================
const uint8_t* buildBang() {
  static uint8_t g[CHAR_BYTES];
  memset(g, 0, sizeof(g));
  // 竖杠: 列 10~13, 行 2~14
  for (int col = 10; col <= 13; col++)
    for (int row = 2; row <= 14; row++)
      g[col * 3 + row / 8] |= (1 << (row % 8));
  // 点: 列 10~13, 行 18~21
  for (int col = 10; col <= 13; col++)
    for (int row = 18; row <= 21; row++)
      g[col * 3 + row / 8] |= (1 << (row % 8));
  return g;
}

// ===================== 显示一段文字并保持 =====================
void showSegment(const uint8_t *chars[], uint8_t count, uint32_t durationMs) {
  clearBuffer();
  for (uint8_t i = 0; i < count; i++) {
    if (chars[i]) drawChar(chars[i], i * CHAR_W, 0);
  }
  flushBuffer();
  delay(durationMs);
}

// ===================== 主逻辑 =====================
void setup() {
  initDisplay();
  clearBuffer();
  flushBuffer();
}

void loop() {
  // 祝大家 (2秒)
  const uint8_t *s1[] = { font_795d, font_5927, font_5bb6 };
  showSegment(s1, 3, 2000);

  // 中秋节 (2秒)
  const uint8_t *s2[] = { font_4e2d, font_79cb, font_8282 };
  showSegment(s2, 3, 2000);

  // 快乐! (2秒)
  const uint8_t *s3[] = { font_5feb, font_4e50, buildBang() };
  showSegment(s3, 3, 2000);
}

// ===================================================================
//  以下为屏幕驱动层 —— 默认提供 MAX7219 级联(24x24)实现
//  如果你用 OLED(SSD1306) / 其他屏幕，只需重写 initDisplay() 和 flushBuffer()
// ===================================================================

// ---------- MAX7219 底层 ----------
static void max7219_send(uint16_t word) {
  digitalWrite(PIN_CS, LOW);
  for (int i = 15; i >= 0; i--) {
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_DIN, (word >> i) & 1);
    digitalWrite(PIN_CLK, HIGH);
  }
  digitalWrite(PIN_CS, HIGH);
}

static void max7219_sendAll(uint8_t reg, uint8_t val) {
  // 向级联中每一块都发送同一条命令
  for (int i = 0; i < MAX7219_CHAIN; i++) max7219_send((reg << 8) | val);
}

void initDisplay() {
  pinMode(PIN_DIN, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);

  max7219_sendAll(0x09, 0x00);   // 不译码
  max7219_sendAll(0x0B, 0x07);   // 扫描 8 行
  max7219_sendAll(0x0A, 0x04);   // 亮度
  max7219_sendAll(0x0C, 0x01);   // 正常工作
  max7219_sendAll(0x0F, 0x00);   // 关闭测试
}

// 把帧缓冲刷新到 3x3 的 MAX7219 阵列
// 约定: 模块从左到右、从上到下排列，每块负责 8x8 区域
// 若你的模块接线/方向不同，调整这里的坐标映射即可
void flushBuffer() {
  for (int row = 0; row < 8; row++) {           // 每个模块 8 行
    digitalWrite(PIN_CS, LOW);
    for (int block = MAX7219_CHAIN - 1; block >= 0; block--) {
      int bx = (block % 3) * 8;                  // 该块左上角 x
      int by = (block / 3) * 8;                  // 该块左上角 y
      uint8_t line = 0;
      for (int c = 0; c < 8; c++) {
        if (fb[by + row][bx + c]) line |= (1 << (7 - c));
      }
      max7219_send(((row + 1) << 8) | line);     // 行号 1~8
    }
    digitalWrite(PIN_CS, HIGH);
  }
}
