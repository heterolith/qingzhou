#!/usr/bin/env python3
"""把生成的 XBM 头文件渲染成 31 帧 PNG 预览, 拼成一张大图."""
import os, re

HDR = "/workspace/睁眼动画_OLED/eye_frames.h"
OUT = "/workspace/睁眼动画_OLED/preview.png"

with open(HDR) as f:
    text = f.read()

W, H = 128, 64
row_bytes = (W + 7) // 8

# 抓所有 frameN = { ... };
frames = {}
for m in re.finditer(r'frame(\d+)\[\] PROGMEM = \{([^}]*)\}', text):
    idx = int(m.group(1))
    bs = [int(x, 16) for x in re.findall(r'0x([0-9a-fA-F]{2})', m.group(2))]
    frames[idx] = bytes(bs)

n = len(frames)
print(f"parsed {n} frames")

from PIL import Image
# 拼接: 6 列 x 6 行
cols, rows = 6, (n + 5) // 6
sheet = Image.new('L', (cols * W, rows * H), 0)
for i in range(n):
    b = frames[i]
    img = Image.new('1', (W, H), 0)
    px = img.load()
    for y in range(H):
        for x in range(W):
            byte = b[y * row_bytes + x // 8]
            if byte & (1 << (x % 8)):
                px[x, y] = 1
    sheet.paste(img, ((i % cols) * W, (i // cols) * H))

sheet.save(OUT)
print(f"wrote {OUT}")
