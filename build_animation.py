#!/usr/bin/env python3
"""读取所有 .h 文件，提取 RGB565 像素数据，生成自包含 HTML 动画。"""
import os
import re
import json

SRC_DIR = "/workspace/.uploads/8bef64e6_睁眼_extracted/1"
OUT_HTML = "/workspace/睁眼动画.html"

# 提取像素值的正则
HEX_RE = re.compile(r"0x([0-9A-Fa-f]{4})")

def extract_pixels(path):
    """返回 list[int]，长度应为 177*100。"""
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        text = f.read()
    vals = [int(m.group(1), 16) for m in HEX_RE.finditer(text)]
    return vals

def main():
    files = sorted(
        f for f in os.listdir(SRC_DIR) if f.endswith(".h")
    )
    frames = []
    for fn in files:
        px = extract_pixels(os.path.join(SRC_DIR, fn))
        if len(px) != 177 * 100:
            print(f"warning: {fn} has {len(px)} pixels, expected {177*100}")
        frames.append(px)
    print(f"loaded {len(frames)} frames, each {len(frames[0])} pixels")

    # 生成 JS 数组（每帧一个数组，元素为 16 进制字符串以减小体积）
    js_frames = ",\n".join(
        "[" + ",".join(f"0x{v:04x}" for v in px) + "]" for px in frames
    )

    html = """<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<title>睁眼动画 (RGB565 逐帧回放)</title>
<style>
  html, body {
    margin: 0; padding: 0;
    background: #0a0a0a;
    color: #ddd;
    font-family: -apple-system, "Segoe UI", "PingFang SC", "Microsoft YaHei", sans-serif;
    min-height: 100vh;
    display: flex; flex-direction: column;
    align-items: center; justify-content: center;
  }
  h1 { font-size: 18px; font-weight: 500; margin: 16px 0 8px; color: #eee; }
  .meta { font-size: 12px; color: #888; margin-bottom: 16px; }
  canvas {
    background: #000;
    image-rendering: pixelated;
    image-rendering: -moz-crisp-edges;
    image-rendering: crisp-edges;
    border: 1px solid #222;
    box-shadow: 0 0 40px rgba(0,0,0,0.8);
  }
  .controls {
    margin-top: 16px;
    display: flex; gap: 12px; align-items: center;
    font-size: 13px;
  }
  button {
    background: #222; color: #eee; border: 1px solid #444;
    padding: 6px 14px; border-radius: 4px; cursor: pointer;
    font-size: 13px;
  }
  button:hover { background: #333; }
  input[type=range] { width: 200px; vertical-align: middle; }
  .frame-info { min-width: 110px; color: #aaa; }
  .scrub { display: flex; align-items: center; gap: 8px; margin-top: 10px; }
  .scrub label { color:#888; font-size: 12px; }
</style>
</head>
<body>
  <h1>睁眼动画 · RGB565 逐帧回放</h1>
  <div class="meta">31 帧 · 177×100 · RGB565 · 来源: 睁眼.zip</div>
  <canvas id="cv" width="177" height="100"></canvas>
  <div class="controls">
    <button id="playBtn">⏸ 暂停</button>
    <span class="frame-info" id="frameInfo">帧 1 / 31</span>
  </div>
  <div class="scrub">
    <label>FPS</label>
    <input id="fps" type="range" min="1" max="30" value="12">
    <span id="fpsVal">12</span>
  </div>
  <div class="scrub">
    <label>缩放</label>
    <input id="zoom" type="range" min="1" max="8" value="4" step="1">
    <span id="zoomVal">4×</span>
  </div>
  <div class="scrub">
    <label>进度</label>
    <input id="seek" type="range" min="0" max="30" value="0" step="1" style="width:300px">
  </div>

<script>
// RGB565 像素数据 (31 帧 × 17700 像素)
const FRAMES = [
__FRAMES__
];
const W = 177, H = 100;
const cv = document.getElementById('cv');
const ctx = cv.getContext('2d');
const imgData = ctx.createImageData(W, H);

// 预解码每帧为 Uint32 ABGR (小端) 直接 putImageData
const decoded = FRAMES.map(px => {
  const buf = new ArrayBuffer(W * H * 4);
  const u32 = new Uint32Array(buf);
  for (let i = 0; i < px.length; i++) {
    const v = px[i];
    const r = ((v >> 11) & 0x1F) << 3;
    const g = ((v >> 5)  & 0x3F) << 2;
    const b = ( v        & 0x1F) << 3;
    // 小端 ABGR
    u32[i] = (255 << 24) | (b << 16) | (g << 8) | r;
  }
  return new Uint8ClampedArray(buf);
});

let cur = 0;
let playing = true;
let fps = 12;
let lastT = 0;

function drawFrame(i) {
  imgData.data.set(decoded[i]);
  ctx.putImageData(imgData, 0, 0);
  document.getElementById('frameInfo').textContent = `帧 ${i+1} / ${FRAMES.length}`;
  document.getElementById('seek').value = i;
}

function loop(t) {
  if (playing) {
    const dt = t - lastT;
    if (dt >= 1000 / fps) {
      cur = (cur + 1) % FRAMES.length;
      drawFrame(cur);
      lastT = t;
    }
  }
  requestAnimationFrame(loop);
}

document.getElementById('playBtn').addEventListener('click', e => {
  playing = !playing;
  e.target.textContent = playing ? '⏸ 暂停' : '▶ 播放';
});
document.getElementById('fps').addEventListener('input', e => {
  fps = +e.target.value;
  document.getElementById('fpsVal').textContent = fps;
});
document.getElementById('zoom').addEventListener('input', e => {
  const z = +e.target.value;
  document.getElementById('zoomVal').textContent = z + '×';
  cv.style.width = (W * z) + 'px';
  cv.style.height = (H * z) + 'px';
});
document.getElementById('seek').addEventListener('input', e => {
  cur = +e.target.value;
  drawFrame(cur);
});

// 初始缩放
cv.style.width = (W * 4) + 'px';
cv.style.height = (H * 4) + 'px';
drawFrame(0);
requestAnimationFrame(loop);
</script>
</body>
</html>
"""
    html = html.replace("__FRAMES__", js_frames)
    with open(OUT_HTML, "w", encoding="utf-8") as f:
        f.write(html)
    print(f"wrote {OUT_HTML}  ({len(html):,} bytes)")

if __name__ == "__main__":
    main()
