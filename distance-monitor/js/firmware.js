/* ==========================================================
   firmware.js — Arduino 固件测距逻辑的 JS 移植版
   与 1.txt 固件完全对齐：
   ① measureOnce / measureMedian（中值滤波，三次取中值）
   ② 三态判断（Invalid / TMP闪烁 / 正常）
   ③ TMP 闪烁间隔（map 越近越快）
   ④ 进度条 fill 计算
   纯前端模拟，不依赖硬件。simObj 为模拟物体（固件无此概念）。
   ========================================================== */

/* ===== 固件常量（对应固件 #define）===== */
const FW = {
  MAX_DISTANCE: 400,      // 量程 cm（对应 MAX_DISTANCE 400.0）
  TMP_THRESHOLD: 15,      // TMP 闪烁阈值 cm（对应 TMP_THRESHOLD 15.0）
  TMP_MIN_INT: 60,        // 闪烁最快 ms（0cm 时）
  TMP_MAX_INT: 500,       // 闪烁最慢 ms（阈值处）
  BAR_WIDTH: 120,         // OLED 进度条总宽 px（对应 barW=120）
};

/* ===== 模拟物体位置（驱动 measureOnce 的"真实距离"，固件无此概念）===== */
const simObj = {
  pos: 50,                // 当前物体位置 cm
  mode: 'wave',           // wave=正弦波动 | manual=手动设置
  manualTarget: 50,       // 手动模式目标 cm
  t: 0,                   // 波动相位
  /* 更新物体位置（每次 measureOnce 前调用）*/
  update() {
    if (this.mode === 'wave') {
      this.t += 0.06;
      this.pos = 95 + 75 * Math.sin(this.t) + 18 * Math.sin(this.t * 2.7);
    } else {
      this.pos += (this.manualTarget - this.pos) * 0.18;
    }
    if (this.pos < 2) this.pos = 2;
    if (this.pos > 430) this.pos = 430;
  }
};

/* 设置模拟物体距离（手动模式用）*/
function setSimDistance(d) {
  simObj.manualTarget = Math.max(2, Math.min(d, 430));
}

/* 设置模拟模式：'wave' | 'manual' */
function setSimMode(mode) {
  simObj.mode = mode;
}

/* ===== 对应 measureOnce：模拟单次超声波测距 =====
   固件：触发 10µs 脉冲 → pulseIn 超时 25ms → duration*0.0343/2
   模拟：返回 物体位置 + 噪声；3% 概率无回波返回 -1 */
function measureOnce() {
  simObj.update();
  const noise = (Math.random() - 0.5) * 2.6;   // ±1.3cm 噪声
  if (Math.random() < 0.03) return -1;          // 3% 无回波（模拟 pulseIn 超时）
  return simObj.pos + noise;
}

/* ===== 对应 measureMedian：三次取中值（与固件排序完全一致）=====
   固件三步交换排序后返回中值 b */
function measureMedian() {
  let a = measureOnce();
  let b = measureOnce();
  let c = measureOnce();
  if (a > b) { const t = a; a = b; b = t; }
  if (b > c) { const t = b; b = c; c = t; }
  if (a > b) { const t = a; a = b; b = t; }
  return b;
}

/* ===== 三态判断（对应固件 loop 的分支）=====
   d<0 或 d>MAX_DISTANCE → invalid
   d<TMP_THRESHOLD → tmp（近距闪烁）
   否则 → normal */
function getDistanceState(d) {
  if (d < 0 || d > FW.MAX_DISTANCE) return 'invalid';
  if (d < FW.TMP_THRESHOLD) return 'tmp';
  return 'normal';
}

/* ===== TMP 闪烁间隔（对应固件 showTmp 的 map）=====
   0cm → 60ms，TMP阈值 → 500ms，越近闪越快 */
function getTmpInterval(d) {
  const dd = Math.max(0, Math.min(d, FW.TMP_THRESHOLD));
  let interval = Math.round(
    FW.TMP_MIN_INT + (dd / FW.TMP_THRESHOLD) * (FW.TMP_MAX_INT - FW.TMP_MIN_INT)
  );
  return Math.max(FW.TMP_MIN_INT, Math.min(FW.TMP_MAX_INT, interval));
}

/* ===== 进度条填充宽度（对应固件 showNormal 的 fill）=====
   fill = d / MAX_DISTANCE * (BAR_WIDTH - 4) */
function getBarFill(d) {
  let fill = Math.floor((d / FW.MAX_DISTANCE) * (FW.BAR_WIDTH - 4));
  return Math.max(0, Math.min(fill, FW.BAR_WIDTH - 4));
}

/* ===== 串口日志格式（对应固件 loop 里的 Serial.print）===== */
function formatSerialLine(d) {
  if (d < 0) return 'd=no echo';
  return 'd=' + d.toFixed(2) + ' cm';
}
