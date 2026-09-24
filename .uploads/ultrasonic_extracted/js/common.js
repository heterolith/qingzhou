/* ==========================================================
   common.js — 公共逻辑层
   所有页面必须引用。负责：
   ① settings 默认值与 localStorage 读写
   ② 传感器互斥等规则
   ③ 恢复默认重置
   ④ 底部导航高亮
   ========================================================== */

/* ===== 默认设置（唯一真相来源） ===== */
const DEFAULTS = {
  threshold: 0.5,          // 安全阈值 m
  volume: 80,              // 蜂鸣音量 %
  sensitivity: 2,          // 灵敏度 1低 2标准 3高
  vibrate: true,           // 震动反馈
  vibrateIntensity: 2,     // 震动强度 1弱 2中 3强
  buzzerOn: false,         // 蜂鸣器
  sensorOn: true,          // 超声波（默认开）
  irOn: false,             // 红外（默认关）
  dualMode: false          // 双传感器模式
};

const MAX_RANGE = 2.5;     // 量程 m

/* ===== 读取设置（带默认值兜底，容错损坏数据） ===== */
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
/* 用法：updateSetting('threshold', 0.8) */
function updateSetting(key, value) {
  const s = loadSettings();
  s[key] = value;
  applySensorRules(s);
  saveSettings(s);
  return s;
}

/* ===== 传感器互斥规则（唯一规则所在处） ===== */
function applySensorRules(s) {
  if (!s.dualMode && s.sensorOn && s.irOn) {
    s.irOn = false;   // 互斥模式下两者同开时，保留超声波
  }
}

/* ===== 恢复全部默认 ===== */
function resetToDefaults() {
  saveSettings({ ...DEFAULTS });
}

/* ===== 传感器显示名（主页徽章 / 数据页摘要卡共用） ===== */
function getSensorInfo(s) {
  if (s.sensorOn && s.irOn) return { name: '双传感器模式',   icon: 'fa-layer-group' };
  if (s.sensorOn)           return { name: '超声波传感器',   icon: 'fa-tower-broadcast' };
  if (s.irOn)               return { name: '红外探测器',     icon: 'fa-satellite-dish' };
  return                       { name: '传感器已全部关闭', icon: 'fa-circle-exclamation', off: true };
}

/* ===== 报警判断 ===== */
function isAlarm(distance, s) {
  return distance < s.threshold;
}

/* ===== 当前生效的报警方式（用于记录文案） ===== */
function getAlarmMethodName(s) {
  if (s.buzzerOn && s.vibrate) return '蜂鸣与震动已触发';
  if (s.buzzerOn)              return '蜂鸣已触发';
  if (s.vibrate)               return '震动已触发';
  return                         '已静默记录';
}

/* ===== 底部导航高亮（每个页面 body 上标 data-page） ===== */
/* 用法：<body data-page="control">，导航按钮上标 data-nav="control" */
function highlightNav() {
  const cur = document.body.dataset.page;
  document.querySelectorAll('.nav-item[data-nav]').forEach(btn => {
    btn.classList.toggle('active', btn.dataset.nav === cur);
  });
}

/* ===== 页面初始化时统一调用 ===== */
document.addEventListener('DOMContentLoaded', highlightNav);
