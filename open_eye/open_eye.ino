/*
 * Eye Animation - ESP8266 + 0.96" I2C OLED (SSD1306, 128x64)
 *
 * Wiring (4 wires):
 *   OLED GND -> ESP8266 GND
 *   OLED VCC -> ESP8266 3V3   (do NOT use 5V, will damage OLED)
 *   OLED SCL -> ESP8266 D1  (GPIO5)
 *   OLED SDA -> ESP8266 D2  (GPIO4)
 *
 * Libraries (install via Library Manager, choose "Install all" if asked):
 *   1. Adafruit SSD1306
 *   2. Adafruit GFX Library
 *
 * No manual library file edits needed. Just upload.
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "eye_frames.h"   // 31 pre-scaled 128x64 monochrome frames

#define OLED_ADDR   0x3C  // most 0.96" OLED use 0x3C; if blank try 0x3D
#define SCREEN_W    128
#define SCREEN_H    64

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);

void setup() {
  Serial.begin(115200);
  Serial.println(F("\n[Eye Animation OLED] boot"));

  // SSD1306 init: no reset pin (-1), I2C address 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 init failed, check wiring/address"));
    while (true) { delay(1000); }  // halt, watch serial
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
  // forward: closed -> open
  for (int i = 0; i < EYE_FRAMES; i++) {
    drawFrame(i);
    delay(50);   // ~20fps
  }
  delay(700);    // hold open

  // reverse: open -> closed
  for (int i = EYE_FRAMES - 1; i >= 0; i--) {
    drawFrame(i);
    delay(50);
  }
  delay(300);    // hold closed
}

void drawFrame(int idx) {
  display.clearDisplay();
  // drawXBitmap eats XBM data directly, 1 = on
  // eyeFrames[idx] in PROGMEM, Adafruit_GFX reads via pgm_read
  display.drawXBitmap(0, 0, eyeFrames[idx], EYE_W, EYE_H, SSD1306_WHITE);
  display.display();
}
