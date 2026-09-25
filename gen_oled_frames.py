#!/usr/bin/env python3
"""
把 31 帧 RGB565 (177x100) 转成 128x64 单色位图,
生成 C 头文件, 每帧 1024 字节 (128x64/8), 适合 SSD1306 直接 drawXBitmap.
"""
import os, re

SRC = "/workspace/.uploads/8bef64e6_睁眼_extracted/1"
OUT = "/workspace/睁眼动画_OLED/eye_frames.h"

W, H = 128, 64          # OLED 分辨率
SRC_W, SRC_H = 177, 100 # 原图

HEX_RE = re.compile(r"0x([0-9A-Fa-f]{4})")

def load_frame(path):
    with open(path) as f:
        return [int(m.group(1),16) for m in HEX_RE.finditer(f.read())]

def to_mono(src, sw, sh, dw, dh):
    """缩放 + 阈值二值化, 返回 dw*dh 的 0/1 list."""
    out = []
    # 用最邻近采样 + 2x2 平均, 简单抗锯齿
    sx = sw / dw
    sy = sh / dh
    for y in range(dh):
        for x in range(dw):
            # 采样原图对应区块的平均亮度
            x0 = int(x*sx); x1 = max(int((x+1)*sx), x0+1)
            y0 = int(y*sy); y1 = max(int((y+1)*sy), y0+1)
            rsum=gsum=bsum=n=0
            for yy in range(y0, min(y1, sh)):
                for xx in range(x0, min(x1, sw)):
                    v = src[yy*sw+xx]
                    r=((v>>11)&0x1F)<<3; g=((v>>5)&0x3F)<<2; b=(v&0x1F)<<3
                    rsum+=r; gsum+=g; bsum+=b; n+=1
            if n==0: n=1
            lum = (rsum*299 + gsum*587 + bsum*114) / (1000*n)
            out.append(1 if lum > 40 else 0)  # 阈值 40, 适合灰白色眼睛
    return out

def to_xbm_bytes(bits, w, h):
    """转 XBM 格式: 每字节 8 像素, LSB first, 行字节对齐到 8."""
    row_bytes = (w + 7) // 8
    buf = bytearray(row_bytes * h)
    for y in range(h):
        for x in range(w):
            if bits[y*w + x]:
                buf[y*row_bytes + x//8] |= (1 << (x % 8))
    return bytes(buf)

def main():
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    files = sorted(f for f in os.listdir(SRC) if f.endswith(".h"))
    with open(OUT, "w") as out:
        out.write("// 自动生成: 31 帧 128x64 单色 XBM, 来源 RGB565 177x100\n")
        out.write(f"#define EYE_W {W}\n#define EYE_H {H}\n#define EYE_FRAMES {len(files)}\n\n")
        for i, fn in enumerate(files):
            src = load_frame(os.path.join(SRC, fn))
            mono = to_mono(src, SRC_W, SRC_H, W, H)
            xbm = to_xbm_bytes(mono, W, H)
            out.write(f"// frame {i} <- {fn}\n")
            out.write(f"static const unsigned char frame{i}[] PROGMEM = {{\n  ")
            for j, b in enumerate(xbm):
                out.write(f"0x{b:02x},")
                if (j+1) % 16 == 0: out.write("\n  ")
            out.write("\n};\n\n")
        # 帧指针表
        out.write("const unsigned char* const eyeFrames[EYE_FRAMES] PROGMEM = {\n  ")
        for i in range(len(files)):
            out.write(f"frame{i}, ")
            if (i+1) % 8 == 0: out.write("\n  ")
        out.write("\n};\n")
    print(f"wrote {OUT}, {os.path.getsize(OUT)} bytes")

if __name__ == "__main__":
    main()
