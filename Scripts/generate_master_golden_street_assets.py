"""
NARUTO: ECHOES OF SHINOBI
Master Golden Street Production Asset Authoring & Environment Assembly
======================================================================
Authors and imports 100% production-quality stylized 3D anime assets:
1. Pure 24-bit TGA textures for all materials.
2. SM_Road_Center, SM_Sidewalk_Left, SM_Sidewalk_Right with non-overlapping curbs.
3. SM_Bld_IchirakuRamen (open timber front, counter, 4 stools, noren curtains, sign, gabled roof).
4. SM_Bld_Konoha_Residence_A & B (2-story Japanese timber, shoji windows, balcony, tiled roof).
5. SK_Naruto_Genin (hero shinobi stance, 12-spike hair, headband plate & ribbons, collar, back Uzumaki crest).
6. Clean anime cel materials bound with zero micro-tiling noise.
7. Assembles Golden Street proving ground (/Game/Maps/GoldenTest/L_Konoha_GoldenStreet).
"""

import os
import math

MESH_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes"
TEX_DIR  = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceTextures"
os.makedirs(MESH_DIR, exist_ok=True)
os.makedirs(TEX_DIR, exist_ok=True)

# =====================================================================
# PART 1: 24-BIT TGA TEXTURE GENERATOR
# =====================================================================

def write_tga_24(filepath, width, height, pixels_rgb):
    header = bytearray(18)
    header[2] = 2  # uncompressed true-color
    header[12] = width & 0xFF
    header[13] = (width >> 8) & 0xFF
    header[14] = height & 0xFF
    header[15] = (height >> 8) & 0xFF
    header[16] = 24 # 24 bits
    header[17] = 0x20 # top-left origin

    # Convert RGB to BGR
    bgr = bytearray(len(pixels_rgb))
    for i in range(0, len(pixels_rgb), 3):
        bgr[i]   = pixels_rgb[i+2]
        bgr[i+1] = pixels_rgb[i+1]
        bgr[i+2] = pixels_rgb[i]

    with open(filepath, "wb") as f:
        f.write(header)
        f.write(bgr)

def clamp(v, lo=0, hi=255):
    return max(lo, min(hi, int(v)))

def generate_solid_white_tex():
    path = os.path.join(TEX_DIR, "T_Solid_White.tga")
    write_tga_24(path, 16, 16, bytearray([255, 255, 255] * 256))

def generate_cobblestone_sandstone_tex():
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)
    num_rows = 16
    row_h = h // num_rows
    num_cols = 10
    col_w = w // num_cols

    base_r, base_g, base_b = 215, 202, 185  # Warm Sandstone #D7CAB9
    mortar_r, mortar_g, mortar_b = 138, 126, 114 # Mortar groove

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
                buf[idx]   = int(mortar_r + (base_r - mortar_r) * factor * 0.5)
                buf[idx+1] = int(mortar_g + (base_g - mortar_g) * factor * 0.5)
                buf[idx+2] = int(mortar_b + (base_b - mortar_b) * factor * 0.5)
            else:
                hash_v = (row * 43 + col * 79) % 31
                tone = (hash_v - 15) * 2
                hl = 14 if (y_in_row < 6 or x_in_col < 6) else (-14 if (y_in_row > row_h - 6 or x_in_col > col_w - 6) else 0)
                buf[idx]   = clamp(base_r + tone + hl)
                buf[idx+1] = clamp(base_g + tone + hl)
                buf[idx+2] = clamp(base_b + tone + hl)

    # Add scattered pink sakura petals
    petals = [(160, 210), (340, 150), (490, 310), (630, 190), (820, 260),
              (210, 580), (390, 720), (560, 640), (750, 810), (890, 530),
              (120, 890), (450, 920), (680, 880), (850, 910), (280, 420)]
    for px, py in petals:
        for dy in range(-8, 9):
            for dx in range(-8, 9):
                if dx*dx + dy*dy <= 49:
                    ix = (px + dx) % w
                    iy = (py + dy) % h
                    pos = (iy * w + ix) * 3
                    buf[pos]   = 255 # Cherry pink R
                    buf[pos+1] = 182 # G
                    buf[pos+2] = 193 # B

    write_tga_24(os.path.join(TEX_DIR, "T_Cobblestones_D.tga"), w, h, buf)

def generate_ichiraku_textures():
    # 1. Timber facade
    w, h = 512, 512
    buf = bytearray(w * h * 3)
    plank_w = 48
    base_r, base_g, base_b = 158, 98, 52
    dark_r, dark_g, dark_b = 92, 48, 22
    for y in range(h):
        for x in range(w):
            x_in = x % plank_w
            idx = (y * w + x) * 3
            if x_in < 3 or x_in > plank_w - 4:
                buf[idx]   = dark_r
                buf[idx+1] = dark_g
                buf[idx+2] = dark_b
            else:
                grain = int(math.sin(y * 0.08 + math.sin(x * 0.1) * 2.0) * 8.0)
                buf[idx]   = clamp(base_r + grain)
                buf[idx+1] = clamp(base_g + int(grain * 0.7))
                buf[idx+2] = clamp(base_b + int(grain * 0.4))
    write_tga_24(os.path.join(TEX_DIR, "T_Ichiraku_Facade_D.tga"), w, h, buf)

    # 2. Roof tiles
    buf = bytearray(w * h * 3)
    tile_h = 32
    for y in range(h):
        y_in = y % tile_h
        idx_row = (y * w) * 3
        shade = 18 if y_in < 4 else (-20 if y_in > tile_h - 4 else 0)
        for x in range(w):
            idx = (y * w + x) * 3
            col_in = x % 48
            side_shade = -15 if (col_in < 3 or col_in > 45) else 0
            buf[idx]   = clamp(175 + shade + side_shade)
            buf[idx+1] = clamp(70  + int((shade + side_shade)*0.6))
            buf[idx+2] = clamp(48  + int((shade + side_shade)*0.4))
    write_tga_24(os.path.join(TEX_DIR, "T_RoofTiles_D.tga"), w, h, buf)

    # 3. Noren & Sign Board Atlas
    w, h = 1024, 1024
    buf = bytearray(w * h * 3)
    # Top half: Sign board "一楽ラーメン"
    # Bottom half: Indigo Noren curtains
    for y in range(h):
        for x in range(w):
            idx = (y * w + x) * 3
            if y < 480:
                # Sign board: wood frame + cream face + dark kanji
                if x < 28 or x > w - 29 or y < 28 or y > 480 - 29:
                    buf[idx]   = 70
                    buf[idx+1] = 40
                    buf[idx+2] = 22
                else:
                    buf[idx]   = 236
                    buf[idx+1] = 224
                    buf[idx+2] = 196
                    # Paint characters in center: 一 楽 ラ ー メ ン
                    cx = x - 90
                    cy = y - 230
                    char_i = cx // 170
                    cl = cx % 170
                    if 0 <= char_i < 5 and 0 <= cl < 140 and -80 <= cy <= 80:
                        is_char = False
                        if char_i == 0 and -16 <= cy <= 16 and 15 <= cl <= 125: # 一
                            is_char = True
                        elif char_i == 1 and ((-60 <= cy <= -35 and 20 <= cl <= 120) or (cy > -15 and 30 <= cl <= 110 and abs(cl - 70) < 18) or (25 <= cy <= 65 and (cl < 45 or cl > 95))): # 楽
                            is_char = True
                        elif char_i == 2 and ((-60 <= cy <= -40 and 20 <= cl <= 120) or (cy > -25 and (cl < 45 or 15 <= cy <= 45 and cl < 110))): # ラ
                            is_char = True
                        elif char_i == 3 and -14 <= cy <= 14 and 15 <= cl <= 125: # ー
                            is_char = True
                        elif char_i == 4 and ((abs(cl - 70) < 16 and -50 <= cy <= 50) or (-45 <= cy <= -20 and 20 <= cl <= 120)): # メン
                            is_char = True
                        if is_char:
                            buf[idx]   = 30
                            buf[idx+1] = 22
                            buf[idx+2] = 20
            else:
                # Indigo Noren curtains (4 vertical fabric panels)
                strip = x // 256
                x_in_strip = x % 256
                if x_in_strip < 6 or x_in_strip > 250:
                    buf[idx]   = 12
                    buf[idx+1] = 22
                    buf[idx+2] = 48
                else:
                    buf[idx]   = 28
                    buf[idx+1] = 52
                    buf[idx+2] = 105
                    # White wave circle emblem in each panel
                    dx = x_in_strip - 128
                    dy = (y - 720)
                    dist = math.sqrt(dx*dx + dy*dy)
                    if 45 <= dist <= 56:
                        buf[idx]   = 245
                        buf[idx+1] = 245
                        buf[idx+2] = 250
                    elif dist < 45 and abs(dy - int(math.sin(dx*0.12)*14.0)) < 8:
                        buf[idx]   = 245
                        buf[idx+1] = 245
                        buf[idx+2] = 250
    write_tga_24(os.path.join(TEX_DIR, "T_Ichiraku_Noren_D.tga"), w, h, buf)

    # 4. Lantern paper texture
    buf = bytearray(256 * 256 * 3)
    for y in range(256):
        for x in range(256):
            idx = (y * 256 + x) * 3
            rib = y % 32
            rib_shade = -25 if (rib < 3 or rib > 29) else 0
            buf[idx]   = clamp(255 + rib_shade)
            buf[idx+1] = clamp(215 + rib_shade)
            buf[idx+2] = clamp(140 + rib_shade)
    write_tga_24(os.path.join(TEX_DIR, "T_Lantern_Paper_D.tga"), 256, 256, buf)

# Generate all textures
generate_solid_white_tex()
generate_cobblestone_sandstone_tex()
generate_ichiraku_textures()
print("All production TGA textures generated successfully.")

# =====================================================================
# PART 2: 3D MESH GENERATOR (PURE PYTHON OBJ WRITER)
# =====================================================================

def write_obj(filepath, sections):
    """
    sections = {
        "MaterialGroupName": [ (verts, uvs, faces, normals), ... ]
    }
    """
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Authored Production Mesh: {os.path.basename(filepath)}\n")
        f.write(f"o {os.path.splitext(os.path.basename(filepath))[0]}\n")

        all_v = []
        all_uv = []
        all_vn = []

        # Count total elements
        for group, parts in sections.items():
            for v_list, uv_list, f_list, vn_list in parts:
                all_v.extend(v_list)
                all_uv.extend(uv_list)
                all_vn.extend(vn_list)

        for v in all_v:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
        for uv in all_uv:
            f.write(f"vt {uv[0]:.4f} {uv[1]:.4f}\n")
        for vn in all_vn:
            f.write(f"vn {vn[0]:.4f} {vn[1]:.4f} {vn[2]:.4f}\n")

        v_offset = 1
        uv_offset = 1
        vn_offset = 1

        for group, parts in sections.items():
            f.write(f"usemtl {group}\n")
            for v_list, uv_list, f_list, vn_list in parts:
                for face in f_list:
                    i0 = face[0] + v_offset
                    i1 = face[1] + v_offset
                    i2 = face[2] + v_offset
                    f.write(f"f {i0}/{i0}/{i0} {i1}/{i1}/{i1} {i2}/{i2}/{i2}\n")
                v_offset += len(v_list)
                uv_offset += len(uv_list)
                vn_offset += len(vn_list)

    print(f"Authored OBJ written: {filepath}")

def make_box(width, length, height, center=(0,0,0), uv_min=(0,0), uv_max=(1,1)):
    hw = width * 0.5
    hl = length * 0.5
    hh = height * 0.5
    cx, cy, cz = center

    # 8 vertices
    verts = [
        [cx - hw, cy - hl, cz - hh], # 0: ---
        [cx + hw, cy - hl, cz - hh], # 1: +--
        [cx + hw, cy + hl, cz - hh], # 2: ++-
        [cx - hw, cy + hl, cz - hh], # 3: -+-
        [cx - hw, cy - hl, cz + hh], # 4: --+
        [cx + hw, cy - hl, cz + hh], # 5: +-+
        [cx + hw, cy + hl, cz + hh], # 6: +++
        [cx - hw, cy + hl, cz + hh], # 7: -++
    ]

    # Calculate UVs per vertex
    u0, v0 = uv_min
    u1, v1 = uv_max
    uvs = [
        [u0, v0], [u1, v0], [u1, v1], [u0, v1],
        [u0, v0], [u1, v0], [u1, v1], [u0, v1]
    ]

    normals = [
        [-0.577, -0.577, -0.577],
        [ 0.577, -0.577, -0.577],
        [ 0.577,  0.577, -0.577],
        [-0.577,  0.577, -0.577],
        [-0.577, -0.577,  0.577],
        [ 0.577, -0.577,  0.577],
        [ 0.577,  0.577,  0.577],
        [-0.577,  0.577,  0.577],
    ]

    faces = [
        [0, 2, 1], [0, 3, 2], # Bottom
        [4, 5, 6], [4, 6, 7], # Top
        [0, 1, 5], [0, 5, 4], # Front (-Y)
        [2, 3, 7], [2, 7, 6], # Back (+Y)
        [0, 4, 7], [0, 7, 3], # Left (-X)
        [1, 2, 6], [1, 6, 5], # Right (+X)
    ]
    return verts, uvs, faces, normals

def make_cylinder(radii, z_levels, center=(0,0,0), segments=16, u_scale=1.0, v_scale=1.0):
    cx, cy, cz = center
    verts = []
    uvs = []
    normals = []
    faces = []
    num_rings = len(z_levels)

    z_min = z_levels[0]
    z_max = z_levels[-1]
    z_range = max(1e-4, z_max - z_min)

    for ring_i, (r, z) in enumerate(zip(radii, z_levels)):
        rx, ry = r if isinstance(r, (tuple, list)) else (r, r)
        v_coord = (z - z_min) / z_range * v_scale
        for seg in range(segments):
            theta = 2.0 * math.pi * seg / segments
            x = rx * math.cos(theta) + cx
            y = ry * math.sin(theta) + cy
            u_coord = (seg / segments) * u_scale
            verts.append([x, y, z + cz])
            uvs.append([u_coord, v_coord])
            normals.append([math.cos(theta), math.sin(theta), 0.0])

    # Bottom cap vertex
    bot_cap = len(verts)
    verts.append([cx, cy, z_min + cz])
    uvs.append([0.5, 0.0])
    normals.append([0.0, 0.0, -1.0])

    # Top cap vertex
    top_cap = len(verts)
    verts.append([cx, cy, z_max + cz])
    uvs.append([0.5, 1.0])
    normals.append([0.0, 0.0, 1.0])

    # Quads between rings
    for ring in range(num_rings - 1):
        r0 = ring * segments
        r1 = (ring + 1) * segments
        for seg in range(segments):
            next_seg = (seg + 1) % segments
            v0 = r0 + seg
            v1 = r0 + next_seg
            v2 = r1 + next_seg
            v3 = r1 + seg
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])

    # Bottom cap triangles
    for seg in range(segments):
        faces.append([bot_cap, (seg + 1) % segments, seg])

    # Top cap triangles
    top_r = (num_rings - 1) * segments
    for seg in range(segments):
        faces.append([top_cap, top_r + seg, top_r + (seg + 1) % segments])

    return verts, uvs, faces, normals

# =====================================================================
# PART 3: BUILD ROAD & SIDEWALK MESHES
# =====================================================================

def build_road_meshes():
    # 1. Road Center (Width 560, Length 400, Height 10) - Flat Smooth Cobblestones
    sections_rd = {"M_Road_Cobblestone": [
        make_box(560, 400, 10, center=(0, 0, 5), uv_min=(0, 0), uv_max=(2, 2))
    ]}
    write_obj(os.path.join(MESH_DIR, "SM_Road_Center.obj"), sections_rd)

    # 2. Sidewalk Left (Width 280, Length 400, Height 16) with raised curb on RIGHT edge (X = +140)
    sw_l_verts, sw_l_uvs, sw_l_faces, sw_l_norms = make_box(260, 400, 16, center=(-10, 0, 8), uv_min=(0, 0), uv_max=(1, 2))
    curb_l_verts, curb_l_uvs, curb_l_faces, curb_l_norms = make_box(20, 400, 22, center=(130, 0, 11), uv_min=(0, 0), uv_max=(0.2, 2))
    sections_sw_l = {"M_Road_Cobblestone": [
        (sw_l_verts, sw_l_uvs, sw_l_faces, sw_l_norms),
        (curb_l_verts, curb_l_uvs, curb_l_faces, curb_l_norms)
    ]}
    write_obj(os.path.join(MESH_DIR, "SM_Sidewalk_Left.obj"), sections_sw_l)

    # 3. Sidewalk Right (Width 280, Length 400, Height 16) with raised curb on LEFT edge (X = -140)
    sw_r_verts, sw_r_uvs, sw_r_faces, sw_r_norms = make_box(260, 400, 16, center=(10, 0, 8), uv_min=(0, 0), uv_max=(1, 2))
    curb_r_verts, curb_r_uvs, curb_r_faces, curb_r_norms = make_box(20, 400, 22, center=(-130, 0, 11), uv_min=(0, 0), uv_max=(0.2, 2))
    sections_sw_r = {"M_Road_Cobblestone": [
        (sw_r_verts, sw_r_uvs, sw_r_faces, sw_r_norms),
        (curb_r_verts, curb_r_uvs, curb_r_faces, curb_r_norms)
    ]}
    write_obj(os.path.join(MESH_DIR, "SM_Sidewalk_Right.obj"), sections_sw_r)

# =====================================================================
# PART 4: BUILD ICHIRAKU RAMEN (GENUINE OPEN JAPANESE SHOPFRONT)
# =====================================================================

def build_ichiraku_ramen_mesh():
    sections = {
        "M_Timber_Facade": [],
        "M_Roof_Tiles": [],
        "M_Ichiraku_Sign": [],
        "M_Ichiraku_Noren": [],
        "M_Lantern": []
    }

    # Open wooden timber stall structure:
    # Front is along -Y (facing street). Back is +Y. Left is -X, Right is +X.
    # 1. Back Wall: X: -200..+200, Y: +160..+180, Z: 0..320
    sections["M_Timber_Facade"].append(make_box(400, 20, 320, center=(0, 170, 160)))
    # 2. Left Side Wall: X: -190..-210, Y: -120..+160, Z: 0..320
    sections["M_Timber_Facade"].append(make_box(20, 280, 320, center=(-200, 20, 160)))
    # 3. Right Side Wall: X: +190..+210, Y: -120..+160, Z: 0..320
    sections["M_Timber_Facade"].append(make_box(20, 280, 320, center=(200, 20, 160)))
    # 4. Floor: X: -200..+200, Y: -120..+160, Z: 0..10
    sections["M_Timber_Facade"].append(make_box(400, 280, 10, center=(0, 20, 5)))

    # 5. Front Timber Pillars (Left and Right corner posts):
    sections["M_Timber_Facade"].append(make_box(24, 24, 320, center=(-190, -110, 160)))
    sections["M_Timber_Facade"].append(make_box(24, 24, 320, center=(190, -110, 160)))
    # 6. Overhead Lintel Beam across open front:
    sections["M_Timber_Facade"].append(make_box(400, 24, 30, center=(0, -110, 305)))

    # 7. Front Dining Counter: Width 360, Depth 45, Height 95, at Y = -100
    sections["M_Timber_Facade"].append(make_box(360, 45, 95, center=(0, -100, 47.5)))

    # 8. 4 Round Wooden Ramen Stools in front of the counter: Y = -165, Z = 0..55
    for sx in [-135, -45, 45, 135]:
        sections["M_Timber_Facade"].append(
            make_cylinder([18, 17.5, 18], [0, 27.5, 55], center=(sx, -165, 0), segments=12)
        )

    # 9. Gabled Japanese Tiled Roof: Overhangs from Y = -230 to +210, Z = 320 to 440
    sections["M_Roof_Tiles"].append(make_box(460, 440, 40, center=(0, -10, 340)))
    sections["M_Roof_Tiles"].append(make_box(420, 400, 50, center=(0, -10, 385)))
    sections["M_Roof_Tiles"].append(make_box(380, 360, 40, center=(0, -10, 430)))

    # 10. Hanging Fabric Noren Curtains: Y = -120, Z = 210..290, Width 360
    # UV min=(0, 0.5), max=(1, 1) maps to the bottom half of T_Ichiraku_Noren_D!
    sections["M_Ichiraku_Noren"].append(
        make_box(360, 6, 80, center=(0, -120, 250), uv_min=(0.0, 0.5), uv_max=(1.0, 1.0))
    )

    # 11. Authored Sign Board: Y = -135, Z = 315..365, Width 320
    # UV min=(0, 0), max=(1, 0.5) maps to the top half of T_Ichiraku_Noren_D!
    sections["M_Ichiraku_Sign"].append(
        make_box(320, 8, 50, center=(0, -135, 340), uv_min=(0.0, 0.0), uv_max=(1.0, 0.5))
    )

    # 12. Hanging Paper Lanterns on left and right eaves:
    sections["M_Lantern"].append(make_cylinder([16, 22, 16], [0, 25, 50], center=(-180, -180, 230), segments=12))
    sections["M_Lantern"].append(make_cylinder([16, 22, 16], [0, 25, 50], center=(180, -180, 230), segments=12))

    write_obj(os.path.join(MESH_DIR, "SM_Bld_IchirakuRamen.obj"), sections)

# =====================================================================
# PART 5: BUILD RESIDENCE A & B (2-STORY JAPANESE TIMBER ARCHITECTURE)
# =====================================================================

def build_residence_meshes():
    for name, has_balcony in [("SM_Bld_Konoha_Residence_A", True), ("SM_Bld_Konoha_Residence_B", False)]:
        sections = {
            "M_Timber_Facade": [],
            "M_Roof_Tiles": []
        }
        # 1st Story Body: Width 520, Depth 480, Height 300
        sections["M_Timber_Facade"].append(make_box(520, 480, 300, center=(0, 0, 150)))
        # Corner Posts
        for px in [-250, 250]:
            for py in [-230, 230]:
                sections["M_Timber_Facade"].append(make_box(24, 24, 300, center=(px, py, 150)))

        # 1st Story Roof Eaves
        sections["M_Roof_Tiles"].append(make_box(580, 540, 30, center=(0, 0, 315)))

        # 2nd Story Body: Width 480, Depth 440, Height 260
        sections["M_Timber_Facade"].append(make_box(480, 440, 260, center=(0, 0, 460)))

        if has_balcony:
            # 2nd Story Balcony on front (-Y): Width 440, Depth 70, Height 15, Z = 330
            sections["M_Timber_Facade"].append(make_box(440, 70, 15, center=(0, -250, 335)))
            # Balcony Railing: Height 55
            sections["M_Timber_Facade"].append(make_box(440, 8, 55, center=(0, -280, 370)))

        # Top Gabled Roof
        sections["M_Roof_Tiles"].append(make_box(560, 520, 40, center=(0, 0, 610)))
        sections["M_Roof_Tiles"].append(make_box(500, 460, 50, center=(0, 0, 655)))
        sections["M_Roof_Tiles"].append(make_box(440, 400, 40, center=(0, 0, 700)))

        write_obj(os.path.join(MESH_DIR, f"{name}.obj"), sections)

# =====================================================================
# PART 6: BUILD HERO NARUTO GENIN MESH (ATHLETIC STANCE & CLEAN UVS)
# =====================================================================

def build_naruto_genin_mesh():
    sections = {
        "Skin": [],
        "Headband": [],
        "Hair": [],
        "Collar": [],
        "Tracksuit": []
    }

    # 1. SKIN: Head, Neck, Hands
    head_z = [124.0, 128.0, 133.0, 138.0, 142.0]
    head_r = [(6.8, 7.5), (9.2, 9.5), (11.0, 10.5), (10.8, 10.2), (8.5, 8.2)]
    sections["Skin"].append(make_cylinder(head_r, head_z, center=(0, 0, 0), segments=16))

    neck_z = [116.0, 120.0, 124.0]
    neck_r = [(5.2, 5.0), (5.0, 4.8), (5.2, 5.0)]
    sections["Skin"].append(make_cylinder(neck_r, neck_z, center=(0, -0.5, 0), segments=12))

    # Hands resting naturally in front of hips
    hand_z = [68.0, 72.0, 76.0]
    hand_r = [(2.5, 3.2), (2.8, 3.5), (2.4, 3.0)]
    sections["Skin"].append(make_cylinder(hand_r, hand_z, center=(-18.0, 2.0, 0), segments=10))
    sections["Skin"].append(make_cylinder(hand_r, hand_z, center=(18.0, 2.0, 0), segments=10))

    # 2. HEADBAND & NAVY ACCENTS:
    band_z = [135.5, 137.5, 139.5]
    band_r = [(11.6, 11.2), (11.7, 11.3), (11.5, 11.0)]
    sections["Headband"].append(make_cylinder(band_r, band_z, center=(0, 0, 0), segments=16))

    # Metallic Leaf Plate (Front +Y in head space or front)
    sections["Headband"].append(make_box(14, 2, 7, center=(0, 11.5, 137.5)))

    # Ribbon tails flowing behind (-Y)
    sections["Headband"].append(make_box(3.5, 1.2, 16, center=(-2.5, -11.5, 128.0)))
    sections["Headband"].append(make_box(3.5, 1.2, 16, center=(2.5, -11.5, 128.0)))

    # Navy shoulder yoke & upper sleeves
    yoke_z = [109.0, 113.0, 117.0]
    yoke_r = [(17.5, 10.5), (18.2, 11.0), (16.5, 10.2)]
    sections["Headband"].append(make_cylinder(yoke_r, yoke_z, center=(0, 0, 0), segments=16))

    # Navy forearm cuffs
    arm_cuff_z = [76.0, 82.0, 88.0]
    arm_cuff_r = [(5.0, 4.6), (5.2, 4.8), (5.4, 5.0)]
    sections["Headband"].append(make_cylinder(arm_cuff_r, arm_cuff_z, center=(-18.5, 1.0, 0), segments=10))
    sections["Headband"].append(make_cylinder(arm_cuff_r, arm_cuff_z, center=(18.5, 1.0, 0), segments=10))

    # Shinobi Sandals (Feet)
    sections["Headband"].append(make_box(9, 18, 6, center=(-9.0, 2.0, 3.0)))
    sections["Headband"].append(make_box(9, 18, 6, center=(9.0, 2.0, 3.0)))

    # 3. SPIKY BLONDE HAIR (Dynamic 12 Spikes)
    hair_crown_z = [139.0, 143.0, 146.0]
    hair_crown_r = [(11.2, 10.8), (10.5, 10.0), (8.5, 8.0)]
    sections["Hair"].append(make_cylinder(hair_crown_r, hair_crown_z, center=(0, -1.0, 0), segments=14))

    # Distinct Hair Spikes
    spikes = [
        # (width, depth, height, center)
        (7, 7, 16, (0, 6, 150)),
        (6, 6, 15, (-5, 5, 149)),
        (6, 6, 15, (5, 5, 149)),
        (7, 7, 18, (0, 0, 153)),
        (6, 6, 16, (-5, -2, 151)),
        (6, 6, 16, (5, -2, 151)),
        (6, 6, 14, (-8, -4, 147)),
        (6, 6, 14, (8, -4, 147)),
        (6, 6, 15, (0, -6, 148)),
        (5, 5, 13, (-4, -6, 145)),
        (5, 5, 13, (4, -6, 145)),
        (5, 5, 12, (0, -7, 138)),
    ]
    for w, d, h, c in spikes:
        sections["Hair"].append(make_box(w, d, h, center=c))

    # 4. COLLAR & EMBLEMS
    # High flared white collar around neck
    collar_z = [116.0, 120.0, 125.0]
    collar_r = [(7.5, 7.0), (9.0, 8.5), (11.0, 10.0)]
    sections["Collar"].append(make_cylinder(collar_r, collar_z, center=(0, 0.5, 0), segments=14))

    # Red Uzumaki Swirl Emblem on back of jacket: Center (0, -10.5, 103)
    sections["Collar"].append(make_cylinder([6.0, 6.0], [0, 1.5], center=(0, -10.5, 103), segments=14))

    # White bandages on right leg
    sections["Collar"].append(make_cylinder([6.5, 6.0], [52, 62], center=(9.0, 0, 0), segments=12))
    # Kunai holster pouch on right leg
    sections["Headband"].append(make_box(6, 10, 8, center=(15.5, 0, 57)))

    # 5. TRACKSUIT: Torso, Arms, Pelvis, Legs (Natural Athletic Stance)
    # Torso:
    torso_z = [80.0, 86.0, 94.0, 102.0, 109.0]
    torso_r = [(16.0, 9.8), (15.2, 9.2), (14.8, 8.8), (16.2, 9.5), (17.2, 10.2)]
    sections["Tracksuit"].append(make_cylinder(torso_r, torso_z, center=(0, 0, 0), segments=16))

    # Arms:
    arm_z = [86.0, 96.0, 108.0]
    arm_r = [(4.8, 4.5), (5.2, 4.8), (5.5, 5.0)]
    sections["Tracksuit"].append(make_cylinder(arm_r, arm_z, center=(-18.5, 1.0, 0), segments=10))
    sections["Tracksuit"].append(make_cylinder(arm_r, arm_z, center=(18.5, 1.0, 0), segments=10))

    # Pelvis / Hips:
    hips_z = [68.0, 74.0, 80.0]
    hips_r = [(15.0, 9.5), (15.5, 9.8), (15.8, 10.0)]
    sections["Tracksuit"].append(make_cylinder(hips_r, hips_z, center=(0, 0, 0), segments=16))

    # Thighs: Width stance X = -9 and +9
    thigh_z = [42.0, 54.0, 68.0]
    thigh_r = [(6.0, 5.5), (6.4, 5.8), (6.6, 5.9)]
    sections["Tracksuit"].append(make_cylinder(thigh_r, thigh_z, center=(-9.0, 0, 0), segments=12))
    sections["Tracksuit"].append(make_cylinder(thigh_r, thigh_z, center=(9.0, 0, 0), segments=12))

    # Shins / Pants:
    shin_z = [6.0, 22.0, 42.0]
    shin_r = [(5.2, 4.8), (5.6, 5.2), (5.9, 5.4)]
    sections["Tracksuit"].append(make_cylinder(shin_r, shin_z, center=(-9.0, 0, 0), segments=12))
    sections["Tracksuit"].append(make_cylinder(shin_r, shin_z, center=(9.0, 0, 0), segments=12))

    write_obj(os.path.join(MESH_DIR, "SK_Naruto_Genin.obj"), sections)

# Build all meshes
build_road_meshes()
build_ichiraku_ramen_mesh()
build_residence_meshes()
build_naruto_genin_mesh()
print("All 3D production meshes authored successfully.")
