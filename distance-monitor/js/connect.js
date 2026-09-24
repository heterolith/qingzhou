/* ==========================================================
   connect.js — WebSocket 连接层
   负责：连接 ESP8266 固件 / 接收真实数据 / 发送控制指令 / 降级模拟
   在 firmware.js 与 common.js 之后加载
   ========================================================== */

/* ===== 连接状态枚举 ===== */
const ConnState = {
  OFFLINE:   'offline',    // 未连接，用模拟
  CONNECTING:'connecting', // 连接中
  ONLINE:    'online',      // 已连固件
  ERROR:     'error',       // 连接错误
};

/* ===== 连接管理器 ===== */
const Conn = {
  ws: null,
  state: ConnState.OFFLINE,
  ip: '',                   // ESP8266 IP 地址
  port: 81,
  lastData: null,           // 最近一次收到的数据 {distance, state, threshold}
  listeners: [],            // 数据回调

  /* ===== 注册数据回调（收到固件数据时调用）===== */
  onData(fn) { this.listeners.push(fn); },

  /* ===== 状态变更回调 ===== */
  onStateChange: null,

  /* ===== 读取连接 IP（从 localStorage）===== */
  loadIP() {
    return localStorage.getItem('dmDeviceIP') || '';
  },

  /* ===== 保存连接 IP ===== */
  saveIP(ip) {
    localStorage.setItem('dmDeviceIP', ip);
  },

  /* ===== 主动连接 ===== */
  connect(ip, port) {
    this.disconnect();
    this.ip = ip;
    this.port = port || 81;
    this.saveIP(ip);
    this.setState(ConnState.CONNECTING);

    if (!ip) {
      this.setState(ConnState.OFFLINE);
      return false;
    }

    try {
      // 端口8080为本地测试同端口WebSocket；真实固件默认81
      const url = `ws://${ip}:${this.port}`;
      this.ws = new WebSocket(url);

      this.ws.onopen = () => {
        this.setState(ConnState.ONLINE);
        console.log('[Conn] 已连固件:', url);
        this.startPing();
      };

      this.ws.onmessage = (ev) => {
        try {
          const msg = JSON.parse(ev.data);
          if (msg.type === 'data') {
            this.lastData = msg;
            this.listeners.forEach(fn => fn(msg));
          } else if (msg.type === 'pong') {
            // 心跳响应，忽略
          }
        } catch (e) {
          console.warn('[Conn] 解析失败:', e);
        }
      };

      this.ws.onclose = () => {
        this.setState(ConnState.OFFLINE);
        console.log('[Conn] 连接关闭');
      };

      this.ws.onerror = () => {
        this.setState(ConnState.ERROR);
        console.warn('[Conn] 连接错误');
      };

      return true;
    } catch (e) {
      this.setState(ConnState.ERROR);
      return false;
    }
  },

  /* ===== 断开 ===== */
  disconnect() {
    this.stopPing();
    if (this.ws) {
      try { this.ws.close(); } catch(e) {}
      this.ws = null;
    }
    if (this.state !== ConnState.OFFLINE) this.setState(ConnState.OFFLINE);
  },

  /* ===== 心跳（每 5 秒发 ping）===== */
  _pingTimer: null,
  startPing() {
    this.stopPing();
    this._pingTimer = setInterval(() => {
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.ws.send(JSON.stringify({ type: 'ping' }));
      }
    }, 5000);
  },
  stopPing() {
    if (this._pingTimer) { clearInterval(this._pingTimer); this._pingTimer = null; }
  },

  /* ===== 发送设置指令给固件 ===== */
  sendSetting(key, value) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      const msg = JSON.stringify({ type: 'set', [key]: value });
      this.ws.send(msg);
      console.log('[Conn] 发送设置:', key, '=', value);
      return true;
    }
    return false;
  },

  /* ===== 状态切换 + 回调通知 ===== */
  setState(s) {
    this.state = s;
    if (this.onStateChange) this.onStateChange(s);
    document.dispatchEvent(new CustomEvent('conn-state', { detail: s }));
  },

  /* ===== 是否已连固件（用真实数据）===== */
  isOnline() {
    return this.state === ConnState.ONLINE && this.ws && this.ws.readyState === WebSocket.OPEN;
  },

  /* ===== 取当前距离（连固件用真实，未连用模拟）===== */
  getDistance() {
    if (this.isOnline() && this.lastData) {
      return this.lastData.distance;
    }
    return null;  // null 表示用模拟
  },

  /* ===== 取当前状态（连固件用真实，未连用模拟）===== */
  getFwState() {
    if (this.isOnline() && this.lastData) {
      return this.lastData.state;
    }
    return null;
  },
};
