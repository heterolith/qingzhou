/*
 * 睁眼动画 - ESP8266 + 0.96" I2C OLED (SSD1306, 128x64)
 *
 * 接线 (4 根线):
 *   OLED GND -> ESP8266 GND
 *   OLED VCC -> ESP8266 3V3   (千万别接 5V, 烧屏)
 *   OLED SCL -> ESP8266 D1  (GPIO5)
 *   OLED SDA -> ESP8266 D2  (GPIO4)
 *
 * 依赖库 (库管理器安装, 含依赖全部安装):
 *   1. Adafruit SSD1306
 *   2. Adafruit GFX Library
 *
 * 无需手动改任何库文件, 直接上传即可.
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "eye_frames.h"   // 31 帧预缩放好的 128x64 单色数据

#define OLED_ADDR   0x3C  // 多数 0.96" OLED 是 0x3C; 若不亮改成 0x3D 试
#define SCREEN_W    128
#define SCREEN_H    64

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n[睁眼动画 OLED] boot"));

  // SSD1306 初始化: 不用 reset 脚 (-1), I2C 地址 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 init failed, 检查接线/地址"));
    while (true) { delay(1000); }  // 卡死, 看串口
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 28);
  display.println(F("Eye animation..."));
  display.display();
  delay(500);
}

void loop() {
  // 正向: 闭 -> 睁
  for (int i = 0; i < EYE_FRAMES; i++) {
    drawFrame(i);
    delay(50);   // ~20fps
  }
  delay(700);    // 睁眼停留

  // 反向: 睁 -> 闭
  for (int i = EYE_FRAMES - 1; i >= 0; i--) {
    drawFrame(i);
    delay(50);
  }
  delay(300);    // 闭眼停留
}

void drawFrame(int idx) {
  display.clearDisplay();
  // drawXBitmap 直接吃 XBM 格式数据, 1 = 亮
  // eyeFrames[idx] 在 PROGMEM, Adafruit_GFX 内部用 pgm_read
  display.drawXBitmap(0, 0, eyeFrames[idx], EYE_W, EYE_H, SSD1306_WHITE);
  display.display();
}
