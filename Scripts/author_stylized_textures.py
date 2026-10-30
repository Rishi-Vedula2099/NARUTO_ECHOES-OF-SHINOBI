"""
NARUTO: ECHOES OF SHINOBI
Pure Standard Python Texture Generator for Stylized Anime Art
Generates production-quality 24-bit/32-bit TGA textures with NO third-party dependencies.
"""

import os
import math

TEXTURE_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceTextures"
os.makedirs(TEXTURE_DIR, exist_ok=True)

def write_tga(filepath, width, height, pixels_rgb):
    """
    Writes uncompressed 24-bit TGA file.
    pixels_rgb: flat bytearray of size width * height * 3 (in R, G, B order).
    TGA stores in B, G, R order.
    """
    header = bytearray(18)
    header[2] = 2  # uncompressed true-color
    header[12] = width & 0xFF
    header[13] = (width >> 8) & 0xFF
    header[14] = height & 0xFF
    header[15] = (height >> 8) & 0xFF
    header[16] = 24 # 24 bits per pixel
    header[17] = 0x20 # top-to-bottom, left-to-right origin

    # Convert RGB to BGR in-place
    bgr_data = bytearray(len(pixels_rgb))
    for i in range(0, len(pixels_rgb), 3):
        r = pixels_rgb[i]
        g = pixels_rgb[i+1]
        b = pixels_rgb[i+2]
        bgr_data[i] = b
        bgr_data[i+1] = g
        bgr_data[i+2] = r

    with open(filepath, "wb") as f:
        f.write(header)
        f.write(bgr_data)
    print(f"Generated TGA: {filepath} ({width}x{height})")

def clamp(val, low=0, high=255):
    return max(low, min(high, int(val)))

def generate_cobblestone_texture():
    """Generates 1024x1024 anime sandstone cobblestones with sakura petals."""
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)

    num_rows = 16
    row_h = h // num_rows
    num_cols = 12
    col_w = w // num_cols

    base_r, base_g, base_b = 210, 198, 178 # Warm sandstone #D2C6B2
    mortar_r, mortar_g, mortar_b = 135, 125, 115 # Darker mortar groove

    for y in range(h):
        row = y // row_h
        y_in_row = y % row_h
        offset = (col_w // 2) if (row % 2 == 1) else 0

        for x in range(w):
            x_adj = (x + offset) % w
            col = x_adj // col_w
            x_in_col = x_adj % col_w

            border_dist_x = min(x_in_col, col_w - x_in_col)
            border_dist_y = min(y_in_row, row_h - y_in_row)
            min_dist = min(border_dist_x, border_dist_y)

            idx = (y * w + x) * 3

            if min_dist < 4:
                factor = min_dist / 4.0
                r = int(mortar_r + (base_r - mortar_r) * factor * 0.4)
                g = int(mortar_g + (base_g - mortar_g) * factor * 0.4)
                b = int(mortar_b + (base_b - mortar_b) * factor * 0.4)
            else:
                hash_val = (row * 37 + col * 73) % 29
                tone_offset = (hash_val - 14) * 2

                highlight = 0
                if y_in_row < 8 or x_in_col < 8:
                    highlight = 15
                elif y_in_row > row_h - 8 or x_in_col > col_w - 8:
                    highlight = -15

                r = clamp(base_r + tone_offset + highlight)
                g = clamp(base_g + tone_offset + highlight)
                b = clamp(base_b + tone_offset + highlight)

            buf[idx] = r
            buf[idx+1] = g
            buf[idx+2] = b

    # Scatter pink sakura petals
    petals = [
        (150, 200), (320, 140), (480, 290), (620, 180), (810, 240),
        (220, 480), (380, 560), (550, 420), (740, 510), (890, 440),
        (180, 750), (340, 820), (520, 710), (670, 850), (830, 780),
        (260, 920), (440, 950), (610, 980), (790, 910), (920, 860)
    ]
    petal_r, petal_g, petal_b = 248, 180, 204 # Sakura pink #F8B4CC

    for px, py in petals:
        for dy in range(-7, 8):
            for dx in range(-7, 8):
                if dx*dx + dy*dy <= 36:
                    nx, ny = (px + dx) % w, (py + dy) % h
                    idx = (ny * w + nx) * 3
                    buf[idx] = petal_r
                    buf[idx+1] = petal_g
                    buf[idx+2] = petal_b

    write_tga(os.path.join(TEXTURE_DIR, "T_Cobblestones_D.tga"), w, h, buf)

def generate_rooftiles_texture():
    """Generates 1024x1024 anime terracotta curved Japanese roof tiles."""
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)

    tile_w = 64
    tile_h = 128

    base_r, base_g, base_b = 210, 78, 38 # Terracotta vermilion #D24E26
    dark_r, dark_g, dark_b = 145, 45, 20 # Shadow edge
    hi_r, hi_g, hi_b = 238, 120, 70      # Cel highlight

    for y in range(h):
        row = y // tile_h
        y_in_tile = y % tile_h
        for x in range(w):
            x_in_tile = x % tile_w
            curve = math.sin((x_in_tile / tile_w) * math.pi)
            y_shadow = 1.0 if y_in_tile > 18 else (y_in_tile / 18.0)

            if curve < 0.2:
                r = dark_r
                g = dark_g
                b = dark_b
            elif curve > 0.85 and y_in_tile > 25:
                r = hi_r
                g = hi_g
                b = hi_b
            else:
                factor = curve * y_shadow
                r = clamp(dark_r + (base_r - dark_r) * factor)
                g = clamp(dark_g + (base_g - dark_g) * factor)
                b = clamp(dark_b + (base_b - dark_b) * factor)

            idx = (y * w + x) * 3
            buf[idx] = r
            buf[idx+1] = g
            buf[idx+2] = b

    write_tga(os.path.join(TEXTURE_DIR, "T_RoofTiles_D.tga"), w, h, buf)

def generate_timber_facade_texture():
    """Generates 1024x1024 Japanese cedar timber wall texture."""
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)

    plank_w = 64
    base_r, base_g, base_b = 152, 90, 45 # Japanese cedar #985A2D
    dark_r, dark_g, dark_b = 95, 50, 22

    for y in range(h):
        for x in range(w):
            col = x // plank_w
            x_in_plank = x % plank_w

            grain = math.sin(y * 0.05 + math.sin(x * 0.1) * 2.0) * 8.0
            col_var = ((col * 31) % 17) - 8

            if x_in_plank < 3 or x_in_plank > plank_w - 4:
                r = dark_r
                g = dark_g
                b = dark_b
            else:
                r = clamp(base_r + grain + col_var)
                g = clamp(base_g + (grain * 0.7) + (col_var * 0.7))
                b = clamp(base_b + (grain * 0.4) + (col_var * 0.4))

            idx = (y * w + x) * 3
            buf[idx] = r
            buf[idx+1] = g
            buf[idx+2] = b

    write_tga(os.path.join(TEXTURE_DIR, "T_Ichiraku_Facade_D.tga"), w, h, buf)

def generate_ichiraku_noren_sign_texture():
    """Generates 1024x1024 atlas containing Ichiraku sign board and blue noren curtains."""
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)

    sign_base_r, sign_base_g, sign_base_b = 230, 215, 185
    sign_frame_r, sign_frame_g, sign_frame_b = 65, 35, 18
    text_r, text_g, text_b = 25, 20, 20

    noren_r, noren_g, noren_b = 24, 48, 92
    noren_slit_r, noren_slit_g, noren_slit_b = 10, 18, 35
    white_r, white_g, white_b = 245, 245, 248

    for y in range(h):
        for x in range(w):
            idx = (y * w + x) * 3

            if y < 512:
                # SIGN BOARD
                if x < 32 or x > w - 33 or y < 32 or y > 512 - 33:
                    r, g, b = sign_frame_r, sign_frame_g, sign_frame_b
                elif x < 44 or x > w - 45 or y < 44 or y > 512 - 45:
                    r, g, b = sign_frame_r + 30, sign_frame_g + 20, sign_frame_b + 10
                else:
                    r, g, b = sign_base_r, sign_base_g, sign_base_b

                    # Draw stylized characters in center: 一 楽 ラ ー メ ン
                    cx = x - 80
                    cy = y - 220
                    char_idx = cx // 170
                    cx_local = cx % 170

                    if 0 <= char_idx < 5 and 0 <= cx_local < 140 and -100 <= cy <= 100:
                        if char_idx == 0: # 一
                            if -18 <= cy <= 18 and 10 <= cx_local <= 130:
                                r, g, b = text_r, text_g, text_b
                        elif char_idx == 1: # 楽
                            if (-70 <= cy <= -40 and 20 <= cx_local <= 120) or \
                               (cy > -20 and 30 <= cx_local <= 110 and abs(cx_local - 70) < 18) or \
                               (30 <= cy <= 75 and (cx_local < 45 or cx_local > 95)):
                                r, g, b = text_r, text_g, text_b
                        elif char_idx == 2: # ラ
                            if (-70 <= cy <= -45 and 20 <= cx_local <= 120) or \
                               (cy > -30 and (cx_local < 45 or 20 <= cy <= 50 and cx_local < 110)):
                                r, g, b = text_r, text_g, text_b
                        elif char_idx == 3: # ー
                            if -16 <= cy <= 16 and 15 <= cx_local <= 125:
                                r, g, b = text_r, text_g, text_b
                        elif char_idx == 4: # メン
                            if (abs(cx_local - 70) < 16 and -60 <= cy <= 60) or \
                               (-50 <= cy <= -20 and 20 <= cx_local <= 120):
                                r, g, b = text_r, text_g, text_b
            else:
                # NOREN CURTAINS (y 512..1023)
                noren_y = y - 512
                strip = x // 256
                x_in_strip = x % 256

                if x_in_strip < 8 or x_in_strip > 247:
                    r, g, b = noren_slit_r, noren_slit_g, noren_slit_b
                else:
                    fabric_fold = math.sin(x_in_strip * 0.08) * 12.0
                    r = clamp(noren_r + fabric_fold)
                    g = clamp(noren_g + fabric_fold)
                    b = clamp(noren_b + fabric_fold)

                    cy = noren_y - 256
                    cx_local = x_in_strip - 128
                    dist_center = math.sqrt(cx_local*cx_local + cy*cy)

                    if (strip == 0 or strip == 3) and 25 < dist_center < 65:
                        angle = math.atan2(cy, cx_local)
                        if math.sin(angle * 3.0 + dist_center * 0.1) > 0.1:
                            r, g, b = white_r, white_g, white_b
                    elif (strip == 1 or strip == 2) and -80 <= cy <= 80 and -50 <= cx_local <= 50:
                        if abs(cx_local) < 16 or abs(cy) < 16:
                            r, g, b = white_r, white_g, white_b

            buf[idx] = r
            buf[idx+1] = g
            buf[idx+2] = b

    write_tga(os.path.join(TEXTURE_DIR, "T_Ichiraku_Noren_D.tga"), w, h, buf)

def generate_naruto_texture():
    """Generates 1024x1024 texture for Naruto Genin with back crest and details."""
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)

    orange_r, orange_g, orange_b = 242, 92, 5
    navy_r, navy_g, navy_b = 18, 30, 52
    crest_red_r, crest_red_g, crest_red_b = 198, 28, 24
    white_r, white_g, white_b = 245, 245, 245
    skin_r, skin_g, skin_b = 252, 222, 202
    hair_r, hair_g, hair_b = 252, 210, 24
    metal_r, metal_g, metal_b = 195, 205, 218

    for y in range(h):
        for x in range(w):
            idx = (y * w + x) * 3

            if x < 512 and y < 512:
                # JACKET BACK with Uzumaki Crest
                r, g, b = orange_r, orange_g, orange_b
                if y < 120:
                    r, g, b = navy_r, navy_g, navy_b
                else:
                    dx = x - 256
                    dy = y - 316
                    dist = math.sqrt(dx*dx + dy*dy)

                    if 82 <= dist <= 96:
                        r, g, b = white_r, white_g, white_b
                    elif dist < 82:
                        angle = math.atan2(dy, dx)
                        spiral = math.sin(angle * 2.5 - dist * 0.18)
                        if spiral > 0.4:
                            r, g, b = crest_red_r - 40, 10, 10
                        else:
                            r, g, b = crest_red_r, crest_red_g, crest_red_b

            elif x >= 512 and y < 512:
                # JACKET FRONT
                r, g, b = orange_r, orange_g, orange_b
                local_x = x - 512
                if y < 140:
                    r, g, b = navy_r, navy_g, navy_b
                if 250 <= local_x <= 262:
                    r, g, b = white_r, white_g, white_b
                dx = local_x - 170
                dy = y - 220
                if dx*dx + dy*dy <= 700:
                    r, g, b = white_r, white_g, white_b

            elif x < 512 and y >= 512:
                # PANTS, BANDAGE & HOLSTER
                local_y = y - 512
                r, g, b = orange_r, orange_g, orange_b
                if 180 <= local_y <= 320 and 260 <= x <= 500:
                    stripe = (local_y // 18) % 2
                    if stripe == 0:
                        r, g, b = white_r, white_g, white_b
                    else:
                        r, g, b = white_r - 25, white_g - 25, white_b - 25
                if 220 <= local_y <= 300 and 380 <= x <= 460:
                    r, g, b = 25, 25, 30

            else:
                # HEADBAND, HAIR, SKIN, SANDALS
                local_x = x - 512
                local_y = y - 512

                if local_y < 160:
                    r, g, b = hair_r, hair_g, hair_b
                elif local_y < 280:
                    r, g, b = navy_r, navy_g, navy_b
                    if 130 <= local_x <= 382 and 180 <= local_y <= 260:
                        r, g, b = metal_r, metal_g, metal_b
                        dx = local_x - 256
                        dy = local_y - 220
                        d = math.sqrt(dx*dx + dy*dy)
                        if 10 <= d <= 26:
                            r, g, b = 40, 45, 55
                elif local_y < 380:
                    r, g, b = skin_r, skin_g, skin_b
                    if 310 <= local_y <= 350 and (130 <= local_x <= 200 or 312 <= local_x <= 382):
                        if (local_y % 14) < 3:
                            r, g, b = 160, 95, 75
                else:
                    r, g, b = navy_r, navy_g, navy_b

            buf[idx] = r
            buf[idx+1] = g
            buf[idx+2] = b

    write_tga(os.path.join(TEXTURE_DIR, "T_Naruto_Genin_D.tga"), w, h, buf)

if __name__ == "__main__":
    print("=== AUTHORING PRODUCTION ANIME TEXTURES ===")
    generate_cobblestone_texture()
    generate_rooftiles_texture()
    generate_timber_facade_texture()
    generate_ichiraku_noren_sign_texture()
    generate_naruto_texture()
    print("=== ALL TEXTURES GENERATED CLEANLY ===")
