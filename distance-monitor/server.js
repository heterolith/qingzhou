/* server.js
   整合服务器：同一端口 8080 同时提供静态文件 + WebSocket 服务
   用于本地测试（真实部署时网页和固件分别在 PC 和 ESP8266 上）
   用法：node server.js
*/
const http = require('http');
const fs = require('fs');
const path = require('path');
const WebSocket = require('ws');

const PORT = 8080;
const ROOT = __dirname;

/* ===== HTTP 静态文件服务 ===== */
const server = http.createServer((req, res) => {
  let urlPath = req.url === '/' ? '/index.html' : req.url;
  // 防止路径穿越
  urlPath = urlPath.split('?')[0];
  const filePath = path.join(ROOT, urlPath);
  if (!filePath.startsWith(ROOT)) { res.writeHead(403); res.end(); return; }

  const ext = path.extname(filePath);
  const types = { '.html':'text/html; charset=utf-8', '.css':'text/css', '.js':'text/javascript', '.ico':'image/x-icon' };
  fs.readFile(filePath, (err, data) => {
    if (err) { res.writeHead(404); res.end('Not Found'); return; }
    res.writeHead(200, { 'Content-Type': types[ext] || 'application/octet-stream' });
    res.end(data);
  });
});

/* ===== WebSocket 服务（模拟 ESP8266 固件）===== */
const wss = new WebSocket.Server({ server });

let threshold = 15;
let t = 0;

function getDistanceState(d) {
  if (d < 0 || d > 400) return 'invalid';
  if (d < threshold) return 'tmp';
  return 'normal';
}

function send(ws) {
  // 模拟固件测距：正弦波动 + 噪声
  t += 0.1;
  let d = 180 + 120 * Math.sin(t) + 15 * Math.sin(t * 2.7) + (Math.random() - 0.5) * 6;
  if (Math.random() < 0.05) d = 3 + Math.random() * 11;   // 5% TMP
  if (Math.random() < 0.03) d = -1;                        // 3% 无回波
  d = Math.max(-1, Math.min(d, 450));

  const state = getDistanceState(d);
  const msg = { type: 'data', distance: parseFloat(d.toFixed(2)), state, threshold, maxRange: 400 };
  if (ws.readyState === WebSocket.OPEN) ws.send(JSON.stringify(msg));
  console.log(`推送 d=${msg.distance}cm state=${state} (阈值=${threshold}cm)`);
}

wss.on('connection', (ws) => {
  console.log('[网页已连接]');
  send(ws);
  const timer = setInterval(() => send(ws), 1000);

  ws.on('message', (raw) => {
    try {
      const msg = JSON.parse(raw.toString());
      if (msg.type === 'set' && msg.threshold != null) {
        threshold = msg.threshold;
        console.log(`[设置更新] threshold = ${threshold} cm`);
      } else if (msg.type === 'ping') {
        ws.send(JSON.stringify({ type: 'pong' }));
      }
    } catch (e) { console.warn('[解析失败]', e.message); }
  });

  ws.on('close', () => { console.log('[网页断开]'); clearInterval(timer); });
});

server.listen(PORT, () => {
  console.log(`整合服务器: http://localhost:${PORT} (HTTP + WebSocket 同端口)`);
  console.log(`WebSocket 端点: ws://localhost:${PORT}`);
});
