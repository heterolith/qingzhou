/* mock-ws-server.js
   模拟 ESP8266 固件的 WebSocket 服务，用于本地测试网页连接逻辑
   启动后每秒广播 {type:"data", distance:随机, state:"normal|tmp|invalid"}
   接收网页发来的 {type:"set", threshold:N} 并应用
   用法：node mock-ws-server.js
*/
const WebSocket = require('ws');

const PORT = 8089;
const wss = new WebSocket.Server({ port: PORT });

let threshold = 15;
let t = 0;

console.log(`Mock 固件 WebSocket 服务已启动 → ws://localhost:${PORT}`);

function getDistanceState(d) {
  if (d < 0 || d > 400) return 'invalid';
  if (d < threshold) return 'tmp';
  return 'normal';
}

wss.on('connection', (ws) => {
  console.log('[网页已连接]');

  // 立即推一次数据
  send(ws);

  // 每秒广播数据
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
    } catch (e) {
      console.warn('[解析失败]', e.message);
    }
  });

  ws.on('close', () => {
    console.log('[网页断开]');
    clearInterval(timer);
  });
});

function send(ws) {
  // 模拟固件测距：正弦波动 + 噪声
  t += 0.1;
  let d = 180 + 120 * Math.sin(t) + 15 * Math.sin(t * 2.7) + (Math.random() - 0.5) * 6;
  // 5% 概率进入 TMP
  if (Math.random() < 0.05) d = 3 + Math.random() * 11;
  // 3% 概率无回波
  if (Math.random() < 0.03) d = -1;

  d = Math.max(-1, Math.min(d, 450));

  const state = getDistanceState(d);
  const msg = {
    type: 'data',
    distance: parseFloat(d.toFixed(2)),
    state,
    threshold,
    maxRange: 400,
  };
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify(msg));
  }
  console.log(`推送 d=${msg.distance}cm state=${state} (阈值=${threshold}cm)`);
}
