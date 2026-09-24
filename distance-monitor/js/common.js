/* ==========================================================
   common.js — 公共逻辑层（对齐固件版）
   所有页面必须引用（在 firmware.js 之后）。
   负责：
   ① settings 默认值与 localStorage 读写
   ② 传感器互斥等规则（Web 增值）
   ③ 恢复默认重置
   ④ 底部导航高亮
   量程/阈值单位对齐固件：cm（MAX_RANGE=400, threshold 默认 15）
   ========================================================== */

/* ===== 默认设置（唯一真相来源）=====
   threshold 对齐固件 TMP_THRESHOLD=15cm
   sensorOn 对齐固件超声波测距开关
   其余（蜂鸣器/震动/双传感器/灵敏度）为 Web 增值 */
const DEFAULTS = {
  threshold: 15,           // TMP/报警阈值 cm（对齐固件 TMP_THRESHOLD 15.0）
  volume: 80,              // 蜂鸣音量 %（Web 增值）
  sensitivity: 2,          // 灵敏度 1低 2标准 3高（Web 增值）
  vibrate: true,           // 震动反馈（Web 增值）
  vibrateIntensity: 2,     // 震动强度 1弱 2中 3强（Web 增值）
  buzzerOn: false,         // 蜂鸣器（Web 增值）
  sensorOn: true,          // 超声波测距开关（对齐固件 HC-SR04）
  irOn: false,             // 红外探测器（Web 增值，固件无）
  dualMode: false,         // 双传感器模式（Web 增值）
  simMode: 'wave',         // 模拟物体模式 wave|manual（Web 模拟用）
  simDistance: 50,         // 模拟物体距离 cm（手动模式）
};

/* 量程 cm（对齐固件 MAX_DISTANCE 400.0）*/
const MAX_RANGE = FW.MAX_DISTANCE;

/* ===== 读取设置（带默认值兜底，容错损坏数据）===== */
function loadSettings() {
  try {
    const saved = JSON.parse(localStorage.getItem('dmSettings') || '{}');
    return { ...DEFAULTS, ...saved };
  } catch (e) {
    return { ...DEFAULTS };
  }
}

/* ===== 保存设置 ===== */
function saveSettings(s) {
  localStorage.setItem('dmSettings', JSON.stringify(s));
}

/* ===== 统一写入入口：改设置必须走这里 ===== */
/* 用法：updateSetting('threshold', 20) */
function updateSetting(key, value) {
  const s = loadSettings();
  s[key] = value;
  applySensorRules(s);
  saveSettings(s);
  // 连固件时同步设置给固件（仅固件支持的参数）
  if (typeof Conn !== 'undefined' && Conn.isOnline()) {
    if (key === 'threshold') {
      Conn.sendSetting('threshold', value);
    }
    // sensorOn / irOn 等传感器开关固件暂不支持运行时切换，只本地生效
  }
  return s;
}

/* ===== 传感器互斥规则（Web 增值，固件无此概念）=====
   非双传感器模式下两者同开时，保留超声波（关闭红外）*/
function applySensorRules(s) {
  if (!s.dualMode && s.sensorOn && s.irOn) {
    s.irOn = false;
  }
}

/* ===== 恢复全部默认 ===== */
function resetToDefaults() {
  saveSettings({ ...DEFAULTS });
}

/* ===== 传感器显示名（主页徽章 / 数据页摘要卡共用）===== */
function getSensorInfo(s) {
  if (s.sensorOn && s.irOn) return { name: '双传感器模式',    icon: 'fa-layer-group' };
  if (s.sensorOn)           return { name: '超声波传感器',    icon: 'fa-tower-broadcast' };
  if (s.irOn)               return { name: '红外探测器',      icon: 'fa-satellite-dish' };
  return                       { name: '传感器已全部关闭', icon: 'fa-circle-exclamation', off: true };
}

/* ===== 报警判断（对齐固件 TMP 阈值概念：低于 threshold 即报警/闪烁）===== */
function isAlarm(distance, s) {
  return distance < s.threshold;
}

/* ===== 当前生效的报警方式（用于记录文案，Web 增值）===== */
function getAlarmMethodName(s) {
  if (s.buzzerOn && s.vibrate) return '蜂鸣与震动已触发';
  if (s.buzzerOn)              return '蜂鸣已触发';
  if (s.vibrate)               return '震动已触发';
  return                         '已静默记录';
}

/* ===== 底部导航高亮（每个页面 body 上标 data-page）===== */
/* 用法：<body data-page="control">，导航按钮上标 data-nav="control" */
function highlightNav() {
  const cur = document.body.dataset.page;
  document.querySelectorAll('.nav-item[data-nav]').forEach(btn => {
    btn.classList.toggle('active', btn.dataset.nav === cur);
  });
}

/* ===== 页面初始化时统一调用 ===== */
document.addEventListener('DOMContentLoaded', highlightNav);
