import os
import struct
import math

def create_tga_image(width, height, fill_color=(250, 220, 192, 255)):
    # 2D buffer of (B, G, R, A)
    # top-down layout
    r, g, b, a = fill_color
    buffer = bytearray([b, g, r, a] * (width * height))
    return buffer

def set_pixel(buf, width, height, x, y, color):
    ix, iy = int(x), int(y)
    if 0 <= ix < width and 0 <= iy < height:
        offset = (iy * width + ix) * 4
        r, g, b, a = color
        if a >= 255:
            buf[offset:offset+4] = bytes([b, g, r, 255])
        elif a > 0:
            # Alpha blend
            alpha = a / 255.0
            old_b, old_g, old_r = buf[offset], buf[offset+1], buf[offset+2]
            nb = int(b * alpha + old_b * (1.0 - alpha))
            ng = int(g * alpha + old_g * (1.0 - alpha))
            nr = int(r * alpha + old_r * (1.0 - alpha))
            buf[offset:offset+4] = bytes([nb, ng, nr, 255])

def draw_filled_circle(buf, width, height, cx, cy, radius, color):
    r_sq = radius * radius
    min_x = max(0, int(cx - radius))
    max_x = min(width - 1, int(cx + radius + 1))
    min_y = max(0, int(cy - radius))
    max_y = min(height - 1, int(cy + radius + 1))
    for y in range(min_y, max_y):
        dy = y - cy
        dy_sq = dy * dy
        for x in range(min_x, max_x):
            dx = x - cx
            if dx * dx + dy_sq <= r_sq:
                set_pixel(buf, width, height, x, y, color)

def draw_filled_ellipse(buf, width, height, cx, cy, rx, ry, color):
    rx_sq = rx * rx
    ry_sq = ry * ry
    min_x = max(0, int(cx - rx))
    max_x = min(width - 1, int(cx + rx + 1))
    min_y = max(0, int(cy - ry))
    max_y = min(height - 1, int(cy + ry + 1))
    for y in range(min_y, max_y):
        dy = y - cy
        term_y = (dy * dy) / max(1e-4, ry_sq)
        for x in range(min_x, max_x):
            dx = x - cx
            if (dx * dx) / max(1e-4, rx_sq) + term_y <= 1.0:
                set_pixel(buf, width, height, x, y, color)

def draw_brush_stroke(buf, width, height, p0, p1, p2, start_r, mid_r, end_r, color, steps=60):
    for i in range(steps + 1):
        t = i / steps
        x = (1 - t)**2 * p0[0] + 2 * (1 - t) * t * p1[0] + t**2 * p2[0]
        y = (1 - t)**2 * p0[1] + 2 * (1 - t) * t * p1[1] + t**2 * p2[1]
        r = (1 - t) * start_r + t * end_r if t < 0.5 else mid_r * (1 - (t - 0.5) * 2) + end_r
        draw_filled_circle(buf, width, height, x, y, max(1.5, r), color)

def draw_line(buf, width, height, p0, p1, thickness, color):
    x0, y0 = p0
    x1, y1 = p1
    dist = math.hypot(x1 - x0, y1 - y0)
    steps = max(1, int(dist * 2))
    for i in range(steps + 1):
        t = i / steps
        x = x0 + t * (x1 - x0)
        y = y0 + t * (y1 - y0)
        draw_filled_circle(buf, width, height, x, y, thickness * 0.5, color)

def save_tga(buf, width, height, filepath):
    os.makedirs(os.path.dirname(filepath), exist_ok=True)
    header = struct.pack(
        "<BBBHHBHHHHBB",
        0, 0, 2, 0, 0, 0,
        0, 0, width, height, 32, 0x20 # 32-bit BGRA, top-down origin
    )
    with open(filepath, "wb") as f:
        f.write(header)
        f.write(buf)

def main():
    W, H = 2048, 2048
    buf = create_tga_image(W, H, fill_color=(250, 220, 192, 255))
    cx, cy = 1024, 1024

    # 1. Soft Warm Cheeks
    draw_filled_ellipse(buf, W, H, cx - 330, cy + 30, 220, 120, (242, 165, 145, 60))
    draw_filled_ellipse(buf, W, H, cx + 330, cy + 30, 220, 120, (242, 165, 145, 60))

    # 2. 3 Cheek Whiskers per side (Deep shinobi navy ink)
    whisker_c = (20, 25, 40, 255)
    # Left cheek (character's right)
    draw_brush_stroke(buf, W, H, (cx - 200, cy + 40), (cx - 360, cy + 20), (cx - 520, cy + 10), 5, 8, 2, whisker_c)
    draw_brush_stroke(buf, W, H, (cx - 210, cy + 120), (cx - 370, cy + 115), (cx - 530, cy + 120), 6, 9, 2, whisker_c)
    draw_brush_stroke(buf, W, H, (cx - 200, cy + 200), (cx - 360, cy + 215), (cx - 510, cy + 240), 5, 8, 1.5, whisker_c)

    # Right cheek (character's left)
    draw_brush_stroke(buf, W, H, (cx + 200, cy + 40), (cx + 360, cy + 20), (cx + 520, cy + 10), 5, 8, 2, whisker_c)
    draw_brush_stroke(buf, W, H, (cx + 210, cy + 120), (cx + 370, cy + 115), (cx + 530, cy + 120), 6, 9, 2, whisker_c)
    draw_brush_stroke(buf, W, H, (cx + 200, cy + 200), (cx + 360, cy + 215), (cx + 510, cy + 240), 5, 8, 1.5, whisker_c)

    # 3. Eyes (Sapphire/Cerulean with crisp catchlights)
    for side in (-1.0, 1.0):
        ecx = cx + side * 320
        ecy = cy - 140
        # White Sclera
        draw_filled_ellipse(buf, W, H, ecx, ecy, 160, 120, (255, 255, 255, 255))
        # Deep Blue Outer Iris
        draw_filled_circle(buf, W, H, ecx, ecy, 90, (15, 45, 95, 255))
        # Cerulean Blue Middle Iris
        draw_filled_circle(buf, W, H, ecx, ecy + 4, 75, (30, 135, 235, 255))
        # Cyan Glow at Bottom of Iris
        draw_filled_ellipse(buf, W, H, ecx, ecy + 25, 55, 38, (85, 205, 255, 255))
        # Black Pupil
        draw_filled_circle(buf, W, H, ecx, ecy + 2, 36, (10, 15, 25, 255))
        # Primary Specular Catchlight
        spec_x = ecx - 28 if side < 0 else ecx - 22
        spec_y = ecy - 28
        draw_filled_circle(buf, W, H, spec_x, spec_y, 20, (255, 255, 255, 255))
        # Secondary Tiny Catchlight
        draw_filled_circle(buf, W, H, ecx + 32, ecy + 28, 10, (255, 255, 255, 220))

        # Thick Upper Eyelash & Wing
        lash_c = (18, 22, 32, 255)
        p_in = (ecx - side * 150, ecy + 5)
        p_mid = (ecx, ecy - 130)
        p_out = (ecx + side * 175, ecy - 10)
        draw_line(buf, W, H, p_in, p_mid, 20, lash_c)
        draw_line(buf, W, H, p_mid, p_out, 22, lash_c)

        # Lower Eyelash
        p_lin = (ecx - side * 70, ecy + 115)
        p_lout = (ecx + side * 70, ecy + 115)
        draw_line(buf, W, H, p_lin, p_lout, 8, (35, 40, 55, 200))

        # Eyelid Crease
        draw_line(buf, W, H, (ecx - side * 90, ecy - 155), (ecx + side * 90, ecy - 150), 7, (195, 140, 115, 220))

        # Spiky Blonde Eyebrow
        brow_c = (245, 195, 25, 255)
        b_in = (ecx - side * 160, ecy - 195)
        b_peak = (ecx, ecy - 235)
        b_out = (ecx + side * 170, ecy - 210)
        draw_line(buf, W, H, b_in, b_peak, 22, brow_c)
        draw_line(buf, W, H, b_peak, b_out, 24, brow_c)

    # 4. Stylized Anime Nose
    draw_filled_circle(buf, W, H, cx, cy + 45, 10, (190, 135, 110, 255))
    draw_filled_circle(buf, W, H, cx + 4, cy + 42, 6, (140, 95, 75, 255))

    # 5. Smirk Mouth
    mouth_c = (55, 30, 30, 255)
    draw_line(buf, W, H, (cx - 120, cy + 270), (cx + 30, cy + 260), 12, mouth_c)
    draw_line(buf, W, H, (cx + 30, cy + 260), (cx + 120, cy + 245), 14, mouth_c) # Smirk upturn
    draw_filled_ellipse(buf, W, H, cx, cy + 305, 35, 14, (200, 135, 110, 180)) # Underlip shadow

    out_path = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceTextures\T_Naruto_Genin_D.tga"
    save_tga(buf, W, H, out_path)
    print(f"Pure Python TGA generated successfully: {out_path} ({os.path.getsize(out_path)} bytes)")

if __name__ == "__main__":
    main()
