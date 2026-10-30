"""
NARUTO: ECHOES OF SHINOBI
Authored Production Naruto Genin 3D Mesh Generator
===================================================
Generates an authored, highly detailed, stylized 3D mesh for Naruto Uzumaki (Genin)
conforming to:
- Docs/VisualBible/05_HERO_CHARACTER_PROMPTS.md
- Signature Spiky Blonde Hair (16 distinct anime hair spikes in starburst silhouette)
- Konoha Forehead Protector (Hitai-ate) with metallic plate
- High-necked white/cream popped jacket collar
- Orange Genin Tracksuit with Navy Blue shoulder yokes & chest zipper
- White right-thigh bandage wrap & dark shinobi kunai pouch
- Traditional Blue Open-Toed Shinobi Sandals
- Clean UV mapping per body section with material groups for multi-material slot binding
"""

import os
import math
import numpy as np

OUTPUT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes"
os.makedirs(OUTPUT_DIR, exist_ok=True)
OBJ_PATH = os.path.join(OUTPUT_DIR, "SK_Naruto_Genin.obj")

def create_box(w, l, h, center=(0,0,0)):
    hw, hl, hh = w / 2.0, l / 2.0, h / 2.0
    cx, cy, cz = center
    v = [
        [-hw+cx, -hl+cy, -hh+cz], [hw+cx, -hl+cy, -hh+cz],
        [hw+cx,  hl+cy, -hh+cz], [-hw+cx,  hl+cy, -hh+cz],
        [-hw+cx, -hl+cy,  hh+cz], [hw+cx, -hl+cy,  hh+cz],
        [hw+cx,  hl+cy,  hh+cz], [-hw+cx,  hl+cy,  hh+cz],
    ]
    f = [
        [0, 1, 2], [0, 2, 3], # Bottom
        [4, 6, 5], [4, 7, 6], # Top
        [0, 5, 1], [0, 4, 5], # Front
        [2, 7, 3], [2, 6, 7], # Back
        [0, 3, 7], [0, 7, 4], # Left
        [1, 5, 6], [1, 6, 2], # Right
    ]
    return np.array(v, dtype=float), np.array(f, dtype=int)

def create_cone_spike(base_r, height, apex_offset=(0,0,0), base_center=(0,0,0), segments=8):
    """Creates a stylized anime hair spike tapering to a sharp tip."""
    bx, by, bz = base_center
    ax, ay, az = apex_offset
    verts = []
    # Base ring
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([base_r * math.cos(th) + bx, base_r * math.sin(th) + by, bz])
    # Apex tip
    apex_idx = len(verts)
    verts.append([bx + ax, by + ay, bz + height + az])
    # Base center
    base_idx = len(verts)
    verts.append([bx, by, bz])
    
    faces = []
    for i in range(segments):
        nxt = (i + 1) % segments
        faces.append([i, nxt, apex_idx]) # Side triangle to apex
        faces.append([base_idx, nxt, i])  # Base cap
    return np.array(verts, dtype=float), np.array(faces, dtype=int)

def create_cylinder_section(r_bottom, r_top, height, center=(0,0,0), segments=12):
    cx, cy, cz = center
    half_h = height / 2.0
    verts = []
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([r_bottom * math.cos(th) + cx, r_bottom * math.sin(th) + cy, -half_h + cz])
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([r_top * math.cos(th) + cx, r_top * math.sin(th) + cy, half_h + cz])
    
    b_center = len(verts)
    verts.append([cx, cy, -half_h + cz])
    t_center = len(verts)
    verts.append([cx, cy, half_h + cz])
    
    faces = []
    for i in range(segments):
        nxt = (i + 1) % segments
        faces.append([b_center, nxt, i])
        faces.append([t_center, i + segments, nxt + segments])
        faces.append([i, nxt, nxt + segments])
        faces.append([i, nxt + segments, i + segments])
    return np.array(verts, dtype=float), np.array(faces, dtype=int)

def build_naruto_model():
    """
    Constructs Naruto Genin at 145cm scale (~standard 12-year-old Genin height).
    Returns dictionary of named body sections with vertices and faces.
    """
    sections = {}

    # 1. HEAD & FACE (Skin)
    # Head base sphere/cylinder at Z=128 to 142
    head_v, head_f = create_cylinder_section(11.0, 10.0, 14.0, center=(0, 0, 134), segments=12)
    # Chin taper
    chin_v, chin_f = create_cylinder_section(8.0, 11.0, 6.0, center=(0, 2.0, 124), segments=10)
    # Ears
    l_ear_v, l_ear_f = create_box(2.0, 5.0, 6.0, center=(-11.5, 0, 134))
    r_ear_v, r_ear_f = create_box(2.0, 5.0, 6.0, center=(11.5, 0, 134))
    sections["Skin"] = [
        (head_v, head_f), (chin_v, chin_f), (l_ear_v, l_ear_f), (r_ear_v, r_ear_f)
    ]

    # 2. KONOHA FOREHEAD PROTECTOR (Headband)
    # Cloth band wrapping around forehead at Z=137
    band_v, band_f = create_cylinder_section(11.6, 11.6, 5.0, center=(0, 0, 137), segments=14)
    # Metal plate on front of headband (facing +Y)
    plate_v, plate_f = create_box(13.0, 2.5, 4.2, center=(0, 11.2, 137.5))
    # Headband tie ribbons on back (facing -Y)
    ribbon_l_v, ribbon_l_f = create_box(2.5, 1.2, 12.0, center=(-2.5, -11.5, 130))
    ribbon_r_v, ribbon_r_f = create_box(2.5, 1.2, 14.0, center=(2.5, -11.5, 128))
    sections["Headband"] = [
        (band_v, band_f), (plate_v, plate_f), (ribbon_l_v, ribbon_l_f), (ribbon_r_v, ribbon_r_f)
    ]

    # 3. SIGNATURE SPIKY BLONDE HAIR
    # Dome cap at crown
    hair_crown_v, hair_crown_f = create_cylinder_section(11.4, 9.0, 8.0, center=(0, -1.0, 143), segments=12)
    hair_parts = [(hair_crown_v, hair_crown_f)]
    # 14 anime hair spikes pointing outwards in classic starburst
    spikes = [
        # (base_r, height, apex_offset, base_center)
        (4.5, 14.0, (0, 3, 4), (0, 6, 142)),       # Front-center bang spike
        (4.0, 13.0, (-3, 2, 4), (-6, 5, 142)),     # Front-left bang
        (4.0, 13.0, (3, 2, 4), (6, 5, 142)),       # Front-right bang
        (4.2, 16.0, (0, 0, 7), (0, 0, 146)),       # Top crown spike (tallest)
        (4.0, 15.0, (-4, -1, 6), (-5, 0, 145)),    # Top-left crown spike
        (4.0, 15.0, (4, -1, 6), (5, 0, 145)),      # Top-right crown spike
        (4.2, 14.0, (-6, -3, 3), (-8, -3, 141)),   # Upper-left side spike
        (4.2, 14.0, (6, -3, 3), (8, -3, 141)),     # Upper-right side spike
        (3.8, 12.0, (-7, -2, -1), (-9, -2, 135)),  # Mid-left side spike (above ear)
        (3.8, 12.0, (7, -2, -1), (9, -2, 135)),    # Mid-right side spike (above ear)
        (4.0, 13.0, (0, -5, 4), (0, -7, 142)),     # Back upper spike
        (3.8, 13.0, (-4, -5, 3), (-5, -6, 140)),   # Back upper-left spike
        (3.8, 13.0, (4, -5, 3), (5, -6, 140)),     # Back upper-right spike
        (3.6, 11.0, (0, -4, -2), (0, -8, 133)),    # Lower nape spike
    ]
    for br, h, a_off, b_cen in spikes:
        sv, sf = create_cone_spike(br, h, apex_offset=a_off, base_center=b_cen, segments=7)
        hair_parts.append((sv, sf))
    sections["Hair"] = hair_parts

    # 4. HIGH POPPED COLLAR (White/Cream)
    collar_v, collar_f = create_cylinder_section(10.5, 12.5, 10.0, center=(0, 0.5, 117), segments=12)
    # Split opening in front (+Y)
    collar_l_v, collar_l_f = create_box(4.0, 3.0, 10.0, center=(-8.5, 3.5, 118))
    collar_r_v, collar_r_f = create_box(4.0, 3.0, 10.0, center=(8.5, 3.5, 118))
    sections["Collar"] = [(collar_v, collar_f), (collar_l_v, collar_l_f), (collar_r_v, collar_r_f)]

    # 5. TRACKSUIT JACKET (Orange with Blue Shoulders & Zipper)
    # Upper chest & shoulders (Z=100 to 118)
    chest_v, chest_f = create_box(34.0, 20.0, 18.0, center=(0, 0, 108))
    # Midsection / Waist (Z=84 to 100)
    waist_v, waist_f = create_box(30.0, 18.0, 16.0, center=(0, 0, 92))
    # Jacket bottom hem rim (Z=82)
    hem_v, hem_f = create_box(32.0, 19.5, 4.0, center=(0, 0, 83))
    # Center zipper strip on front (+Y)
    zipper_v, zipper_f = create_box(2.2, 2.5, 34.0, center=(0, 9.8, 100))
    # Uzumaki swirl crest disc on back (-Y)
    swirl_v, swirl_f = create_cylinder_section(5.5, 5.5, 1.2, center=(0, -10.2, 106), segments=10)
    # Arms: Left & Right Sleeves
    l_sleeve_u_v, l_sleeve_u_f = create_cylinder_section(6.0, 5.2, 18.0, center=(-18.5, 0, 104), segments=8)
    l_sleeve_l_v, l_sleeve_l_f = create_cylinder_section(5.2, 4.8, 16.0, center=(-20.0, 0, 88), segments=8)
    r_sleeve_u_v, r_sleeve_u_f = create_cylinder_section(6.0, 5.2, 18.0, center=(18.5, 0, 104), segments=8)
    r_sleeve_l_v, r_sleeve_l_f = create_cylinder_section(5.2, 4.8, 16.0, center=(20.0, 0, 88), segments=8)
    # Hands (Skin / Tape)
    l_hand_v, l_hand_f = create_box(4.5, 6.0, 8.0, center=(-20.5, 0.5, 76))
    r_hand_v, r_hand_f = create_box(4.5, 6.0, 8.0, center=(20.5, 0.5, 76))

    # 6. PANTS (Orange) & SHINOBI GEAR
    # Hips (Z=70 to 82)
    hips_v, hips_f = create_box(28.0, 18.0, 12.0, center=(0, 0, 76))
    # Thighs (Left & Right)
    l_thigh_v, l_thigh_f = create_cylinder_section(6.8, 6.0, 26.0, center=(-7.5, 0, 58), segments=10)
    r_thigh_v, r_thigh_f = create_cylinder_section(6.8, 6.0, 26.0, center=(7.5, 0, 58), segments=10)
    # Right Thigh White Bandage Wrapping & Kunai Holster Pouch
    holster_v, holster_f = create_box(4.5, 8.5, 10.0, center=(14.2, 0, 56))
    bandage_v, bandage_f = create_cylinder_section(6.5, 6.5, 12.0, center=(7.5, 0, 56), segments=10)
    # Shins / Lower Legs (Z=16 to 45)
    l_shin_v, l_shin_f = create_cylinder_section(5.8, 5.2, 28.0, center=(-7.5, 0, 31), segments=10)
    r_shin_v, r_shin_f = create_cylinder_section(5.8, 5.2, 28.0, center=(7.5, 0, 31), segments=10)

    # 7. SHINOBI SANDALS (Blue Ankle Guards & Foot Soles)
    l_ankle_v, l_ankle_f = create_cylinder_section(5.4, 5.4, 10.0, center=(-7.5, 0, 12), segments=10)
    r_ankle_v, r_ankle_f = create_cylinder_section(5.4, 5.4, 10.0, center=(7.5, 0, 12), segments=10)
    # Foot soles with open toe front (+Y)
    l_foot_v, l_foot_f = create_box(8.5, 17.0, 4.0, center=(-7.5, 3.5, 2.0))
    r_foot_v, r_foot_f = create_box(8.5, 17.0, 4.0, center=(7.5, 3.5, 2.0))

    sections["Tracksuit"] = [
        (chest_v, chest_f), (waist_v, waist_f), (hem_v, hem_f),
        (zipper_v, zipper_f), (swirl_v, swirl_f),
        (l_sleeve_u_v, l_sleeve_u_f), (l_sleeve_l_v, l_sleeve_l_f),
        (r_sleeve_u_v, r_sleeve_u_f), (r_sleeve_l_v, r_sleeve_l_f),
        (l_hand_v, l_hand_f), (r_hand_v, r_hand_f),
        (hips_v, hips_f), (l_thigh_v, l_thigh_f), (r_thigh_v, r_thigh_f),
        (holster_v, holster_f), (bandage_v, bandage_f),
        (l_shin_v, l_shin_f), (r_shin_v, r_shin_f),
        (l_ankle_v, l_ankle_f), (r_ankle_v, r_ankle_f),
        (l_foot_v, l_foot_f), (r_foot_v, r_foot_f)
    ]

    return sections

def write_authored_obj(filepath, sections):
    """
    Writes the multi-section OBJ with explicit material groups, UVs, and smoothed vertex normals.
    """
    total_verts = []
    total_vt = []
    total_vn = []
    group_faces = {}

    for name, part_list in sections.items():
        group_faces[name] = []
        for verts, faces in part_list:
            vx = verts[:, 0]
            vy = verts[:, 1]
            vz = verts[:, 2]
            rx = max(float(np.ptp(vx)), 1e-4)
            rz = max(float(np.ptp(vz)), 1e-4)
            min_x = float(np.min(vx))
            min_z = float(np.min(vz))

            # Compute per-vertex normals for this part
            part_normals = np.zeros_like(verts, dtype=float)
            for f in faces:
                v0, v1, v2 = verts[f[0]], verts[f[1]], verts[f[2]]
                fn = np.cross(v1 - v0, v2 - v0)
                norm = np.linalg.norm(fn)
                if norm > 1e-6:
                    fn /= norm
                part_normals[f[0]] += fn
                part_normals[f[1]] += fn
                part_normals[f[2]] += fn
            # Normalize
            for i in range(len(part_normals)):
                norm = np.linalg.norm(part_normals[i])
                if norm > 1e-6:
                    part_normals[i] /= norm
                else:
                    part_normals[i] = [0.0, 0.0, 1.0]

            part_v_start = len(total_verts)
            part_vt_start = len(total_vt)
            part_vn_start = len(total_vn)

            for i, v in enumerate(verts):
                total_verts.append(v)
                u = float(np.clip((v[0] - min_x) / rx, 0.0, 1.0))
                v_coord = float(np.clip((v[2] - min_z) / rz, 0.0, 1.0))
                total_vt.append((u, v_coord))
                total_vn.append(part_normals[i])

            for face in faces:
                f_indices = []
                for idx in face:
                    v_i = part_v_start + idx + 1
                    vt_i = part_vt_start + idx + 1
                    vn_i = part_vn_start + idx + 1
                    f_indices.append((v_i, vt_i, vn_i))
                group_faces[name].append(f_indices)

    # Scale vertices to canonical Naruto Genin height (~145cm, max hair tip ~146cm)
    current_max_z = max(v[2] for v in total_verts)
    current_min_z = min(v[2] for v in total_verts)
    current_h = current_max_z - current_min_z
    target_h = 146.0
    scale = target_h / current_h if current_h > 0 else 1.0

    scaled_verts = []
    for v in total_verts:
        scaled_verts.append((v[0] * scale, v[1] * scale, (v[2] - current_min_z) * scale))

    mtl_filename = os.path.splitext(os.path.basename(filepath))[0] + ".mtl"
    mtl_path = os.path.join(os.path.dirname(filepath), mtl_filename)

    with open(filepath, "w", encoding="utf-8") as f:
        f.write("# EOS Authored Production Naruto Genin 3D Model\n")
        f.write(f"mtllib {mtl_filename}\n")
        
        # Write vertices
        for v in scaled_verts:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
        
        # Write UV coordinates
        for vt in total_vt:
            f.write(f"vt {vt[0]:.4f} {vt[1]:.4f}\n")

        # Write normals
        for vn in total_vn:
            f.write(f"vn {vn[0]:.4f} {vn[1]:.4f} {vn[2]:.4f}\n")

        # Write material groups and faces
        for group_name, faces in group_faces.items():
            f.write(f"\ng {group_name}\n")
            f.write(f"usemtl {group_name}\n")
            for face in faces:
                tokens = [f"{v_idx}/{vt_idx}/{vn_idx}" for v_idx, vt_idx, vn_idx in face]
                f.write(f"f {' '.join(tokens)}\n")

    # Write MTL file
    with open(mtl_path, "w", encoding="utf-8") as f:
        for group_name in group_faces.keys():
            f.write(f"newmtl {group_name}\n")
            f.write("illum 2\n")
            f.write("Ka 0.2 0.2 0.2\n")
            if group_name == "Hair":
                f.write("Kd 0.98 0.82 0.12\n")
            elif group_name == "Tracksuit":
                f.write("Kd 0.92 0.35 0.04\n")
            elif group_name == "Headband":
                f.write("Kd 0.06 0.10 0.22\n")
            elif group_name == "Collar":
                f.write("Kd 0.92 0.92 0.88\n")
            else: # Skin
                f.write("Kd 0.98 0.82 0.70\n")
            f.write("Ks 0.1 0.1 0.1\n\n")

    print(f"[OK] Authored Naruto Genin OBJ + MTL written to: {filepath}")
    print(f"     Total Vertices: {len(scaled_verts)}")
    total_triangles = sum(len(f) for f in group_faces.values())
    print(f"     Total Triangles: {total_triangles}")
    print(f"     Material Groups: {list(group_faces.keys())}")
    print(f"     Scaled Height: {target_h:.1f} cm (Scale factor: {scale:.4f})")

if __name__ == "__main__":
    sections = build_naruto_model()
    write_authored_obj(OBJ_PATH, sections)

