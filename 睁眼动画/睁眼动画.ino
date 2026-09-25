/*
 * 睁眼动画 - ESP8266 + TFT 显示屏
 * 图像: 177 x 100, RGB565, 31 帧
 * 依赖库: TFT_eSPI (by Bodmer)  —— 通过库管理器安装
 *
 * 接线 (ESP8266 NodeMCU 为例, 软件 SPI):
 *   TFT_SCK  -> D5  (GPIO14)
 *   TFT_MOSI -> D7  (GPIO13)
 *   TFT_CS   -> D8  (GPIO15)
 *   TFT_DC   -> D1  (GPIO5)
 *   TFT_RST  -> D2  (GPIO4)
 *   TFT_VCC  -> 3V3 / VCC (注意背光 BL 也接 3V3)
 *   TFT_GND  -> GND
 *
 * 使用前请在 TFT_eSPI 库目录下编辑 User_Setup.h,
 * 选定你的驱动芯片 (常见 ST7735 / ILI9341 / GC9A01A / ST7789 等),
 * 并把屏幕几何改为可容纳 177x100 (例如 ST7735_GREENTAB3 128x128 装不下,
 * 需用 160x80 / 128x160 / 240x240 / 240x135 等屏)。
 */

#include <TFT_eSPI.h>

// --- 31 帧图像数据 (RGB565, 存于 Flash) ---
#include "img_1000160910.h"
#include "img_1000160911.h"
#include "img_1000160912.h"
#include "img_1000160913.h"
#include "img_1000160914.h"
#include "img_1000160915.h"
#include "img_1000160916.h"
#include "img_1000160917.h"
#include "img_1000160918.h"
#include "img_1000160919.h"
#include "img_1000160920.h"
#include "img_1000160921.h"
#include "img_1000160922.h"
#include "img_1000160923.h"
#include "img_1000160924.h"
#include "img_1000160925.h"
#include "img_1000160926.h"
#include "img_1000160927.h"
#include "img_1000160928.h"
#include "img_1000160929.h"
#include "img_1000160930.h"
#include "img_1000160931.h"
#include "img_1000160932.h"
#include "img_1000160933.h"
#include "img_1000160934.h"
#include "img_1000160935.h"
#include "img_1000160936.h"
#include "img_1000160937.h"
#include "img_1000160938.h"
#include "img_1000160939.h"
#include "img_1000160940.h"

// 图像几何
#define IMG_W 177
#define IMG_H 100
#define FRAME_COUNT 31

// 帧指针表 —— 指向 Flash 中的 PROGMEM 数组
const uint16_t* const frames[FRAME_COUNT] PROGMEM = {
  img_1000160910, img_1000160911, img_1000160912, img_1000160913,
  img_1000160914, img_1000160915, img_1000160916, img_1000160917,
  img_1000160918, img_1000160919, img_1000160920, img_1000160921,
  img_1000160922, img_1000160923, img_1000160924, img_1000160925,
  img_1000160926, img_1000160927, img_1000160928, img_1000160929,
  img_1000160930, img_1000160931, img_1000160932, img_1000160933,
  img_1000160934, img_1000160935, img_1000160936, img_1000160937,
  img_1000160938, img_1000160939, img_1000160940
};

TFT_eSPI tft = TFT_eSPI();

// 画在屏幕上的起始坐标 (居中)
int x0, y0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n[睁眼动画] boot"));

  tft.init();
  tft.setRotation(0);          // 按你的屏幕方向调整 0/1/2/3
  tft.fillScreen(TFT_BLACK);

  // 把 177x100 居中显示
  x0 = (tft.width()  - IMG_W) / 2;
  y0 = (tft.height() - IMG_H) / 2;
  if (x0 < 0) x0 = 0;
  if (y0 < 0) y0 = 0;

  Serial.printf("[睁眼动画] screen=%dx%d img=%dx%d offset=(%d,%d)\n",
                tft.width(), tft.height(), IMG_W, IMG_H, x0, y0);
}

void loop() {
  // 正向播放: 闭眼 -> 睁眼
  for (int i = 0; i < FRAME_COUNT; i++) {
    drawFrame(i);
    delay(60);   // ~16 fps, 按效果调整
  }

  delay(800);    // 睁眼停留

  // 反向播放: 睁眼 -> 闭眼
  for (int i = FRAME_COUNT - 1; i >= 0; i--) {
    drawFrame(i);
    delay(60);
  }

  delay(400);    // 闭眼停留后循环
}

void drawFrame(int idx) {
  // pushImage 第 5 个参数为 const uint16_t*,
  // 配合 ESP8266 的 PROGMEM, TFT_eSPI 内部会用 pgm_read_word 读取。
  tft.pushImage(x0, y0, IMG_W, IMG_H, (uint16_t*)frames[idx]);
}
