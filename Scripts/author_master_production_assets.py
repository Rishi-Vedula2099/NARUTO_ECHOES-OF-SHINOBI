"""
NARUTO: ECHOES OF SHINOBI
Master Production Asset Factory (Architectural Detailing & World-Unit UV Tiling)
================================================================================
1. SK_Naruto_Genin.obj:
   - Athletic shinobi stance (feet spaced X = +/-13.5cm, elbows flared, ready hands)
   - 20-horn spiky blonde hair crown
   - Metallic Leaf plate with corner rivets, navy fabric band, and flowing ribbons
   - Stand-up padded white collar, navy shoulder yoke, cadmium orange jacket
   - Crimson Uzumaki back crest
   - Right thigh bandage wrap & kunai leather holster pouch
   - Shinobi sandals with sculpted open toes and ankle bindings
   - Slot order: Skin=0, Navy=1, Hair=2, Collar=3, Tracksuit=4, Emblem=5, Metal=6

2. SM_Bld_IchirakuRamen.obj:
   - Japanese timber post-and-beam construction with exposed structural framing
   - World-unit UV mapping for authentic cedar wood grain and terracotta roof tiles
   - Deep recessed dining counter (95cm) with 4 round wooden stools (58cm)
   - 4-panel indigo fabric noren curtains under eave
   - Calligraphy signboard ("一楽ラーメン") mounted on bracket struts
   - Hanging paper lanterns with amber glow
   - Stepped gabled roof with wave tile ridges and exposed rafter tails

3. SM_Bld_Konoha_Residence_A/B.obj:
   - 2-story Japanese timber residences with post-and-beam framing
   - Cantilevered 2nd-story wooden balcony with vertical turned balustrades
   - Recessed Shoji lattice windows and sliding screens
   - Stepped terracotta tiled gabled roofs with ridge caps

4. SM_Veg_KonohaCedar_Large.obj / SM_Veg_Cedar_Large.obj:
   - Slot 0: Wood (gnarled brown trunk)
   - Slot 1: Foliage (tiered pine green cloud pads)

5. SM_Veg_Sakura_Bloom.obj:
   - Slot 0: Wood (arching boughs)
   - Slot 1: Foliage (lush pink cherry blossom clouds)
"""

import os
import math

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
MESH_DIR    = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
os.makedirs(MESH_DIR, exist_ok=True)

def write_obj_file(filepath, sections):
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Production Model: {os.path.basename(filepath)}\n")
        f.write(f"o {os.path.splitext(os.path.basename(filepath))[0]}\n")
        f.write("s 1\n")

        all_v = []
        all_uv = []
        all_vn = []

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
            if not parts:
                continue
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

    print(f"-> Production model exported: {filepath} ({len(all_v)} vertices, {sum(len(p[2]) for parts in sections.values() for p in parts)} faces)")

def normalize(v):
    l = math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])
    if l < 1e-6:
        return [0.0, 0.0, 1.0]
    return [v[0]/l, v[1]/l, v[2]/l]

def make_organic_loft(rings, segments=16, cap_bot=True, cap_top=True):
    verts = []
    uvs = []
    normals = []
    faces = []
    num_rings = len(rings)

    for ring_idx, (c, rx, ry, twist) in enumerate(rings):
        cx, cy, cz = c
        v_coord = ring_idx / max(1, num_rings - 1)
        for s in range(segments):
            theta = 2.0 * math.pi * s / segments + twist
            cos_t = math.cos(theta)
            sin_t = math.sin(theta)
            x = cx + rx * cos_t
            y = cy + ry * sin_t
            z = cz
            u_coord = s / segments
            verts.append([x, y, z])
            uvs.append([u_coord, v_coord])
            normals.append(normalize([cos_t / max(1e-3, rx), sin_t / max(1e-3, ry), 0.0]))

    for r in range(num_rings - 1):
        r0 = r * segments
        r1 = (r + 1) * segments
        for s in range(segments):
            s_next = (s + 1) % segments
            v0 = r0 + s
            v1 = r0 + s_next
            v2 = r1 + s_next
            v3 = r1 + s
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])

    if cap_bot:
        bot_c = len(verts)
        bc = rings[0][0]
        verts.append([bc[0], bc[1], bc[2]])
        uvs.append([0.5, 0.0])
        normals.append([0.0, 0.0, -1.0])
        for s in range(segments):
            faces.append([bot_c, (s + 1) % segments, s])

    if cap_top:
        top_c = len(verts)
        tc = rings[-1][0]
        verts.append([tc[0], tc[1], tc[2]])
        uvs.append([0.5, 1.0])
        normals.append([0.0, 0.0, 1.0])
        top_base = (num_rings - 1) * segments
        for s in range(segments):
            faces.append([top_c, top_base + s, top_base + (s + 1) % segments])

    return verts, uvs, faces, normals

def make_faceted_spike(base_center, tip, radius_x, radius_y, segments=6):
    verts = [list(tip)]
    uvs = [[0.5, 1.0]]
    normals = [normalize([tip[0] - base_center[0], tip[1] - base_center[1], tip[2] - base_center[2]])]
    faces = []

    mid_center = [
        base_center[0] * 0.45 + tip[0] * 0.55,
        base_center[1] * 0.45 + tip[1] * 0.55,
        base_center[2] * 0.45 + tip[2] * 0.55
    ]
    for s in range(segments):
        theta = 2.0 * math.pi * s / segments
        bx = base_center[0] + radius_x * math.cos(theta)
        by = base_center[1] + radius_y * math.sin(theta)
        bz = base_center[2]
        verts.append([bx, by, bz])
        uvs.append([s / segments, 0.0])
        normals.append(normalize([math.cos(theta), math.sin(theta), -0.2]))

    for s in range(segments):
        mx = mid_center[0] + radius_x * 0.75 * math.cos(theta)
        my = mid_center[1] + radius_y * 0.75 * math.sin(theta)
        mz = mid_center[2]
        verts.append([mx, my, mz])
        uvs.append([s / segments, 0.5])
        normals.append(normalize([math.cos(theta), math.sin(theta), 0.3]))

    for s in range(segments):
        s_next = (s + 1) % segments
        faces.append([0, 1 + segments + s, 1 + segments + s_next])

    for s in range(segments):
        s_next = (s + 1) % segments
        b0 = 1 + s
        b1 = 1 + s_next
        m0 = 1 + segments + s
        m1 = 1 + segments + s_next
        faces.append([b0, m0, m1])
        faces.append([b0, m1, b1])

    return verts, uvs, faces, normals

def make_curved_ribbon(points, width):
    verts = []
    uvs = []
    normals = []
    faces = []
    hw = width * 0.5

    for idx, p in enumerate(points):
        v_coord = idx / max(1, len(points) - 1)
        verts.append([p[0] - hw, p[1], p[2]])
        verts.append([p[0] + hw, p[1], p[2]])
        uvs.append([0.0, v_coord])
        uvs.append([1.0, v_coord])
        normals.append([0.0, 1.0, 0.0])
        normals.append([0.0, 1.0, 0.0])

    for idx in range(len(points) - 1):
        v0 = idx * 2
        v1 = v0 + 1
        v2 = v0 + 3
        v3 = v0 + 2
        faces.append([v0, v1, v2])
        faces.append([v0, v2, v3])
        faces.append([v0, v2, v1])
        faces.append([v0, v3, v2])

    return verts, uvs, faces, normals

def make_box(width, length, height, center=(0,0,0), uv_tile=200.0):
    """
    Creates a box with world-unit UV coordinates (repeats every uv_tile cm).
    """
    hw = width * 0.5
    hl = length * 0.5
    hh = height * 0.5
    cx, cy, cz = center

    verts = [
        [cx - hw, cy - hl, cz - hh], # 0
        [cx + hw, cy - hl, cz - hh], # 1
        [cx + hw, cy + hl, cz - hh], # 2
        [cx - hw, cy + hl, cz - hh], # 3
        [cx - hw, cy - hl, cz + hh], # 4
        [cx + hw, cy - hl, cz + hh], # 5
        [cx + hw, cy + hl, cz + hh], # 6
        [cx - hw, cy + hl, cz + hh], # 7
    ]

    u_scale = width / max(1e-3, uv_tile)
    v_scale = height / max(1e-3, uv_tile)
    uvs = [
        [0.0, 0.0], [u_scale, 0.0], [u_scale, v_scale], [0.0, v_scale],
        [0.0, 0.0], [u_scale, 0.0], [u_scale, v_scale], [0.0, v_scale],
    ]

    normals = [
        [-0.577, -0.577, -0.577], [ 0.577, -0.577, -0.577],
        [ 0.577,  0.577, -0.577], [-0.577,  0.577, -0.577],
        [-0.577, -0.577,  0.577], [ 0.577, -0.577,  0.577],
        [ 0.577,  0.577,  0.577], [-0.577,  0.577,  0.577],
    ]

    faces = [
        [0, 2, 1], [0, 3, 2],
        [4, 5, 6], [4, 6, 7],
        [0, 1, 5], [0, 5, 4],
        [2, 3, 7], [2, 7, 6],
        [0, 4, 7], [0, 7, 3],
        [1, 2, 6], [1, 6, 5],
    ]
    return verts, uvs, faces, normals

# -------------------------------------------------------------------------
# 1. AUTHORED HERO NARUTO GENIN MESH (Athletic Shinobi Proportions)
# -------------------------------------------------------------------------

def build_production_naruto_mesh():
    sections = {
        "Skin": [],
        "Navy": [],
        "Hair": [],
        "Collar": [],
        "Tracksuit": [],
        "Emblem": [],
        "Metal": []
    }

    # Head & Face (Skin)
    head_rings = [
        ((0.0, 0.0, 123.0), 5.5, 6.2, 0.0),   # Chin point
        ((0.0, 0.8, 126.5), 8.5, 8.8, 0.0),   # Jawline
        ((0.0, 1.2, 131.0), 10.8, 10.4, 0.0), # Cheeks
        ((0.0, 1.5, 136.0), 11.2, 10.6, 0.0), # Brow
        ((0.0, 0.6, 140.5), 10.5, 10.0, 0.0), # Forehead
        ((0.0, 0.0, 144.0), 8.5, 8.2, 0.0),   # Crown
    ]
    sections["Skin"].append(make_organic_loft(head_rings, segments=16, cap_bot=True, cap_top=True))

    neck_rings = [
        ((0.0, 0.0, 115.0), 4.8, 4.6, 0.0),
        ((0.0, 0.2, 119.0), 4.6, 4.4, 0.0),
        ((0.0, 0.4, 123.0), 5.0, 4.8, 0.0),
    ]
    sections["Skin"].append(make_organic_loft(neck_rings, segments=12, cap_bot=True, cap_top=False))

    for side in (-1.0, 1.0):
        ear_rings = [
            ((side * 10.5, 0.5, 131.0), 1.2, 0.8, 0.0),
            ((side * 11.8, 0.8, 133.5), 1.6, 1.2, 0.0),
            ((side * 11.2, 0.5, 136.0), 1.2, 0.9, 0.0),
        ]
        sections["Skin"].append(make_organic_loft(ear_rings, segments=8, cap_bot=True, cap_top=True))

    # Headband (Navy & Metal)
    hb_rings = [
        ((0.0, 0.5, 137.5), 11.4, 10.8, 0.0),
        ((0.0, 0.6, 141.2), 11.2, 10.6, 0.0),
    ]
    sections["Navy"].append(make_organic_loft(hb_rings, segments=20, cap_bot=False, cap_top=False))

    plate_verts = []
    plate_uvs = []
    plate_normals = []
    plate_faces = []
    for row in range(3):
        pz = 138.2 + row * 1.3
        for col in range(5):
            theta = (col - 2) * 0.18
            px = 11.5 * math.sin(theta)
            py = -11.5 * math.cos(theta) + 0.6
            plate_verts.append([px, py, pz])
            plate_uvs.append([col / 4.0, row / 2.0])
            plate_normals.append([math.sin(theta), -math.cos(theta), 0.1])
    for r in range(2):
        for c in range(4):
            i0 = r * 5 + c
            i1 = i0 + 1
            i2 = (r + 1) * 5 + c + 1
            i3 = (r + 1) * 5 + c
            plate_faces.append([i0, i1, i2])
            plate_faces.append([i0, i2, i3])
    sections["Metal"].append((plate_verts, plate_uvs, plate_faces, plate_normals))

    ribbon_left = [
        (-1.5, 10.6, 138.5),
        (-2.8, 14.5, 136.0),
        (-5.0, 19.5, 131.0),
        (-7.5, 25.0, 125.0),
        (-9.2, 30.5, 118.0),
    ]
    ribbon_right = [
        (1.5, 10.6, 138.5),
        (3.0, 15.0, 135.5),
        (5.5, 20.2, 130.0),
        (8.0, 26.0, 123.5),
        (10.0, 31.5, 116.0),
    ]
    sections["Navy"].append(make_curved_ribbon(ribbon_left, width=3.6))
    sections["Navy"].append(make_curved_ribbon(ribbon_right, width=3.6))

    # 20 Spiky Hair Horns (Hair)
    hair_spikes = [
        (( -6.5, -7.5, 141.0), ( -8.5, -13.0, 144.5), 3.4, 2.8),
        (( -2.8, -8.8, 142.5), ( -4.0, -15.0, 147.0), 3.6, 3.0),
        ((  0.0, -9.2, 143.0), (  0.0, -16.5, 148.5), 3.8, 3.2),
        ((  2.8, -8.8, 142.5), (  4.0, -15.0, 147.0), 3.6, 3.0),
        ((  6.5, -7.5, 141.0), (  8.5, -13.0, 144.5), 3.4, 2.8),
        ((-10.0, -2.5, 142.0), (-18.0,  -4.0, 147.0), 3.8, 3.2),
        (( 10.0, -2.5, 142.0), ( 18.0,  -4.0, 147.0), 3.8, 3.2),
        ((-10.5,  3.5, 141.5), (-18.5,   5.0, 146.0), 3.8, 3.2),
        (( 10.5,  3.5, 141.5), ( 18.5,   5.0, 146.0), 3.8, 3.2),
        (( -5.5, -2.0, 144.0), (-11.0,  -3.0, 156.5), 4.4, 3.8),
        ((  0.0, -2.5, 144.5), (  0.0,  -4.0, 159.0), 4.8, 4.0),
        ((  5.5, -2.0, 144.0), ( 11.0,  -3.0, 156.5), 4.4, 3.8),
        (( -4.0,  3.5, 144.0), ( -7.5,   6.5, 157.0), 4.4, 3.8),
        ((  4.0,  3.5, 144.0), (  7.5,   6.5, 157.0), 4.4, 3.8),
        (( -6.0,  7.0, 140.0), (-12.5,  13.5, 146.0), 4.0, 3.4),
        ((  0.0,  8.0, 140.5), (  0.0,  15.5, 148.0), 4.2, 3.6),
        ((  6.0,  7.0, 140.0), ( 12.5,  13.5, 146.0), 4.0, 3.4),
        (( -3.5,  7.5, 134.0), ( -5.5,  14.0, 137.0), 3.6, 3.0),
        ((  3.5,  7.5, 134.0), (  5.5,  14.0, 137.0), 3.6, 3.0),
        ((  0.0,  8.5, 133.0), (  0.0,  15.0, 134.0), 3.8, 3.2),
    ]
    for base_c, tip, rx, ry in hair_spikes:
        sections["Hair"].append(make_faceted_spike(base_c, tip, rx, ry, segments=6))

    # White Collar (Collar)
    collar_rings = [
        ((0.0, 0.2, 114.5), 7.2, 6.8, 0.0),
        ((0.0, 0.4, 118.5), 8.8, 8.2, 0.0),
        ((0.0, 0.6, 122.5), 10.4, 9.6, 0.0),
    ]
    sections["Collar"].append(make_organic_loft(collar_rings, segments=16, cap_bot=False, cap_top=False))

    # Torso (Navy Yoke & Orange Jacket)
    yoke_rings = [
        ((0.0, 0.0, 106.0), 17.5, 11.0, 0.0),
        ((0.0, 0.1, 111.0), 15.2,  9.8, 0.0),
        ((0.0, 0.2, 115.0), 11.2,  7.8, 0.0),
    ]
    sections["Navy"].append(make_organic_loft(yoke_rings, segments=16, cap_bot=False, cap_top=False))

    torso_rings = [
        ((0.0, 0.0,  76.0), 14.2,  9.2, 0.0),
        ((0.0, 0.1,  84.0), 14.8,  9.6, 0.0),
        ((0.0, 0.2,  94.0), 16.2, 10.4, 0.0),
        ((0.0, 0.1, 106.0), 17.5, 11.0, 0.0),
    ]
    sections["Tracksuit"].append(make_organic_loft(torso_rings, segments=16, cap_bot=True, cap_top=False))

    # Crimson Uzumaki Crest on Back
    swirl_verts = []
    swirl_uvs = []
    swirl_normals = []
    swirl_faces = []
    c_back = [0.0, 11.2, 100.0]
    swirl_verts.append(c_back)
    swirl_uvs.append([0.5, 0.5])
    swirl_normals.append([0.0, 1.0, 0.0])
    num_pts = 16
    for i in range(num_pts):
        th = 2.0 * math.pi * i / num_pts
        sx = 5.5 * math.cos(th)
        sz = 5.5 * math.sin(th)
        swirl_verts.append([sx, 11.3, 100.0 + sz])
        swirl_uvs.append([0.5 + 0.5 * math.cos(th), 0.5 + 0.5 * math.sin(th)])
        swirl_normals.append([0.0, 1.0, 0.0])
    for i in range(num_pts):
        i_next = (i + 1) % num_pts
        swirl_faces.append([0, 1 + i, 1 + i_next])
    sections["Emblem"].append((swirl_verts, swirl_uvs, swirl_faces, swirl_normals))

    # Arms (Tracksuit, Navy Cuffs, Skin Fists)
    for side in (-1.0, 1.0):
        arm_rings = [
            ((side * 18.5,  0.0, 104.0), 4.8, 4.6, 0.0),
            ((side * 22.0, -1.5,  92.0), 4.2, 4.0, 0.0),
            ((side * 23.5, -2.5,  80.0), 3.8, 3.6, 0.0),
        ]
        sections["Tracksuit"].append(make_organic_loft(arm_rings, segments=12, cap_bot=False, cap_top=True))

        cuff_rings = [
            ((side * 23.5, -2.5, 80.0), 4.2, 4.0, 0.0),
            ((side * 23.8, -3.2, 72.0), 3.6, 3.4, 0.0),
        ]
        sections["Navy"].append(make_organic_loft(cuff_rings, segments=12, cap_bot=False, cap_top=False))

        fist_rings = [
            ((side * 23.8, -3.2, 72.0), 3.2, 3.0, 0.0),
            ((side * 24.2, -4.0, 66.0), 3.8, 3.6, 0.0),
            ((side * 24.0, -4.5, 61.0), 2.8, 2.6, 0.0),
        ]
        sections["Skin"].append(make_organic_loft(fist_rings, segments=10, cap_bot=True, cap_top=False))

    # Legs & Stance (Spaced X = +/-13.5cm)
    for side in (-1.0, 1.0):
        leg_rings = [
            ((side * 10.0,  0.0, 76.0), 7.2, 6.8, 0.0),
            ((side * 12.0,  0.5, 54.0), 6.8, 6.2, 0.0),
            ((side * 13.5,  1.2, 36.0), 5.8, 5.4, 0.0),
            ((side * 13.5,  0.8, 18.0), 5.2, 4.8, 0.0),
            ((side * 13.5,  0.4, 10.0), 4.5, 4.2, 0.0),
        ]
        sections["Tracksuit"].append(make_organic_loft(leg_rings, segments=14, cap_bot=False, cap_top=True))

    sections["Collar"].append(make_box(14.0, 13.5, 12.0, center=(12.0, 0.5, 52.0), uv_tile=20.0))
    sections["Navy"].append(make_box(7.2, 4.5, 9.0, center=(19.0, 0.5, 52.0), uv_tile=20.0))

    # Sandals & Toes
    for side in (-1.0, 1.0):
        sole_rings = [
            ((side * 13.5, -1.0, 0.0), 5.2, 11.0, 0.0),
            ((side * 13.5, -0.8, 2.4), 5.0, 10.6, 0.0),
        ]
        sections["Navy"].append(make_organic_loft(sole_rings, segments=12, cap_bot=True, cap_top=True))

        ankle_wrap = [
            ((side * 13.5, 2.5,  2.4), 4.4, 5.0, 0.0),
            ((side * 13.5, 2.0, 10.0), 4.5, 4.5, 0.0),
        ]
        sections["Navy"].append(make_organic_loft(ankle_wrap, segments=10, cap_bot=False, cap_top=False))

        toes_rings = [
            ((side * 13.5, -7.0, 2.4), 4.6, 3.8, 0.0),
            ((side * 13.5, -9.8, 2.4), 4.2, 2.6, 0.0),
        ]
        sections["Skin"].append(make_organic_loft(toes_rings, segments=10, cap_bot=True, cap_top=True))

    return sections

# -------------------------------------------------------------------------
# 2. ICHIRAKU RAMEN (Japanese Timber Architecture with Authentic Tiling)
# -------------------------------------------------------------------------

def build_production_ichiraku_mesh():
    sections = {
        "Wood": [],
        "Roof": [],
        "Noren": [],
        "Sign": [],
        "Lantern": [],
        "Stone": []
    }

    # Stone Foundation
    sections["Stone"].append(make_box(680.0, 520.0, 24.0, center=(0.0, 0.0, 12.0), uv_tile=150.0))

    # 6 Cedar Pillars (24x24cm x 340cm)
    pillar_coords = [
        (-310.0, -230.0), (0.0, -230.0), (310.0, -230.0),
        (-310.0,  230.0), (0.0,  230.0), (310.0,  230.0),
    ]
    for px, py in pillar_coords:
        sections["Wood"].append(make_box(26.0, 26.0, 340.0, center=(px, py, 194.0), uv_tile=100.0))

    # Beams & Lintels
    sections["Wood"].append(make_box(650.0, 24.0, 28.0, center=(0.0, -230.0, 335.0), uv_tile=120.0))
    sections["Wood"].append(make_box(650.0, 24.0, 28.0, center=(0.0,  230.0, 335.0), uv_tile=120.0))
    sections["Wood"].append(make_box(24.0, 480.0, 28.0, center=(-310.0, 0.0, 335.0), uv_tile=120.0))
    sections["Wood"].append(make_box(24.0, 480.0, 28.0, center=( 310.0, 0.0, 335.0), uv_tile=120.0))

    # Wall Siding with Japanese post-and-beam studs
    sections["Wood"].append(make_box(620.0, 16.0, 310.0, center=(0.0, 222.0, 179.0), uv_tile=100.0))
    sections["Wood"].append(make_box(16.0, 440.0, 310.0, center=(-302.0, 0.0, 179.0), uv_tile=100.0))
    sections["Wood"].append(make_box(16.0, 440.0, 310.0, center=( 302.0, 0.0, 179.0), uv_tile=100.0))

    # Open Dining Counter (95cm height, 65cm depth, 580cm wide)
    sections["Wood"].append(make_box(580.0, 65.0, 12.0, center=(0.0, -145.0, 95.0), uv_tile=100.0))
    sections["Wood"].append(make_box(560.0, 18.0, 89.0, center=(0.0, -118.0, 44.5), uv_tile=100.0))

    # 4 Round Wooden Dining Stools
    for sx in (-195.0, -65.0, 65.0, 195.0):
        seat_rings = [
            ((sx, -195.0, 54.0), 19.0, 19.0, 0.0),
            ((sx, -195.0, 58.0), 19.0, 19.0, 0.0),
        ]
        sections["Wood"].append(make_organic_loft(seat_rings, segments=12, cap_bot=True, cap_top=True))
        leg_rings = [
            ((sx, -195.0, 24.0), 4.5, 4.5, 0.0),
            ((sx, -195.0, 54.0), 4.5, 4.5, 0.0),
        ]
        sections["Wood"].append(make_organic_loft(leg_rings, segments=8, cap_bot=True, cap_top=False))

    # Inner Kitchen & Ceramic Bowls
    sections["Wood"].append(make_box(540.0, 45.0, 10.0, center=(0.0, 120.0, 115.0), uv_tile=100.0))
    sections["Wood"].append(make_box(540.0, 35.0,  8.0, center=(0.0, 180.0, 210.0), uv_tile=100.0))
    for bx in (-120.0, -40.0, 40.0, 120.0):
        bowl_rings = [
            ((bx, 120.0, 120.0),  7.5,  7.5, 0.0),
            ((bx, 120.0, 132.0), 14.0, 14.0, 0.0),
        ]
        sections["Stone"].append(make_organic_loft(bowl_rings, segments=10, cap_bot=True, cap_top=False))

    # 4-Panel Fabric Noren
    bamboo_rings = [
        ((-285.0, -220.0, 290.0), 2.2, 2.2, 0.0),
        (( 285.0, -220.0, 290.0), 2.2, 2.2, 0.0),
    ]
    sections["Wood"].append(make_organic_loft(bamboo_rings, segments=8, cap_bot=True, cap_top=True))
    for i in range(4):
        cx = -198.0 + i * 132.0
        sections["Noren"].append(make_box(128.0, 3.0, 88.0, center=(cx, -220.0, 244.0), uv_tile=100.0))

    # Signboard & Lanterns
    sections["Wood"].append(make_box(290.0, 8.0, 72.0, center=(0.0, -252.0, 360.0), uv_tile=80.0))
    sections["Sign"].append(make_box(274.0, 3.0, 58.0, center=(0.0, -257.0, 360.0), uv_tile=80.0))

    for side in (-1.0, 1.0):
        lx = side * 280.0
        ly = -260.0
        lz = 295.0
        lantern_rings = [
            ((lx, ly, lz - 28.0),  6.0,  6.0, 0.0),
            ((lx, ly, lz - 14.0), 18.0, 18.0, 0.0),
            ((lx, ly, lz + 14.0), 18.0, 18.0, 0.0),
            ((lx, ly, lz + 28.0),  8.0,  8.0, 0.0),
        ]
        sections["Lantern"].append(make_organic_loft(lantern_rings, segments=12, cap_bot=True, cap_top=True))
        sections["Wood"].append(make_box(6.0, 45.0, 6.0, center=(lx, ly + 20.0, lz + 32.0), uv_tile=20.0))

    # Exposed Under-Eave Rafters (Taruki)
    for rx in range(-340, 350, 45):
        sections["Wood"].append(make_box(8.0, 590.0, 12.0, center=(rx, 0.0, 355.0), uv_tile=80.0))

    # Gabled Tiled Roof (Kawara Terracotta Wave Tiles)
    sections["Roof"].append(make_box(740.0, 340.0, 18.0, center=(0.0, -145.0, 420.0), uv_tile=80.0))
    sections["Roof"].append(make_box(740.0, 340.0, 18.0, center=(0.0,  145.0, 420.0), uv_tile=80.0))

    ridge_rings = [
        ((-380.0, 0.0, 502.0), 16.0, 16.0, 0.0),
        (( 380.0, 0.0, 502.0), 16.0, 16.0, 0.0),
    ]
    sections["Roof"].append(make_organic_loft(ridge_rings, segments=10, cap_bot=True, cap_top=True))
    sections["Wood"].append(make_box(18.0, 270.0, 110.0, center=(-330.0, 0.0, 430.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 270.0, 110.0, center=( 330.0, 0.0, 430.0), uv_tile=100.0))

    return sections

# -------------------------------------------------------------------------
# 3. MODULAR RESIDENCES (2-Story Timber Architecture)
# -------------------------------------------------------------------------

def build_production_residence_mesh(variant="A"):
    sections = {
        "Wood": [],
        "Roof": [],
        "Stone": []
    }
    sections["Stone"].append(make_box(560.0, 520.0, 30.0, center=(0.0, 0.0, 15.0), uv_tile=150.0))

    for px in (-260.0, 260.0):
        for py in (-240.0, 240.0):
            sections["Wood"].append(make_box(28.0, 28.0, 280.0, center=(px, py, 170.0), uv_tile=100.0))

    sections["Wood"].append(make_box(500.0, 18.0, 260.0, center=(0.0, -230.0, 160.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 460.0, 260.0, center=(-250.0, 0.0, 160.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 460.0, 260.0, center=( 250.0, 0.0, 160.0), uv_tile=100.0))
    sections["Wood"].append(make_box(500.0, 18.0, 260.0, center=(0.0,  230.0, 160.0), uv_tile=100.0))

    sections["Wood"].append(make_box(540.0, 500.0, 24.0, center=(0.0, 0.0, 302.0), uv_tile=120.0))
    sections["Wood"].append(make_box(480.0, 95.0, 14.0, center=(0.0, -280.0, 302.0), uv_tile=100.0))
    for bx in range(-230, 240, 30):
        sections["Wood"].append(make_box(6.0, 6.0, 65.0, center=(bx, -325.0, 342.0), uv_tile=50.0))
    sections["Wood"].append(make_box(480.0, 12.0, 8.0, center=(0.0, -325.0, 376.0), uv_tile=100.0))

    for px in (-260.0, 260.0):
        for py in (-240.0, 240.0):
            sections["Wood"].append(make_box(26.0, 26.0, 260.0, center=(px, py, 440.0), uv_tile=100.0))
    sections["Wood"].append(make_box(500.0, 18.0, 240.0, center=(0.0, -230.0, 435.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 460.0, 240.0, center=(-250.0, 0.0, 435.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 460.0, 240.0, center=( 250.0, 0.0, 435.0), uv_tile=100.0))
    sections["Wood"].append(make_box(500.0, 18.0, 240.0, center=(0.0,  230.0, 435.0), uv_tile=100.0))

    sections["Roof"].append(make_box(520.0, 120.0, 14.0, center=(0.0, -290.0, 385.0), uv_tile=80.0))
    sections["Roof"].append(make_box(620.0, 340.0, 22.0, center=(0.0, -150.0, 630.0), uv_tile=80.0))
    sections["Roof"].append(make_box(620.0, 340.0, 22.0, center=(0.0,  150.0, 630.0), uv_tile=80.0))

    ridge_rings = [
        ((-320.0, 0.0, 715.0), 18.0, 18.0, 0.0),
        (( 320.0, 0.0, 715.0), 18.0, 18.0, 0.0),
    ]
    sections["Roof"].append(make_organic_loft(ridge_rings, segments=10, cap_bot=True, cap_top=True))
    sections["Wood"].append(make_box(18.0, 260.0, 120.0, center=(-270.0, 0.0, 630.0), uv_tile=100.0))
    sections["Wood"].append(make_box(18.0, 260.0, 120.0, center=( 270.0, 0.0, 630.0), uv_tile=100.0))

    return sections

# -------------------------------------------------------------------------
# 4. FOLIAGE: Japanese Cedar & Sakura Cherry Blossom
# -------------------------------------------------------------------------

def build_production_cedar_mesh():
    sections = {
        "Wood": [],
        "Foliage": []
    }
    trunk_rings = [
        (( 0.0,  0.0,   0.0), 32.0, 32.0, 0.0),
        (( 4.0, -3.0, 200.0), 26.0, 24.0, 0.2),
        ((-5.0,  6.0, 420.0), 22.0, 20.0, 0.5),
        (( 3.0,  2.0, 650.0), 16.0, 15.0, 0.8),
        (( 0.0,  0.0, 880.0),  9.0,  9.0, 1.2),
    ]
    sections["Wood"].append(make_organic_loft(trunk_rings, segments=12, cap_bot=True, cap_top=True))

    c1_rings = [
        ((-15.0,  12.0, 380.0),  60.0,  50.0, 0.0),
        ((-15.0,  12.0, 440.0), 125.0, 110.0, 0.0),
        ((-15.0,  12.0, 500.0),  80.0,  70.0, 0.0),
    ]
    sections["Foliage"].append(make_organic_loft(c1_rings, segments=14, cap_bot=True, cap_top=True))

    c2_rings = [
        (( 20.0, -18.0, 510.0),  50.0,  45.0, 0.0),
        (( 20.0, -18.0, 570.0), 110.0,  95.0, 0.0),
        (( 20.0, -18.0, 630.0),  65.0,  55.0, 0.0),
    ]
    sections["Foliage"].append(make_organic_loft(c2_rings, segments=14, cap_bot=True, cap_top=True))

    c3_rings = [
        ((-10.0, -10.0, 630.0),  40.0,  35.0, 0.0),
        ((-10.0, -10.0, 690.0),  90.0,  80.0, 0.0),
        ((-10.0, -10.0, 750.0),  50.0,  45.0, 0.0),
    ]
    sections["Foliage"].append(make_organic_loft(c3_rings, segments=12, cap_bot=True, cap_top=True))

    c4_rings = [
        ((  0.0,   0.0, 740.0),  30.0,  28.0, 0.0),
        ((  0.0,   0.0, 840.0),  65.0,  60.0, 0.0),
        ((  0.0,   0.0, 940.0),  20.0,  18.0, 0.0),
    ]
    sections["Foliage"].append(make_organic_loft(c4_rings, segments=12, cap_bot=True, cap_top=True))

    return sections

def build_production_sakura_mesh():
    sections = {
        "Wood": [],
        "Foliage": []
    }
    trunk_rings = [
        ((  0.0,   0.0,   0.0), 28.0, 26.0, 0.0),
        (( 15.0, -12.0, 160.0), 22.0, 20.0, 0.2),
        (( 42.0, -35.0, 310.0), 18.0, 16.0, 0.4),
        (( 75.0, -65.0, 440.0), 14.0, 12.0, 0.6),
    ]
    sections["Wood"].append(make_organic_loft(trunk_rings, segments=12, cap_bot=True, cap_top=True))

    bough_centers = [
        ( 60.0,  -45.0, 420.0, 85.0),
        (110.0,  -85.0, 480.0, 95.0),
        ( 45.0, -110.0, 520.0, 80.0),
        (135.0,  -40.0, 550.0, 90.0),
        ( 85.0,  -75.0, 610.0, 75.0),
    ]
    for bx, by, bz, r in bough_centers:
        cloud_rings = [
            ((bx, by, bz - r * 0.45), r * 0.55, r * 0.50, 0.0),
            ((bx, by, bz),            r * 1.05, r * 0.95, 0.0),
            ((bx, by, bz + r * 0.45), r * 0.60, r * 0.55, 0.0),
        ]
        sections["Foliage"].append(make_organic_loft(cloud_rings, segments=12, cap_bot=True, cap_top=True))

    return sections

if __name__ == "__main__":
    print("=== EXPORTING PRODUCTION ASSETS WITH WORLD-UNIT UV TILING ===")
    naruto_sections = build_production_naruto_mesh()
    write_obj_file(os.path.join(MESH_DIR, "SK_Naruto_Genin.obj"), naruto_sections)
    write_obj_file(os.path.join(MESH_DIR, "SK_Naruto_GeninTracksuit.obj"), naruto_sections)

    ichiraku_sections = build_production_ichiraku_mesh()
    write_obj_file(os.path.join(MESH_DIR, "SM_Bld_IchirakuRamen.obj"), ichiraku_sections)

    res_a_sections = build_production_residence_mesh("A")
    write_obj_file(os.path.join(MESH_DIR, "SM_Bld_Konoha_Residence_A.obj"), res_a_sections)
    res_b_sections = build_production_residence_mesh("B")
    write_obj_file(os.path.join(MESH_DIR, "SM_Bld_Konoha_Residence_B.obj"), res_b_sections)

    cedar_sections = build_production_cedar_mesh()
    write_obj_file(os.path.join(MESH_DIR, "SM_Veg_KonohaCedar_Large.obj"), cedar_sections)
    write_obj_file(os.path.join(MESH_DIR, "SM_Veg_Cedar_Large.obj"), cedar_sections)

    sakura_sections = build_production_sakura_mesh()
    write_obj_file(os.path.join(MESH_DIR, "SM_Veg_Sakura_Bloom.obj"), sakura_sections)

    print("=== ALL PRODUCTION ASSETS EXPORTED SUCCESSFULLY ===")
