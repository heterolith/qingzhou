/* ==========================================================
   firmware_wifi.ino — 距离监测仪 WiFi 联机版固件
   基于 1.txt 扩展：保留 HC-SR04 + OLED + 三态显示
   新增：WiFi 连接 + WebSocket 服务
   通信协议（JSON）：
     固件 → 网页：{type:"data", distance:50.2, state:"normal"}
     网页 → 固件：{type:"set", threshold:20}
   依赖库（Arduino IDE 库管理器安装）：
     - ESP8266 board package
     - Adafruit_GFX / Adafruit_SSD1306（OLED 驱动）
     - Links2004/arduinoWebSockets（WebSocket 服务端）
   ========================================================== */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ===== WiFi 配置（改成你的）===== */
const char* WIFI_SSID = "YOUR_SSID";
const char* WIFI_PASS = "YOUR_PASSWORD";

/* ===== 固件常量（对齐 1.txt）===== */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
#define OLED_ADDR    0x3C

#define TRIG_PIN 14  // D5
#define ECHO_PIN 12  // D6

#define MAX_DISTANCE 400.0
#define TMP_THRESHOLD_DEFAULT 15.0  // 默认 TMP 阈值，可被网页修改

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WebSocketsServer webSocket(81);  // WebSocket 端口 81

/* ===== 运行时可调参数（网页可改）===== */
float tmpThreshold = TMP_THRESHOLD_DEFAULT;

/* ===== 上次广播时间（避免刷屏，每秒一次）===== */
unsigned long lastBroadcast = 0;

/* ==========================================================
   OLED 显示函数（与 1.txt 完全一致，保持不变）
   ========================================================== */
void printCentered(const String &text, int y, int textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2 - x1;
  display.setCursor(x, y);
  display.print(text);
}

float measureOnce() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 25000);
  if (duration == 0) return -1;
  return duration * 0.0343 / 2.0;
}

float measureMedian() {
  float a = measureOnce(); delay(15);
  float b = measureOnce(); delay(15);
  float c = measureOnce();

  if (a > b) { float t = a; a = b; b = t; }
  if (b > c) { float t = b; b = c; c = t; }
  if (a > b) { float t = a; a = b; b = t; }
  return b;
}

void drawHeader() {
  printCentered("DISTANCE", 0, 1);
}

void showInvalid() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawHeader();
  printCentered("Invalid", 22, 2);
  display.display();
  delay(200);
}

void showTmp(float d) {
  int interval = map((int)d, 0, (int)tmpThreshold, 60, 500);
  if (interval < 60) interval = 60;
  if (interval > 500) interval = 500;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawHeader();
  printCentered("TMP", 14, 4);
  printCentered("CM", 46, 1);
  display.display();
  delay(interval);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawHeader();
  display.display();
  delay(interval);
}

void showNormal(float d) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  drawHeader();

  printCentered(String((int)d), 12, 4);
  printCentered("CM", 46, 1);

  int barX = 4, barY = 58, barW = 120, barH = 4;
  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);

  int fill = (int)((d / MAX_DISTANCE) * (barW - 4));
  if (fill < 0) fill = 0;
  if (fill > barW - 4) fill = barW - 4;
  display.fillRect(barX + 2, barY + 2, fill, barH - 4, SSD1306_WHITE);

  display.display();
  delay(150);
}

/* ===== 前向声明（C++ 要求函数定义前可见）===== */
void broadcastData(uint8_t except = 255);

/* ==========================================================
   状态判断（与网页 firmware.js getDistanceState 对齐）
   ========================================================== */
String getDistanceState(float d) {
  if (d < 0 || d > MAX_DISTANCE) return "invalid";
  if (d < tmpThreshold) return "tmp";
  return "normal";
}

/* ==========================================================
   WebSocket 事件处理：接收网页指令
   ========================================================== */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] 网页断开\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] 网页已连接\n", num);
      // 立即推送当前状态
      broadcastData(num);
      break;
    case WStype_TEXT: {
      // 解析 JSON 指令
      StaticJsonDocument<200> doc;
      DeserializationError err = deserializeJson(doc, payload, length);
      if (err) {
        Serial.println("JSON 解析失败");
        return;
      }
      String type = doc["type"] | "";
      if (type == "set") {
        if (doc.containsKey("threshold")) {
          tmpThreshold = doc["threshold"];
          Serial.printf("阈值已更新: %.1f cm\n", tmpThreshold);
        }
      } else if (type == "ping") {
        // 心跳响应
        webSocket.sendTXT(num, "{\"type\":\"pong\"}");
      }
      break;
    }
    default: break;
  }
}

/* ==========================================================
   广播测距数据给所有客户端（或指定客户端）
   ========================================================== */
void broadcastData(uint8_t except = 255) {
  static float lastD = -999;
  float d = measureMedian();

  // 状态判断
  String state = getDistanceState(d);

  // 构建 JSON
  StaticJsonDocument<128> doc;
  doc["type"] = "data";
  doc["distance"] = d;
  doc["state"] = state;
  doc["threshold"] = tmpThreshold;
  doc["maxRange"] = MAX_DISTANCE;

  String json;
  serializeJson(doc, json);

  // 广播给所有客户端
  webSocket.broadcastTXT(json);

  // OLED 显示（保留固件原三态逻辑）
  if (d < 0 || d > MAX_DISTANCE) {
    showInvalid();
  } else if (d < tmpThreshold) {
    showTmp(d);
  } else {
    showNormal(d);
  }

  // 串口日志（保留 1.txt 格式）
  Serial.print("d=");
  if (d < 0) Serial.println("no echo");
  else { Serial.print(d); Serial.println(" cm"); }
}

/* ==========================================================
   setup
   ========================================================== */
void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);

  Wire.begin(4, 5);  // SDA=GPIO4(D2), SCL=GPIO5(D1)

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
      Serial.println("OLED init failed");
      while (true) delay(1000);
    }
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.display();
  Serial.println("OLED ok");

  // WiFi 连接
  Serial.printf("连接 WiFi: %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi 已连接，IP: ");
  Serial.println(WiFi.localIP());

  // 启动 WebSocket 服务
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket 服务已启动 (端口 81)");

  // 在 OLED 上显示 IP
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  printCentered("WiFi OK", 8, 2);
  printCentered(WiFi.localIP().toString(), 34, 1);
  printCentered(":81", 50, 1);
  display.display();
  delay(2500);
}

/* ==========================================================
   loop：每秒测距 + 广播 + WebSocket 维护
   ========================================================== */
void loop() {
  webSocket.loop();  // 必须循环调用，处理 WebSocket 事件

  unsigned long now = millis();
  if (now - lastBroadcast >= 1000) {  // 每秒一次（与网页 setInterval 一致）
    lastBroadcast = now;
    broadcastData();
  }
}
