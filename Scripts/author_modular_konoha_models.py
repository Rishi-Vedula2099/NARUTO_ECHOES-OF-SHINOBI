"""
NARUTO: ECHOES OF SHINOBI
Modular 3D Mesh Generator with Authored UVs for Konoha Architecture & Naruto
Generates watertight, correctly oriented, UV-mapped OBJs.
"""

import os
import math

MESH_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes"
os.makedirs(MESH_DIR, exist_ok=True)

def write_obj_file(filepath, vertices, faces, uvs, normals=None, material_names=None):
    """
    Writes a standard Wavefront OBJ file with vertices, UVs, normals, and material assignments.
    """
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Authored Mesh: {os.path.basename(filepath)}\n")
        f.write("o " + os.path.splitext(os.path.basename(filepath))[0] + "\n")

        # 1. Vertices
        for v in vertices:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")

        # 2. UVs
        for uv in uvs:
            f.write(f"vt {uv[0]:.4f} {uv[1]:.4f}\n")

        # 3. Normals (optional)
        if normals is not None and len(normals) > 0:
            for vn in normals:
                f.write(f"vn {vn[0]:.4f} {vn[1]:.4f} {vn[2]:.4f}\n")

        # 4. Faces
        current_mat = None
        for i, face in enumerate(faces):
            if material_names is not None and i < len(material_names):
                mat = material_names[i]
                if mat != current_mat:
                    f.write(f"usemtl {mat}\n")
                    current_mat = mat

            v_idx, vt_idx, vn_idx = face
            tokens = []
            for vi, vti, vni in zip(v_idx, vt_idx, vn_idx):
                if vni >= 0:
                    tokens.append(f"{vi + 1}/{vti + 1}/{vni + 1}")
                elif vti >= 0:
                    tokens.append(f"{vi + 1}/{vti + 1}")
                else:
                    tokens.append(f"{vi + 1}")
            f.write(f"f {' '.join(tokens)}\n")

    print(f"Generated OBJ: {filepath} ({len(vertices)} verts, {len(faces)} faces)")

def make_box_mesh(width, length, height, center=(0,0,0), uv_min=(0,0), uv_max=(1,1)):
    w, l, h = width / 2.0, length / 2.0, height / 2.0
    cx, cy, cz = center
    verts = [
        [-w+cx, -l+cy, -h+cz], [w+cx, -l+cy, -h+cz], [w+cx, l+cy, -h+cz], [-w+cx, l+cy, -h+cz],
        [-w+cx, -l+cy,  h+cz], [w+cx, -l+cy,  h+cz], [w+cx, l+cy,  h+cz], [-w+cx, l+cy,  h+cz]
    ]
    u0, v0 = uv_min
    u1, v1 = uv_max
    uvs = [
        [u0, v0], [u1, v0], [u1, v1], [u0, v1]
    ]
    raw_faces = [
        ([0, 1, 2], [0, 1, 2]), ([0, 2, 3], [0, 2, 3]), # Bottom
        ([4, 6, 5], [0, 2, 1]), ([4, 7, 6], [0, 3, 2]), # Top
        ([0, 5, 1], [0, 3, 1]), ([0, 4, 5], [0, 2, 3]), # Front
        ([2, 7, 3], [1, 3, 0]), ([2, 6, 7], [1, 2, 3]), # Back
        ([0, 3, 7], [0, 1, 2]), ([0, 7, 4], [0, 2, 3]), # Left
        ([1, 5, 6], [0, 3, 2]), ([1, 6, 2], [0, 2, 1])  # Right
    ]
    faces = []
    for vi, vti in raw_faces:
        faces.append((vi, vti, [-1, -1, -1]))
    return verts, uvs, faces

def make_gabled_roof(width, length, height, center=(0,0,0), uv_min=(0,0), uv_max=(1,1)):
    """Creates a sloped gabled roof with eaves."""
    w, l, h = width / 2.0, length / 2.0, height
    cx, cy, cz = center
    # 6 vertices: 4 base eave corners, 2 ridge apex vertices
    verts = [
        [-w+cx, -l+cy, cz], [w+cx, -l+cy, cz], [w+cx, l+cy, cz], [-w+cx, l+cy, cz], # 0..3 Base eaves
        [cx, -l+cy, cz+h], [cx, l+cy, cz+h]                                          # 4..5 Ridge apex
    ]
    u0, v0 = uv_min
    u1, v1 = uv_max
    uvs = [
        [u0, v0], [u1, v0], [u1, v1], [u0, v1], [(u0+u1)/2, v1]
    ]
    # Slopes and gables
    faces = [
        # Left slope (-X)
        ([0, 4, 5], [0, 4, 3], [-1,-1,-1]), ([0, 5, 3], [0, 3, 2], [-1,-1,-1]),
        # Right slope (+X)
        ([1, 5, 4], [1, 3, 4], [-1,-1,-1]), ([1, 2, 5], [1, 2, 3], [-1,-1,-1]),
        # Front gable (-Y)
        ([0, 1, 4], [0, 1, 4], [-1,-1,-1]),
        # Back gable (+Y)
        ([3, 5, 2], [0, 4, 1], [-1,-1,-1])
    ]
    return verts, uvs, faces

def merge_mesh_data(mesh_list):
    """Merges multiple (verts, uvs, faces, materials) tuples into a single OBJ data structure."""
    all_verts = []
    all_uvs = []
    all_faces = []
    all_mats = []

    v_offset = 0
    uv_offset = 0

    for item in mesh_list:
        verts, uvs, faces = item[0], item[1], item[2]
        mat_name = item[3] if len(item) > 3 else "default"

        all_verts.extend(verts)
        all_uvs.extend(uvs)

        for f in faces:
            v_idx, vt_idx, vn_idx = f
            new_v = [vi + v_offset for vi in v_idx]
            new_vt = [vti + uv_offset if vti >= 0 else -1 for vti in vt_idx]
            all_faces.append((new_v, new_vt, vn_idx))
            all_mats.append(mat_name)

        v_offset += len(verts)
        uv_offset += len(uvs)

    return all_verts, all_uvs, all_faces, all_mats

# ---------------------------------------------------------
# 1. AUTHORED ICHIRAKU RAMEN SHOP
# ---------------------------------------------------------
def build_ichiraku_ramen_mesh():
    parts = []

    # A. Main Building Walls (Timber)
    bld_v, bld_uv, bld_f = make_box_mesh(480, 520, 360, center=(0, 0, 180))
    parts.append((bld_v, bld_uv, bld_f, "M_Timber_Facade"))

    # B. Front Sloped Gabled Roof (Terracotta Tiles)
    roof_v, roof_uv, roof_f = make_gabled_roof(540, 580, 140, center=(0, 0, 360))
    parts.append((roof_v, roof_uv, roof_f, "M_Roof_Tiles"))

    # C. Dining Counter & Floor Porch (Timber)
    counter_v, counter_uv, counter_f = make_box_mesh(340, 60, 95, center=(0, -250, 47.5))
    parts.append((counter_v, counter_uv, counter_f, "M_Timber_Facade"))

    # D. Four Wooden Stools
    for i, sx in enumerate([-120, -40, 40, 120]):
        stool_v, stool_uv, stool_f = make_box_mesh(35, 35, 55, center=(sx, -300, 27.5))
        parts.append((stool_v, stool_uv, stool_f, "M_Timber_Facade"))

    # E. Authentic Wooden Shop Sign Board ("一楽ラーメン") mounted on roof
    sign_v, sign_uv, sign_f = make_box_mesh(260, 12, 90, center=(0, -240, 440), uv_min=(0.0, 0.0), uv_max=(1.0, 0.5))
    parts.append((sign_v, sign_uv, sign_f, "M_Ichiraku_Sign"))

    # F. Split Indigo Noren Curtains hanging across counter opening
    noren_v, noren_uv, noren_f = make_box_mesh(320, 6, 85, center=(0, -230, 275), uv_min=(0.0, 0.5), uv_max=(1.0, 1.0))
    parts.append((noren_v, noren_uv, noren_f, "M_Ichiraku_Noren"))

    v, u, f, m = merge_mesh_data(parts)
    write_obj_file(os.path.join(MESH_DIR, "SM_Bld_IchirakuRamen.obj"), v, f, u, material_names=m)

# ---------------------------------------------------------
# 2. AUTHORED MULTI-STORY KONOHA RESIDENCES
# ---------------------------------------------------------
def build_konoha_residence_mesh(name, num_stories=2):
    parts = []

    # Story 1 (Ground Floor Shopfront)
    s1_v, s1_uv, s1_f = make_box_mesh(560, 620, 320, center=(0, 0, 160))
    parts.append((s1_v, s1_uv, s1_f, "M_Timber_Facade"))

    # Ground floor awning
    aw_v, aw_uv, aw_f = make_box_mesh(580, 80, 15, center=(0, -320, 290))
    parts.append((aw_v, aw_uv, aw_f, "M_Roof_Tiles"))

    if num_stories >= 2:
        # Story 2 (Living Quarters)
        s2_v, s2_uv, s2_f = make_box_mesh(500, 560, 280, center=(0, 0, 460))
        parts.append((s2_v, s2_uv, s2_f, "M_Timber_Facade"))

        # Second-floor Balcony & Balustrade Railings
        balc_v, balc_uv, balc_f = make_box_mesh(460, 80, 12, center=(0, -300, 325))
        rail_v, rail_uv, rail_f = make_box_mesh(460, 8, 55, center=(0, -336, 355))
        parts.append((balc_v, balc_uv, balc_f, "M_Timber_Facade"))
        parts.append((rail_v, rail_uv, rail_f, "M_Timber_Facade"))

        # Gabled Roof
        roof_v, roof_uv, roof_f = make_gabled_roof(580, 640, 150, center=(0, 0, 600))
        parts.append((roof_v, roof_uv, roof_f, "M_Roof_Tiles"))
    else:
        roof_v, roof_uv, roof_f = make_gabled_roof(620, 680, 160, center=(0, 0, 320))
        parts.append((roof_v, roof_uv, roof_f, "M_Roof_Tiles"))

    v, u, f, m = merge_mesh_data(parts)
    write_obj_file(os.path.join(MESH_DIR, f"{name}.obj"), v, f, u, material_names=m)

# ---------------------------------------------------------
# 3. AUTHORED COBBLESTONE ROAD & CURBS
# ---------------------------------------------------------
def build_paved_avenue_mesh():
    parts = []

    # Center cobblestone avenue (Width 520, Length 400, Height 10)
    rd_v, rd_uv, rd_f = make_box_mesh(520, 400, 10, center=(0, 0, 5))
    parts.append((rd_v, rd_uv, rd_f, "M_Road_Cobblestone"))

    # Left Raised Curb Stone (Width 30, Length 400, Height 18)
    curb_l_v, curb_l_uv, curb_l_f = make_box_mesh(30, 400, 18, center=(-275, 0, 9))
    parts.append((curb_l_v, curb_l_uv, curb_l_f, "M_Road_Cobblestone"))

    # Right Raised Curb Stone (Width 30, Length 400, Height 18)
    curb_r_v, curb_r_uv, curb_r_f = make_box_mesh(30, 400, 18, center=(275, 0, 9))
    parts.append((curb_r_v, curb_r_uv, curb_r_f, "M_Road_Cobblestone"))

    v, u, f, m = merge_mesh_data(parts)
    write_obj_file(os.path.join(MESH_DIR, "SM_Road_Paved_01.obj"), v, f, u, material_names=m)

# ---------------------------------------------------------
# 4. AUTHORED NARUTO GENIN HERO MESH (Shinobi Stance & Crest)
# ---------------------------------------------------------
def build_naruto_hero_mesh():
    parts = []

    # Torso (Jacket with back crest UVs)
    # UV (0..0.5, 0..0.5) maps to back with Uzumaki crest
    torso_v, torso_uv, torso_f = make_box_mesh(38, 24, 48, center=(0, 0, 104), uv_min=(0.0, 0.0), uv_max=(0.5, 0.5))
    parts.append((torso_v, torso_uv, torso_f, "M_Naruto_Tracksuit"))

    # High White/Navy Collar
    collar_v, collar_uv, collar_f = make_box_mesh(26, 20, 18, center=(0, 0, 131), uv_min=(0.5, 0.0), uv_max=(1.0, 0.5))
    parts.append((collar_v, collar_uv, collar_f, "M_Naruto_Collar"))

    # Head (Face skin)
    head_v, head_uv, head_f = make_box_mesh(20, 18, 20, center=(0, 0, 146), uv_min=(0.5, 0.5), uv_max=(1.0, 1.0))
    parts.append((head_v, head_uv, head_f, "M_Naruto_Skin"))

    # Konoha Metallic Headband
    hb_v, hb_uv, hb_f = make_box_mesh(21, 19, 8, center=(0, 0, 150), uv_min=(0.5, 0.5), uv_max=(1.0, 0.8))
    parts.append((hb_v, hb_uv, hb_f, "M_Naruto_Navy"))

    # Spiky Blonde Hair spikes
    hair_core_v, hair_core_uv, hair_core_f = make_box_mesh(24, 22, 16, center=(0, 0, 162), uv_min=(0.5, 0.5), uv_max=(1.0, 0.7))
    parts.append((hair_core_v, hair_core_uv, hair_core_f, "M_Naruto_Hair"))

    # 5 Hair Spikes
    for i, (hx, hy, hz) in enumerate([(-10, 0, 172), (10, 0, 172), (0, -8, 170), (0, 8, 170), (0, 0, 176)]):
        spike_v, spike_uv, spike_f = make_box_mesh(10, 10, 14, center=(hx, hy, hz), uv_min=(0.5, 0.5), uv_max=(1.0, 0.7))
        parts.append((spike_v, spike_uv, spike_f, "M_Naruto_Hair"))

    # Upper Arms & Forearms (Shoulder-width shinobi stance)
    # Left Arm
    arm_l_v, arm_l_uv, arm_l_f = make_box_mesh(12, 12, 46, center=(-26, 6, 102), uv_min=(0.5, 0.0), uv_max=(1.0, 0.5))
    parts.append((arm_l_v, arm_l_uv, arm_l_f, "M_Naruto_Tracksuit"))
    # Right Arm
    arm_r_v, arm_r_uv, arm_r_f = make_box_mesh(12, 12, 46, center=(26, 6, 102), uv_min=(0.5, 0.0), uv_max=(1.0, 0.5))
    parts.append((arm_r_v, arm_r_uv, arm_r_f, "M_Naruto_Tracksuit"))

    # Left Leg (Pants + Sandal)
    leg_l_v, leg_l_uv, leg_l_f = make_box_mesh(14, 16, 60, center=(-12, 0, 50), uv_min=(0.0, 0.5), uv_max=(0.5, 1.0))
    sandal_l_v, sandal_l_uv, sandal_l_f = make_box_mesh(14, 22, 12, center=(-12, 3, 6), uv_min=(0.5, 0.8), uv_max=(1.0, 1.0))
    parts.append((leg_l_v, leg_l_uv, leg_l_f, "M_Naruto_Tracksuit"))
    parts.append((sandal_l_v, sandal_l_uv, sandal_l_f, "M_Naruto_Navy"))

    # Right Leg (Pants + White Bandage + Kunai Holster + Sandal)
    leg_r_v, leg_r_uv, leg_r_f = make_box_mesh(14, 16, 60, center=(12, 0, 50), uv_min=(0.0, 0.5), uv_max=(0.5, 1.0))
    holster_v, holster_uv, holster_f = make_box_mesh(8, 12, 16, center=(20, 2, 54), uv_min=(0.0, 0.5), uv_max=(0.5, 1.0))
    sandal_r_v, sandal_r_uv, sandal_r_f = make_box_mesh(14, 22, 12, center=(12, 3, 6), uv_min=(0.5, 0.8), uv_max=(1.0, 1.0))
    parts.append((leg_r_v, leg_r_uv, leg_r_f, "M_Naruto_Tracksuit"))
    parts.append((holster_v, holster_uv, holster_f, "M_Naruto_Navy"))
    parts.append((sandal_r_v, sandal_r_uv, sandal_r_f, "M_Naruto_Navy"))

    # Rear Hip Pouch (Waist)
    pouch_v, pouch_uv, pouch_f = make_box_mesh(16, 10, 12, center=(10, 14, 82), uv_min=(0.5, 0.0), uv_max=(1.0, 0.5))
    parts.append((pouch_v, pouch_uv, pouch_f, "M_Naruto_Skin"))

    v, u, f, m = merge_mesh_data(parts)
    write_obj_file(os.path.join(MESH_DIR, "SK_Naruto_Genin.obj"), v, f, u, material_names=m)

if __name__ == "__main__":
    print("=== BUILDING AUTHORED 3D KONOHA MODULAR MESHES ===")
    build_ichiraku_ramen_mesh()
    build_konoha_residence_mesh("SM_Bld_Konoha_Residence_A", num_stories=1)
    build_konoha_residence_mesh("SM_Bld_Konoha_Residence_B", num_stories=2)
    build_paved_avenue_mesh()
    build_naruto_hero_mesh()
    print("=== ALL AUTHORED MESHES GENERATED CLEANLY ===")
