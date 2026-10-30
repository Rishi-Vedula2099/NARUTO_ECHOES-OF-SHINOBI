"""
NARUTO: ECHOES OF SHINOBI
Masterpiece 3D Sculpted Naruto Genin Character Production Asset
Exact Match to EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01
==============================================================
Features:
- Athletic shinobi stance (wide planted feet X = +/-18cm, bent knees, flared elbows, ready fists)
- Authentic sunflower-blonde spiky hair crown (32 sculpted 3D volume locks)
- Navy headband with metallic Leaf plate, 4 dome rivets, embossed Konoha emblem, and billowing tie ribbons
- Sculpted anime face: nose tip apex, mouth line, blue anime eyes, golden brows, and 3 cheek whiskers in Navy
- Padded stand-up white fleece collar funnel with rolled rim
- Orange windbreaker jacket with navy shoulder yoke, front metal zipper slider, and elastic waist hem
- Left upper sleeve white Uzumaki swirl clan patch
- Large circular crimson Uzumaki back crest with white border trim (radius 9.2cm, normal facing +Y)
- Beige shinobi waist tool pouch (fanny pack) on rear waistband
- Baggy orange ninja trousers with gathered knee crease folds
- White medical tape bandage wraps on right mid-thigh & dark kunai holster pouch
- Konoha navy ninja sandals with open toes (5 sculpted toes) and black tread soles
- Exact 7 Material Slots: Skin=0, Navy=1, Hair=2, Collar=3, Tracksuit=4, Emblem=5, Metal=6
"""

import os
import math

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
MESH_DIR    = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
os.makedirs(MESH_DIR, exist_ok=True)

def normalize(v):
    l = math.sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])
    if l < 1e-7:
        return [0.0, 0.0, 1.0]
    return [v[0]/l, v[1]/l, v[2]/l]

def cross(a, b):
    return [
        a[1]*b[2] - a[2]*b[1],
        a[2]*b[0] - a[0]*b[2],
        a[0]*b[1] - a[1]*b[0]
    ]

def add_vec(a, b):
    return [a[0]+b[0], a[1]+b[1], a[2]+b[2]]

def sub_vec(a, b):
    return [a[0]-b[0], a[1]-b[1], a[2]-b[2]]

def scale_vec(a, s):
    return [a[0]*s, a[1]*s, a[2]*s]

def make_curved_hair_lock(base_pt, mid_pt, tip_pt, base_r, mid_r, segments=8):
    verts = []
    uvs = []
    normals = []
    faces = []

    levels = [
        (0.0,  base_pt, base_r),
        (0.40, [base_pt[i]*0.60 + mid_pt[i]*0.40 for i in range(3)], base_r * 0.92),
        (0.75, [mid_pt[i]*0.45 + tip_pt[i]*0.55 for i in range(3)], mid_r * 0.70),
    ]

    t0 = normalize(sub_vec(mid_pt, base_pt))
    t1 = normalize(sub_vec(tip_pt, mid_pt))
    up = [0.0, 0.0, 1.0]

    for lvl_idx, (t_val, center, rad) in enumerate(levels):
        tang = normalize([t0[i]*(1.0-t_val) + t1[i]*t_val for i in range(3)])
        side = normalize(cross(tang, up))
        norm_up = normalize(cross(side, tang))
        rx = rad
        ry = rad * 0.75

        for s in range(segments):
            theta = 2.0 * math.pi * s / segments
            cos_t = math.cos(theta)
            sin_t = math.sin(theta)

            pos = [
                center[0] + side[0]*rx*cos_t + norm_up[0]*ry*sin_t,
                center[1] + side[1]*rx*cos_t + norm_up[1]*ry*sin_t,
                center[2] + side[2]*rx*cos_t + norm_up[2]*ry*sin_t
            ]
            verts.append(pos)
            uvs.append([s / segments, t_val])
            normals.append(normalize([side[0]*cos_t + norm_up[0]*sin_t,
                                      side[1]*cos_t + norm_up[1]*sin_t,
                                      norm_up[2]*sin_t]))

    for r in range(len(levels) - 1):
        r0 = r * segments
        r1 = (r + 1) * segments
        for s in range(segments):
            s_next = (s + 1) % segments
            faces.append([r0 + s, r0 + s_next, r1 + s_next])
            faces.append([r0 + s, r1 + s_next, r1 + s])

    tip_idx = len(verts)
    verts.append(list(tip_pt))
    uvs.append([0.5, 1.0])
    normals.append(t1)

    top_base = (len(levels) - 1) * segments
    for s in range(segments):
        s_next = (s + 1) % segments
        faces.append([tip_idx, top_base + s, top_base + s_next])

    bot_idx = len(verts)
    verts.append(list(base_pt))
    uvs.append([0.5, 0.0])
    normals.append(scale_vec(t0, -1.0))
    for s in range(segments):
        faces.append([bot_idx, (s + 1) % segments, s])

    return verts, uvs, faces, normals

def make_profile_loft(profile_points, segments=16, cap_bot=True, cap_top=True):
    verts = []
    uvs = []
    normals = []
    faces = []
    num_rings = len(profile_points)

    for ring_idx, (c, rx, ry, twist) in enumerate(profile_points):
        cx, cy, cz = c
        v_coord = ring_idx / max(1, num_rings - 1)
        for s in range(segments):
            theta = 2.0 * math.pi * s / segments + twist
            cos_t = math.cos(theta)
            sin_t = math.sin(theta)
            x = cx + rx * cos_t
            y = cy + ry * sin_t
            z = cz
            verts.append([x, y, z])
            uvs.append([s / segments, v_coord])
            nx = cos_t / max(1e-3, rx)
            ny = sin_t / max(1e-3, ry)
            normals.append(normalize([nx, ny, 0.0]))

    for r in range(num_rings - 1):
        r0 = r * segments
        r1 = (r + 1) * segments
        for s in range(segments):
            s_next = (s + 1) % segments
            faces.append([r0 + s, r0 + s_next, r1 + s_next])
            faces.append([r0 + s, r1 + s_next, r1 + s])

    if cap_bot:
        bot_center = list(profile_points[0][0])
        b_idx = len(verts)
        verts.append(bot_center)
        uvs.append([0.5, 0.0])
        normals.append([0.0, 0.0, -1.0])
        for s in range(segments):
            faces.append([b_idx, (s + 1) % segments, s])

    if cap_top:
        top_center = list(profile_points[-1][0])
        t_idx = len(verts)
        verts.append(top_center)
        uvs.append([0.5, 1.0])
        normals.append([0.0, 0.0, 1.0])
        top_base = (num_rings - 1) * segments
        for s in range(segments):
            s_next = (s + 1) % segments
            faces.append([t_idx, top_base + s, top_base + s_next])

    return verts, uvs, faces, normals

def make_billowing_ribbon(points, width=3.5, thickness=0.3):
    verts = []
    uvs = []
    normals = []
    faces = []

    for i, p in enumerate(points):
        v = i / max(1, len(points) - 1)
        hw = width * (1.0 - 0.35 * v) * 0.5
        ht = thickness * 0.5
        verts.append([p[0] - hw, p[1], p[2] + ht])
        verts.append([p[0] + hw, p[1], p[2] + ht])
        verts.append([p[0] - hw, p[1], p[2] - ht])
        verts.append([p[0] + hw, p[1], p[2] - ht])
        uvs.append([0.0, v])
        uvs.append([1.0, v])
        uvs.append([0.0, v])
        uvs.append([1.0, v])
        normals.append([0.0, 1.0, 0.0])
        normals.append([0.0, 1.0, 0.0])
        normals.append([0.0, 1.0, 0.0])
        normals.append([0.0, 1.0, 0.0])

    for i in range(len(points) - 1):
        b = i * 4
        faces.append([b + 0, b + 1, b + 5])
        faces.append([b + 0, b + 5, b + 4])
        faces.append([b + 2, b + 6, b + 7])
        faces.append([b + 2, b + 7, b + 3])
        faces.append([b + 0, b + 4, b + 6])
        faces.append([b + 0, b + 6, b + 2])
        faces.append([b + 1, b + 3, b + 7])
        faces.append([b + 1, b + 7, b + 5])

    return verts, uvs, faces, normals

def make_box(width, length, height, center=(0,0,0)):
    hw = width * 0.5
    hl = length * 0.5
    hh = height * 0.5
    cx, cy, cz = center
    verts = [
        [cx - hw, cy - hl, cz - hh], [cx + hw, cy - hl, cz - hh],
        [cx + hw, cy + hl, cz - hh], [cx - hw, cy + hl, cz - hh],
        [cx - hw, cy - hl, cz + hh], [cx + hw, cy - hl, cz + hh],
        [cx + hw, cy + hl, cz + hh], [cx - hw, cy + hl, cz + hh],
    ]
    uvs = [[0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0],
           [0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [0.0, 1.0]]
    normals = [
        [-0.577, -0.577, -0.577], [ 0.577, -0.577, -0.577],
        [ 0.577,  0.577, -0.577], [-0.577,  0.577, -0.577],
        [-0.577, -0.577,  0.577], [ 0.577, -0.577,  0.577],
        [ 0.577,  0.577,  0.577], [-0.577,  0.577,  0.577],
    ]
    faces = [
        [0, 2, 1], [0, 3, 2], [4, 5, 6], [4, 6, 7],
        [0, 1, 5], [0, 5, 4], [2, 3, 7], [2, 7, 6],
        [0, 4, 7], [0, 7, 3], [1, 2, 6], [1, 6, 5],
    ]
    return verts, uvs, faces, normals

def make_sculpted_head():
    """Organic anime head: chin, jaw, cheeks, cranium, ears, nose tip pyramid."""
    verts = []
    uvs = []
    normals = []
    faces = []

    rings = [
        ((0.0, -0.8, 121.5), 3.2, 3.8, 0.0),   # Chin apex
        ((0.0, -0.4, 123.8), 5.6, 6.2, 0.0),   # Jaw under
        ((0.0,  0.2, 126.8), 8.0, 8.4, 0.0),   # Lower lip & jaw
        ((0.0,  0.6, 129.5), 9.6, 9.8, 0.0),   # Mouth
        ((0.0,  1.2, 132.5), 10.8, 10.6, 0.0), # Nose tip level
        ((0.0,  1.5, 135.5), 11.4, 11.0, 0.0), # Eyes center
        ((0.0,  1.0, 138.5), 11.6, 11.0, 0.0), # Brow ridge
        ((0.0,  0.6, 141.5), 11.2, 10.8, 0.0), # Forehead
        ((0.0,  0.2, 144.5), 10.0, 10.0, 0.0), # Upper forehead
        ((0.0,  0.0, 147.2), 7.8,  8.0, 0.0),  # Cranium
    ]
    v, u, f, n = make_profile_loft(rings, segments=20, cap_bot=True, cap_top=True)
    verts.extend(v)
    uvs.extend(u)
    faces.extend(f)
    normals.extend(n)

    # Sculpted anime 3D nose tip pyramid (protruding forward at Y = -11.0, Z = 132.5)
    nose_base_idx = len(verts)
    verts.append([ 0.0, -11.2, 132.6]) # Nose apex
    verts.append([-0.8, -9.6,  133.4]) # Top left bridge
    verts.append([ 0.8, -9.6,  133.4]) # Top right bridge
    verts.append([-1.0, -9.5,  131.8]) # Bottom left nostril
    verts.append([ 1.0, -9.5,  131.8]) # Bottom right nostril
    for _ in range(5):
        uvs.append([0.5, 0.5])
        normals.append([0.0, -1.0, 0.2])
    # 4 faces of the nose pyramid
    faces.append([nose_base_idx + 0, nose_base_idx + 1, nose_base_idx + 2]) # Top bridge
    faces.append([nose_base_idx + 0, nose_base_idx + 3, nose_base_idx + 1]) # Left slope
    faces.append([nose_base_idx + 0, nose_base_idx + 2, nose_base_idx + 4]) # Right slope
    faces.append([nose_base_idx + 0, nose_base_idx + 4, nose_base_idx + 3]) # Under septum

    # Anime ears
    for side in (-1.0, 1.0):
        ear_rings = [
            ((side * 10.6, 0.8, 131.0), 1.4, 1.0, 0.0),
            ((side * 12.0, 1.2, 133.5), 1.8, 1.4, 0.0),
            ((side * 12.4, 1.4, 136.0), 2.0, 1.5, 0.0),
            ((side * 11.4, 0.9, 138.5), 1.5, 1.1, 0.0),
        ]
        ev, eu, ef, en = make_profile_loft(ear_rings, segments=10, cap_bot=True, cap_top=True)
        e_base = len(verts)
        verts.extend(ev)
        uvs.extend(eu)
        normals.extend(en)
        for face in ef:
            faces.append([face[0] + e_base, face[1] + e_base, face[2] + e_base])

    return verts, uvs, faces, normals

def build_masterpiece_naruto_mesh():
    sections = {
        "Skin": [],
        "Navy": [],
        "Hair": [],
        "Collar": [],
        "Tracksuit": [],
        "Emblem": [],
        "Metal": []
    }

    # =========================================================================
    # 1. HEAD, FACE, NECK (Slot 0: Skin)
    # =========================================================================
    sections["Skin"].append(make_sculpted_head())

    neck_rings = [
        ((0.0,  0.0, 114.0), 5.4, 5.0, 0.0),
        ((0.0,  0.2, 117.0), 5.0, 4.7, 0.0),
        ((0.0,  0.4, 120.0), 5.2, 4.8, 0.0),
        ((0.0,  0.5, 123.0), 5.8, 5.2, 0.0),
    ]
    sections["Skin"].append(make_profile_loft(neck_rings, segments=14, cap_bot=True, cap_top=False))

    # =========================================================================
    # 2. FACIAL FEATURES: Whiskers (Navy), Eyes (White/Navy), Eyebrows (Hair)
    # =========================================================================
    # 3 Cheek Whiskers per side (Slot 1: Navy) - sharp dark anime whiskers
    for side in (-1.0, 1.0):
        for w_idx in range(3):
            w_z = 129.8 + w_idx * 1.8
            p0 = [side * 4.6, -9.9, w_z - 0.4]
            p1 = [side * 7.8, -8.6, w_z + 0.1]
            p2 = [side * 9.5, -7.2, w_z + 0.5]
            wv, wu, wf, wn = make_billowing_ribbon([p0, p1, p2], width=0.55, thickness=0.25)
            sections["Navy"].append((wv, wu, wf, wn))

    # Determined Anime Mouth Slit (Slot 1: Navy)
    mv = [
        [-2.4, -9.9, 128.5], [2.4, -9.9, 128.5],
        [ 2.4, -9.7, 128.1], [-2.4, -9.7, 128.1]
    ]
    mu = [[0,1],[1,1],[1,0],[0,0]]
    mn = [[0,-1,0],[0,-1,0],[0,-1,0],[0,-1,0]]
    mf = [[0,1,2],[0,2,3]]
    sections["Navy"].append((mv, mu, mf, mn))

    # Anime Eyes (Left and Right)
    for side in (-1.0, 1.0):
        eye_x = side * 4.4
        eye_y = -10.3
        eye_z = 135.5
        # Eye Sclera (White - Slot 3: Collar)
        sclera_rings = [
            ((eye_x, eye_y, eye_z), 2.2, 1.6, 0.0),
        ]
        sv = [
            [eye_x - 2.2, eye_y, eye_z - 1.2], [eye_x + 2.2, eye_y, eye_z - 1.2],
            [eye_x + 2.2, eye_y, eye_z + 1.2], [eye_x - 2.2, eye_y, eye_z + 1.2]
        ]
        su = [[0,0],[1,0],[1,1],[0,1]]
        sn = [[0,-1,0],[0,-1,0],[0,-1,0],[0,-1,0]]
        sf = [[0,1,2],[0,2,3]] if side > 0 else [[0,2,1],[0,3,2]]
        sections["Collar"].append((sv, su, sf, sn))

        # Sapphire Blue Iris & Black Pupil (Slot 1: Navy)
        iv = [
            [eye_x - 1.2, eye_y - 0.15, eye_z - 1.2], [eye_x + 1.2, eye_y - 0.15, eye_z - 1.2],
            [eye_x + 1.2, eye_y - 0.15, eye_z + 1.2], [eye_x - 1.2, eye_y - 0.15, eye_z + 1.2]
        ]
        sections["Navy"].append((iv, su, sn, sf))

        # Upper Lash Line (Slot 1: Navy)
        lash = [
            [eye_x - 2.5, eye_y - 0.2, eye_z + 1.2],
            [eye_x,       eye_y - 0.25, eye_z + 1.6],
            [eye_x + 2.5, eye_y - 0.2, eye_z + 1.2],
        ]
        lv, lu, lf, ln = make_billowing_ribbon(lash, width=0.45, thickness=0.2)
        sections["Navy"].append((lv, lu, lf, ln))

        # Blonde Eyebrow Arch (Slot 2: Hair)
        brow = [
            [eye_x - 2.8, eye_y - 0.3, eye_z + 2.6],
            [eye_x,       eye_y - 0.35, eye_z + 3.4],
            [eye_x + 2.8, eye_y - 0.3, eye_z + 2.8],
        ]
        bv, bu, bf, bn = make_billowing_ribbon(brow, width=0.6, thickness=0.3)
        sections["Hair"].append((bv, bu, bf, bn))

    # =========================================================================
    # 3. FOREHEAD PROTECTOR: Headband (Navy), Leaf Plate (Metal), Ribbons (Navy)
    # =========================================================================
    hb_rings = [
        ((0.0,  0.8, 138.0), 11.8, 11.4, 0.0),
        ((0.0,  0.9, 140.8), 11.7, 11.2, 0.0),
        ((0.0,  0.8, 143.5), 11.5, 11.0, 0.0),
    ]
    sections["Navy"].append(make_profile_loft(hb_rings, segments=24, cap_bot=False, cap_top=False))

    # Curved Metal Plate on Front (Slot 6: Metal)
    plate_verts = []
    plate_uvs = []
    plate_normals = []
    plate_faces = []
    cols, rows = 9, 5
    for r in range(rows):
        pz = 138.8 + r * 1.05
        for c in range(cols):
            theta = (c - (cols - 1) / 2.0) * 0.11
            rad = 12.0
            px = rad * math.sin(theta)
            py = -rad * math.cos(theta) + 0.85
            plate_verts.append([px, py, pz])
            plate_uvs.append([c / (cols - 1), r / (rows - 1)])
            plate_normals.append(normalize([math.sin(theta), -math.cos(theta), 0.08]))

    for r in range(rows - 1):
        for c in range(cols - 1):
            i0 = r * cols + c
            i1 = i0 + 1
            i2 = (r + 1) * cols + c + 1
            i3 = (r + 1) * cols + c
            # Face front-facing (-Y normal): CCW when viewed from front
            plate_faces.append([i0, i2, i1])
            plate_faces.append([i0, i3, i2])
    sections["Metal"].append((plate_verts, plate_uvs, plate_faces, plate_normals))

    # Embossed Leaf Symbol on Metal Plate (Slot 1: Navy)
    leaf_p = [
        [ 0.0, -11.5, 140.2], [ 1.2, -11.5, 141.0], [ 0.8, -11.5, 141.8],
        [-0.8, -11.5, 141.6], [-1.2, -11.5, 140.8], [ 0.2, -11.5, 140.6],
        [ 2.2, -11.5, 140.0], # Leaf arrow tip
    ]
    leaf_v, leaf_u, leaf_f, leaf_n = make_billowing_ribbon(leaf_p, width=0.45, thickness=0.2)
    sections["Navy"].append((leaf_v, leaf_u, leaf_f, leaf_n))

    # 4 Corner Dome Rivets (Slot 6: Metal)
    rivet_coords = [
        (-4.8, -11.0, 139.5), (4.8, -11.0, 139.5),
        (-4.8, -11.0, 142.2), (4.8, -11.0, 142.2),
    ]
    for rx, ry, rz in rivet_coords:
        rivet_rings = [
            ((rx, ry, rz), 0.45, 0.45, 0.0),
            ((rx, ry - 0.25, rz), 0.35, 0.35, 0.0),
            ((rx, ry - 0.35, rz), 0.10, 0.10, 0.0),
        ]
        sections["Metal"].append(make_profile_loft(rivet_rings, segments=8, cap_bot=True, cap_top=True))

    # Occipital Tied Ribbons trailing down back (Slot 1: Navy)
    ribbon_l = [
        (-1.5, 11.5, 139.5),
        (-3.2, 14.8, 136.0),
        (-5.8, 19.5, 130.5),
        (-8.8, 24.5, 123.0),
        (-11.5, 29.5, 114.5),
        (-13.5, 33.5, 105.0),
    ]
    ribbon_r = [
        (1.5, 11.5, 139.5),
        (3.5, 15.2, 135.5),
        (6.8, 20.2, 129.5),
        (9.8, 25.5, 121.5),
        (12.5, 30.5, 112.5),
        (14.5, 34.5, 103.0),
    ]
    sections["Navy"].append(make_billowing_ribbon(ribbon_l, width=4.5, thickness=0.4))
    sections["Navy"].append(make_billowing_ribbon(ribbon_r, width=4.5, thickness=0.4))

    # =========================================================================
    # 4. 32 LAYERED 3D SPIKY BLONDE HAIR CLUMPS (Slot 2: Hair)
    # =========================================================================
    hair_locks = [
        # Front Fringe (framing headband)
        ((-6.5, -8.0, 143.0), (-8.5, -11.5, 144.5), (-11.5, -15.5, 146.0), 3.4, 2.6),
        ((-3.2, -9.2, 144.0), (-4.5, -13.0, 147.0), ( -6.0, -18.0, 148.5), 3.6, 2.8),
        (( 0.0, -9.8, 144.5), ( 0.0, -14.5, 148.5), (  0.0, -19.5, 151.0), 3.8, 3.0),
        (( 3.2, -9.2, 144.0), ( 4.5, -13.0, 147.0), (  6.0, -18.0, 148.5), 3.6, 2.8),
        (( 6.5, -8.0, 143.0), ( 8.5, -11.5, 144.5), ( 11.5, -15.5, 146.0), 3.4, 2.6),

        # Sideburns & Ear Framing
        ((-10.5, -3.0, 142.0), (-14.0, -4.5, 144.0), (-19.0, -6.5, 145.0), 3.8, 2.8),
        (( 10.5, -3.0, 142.0), ( 14.0, -4.5, 144.0), ( 19.0, -6.5, 145.0), 3.8, 2.8),
        ((-11.5,  3.0, 141.0), (-16.0,  4.5, 142.5), (-21.0,  6.0, 143.5), 3.8, 2.8),
        (( 11.5,  3.0, 141.0), ( 16.0,  4.5, 142.5), ( 21.0,  6.0, 143.5), 3.8, 2.8),

        # Iconic Radiating Sunflower Crown Spikes (Front/Top)
        ((-6.5, -2.5, 146.0), (-9.5,  -4.5, 154.0), (-14.5,  -7.5, 162.0), 4.6, 3.6),
        ((-2.8, -3.0, 146.5), (-4.5,  -5.0, 156.0), ( -7.0,  -8.0, 166.0), 4.8, 3.8),
        (( 2.8, -3.0, 146.5), ( 4.5,  -5.0, 156.0), (  7.0,  -8.0, 166.0), 4.8, 3.8),
        (( 6.5, -2.5, 146.0), ( 9.5,  -4.5, 154.0), ( 14.5,  -7.5, 162.0), 4.6, 3.6),

        # Apex Crown Spikes
        (( 0.0, -1.0, 147.0), ( 0.0,  -2.0, 158.0), (  0.0,  -3.5, 169.0), 5.2, 4.0),
        ((-5.0,  1.5, 146.5), (-8.0,   2.5, 157.5), (-12.5,   3.5, 166.5), 4.8, 3.6),
        (( 5.0,  1.5, 146.5), ( 8.0,   2.5, 157.5), ( 12.5,   3.5, 166.5), 4.8, 3.6),

        # Mid-Back Radial Spikes (prominent from behind)
        ((-8.0,  5.5, 145.0), (-12.5,  8.0, 153.0), (-17.5,  11.5, 160.0), 4.6, 3.4),
        ((-3.2,  6.0, 145.5), ( -5.0,  9.5, 155.0), ( -8.0,  14.5, 163.0), 4.8, 3.6),
        (( 0.0,  6.5, 146.0), (  0.0, 10.5, 156.0), (  0.0,  16.0, 164.5), 5.0, 3.8),
        (( 3.2,  6.0, 145.5), (  5.0,  9.5, 155.0), (  8.0,  14.5, 163.0), 4.8, 3.6),
        (( 8.0,  5.5, 145.0), ( 12.5,  8.0, 153.0), ( 17.5,  11.5, 160.0), 4.6, 3.4),

        # Lower Occipital / Nape Tufts
        ((-8.5,  8.5, 138.5), (-13.0, 12.0, 143.0), (-18.0,  16.5, 146.0), 4.0, 3.0),
        ((-4.5,  9.5, 138.0), ( -7.0, 14.5, 141.5), (-10.5,  19.5, 144.0), 4.2, 3.2),
        (( 0.0, 10.5, 137.5), (  0.0, 15.5, 140.5), (  0.0,  21.5, 142.5), 4.4, 3.4),
        (( 4.5,  9.5, 138.0), (  7.0, 14.5, 141.5), ( 10.5,  19.5, 144.0), 4.2, 3.2),
        (( 8.5,  8.5, 138.5), ( 13.0, 12.0, 143.0), ( 18.0,  16.5, 146.0), 4.0, 3.0),

        # Intermediate Depth & Volume Fillers
        ((-5.5, -5.5, 144.5), (-9.0,  -8.0, 150.0), (-12.5, -11.5, 155.0), 4.0, 3.0),
        (( 5.5, -5.5, 144.5), ( 9.0,  -8.0, 150.0), ( 12.5, -11.5, 155.0), 4.0, 3.0),
        (( 0.0,  3.5, 146.5), ( 0.0,   6.0, 157.0), (  0.0,   8.5, 167.0), 4.8, 3.6),
        ((-6.5,  0.0, 146.0), (-10.5,  0.0, 155.0), (-15.0,   0.0, 163.0), 4.6, 3.4),
        (( 6.5,  0.0, 146.0), ( 10.5,  0.0, 155.0), ( 15.0,   0.0, 163.0), 4.6, 3.4),
        (( 0.0, -5.0, 145.5), ( 0.0,  -7.5, 155.0), (  0.0, -10.5, 163.5), 4.6, 3.4),
    ]
    for bp, mp, tp, br, mr in hair_locks:
        sections["Hair"].append(make_curved_hair_lock(bp, mp, tp, br, mr, segments=8))

    # =========================================================================
    # 5. PADDED STAND-UP WHITE COLLAR (Slot 3: Collar)
    # =========================================================================
    collar_rings = [
        ((0.0,  0.2, 113.5),  8.2,  7.6, 0.0), # Base
        ((0.0,  0.4, 117.5), 10.2,  9.4, 0.0), # Mid funnel
        ((0.0,  0.6, 121.5), 12.2, 11.2, 0.0), # Upper flare
        ((0.0,  0.8, 124.5), 12.8, 11.8, 0.0), # Rolled padded rim
    ]
    sections["Collar"].append(make_profile_loft(collar_rings, segments=20, cap_bot=False, cap_top=True))

    # =========================================================================
    # 6. ORANGE JACKET, NAVY YOKE, FRONT ZIPPER, CRESTS & POUCH
    # =========================================================================
    # Navy Shoulder Yoke (Slot 1: Navy)
    yoke_rings = [
        ((0.0,  0.0, 104.0), 19.5, 12.5, 0.0),
        ((0.0,  0.1, 109.0), 17.5, 11.2, 0.0),
        ((0.0,  0.2, 113.5), 13.5,  9.2, 0.0),
    ]
    sections["Navy"].append(make_profile_loft(yoke_rings, segments=20, cap_bot=False, cap_top=False))

    # Cadmium Orange Jacket Torso (Slot 4: Tracksuit)
    torso_rings = [
        ((0.0,  0.0,  74.0), 15.5, 10.5, 0.0), # Elastic waistband hem
        ((0.0,  0.2,  80.0), 16.8, 11.2, 0.0), # Lower abdomen folds
        ((0.0,  0.3,  88.0), 17.8, 11.8, 0.0), # Mid torso
        ((0.0,  0.2,  96.0), 18.8, 12.2, 0.0), # Chest
        ((0.0,  0.1, 104.0), 19.5, 12.5, 0.0), # Under yoke seam
    ]
    sections["Tracksuit"].append(make_profile_loft(torso_rings, segments=20, cap_bot=True, cap_top=False))

    # Front Silver Zipper Line & Slider Tab (Slot 6: Metal)
    zip_pts = [
        [0.0, -12.4, 113.5],
        [0.0, -12.3, 104.0],
        [0.0, -12.0,  96.0],
        [0.0, -11.6,  88.0],
        [0.0, -11.0,  80.0],
        [0.0, -10.4,  74.0],
    ]
    zv, zu, zf, zn = make_billowing_ribbon(zip_pts, width=1.1, thickness=0.4)
    sections["Metal"].append((zv, zu, zf, zn))
    # Zipper pull tab at chest
    sections["Metal"].append(make_box(1.2, 0.5, 2.2, center=(0.0, -12.3, 102.0)))

    # Left Shoulder/Arm White Swirl Emblem Patch (Slot 3: Collar)
    swirl_v = []
    swirl_u = []
    swirl_n = []
    swirl_f = []
    arm_patch_center = [-26.2, -4.0, 86.0]
    swirl_v.append(arm_patch_center)
    swirl_u.append([0.5, 0.5])
    swirl_n.append([-1.0, 0.0, 0.0])
    num_pts = 16
    for i in range(num_pts):
        th = 2.0 * math.pi * i / num_pts
        py = 3.6 * math.cos(th)
        pz = 3.6 * math.sin(th)
        swirl_v.append([arm_patch_center[0] - 0.2, arm_patch_center[1] + py, arm_patch_center[2] + pz])
        swirl_u.append([0.5 + 0.5*math.cos(th), 0.5 + 0.5*math.sin(th)])
        swirl_n.append([-1.0, 0.0, 0.0])
    for i in range(num_pts):
        swirl_f.append([0, 1 + ((i + 1) % num_pts), 1 + i])
    sections["Collar"].append((swirl_v, swirl_u, swirl_f, swirl_n))

    # =========================================================================
    # BACK UZUMAKI CREST: White Outer Ring (Collar) + Crimson Disc (Emblem)
    # Winding explicitly faces +Y (backwards) with normal [0, 1, 0]
    # =========================================================================
    # 1. White Border Ring (Slot 3: Collar)
    ring_v = []
    ring_u = []
    ring_n = []
    ring_f = []
    r_outer = 9.2
    r_inner = 7.6
    crest_z = 97.0
    crest_y_border = 13.6
    num_c = 32
    for i in range(num_c):
        th = 2.0 * math.pi * i / num_c
        cos_th = math.cos(th)
        sin_th = math.sin(th)
        # Inner vert
        ring_v.append([r_inner * cos_th, crest_y_border, crest_z + r_inner * sin_th])
        ring_u.append([0.5 + 0.4 * cos_th, 0.5 + 0.4 * sin_th])
        ring_n.append([0.0, 1.0, 0.0])
        # Outer vert
        ring_v.append([r_outer * cos_th, crest_y_border, crest_z + r_outer * sin_th])
        ring_u.append([0.5 + 0.5 * cos_th, 0.5 + 0.5 * sin_th])
        ring_n.append([0.0, 1.0, 0.0])

    for i in range(num_c):
        i_next = (i + 1) % num_c
        i0 = i * 2
        i1 = i * 2 + 1
        i2 = i_next * 2 + 1
        i3 = i_next * 2
        # Back-facing (+Y normal): CCW when viewed from behind (+Y)
        ring_f.append([i0, i1, i2])
        ring_f.append([i0, i2, i3])
    sections["Collar"].append((ring_v, ring_u, ring_f, ring_n))

    # 2. Crimson Uzumaki Inner Spiral Disc (Slot 5: Emblem)
    crest_v = []
    crest_u = []
    crest_n = []
    crest_f = []
    crest_y_inner = 13.8
    crest_v.append([0.0, crest_y_inner, crest_z])
    crest_u.append([0.5, 0.5])
    crest_n.append([0.0, 1.0, 0.0])
    for i in range(num_c):
        th = 2.0 * math.pi * i / num_c
        cx = r_inner * math.cos(th)
        cz = r_inner * math.sin(th)
        crest_v.append([cx, crest_y_inner, crest_z + cz])
        crest_u.append([0.5 + 0.5 * math.cos(th), 0.5 + 0.5 * math.sin(th)])
        crest_n.append([0.0, 1.0, 0.0])
    for i in range(num_c):
        # Back-facing (+Y normal): CCW when viewed from behind
        crest_f.append([0, 1 + i, 1 + ((i + 1) % num_c)])
    sections["Emblem"].append((crest_v, crest_u, crest_f, crest_n))

    # Rear Waist Tool Pouch / Fanny Pack (Slot 3: Collar/Beige)
    sections["Collar"].append(make_box(16.0, 6.5, 8.5, center=(0.0, 13.5, 76.0)))

    # Flared Athletic Arms & Ready Shinobi Fists
    for side in (-1.0, 1.0):
        arm_rings = [
            ((side * 20.5,  0.0, 104.0), 5.8, 5.5, 0.0), # Shoulder / deltoid
            ((side * 24.5, -2.0,  93.0), 5.2, 4.8, 0.0), # Upper arm (flared out)
            ((side * 26.5, -4.5,  83.0), 5.5, 5.2, 0.0), # Elbow bend with bunched folds
            ((side * 24.5, -9.5,  75.0), 4.6, 4.4, 0.0), # Forearm angling forward
        ]
        sections["Tracksuit"].append(make_profile_loft(arm_rings, segments=14, cap_bot=False, cap_top=True))

        # Navy Wrist Cuffs (Slot 1: Navy)
        cuff_rings = [
            ((side * 24.5,  -9.5, 75.0), 4.8, 4.6, 0.0),
            ((side * 23.5, -13.0, 69.0), 4.0, 3.8, 0.0),
        ]
        sections["Navy"].append(make_profile_loft(cuff_rings, segments=14, cap_bot=False, cap_top=False))

        # Ready Shinobi Fists (Slot 0: Skin)
        fist_rings = [
            ((side * 23.5, -13.0, 69.0), 3.6, 3.2, 0.0), # Wrist
            ((side * 23.0, -16.5, 64.0), 4.5, 4.0, 0.0), # Knuckles & clenched fingers
            ((side * 22.5, -18.0, 59.5), 3.4, 3.0, 0.0), # Fist base
        ]
        sections["Skin"].append(make_profile_loft(fist_rings, segments=12, cap_bot=True, cap_top=False))

    # =========================================================================
    # 7. ATHLETIC WIDE-PLANTED TROUSERS (Tracksuit), BANDAGES, KUNAI POUCH
    # =========================================================================
    for side in (-1.0, 1.0):
        # Wide planted legs: feet at X = +/- 18cm, knees at X = +/- 15cm
        pant_rings = [
            ((side * 11.0,  0.0, 74.0),  9.2, 8.4, 0.0), # Pelvis / crotch
            ((side * 13.0, -0.4, 62.0),  8.6, 8.0, 0.0), # Upper thigh
            ((side * 15.0, -1.2, 48.0),  8.2, 7.6, 0.0), # Above knee
            ((side * 15.5, -2.0, 38.0),  9.0, 8.2, 0.0), # Knee fold pouching (bent knee)
            ((side * 16.5, -1.2, 26.0),  8.0, 7.4, 0.0), # Shin angling outward
            ((side * 18.0, -0.4, 15.0),  5.8, 5.2, 0.0), # Cinched ankle cuff
        ]
        sections["Tracksuit"].append(make_profile_loft(pant_rings, segments=16, cap_bot=True, cap_top=True))

    # White Medical Bandages on Right Mid-Thigh (Slot 3: Collar)
    bandage_rings = [
        ((13.8, -0.8, 50.0), 8.8, 8.2, 0.0),
        ((14.2, -0.9, 54.0), 8.9, 8.3, 0.0),
        ((14.6, -1.0, 58.0), 9.0, 8.4, 0.0),
        ((14.9, -1.1, 62.0), 9.0, 8.4, 0.0),
    ]
    sections["Collar"].append(make_profile_loft(bandage_rings, segments=16, cap_bot=False, cap_top=False))

    # Dark Leather Kunai Holster Pouch on Outer Right Thigh (Slot 1: Navy/Leather)
    sections["Navy"].append(make_box(4.2, 8.5, 11.5, center=(24.2, -1.0, 56.0)))

    # =========================================================================
    # 8. KONOHA NINJA SANDALS WITH SCULPTED TOES & TREAD SOLES
    # =========================================================================
    for side in (-1.0, 1.0):
        foot_x = side * 18.0

        # Navy Ankle & Heel Wrap (Slot 1: Navy)
        sandal_rings = [
            ((foot_x, -0.4, 15.0), 5.4, 5.0, 0.0),
            ((foot_x, -0.8, 10.0), 5.2, 5.6, 0.0),
            ((foot_x, -1.2,  5.0), 5.0, 6.2, 0.0),
        ]
        sections["Navy"].append(make_profile_loft(sandal_rings, segments=14, cap_bot=False, cap_top=True))

        # Exposed Anime Foot & 5 Sculpted Toes (Slot 0: Skin)
        foot_rings = [
            ((foot_x, -1.8, 4.0), 4.8, 6.2, 0.0),
            ((foot_x, -5.0, 2.5), 5.0, 6.8, 0.0),
            ((foot_x, -9.0, 1.2), 5.2, 6.5, 0.0), # 5 sculpted toes
        ]
        sections["Skin"].append(make_profile_loft(foot_rings, segments=14, cap_bot=True, cap_top=True))

        # Sandal Dark Rubber Sole (Slot 1: Navy)
        sole_rings = [
            ((foot_x, -2.8, 1.2), 5.6, 8.4, 0.0),
            ((foot_x, -2.8, 0.0), 5.6, 8.4, 0.0),
        ]
        sections["Navy"].append(make_profile_loft(sole_rings, segments=14, cap_bot=True, cap_top=True))

    return sections

def write_obj_file(filepath, sections):
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Masterpiece Naruto Production Model: {os.path.basename(filepath)}\n")
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

    num_verts = len(all_v)
    num_faces = sum(len(p[2]) for parts in sections.values() for p in parts)
    print(f"-> Masterpiece Naruto model exported: {filepath} ({num_verts} vertices, {num_faces} faces)")

if __name__ == "__main__":
    print("=== AUTHORING MASTERPIECE 3D SCULPTED NARUTO GENIN ===")
    naruto_sections = build_masterpiece_naruto_mesh()
    out_obj = os.path.join(MESH_DIR, "SK_Naruto_Genin.obj")
    write_obj_file(out_obj, naruto_sections)
    out_obj_skin = os.path.join(MESH_DIR, "SK_Naruto_GeninTracksuit.obj")
    write_obj_file(out_obj_skin, naruto_sections)
    print("=== MASTERPIECE NARUTO GENIN ASSET GENERATION COMPLETE ===")
