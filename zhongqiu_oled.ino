/*
 * 中秋节祝福显示 - I2C OLED 版
 * 屏幕: 0.96寸 I2C OLED (SSD1306, 128x64)
 * 字模: 24x24 纵向取模 Bit0在上 (font_zh24.h)
 *
 * 接线:
 *   OLED VCC -> ESP8266 3.3V
 *   OLED GND -> ESP8266 GND
 *   OLED SCL -> D1 (GPIO5)
 *   OLED SDA -> D2 (GPIO4)
 *
 * 需要库: Arduino IDE -> 工具 -> 管理库 -> 搜索 "u8g2" -> 安装
 */

#include <U8g2lib.h>
#include <Wire.h>
#include "font_zh24.h"

// SSD1306 128x64 I2C (如果是 SH1106 把 SSD1306 改成 SH1106)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

#define CHAR_W  24
#define CHAR_H  24
#define SCREEN_W 128
#define SCREEN_H 64

// ===================== 绘制单个 24x24 字 =====================
void drawChar(const uint8_t *data, int x0, int y0) {
  for (int col = 0; col < CHAR_W; col++) {
    for (int page = 0; page < CHAR_H / 8; page++) {   // 3 页
      uint8_t b = data[col * (CHAR_H / 8) + page];
      for (int bit = 0; bit < 8; bit++) {
        if (b & (1 << bit)) {
          u8g2.drawPixel(x0 + col, y0 + page * 8 + bit);
        }
      }
    }
  }
}

// ===================== 内置「!」字模 =====================
const uint8_t* buildBang() {
  static uint8_t g[CHAR_W * CHAR_H / 8];
  memset(g, 0, sizeof(g));
  for (int col = 10; col <= 13; col++)
    for (int row = 2; row <= 14; row++)
      g[col * 3 + row / 8] |= (1 << (row % 8));
  for (int col = 10; col <= 13; col++)
    for (int row = 18; row <= 21; row++)
      g[col * 3 + row / 8] |= (1 << (row % 8));
  return g;
}

// ===================== 显示一段文字（居中）=====================
void showSegment(const uint8_t *chars[], uint8_t count, uint32_t durationMs) {
  u8g2.clearBuffer();
  int totalW = count * CHAR_W;
  int x0 = (SCREEN_W - totalW) / 2;
  int y0 = (SCREEN_H - CHAR_H) / 2;
  for (uint8_t i = 0; i < count; i++) {
    if (chars[i]) drawChar(chars[i], x0 + i * CHAR_W, y0);
  }
  u8g2.sendBuffer();
  delay(durationMs);
}

// ===================== 主逻辑 =====================
void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.sendBuffer();
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
