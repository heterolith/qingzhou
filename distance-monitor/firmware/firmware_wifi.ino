/* ==========================================================
   firmware_wifi.ino — 距离监测仪 WiFi 联机版固件（中文版）
   OLED 显示全部中文：
     - 启动：网络名称 + WiFi 名
     - 正常测距：距离数字 + 底部进度条（越近越满）
     - 小于15cm：过近警告，距离越近闪烁越快
     - 测不到：无效
   依赖库（Arduino IDE 库管理器安装）：
     - ESP8266 board package
     - U8g2（OLED 驱动，支持中文字体）
     - Links2004/arduinoWebSockets（WebSocket 服务端）
     - ArduinoJson
   ========================================================== */

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <U8g2lib.h>
#include <Wire.h>

/* ===== WiFi 配置 ===== */
const char* WIFI_SSID = "iQOONeo 10";
const char* WIFI_PASS = "147963Zxcvbnm.";

/* ===== 引脚与常量 ===== */
#define TRIG_PIN 14  // D5
#define ECHO_PIN 12  // D6

#define MAX_DISTANCE 400.0
#define TMP_THRESHOLD 15.0   // 小于此值触发"过近警告"

/* ===== OLED 128x64 I2C：SDA=GPIO4(D2), SCL=GPIO5(D1) ===== */
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 5, 4);

/* ===== WebSocket ===== */
WebSocketsServer webSocket(81);

/* ===== 运行时参数（网页可改阈值）===== */
float tmpThreshold = TMP_THRESHOLD;

/* ===== 计时器 ===== */
unsigned long lastBroadcast = 0;
unsigned long lastWifiShow = 0;

/* ===== 状态 ===== */
bool wifiConnected = false;
bool wsStarted = false;

/* ==========================================================
   工具：画进度条（越近越满）
   ========================================================== */
void drawProgressBar(float d) {
  int barX = 4, barY = 56, barW = 120, barH = 6;
  u8g2.drawFrame(barX, barY, barW, barH);

  if (d < 0) d = 0;
  if (d > MAX_DISTANCE) d = MAX_DISTANCE;

  // 越近越满：d=0 → 满，d=MAX → 空
  int fill = (int)((1.0 - d / MAX_DISTANCE) * (barW - 4));
  if (fill < 0) fill = 0;
  if (fill > barW - 4) fill = barW - 4;
  u8g2.drawBox(barX + 2, barY + 2, fill, barH - 4);
}

/* ==========================================================
   工具：居中绘制字符串
   ========================================================== */
void drawCentered(const char* text, int y) {
  int w = u8g2.getStrWidth(text);
  int x = (128 - w) / 2;
  u8g2.drawStr(x, y, text);
}

/* ==========================================================
   测距（三次取中值，与 1.txt 一致）
   ========================================================== */
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

/* ==========================================================
   显示：无效（测不到 / 超量程）
   ========================================================== */
void showInvalid() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered("无效", 32);
  drawProgressBar(MAX_DISTANCE);  // 无效时空条
  u8g2.sendBuffer();
  delay(200);
}

/* ==========================================================
   显示：过近警告（< tmpThreshold）
   距离越近，闪烁间隔越短（0cm→60ms，15cm→500ms）
   ========================================================== */
void showTmp(float d) {
  int interval = map((int)d, 0, (int)tmpThreshold, 60, 500);
  if (interval < 60) interval = 60;
  if (interval > 500) interval = 500;

  String distStr = String((int)d) + " cm";

  // 亮屏
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered("过近警告", 20);
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered(distStr.c_str(), 42);
  drawProgressBar(d);
  u8g2.sendBuffer();
  delay(interval);

  // 灭屏（闪烁）
  u8g2.clearBuffer();
  u8g2.sendBuffer();
  delay(interval);
}

/* ==========================================================
   显示：正常测距（≥ tmpThreshold）
   ========================================================== */
void showNormal(float d) {
  String distStr = String((int)d) + " cm";

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered("距离", 18);
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered(distStr.c_str(), 40);
  drawProgressBar(d);
  u8g2.sendBuffer();
  delay(150);
}

/* ==========================================================
   显示：连接中（网络名称 + WiFi 名 + 动画）
   ========================================================== */
void showConnecting() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered("网络名称", 14);

  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered(WIFI_SSID, 32);

  // 动画点
  int dots = (millis() / 400) % 4;
  String dotsStr = "";
  for (int i = 0; i < dots; i++) dotsStr += ".";
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered(dotsStr.c_str(), 50);

  u8g2.sendBuffer();
}

/* ==========================================================
   显示：连接成功（IP + 端口，2.5秒）
   ========================================================== */
void showWiFiOK() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered("连接成功", 14);

  u8g2.setFont(u8g2_font_wqy16_t_gb2312);
  drawCentered(WiFi.localIP().toString().c_str(), 32);
  drawCentered(":81", 48);

  u8g2.sendBuffer();
}

/* ===== 前向声明 ===== */
void broadcastData(uint8_t except = 255);

/* ==========================================================
   状态判断
   ========================================================== */
String getDistanceState(float d) {
  if (d < 0 || d > MAX_DISTANCE) return "invalid";
  if (d < tmpThreshold) return "tmp";
  return "normal";
}

/* ==========================================================
   WebSocket 事件处理
   ========================================================== */
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] 网页断开\n", num);
      break;
    case WStype_CONNECTED:
      Serial.printf("[%u] 网页已连接\n", num);
      broadcastData(num);
      break;
    case WStype_TEXT: {
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
        webSocket.sendTXT(num, "{\"type\":\"pong\"}");
      }
      break;
    }
    default: break;
  }
}

/* ==========================================================
   广播测距数据
   ========================================================== */
void broadcastData(uint8_t except) {
  float d = measureMedian();
  String state = getDistanceState(d);

  StaticJsonDocument<128> doc;
  doc["type"] = "data";
  doc["distance"] = d;
  doc["state"] = state;
  doc["threshold"] = tmpThreshold;
  doc["maxRange"] = MAX_DISTANCE;

  String json;
  serializeJson(doc, json);
  webSocket.broadcastTXT(json);

  // OLED 显示
  if (d < 0 || d > MAX_DISTANCE) {
    showInvalid();
  } else if (d < tmpThreshold) {
    showTmp(d);
  } else {
    showNormal(d);
  }

  // 串口日志
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
  u8g2.begin();
  u8g2.enableUTF8Print();

  Serial.println("OLED ok");
  Serial.printf("连接 WiFi: %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  showConnecting();
}

/* ==========================================================
   loop
   ========================================================== */
void loop() {
  // 阶段1：WiFi 未连上
  if (!wifiConnected) {
    if (WiFi.status() == WL_CONNECTED) {
      wifiConnected = true;
      Serial.println();
      Serial.print("WiFi 已连接，IP: ");
      Serial.println(WiFi.localIP());

      webSocket.begin();
      webSocket.onEvent(webSocketEvent);
      wsStarted = true;
      Serial.println("WebSocket 服务已启动 (端口 81)");

      showWiFiOK();
      lastBroadcast = millis();
      delay(2500);
    } else {
      if (millis() - lastWifiShow > 350) {
        lastWifiShow = millis();
        showConnecting();
        Serial.print(".");
      }
    }
    return;
  }

  // 阶段2：正常测距
  if (wsStarted) webSocket.loop();

  unsigned long now = millis();
  if (now - lastBroadcast >= 1000) {
    lastBroadcast = now;
    broadcastData();
  }
}
