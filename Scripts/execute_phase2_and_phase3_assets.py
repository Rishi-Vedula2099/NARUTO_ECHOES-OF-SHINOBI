"""
NARUTO: ECHOES OF SHINOBI
Phase 2 & Phase 3: High-Fidelity Character & Architectural Asset Authoring
=========================================================================
Pure standard library Python implementation (zero external dependencies).
"""

import os
import math
import unreal

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SRC_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
os.makedirs(SRC_DIR, exist_ok=True)

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

# =====================================================================
# VECTOR MATH HELPERS (PURE PYTHON)
# =====================================================================

def vec_sub(a, b):
    return [a[0] - b[0], a[1] - b[1], a[2] - b[2]]

def vec_cross(a, b):
    return [
        a[1] * b[2] - a[2] * b[1],
        a[2] * b[0] - a[0] * b[2],
        a[0] * b[1] - a[1] * b[0]
    ]

def vec_norm(v):
    return math.sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2])

def vec_normalize(v):
    n = vec_norm(v)
    if n > 1e-6:
        return [v[0] / n, v[1] / n, v[2] / n]
    return [0.0, 0.0, 1.0]

# =====================================================================
# GEOMETRY BUILDERS
# =====================================================================

def create_smooth_loft(radii_list, z_list, center=(0, 0, 0), segments=16):
    cx, cy, cz = center
    verts = []
    num_rings = len(z_list)
    
    for r_pair, z in zip(radii_list, z_list):
        rx, ry = r_pair if isinstance(r_pair, (tuple, list)) else (r_pair, r_pair)
        for i in range(segments):
            theta = 2.0 * math.pi * i / segments
            x = rx * math.cos(theta) + cx
            y = ry * math.sin(theta) + cy
            verts.append([x, y, z + cz])
            
    bottom_cap_idx = len(verts)
    verts.append([cx, cy, z_list[0] + cz])
    top_cap_idx = len(verts)
    verts.append([cx, cy, z_list[-1] + cz])
    
    faces = []
    for ring in range(num_rings - 1):
        r0 = ring * segments
        r1 = (ring + 1) * segments
        for i in range(segments):
            next_i = (i + 1) % segments
            v0 = r0 + i
            v1 = r0 + next_i
            v2 = r1 + next_i
            v3 = r1 + i
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])
            
    for i in range(segments):
        next_i = (i + 1) % segments
        faces.append([bottom_cap_idx, next_i, i])
        
    top_r = (num_rings - 1) * segments
    for i in range(segments):
        next_i = (i + 1) % segments
        faces.append([top_cap_idx, top_r + i, top_r + next_i])
        
    normals = [[0.0, 0.0, 0.0] for _ in range(len(verts))]
    for f in faces:
        v0, v1, v2 = verts[f[0]], verts[f[1]], verts[f[2]]
        fn = vec_cross(vec_sub(v1, v0), vec_sub(v2, v0))
        n = vec_norm(fn)
        if n > 1e-6:
            fn = [fn[0] / n, fn[1] / n, fn[2] / n]
            for idx in f:
                normals[idx][0] += fn[0]
                normals[idx][1] += fn[1]
                normals[idx][2] += fn[2]
                
    for i in range(len(normals)):
        normals[i] = vec_normalize(normals[i])
        
    return verts, faces, normals

def create_stylized_spike(base_rx, base_ry, height, apex_offset=(0, 0, 0), base_center=(0, 0, 0), segments=8):
    bx, by, bz = base_center
    ax, ay, az = apex_offset
    verts = []
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([base_rx * math.cos(th) + bx, base_ry * math.sin(th) + by, bz])
    apex_idx = len(verts)
    verts.append([bx + ax, by + ay, bz + height + az])
    base_idx = len(verts)
    verts.append([bx, by, bz])
    
    faces = []
    for i in range(segments):
        next_i = (i + 1) % segments
        faces.append([i, next_i, apex_idx])
        faces.append([base_idx, i, next_i])
        
    normals = [[0.0, 0.0, 0.0] for _ in range(len(verts))]
    for f in faces:
        v0, v1, v2 = verts[f[0]], verts[f[1]], verts[f[2]]
        fn = vec_cross(vec_sub(v1, v0), vec_sub(v2, v0))
        n = vec_norm(fn)
        if n > 1e-6:
            fn = [fn[0] / n, fn[1] / n, fn[2] / n]
            for idx in f:
                normals[idx][0] += fn[0]
                normals[idx][1] += fn[1]
                normals[idx][2] += fn[2]
    for i in range(len(normals)):
        normals[i] = vec_normalize(normals[i])
        
    return verts, faces, normals

def write_multi_group_obj(filepath, section_dict):
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Stylized Production Mesh: {os.path.basename(filepath)}\n")
        f.write("o " + os.path.splitext(os.path.basename(filepath))[0] + "\n")
        
        all_verts = []
        all_normals = []
        
        for group_name, parts in section_dict.items():
            for verts, faces, norms in parts:
                for v in verts:
                    all_verts.append(v)
                for n in norms:
                    all_normals.append(n)
                    
        for v in all_verts:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
        for n in all_normals:
            f.write(f"vn {n[0]:.4f} {n[1]:.4f} {n[2]:.4f}\n")
            
        f.write("vt 0.0000 0.0000\n")
        f.write("vt 1.0000 0.0000\n")
        f.write("vt 1.0000 1.0000\n")
        f.write("vt 0.0000 1.0000\n")
        
        curr_v_offset = 1
        for group_name, parts in section_dict.items():
            f.write(f"usemtl {group_name}\n")
            for verts, faces, norms in parts:
                for face in faces:
                    i0 = face[0] + curr_v_offset
                    i1 = face[1] + curr_v_offset
                    i2 = face[2] + curr_v_offset
                    f.write(f"f {i0}/1/{i0} {i1}/2/{i1} {i2}/3/{i2}\n")
                curr_v_offset += len(verts)

    print(f"Authored production OBJ written: {filepath}")

# =====================================================================
# NARUTO MESH
# =====================================================================

def build_naruto_anatomical_mesh():
    sections = {
        "Skin": [],
        "Headband": [],
        "Hair": [],
        "Collar": [],
        "Tracksuit": []
    }
    
    # 1. SKIN (Face, Chin, Ears, Neck, Hands)
    head_z = [124.0, 127.0, 131.0, 136.0, 140.0, 143.0]
    head_r = [(6.5, 7.5), (9.0, 9.5), (11.0, 10.5), (11.2, 10.8), (10.5, 10.2), (8.5, 8.5)]
    sections["Skin"].append(create_smooth_loft(head_r, head_z, center=(0, 0, 0), segments=16))
    
    neck_z = [116.0, 120.0, 124.0]
    neck_r = [(5.2, 5.0), (5.0, 4.8), (5.2, 5.0)]
    sections["Skin"].append(create_smooth_loft(neck_r, neck_z, center=(0, -0.5, 0), segments=12))
    
    ear_z = [131.0, 134.0, 137.0]
    ear_r = [(1.5, 2.5), (1.8, 3.0), (1.2, 2.0)]
    sections["Skin"].append(create_smooth_loft(ear_r, ear_z, center=(-11.2, 0, 0), segments=8))
    sections["Skin"].append(create_smooth_loft(ear_r, ear_z, center=(11.2, 0, 0), segments=8))
    
    hand_z = [72.0, 75.0, 78.0]
    hand_r = [(2.5, 3.2), (2.8, 3.5), (2.4, 3.0)]
    sections["Skin"].append(create_smooth_loft(hand_r, hand_z, center=(-20.5, 0.5, 0), segments=10))
    sections["Skin"].append(create_smooth_loft(hand_r, hand_z, center=(20.5, 0.5, 0), segments=10))
    
    # 2. HEADBAND & NAVY ACCENTS
    band_z = [136.0, 138.0, 140.0]
    band_r = [(11.6, 11.2), (11.7, 11.3), (11.5, 11.0)]
    sections["Headband"].append(create_smooth_loft(band_r, band_z, center=(0, 0, 0), segments=16))
    
    plate_z = [136.2, 138.0, 139.8]
    plate_r = [(7.0, 1.2), (7.2, 1.4), (7.0, 1.2)]
    sections["Headband"].append(create_smooth_loft(plate_r, plate_z, center=(0, 11.4, 0), segments=12))
    
    ribbon_z = [122.0, 128.0, 134.0]
    ribbon_r = [(1.8, 0.8), (1.6, 0.8), (1.4, 0.8)]
    sections["Headband"].append(create_smooth_loft(ribbon_r, ribbon_z, center=(-2.5, -11.6, 0), segments=8))
    sections["Headband"].append(create_smooth_loft(ribbon_r, ribbon_z, center=(2.5, -11.6, 0), segments=8))
    
    yoke_z = [110.0, 114.0, 118.0]
    yoke_r = [(17.5, 10.5), (18.2, 11.0), (16.5, 10.2)]
    sections["Headband"].append(create_smooth_loft(yoke_r, yoke_z, center=(0, 0, 0), segments=16))
    
    sleeve_z = [102.0, 108.0, 114.0]
    sleeve_r = [(5.4, 5.0), (5.8, 5.4), (6.0, 5.5)]
    sections["Headband"].append(create_smooth_loft(sleeve_r, sleeve_z, center=(-18.5, 0, 0), segments=10))
    sections["Headband"].append(create_smooth_loft(sleeve_r, sleeve_z, center=(18.5, 0, 0), segments=10))
    
    holster_z = [52.0, 56.0, 60.0]
    holster_r = [(2.5, 4.5), (2.8, 4.8), (2.5, 4.5)]
    sections["Headband"].append(create_smooth_loft(holster_r, holster_z, center=(14.2, 0, 0), segments=8))
    
    ankle_z = [6.0, 10.0, 14.0]
    ankle_r = [(5.2, 5.0), (5.0, 4.8), (5.2, 5.0)]
    sections["Headband"].append(create_smooth_loft(ankle_r, ankle_z, center=(-7.5, 0, 0), segments=10))
    sections["Headband"].append(create_smooth_loft(ankle_r, ankle_z, center=(7.5, 0, 0), segments=10))
    
    foot_z = [0.0, 3.0, 5.0]
    foot_r = [(4.5, 8.5), (4.8, 9.0), (4.5, 8.5)]
    sections["Headband"].append(create_smooth_loft(foot_r, foot_z, center=(-7.5, 3.0, 0), segments=12))
    sections["Headband"].append(create_smooth_loft(foot_r, foot_z, center=(7.5, 3.0, 0), segments=12))
    
    # 3. SPIKY BLONDE HAIR
    hair_crown_z = [140.0, 144.0, 147.0]
    hair_crown_r = [(11.2, 10.8), (10.5, 10.0), (8.5, 8.0)]
    sections["Hair"].append(create_smooth_loft(hair_crown_r, hair_crown_z, center=(0, -1.0, 0), segments=14))
    
    hair_spikes = [
        (4.5, 4.0, 15.0, (0, 3, 5), (0, 7, 143)),
        (4.0, 3.8, 14.0, (-3, 3, 4), (-6, 6, 143)),
        (4.0, 3.8, 14.0, (3, 3, 4), (6, 6, 143)),
        (4.5, 4.5, 18.0, (0, 0, 8), (0, 0, 147)),
        (4.2, 4.0, 16.0, (-4, -1, 7), (-5, 0, 146)),
        (4.2, 4.0, 16.0, (4, -1, 7), (5, 0, 146)),
        (4.2, 4.0, 15.0, (-6, -3, 4), (-8, -3, 142)),
        (4.2, 4.0, 15.0, (6, -3, 4), (8, -3, 142)),
        (3.8, 3.5, 13.0, (-7, -2, 0), (-9, -2, 136)),
        (3.8, 3.5, 13.0, (7, -2, 0), (9, -2, 136)),
        (4.0, 3.8, 14.0, (0, -6, 5), (0, -7, 143)),
        (3.8, 3.6, 14.0, (-4, -6, 4), (-5, -6, 141)),
        (3.8, 3.6, 14.0, (4, -6, 4), (5, -6, 141)),
        (3.6, 3.4, 12.0, (0, -5, -2), (0, -8, 134)),
    ]
    for rx, ry, h, a_off, b_cen in hair_spikes:
        sections["Hair"].append(create_stylized_spike(rx, ry, h, apex_offset=a_off, base_center=b_cen, segments=8))
        
    # 4. COLLAR & WHITE ACCENTS
    collar_z = [116.0, 120.0, 125.0]
    collar_r = [(7.5, 7.0), (9.0, 8.5), (11.0, 10.0)]
    sections["Collar"].append(create_smooth_loft(collar_r, collar_z, center=(0, 0.5, 0), segments=14))
    
    bandage_z = [50.0, 56.0, 62.0]
    bandage_r = [(6.9, 6.1), (7.0, 6.2), (6.8, 6.0)]
    sections["Collar"].append(create_smooth_loft(bandage_r, bandage_z, center=(7.5, 0, 0), segments=12))
    
    swirl_z = [103.0, 106.0, 109.0]
    swirl_r = [(5.5, 1.2), (5.8, 1.4), (5.5, 1.2)]
    sections["Collar"].append(create_smooth_loft(swirl_r, swirl_z, center=(0, -10.4, 0), segments=12))
    
    # 5. TRACKSUIT
    torso_z = [80.0, 86.0, 94.0, 102.0, 110.0]
    torso_r = [(16.0, 9.8), (15.2, 9.2), (14.8, 8.8), (16.2, 9.5), (17.2, 10.2)]
    sections["Tracksuit"].append(create_smooth_loft(torso_r, torso_z, center=(0, 0, 0), segments=16))
    
    arm_z = [78.0, 88.0, 102.0]
    arm_r = [(4.6, 4.4), (5.0, 4.8), (5.4, 5.0)]
    sections["Tracksuit"].append(create_smooth_loft(arm_r, arm_z, center=(-20.0, 0, 0), segments=10))
    sections["Tracksuit"].append(create_smooth_loft(arm_r, arm_z, center=(20.0, 0, 0), segments=10))
    
    hips_z = [68.0, 74.0, 80.0]
    hips_r = [(15.0, 9.5), (15.5, 9.8), (15.8, 10.0)]
    sections["Tracksuit"].append(create_smooth_loft(hips_r, hips_z, center=(0, 0, 0), segments=16))
    
    thigh_z = [44.0, 56.0, 68.0]
    thigh_r = [(6.2, 5.6), (6.6, 5.9), (6.8, 6.0)]
    sections["Tracksuit"].append(create_smooth_loft(thigh_r, thigh_z, center=(-7.5, 0, 0), segments=12))
    sections["Tracksuit"].append(create_smooth_loft(thigh_r, thigh_z, center=(7.5, 0, 0), segments=12))
    
    shin_z = [14.0, 28.0, 44.0]
    shin_r = [(5.4, 5.0), (5.8, 5.3), (6.1, 5.5)]
    sections["Tracksuit"].append(create_smooth_loft(shin_r, shin_z, center=(-7.5, 0, 0), segments=12))
    sections["Tracksuit"].append(create_smooth_loft(shin_r, shin_z, center=(7.5, 0, 0), segments=12))
    
    obj_path = os.path.join(SRC_DIR, "SK_Naruto_Genin.obj")
    write_multi_group_obj(obj_path, sections)
    return obj_path

# =====================================================================
# ICHIRAKU RAMEN MESH
# =====================================================================

def build_ichiraku_ramen_production():
    sections = {
        "M_Timber_Facade": [],
        "M_Roof_Tiles": [],
        "M_Ichiraku_Sign": [],
        "M_Ichiraku_Noren": []
    }
    
    bld_z = [0.0, 180.0, 360.0]
    bld_r = [(240.0, 260.0), (240.0, 260.0), (240.0, 260.0)]
    sections["M_Timber_Facade"].append(create_smooth_loft(bld_r, bld_z, center=(0, 0, 0), segments=16))
    
    counter_z = [0.0, 47.5, 95.0]
    counter_r = [(170.0, 30.0), (170.0, 30.0), (170.0, 30.0)]
    sections["M_Timber_Facade"].append(create_smooth_loft(counter_r, counter_z, center=(0, -250, 0), segments=12))
    
    stool_z = [0.0, 27.5, 55.0]
    stool_r = [(18.0, 18.0), (17.5, 17.5), (18.0, 18.0)]
    for sx in [-120, -40, 40, 120]:
        sections["M_Timber_Facade"].append(create_smooth_loft(stool_r, stool_z, center=(sx, -300, 0), segments=10))
        
    roof_z = [360.0, 420.0, 500.0]
    roof_r = [(270.0, 290.0), (240.0, 260.0), (210.0, 230.0)]
    sections["M_Roof_Tiles"].append(create_smooth_loft(roof_r, roof_z, center=(0, 0, 0), segments=16))
    
    sign_z = [390.0, 435.0, 480.0]
    sign_r = [(130.0, 6.0), (130.0, 6.0), (130.0, 6.0)]
    sections["M_Ichiraku_Sign"].append(create_smooth_loft(sign_r, sign_z, center=(0, -240, 0), segments=8))
    
    noren_z = [230.0, 270.0, 315.0]
    noren_r = [(160.0, 4.0), (160.0, 4.0), (160.0, 4.0)]
    sections["M_Ichiraku_Noren"].append(create_smooth_loft(noren_r, noren_z, center=(0, -230, 0), segments=8))
    
    obj_path = os.path.join(SRC_DIR, "SM_Bld_IchirakuRamen.obj")
    write_multi_group_obj(obj_path, sections)
    return obj_path

# =====================================================================
# IMPORT & SETUP IN UE5
# =====================================================================

unreal.log("=== Generating Authored Anatomical Naruto & Ichiraku Ramen Meshes ===")
naruto_obj = build_naruto_anatomical_mesh()
ichiraku_obj = build_ichiraku_ramen_production()

# Import SK_Naruto_Genin
dest_dir = "/Game/Characters/Naruto/Meshes"
asset_name = "SK_Naruto_Genin"
pkg_path = f"{dest_dir}/{asset_name}"

task = unreal.AssetImportTask()
task.filename = naruto_obj
task.destination_path = dest_dir
task.destination_name = asset_name
task.replace_existing = True
task.automated = True
task.save = True

opts = unreal.FbxImportUI()
opts.import_mesh = True
opts.import_as_skeletal = False
opts.import_materials = False
opts.import_textures = False
opts.static_mesh_import_data.combine_meshes = True
try:
    opts.static_mesh_import_data.normal_import_method = unreal.FBXNormalImportMethod.FBXNIM_IMPORT_NORMALS
except Exception:
    pass
task.options = opts

AT.import_asset_tasks([task])
unreal.log(f"-> Imported {pkg_path}")

mesh = unreal.load_asset(pkg_path)
if mesh and isinstance(mesh, unreal.StaticMesh):
    mi_skin      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")
    mi_navy      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Navy")
    mi_hair      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Hair")
    mi_collar    = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Collar")
    mi_tracksuit = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
    
    mats = [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit]
    num_sections = mesh.get_num_sections(0)
    unreal.log(f"Binding {num_sections} sections of Naruto to anime cel materials:")
    for i in range(num_sections):
        mat = mats[i % len(mats)]
        mesh.set_material(i, mat)
        unreal.log(f"  Slot {i}: {mat.get_name() if mat else 'None'}")
    EAL.save_asset(mesh.get_path_name())

    # Update skin duplicate
    skin_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
    if EAL.does_asset_exist(skin_path):
        EAL.delete_asset(skin_path)
    EAL.duplicate_asset(pkg_path, skin_path)
    skin_mesh = unreal.load_asset(skin_path)
    if skin_mesh and isinstance(skin_mesh, unreal.StaticMesh):
        for i in range(skin_mesh.get_num_sections(0)):
            skin_mesh.set_material(i, mats[i % len(mats)])
        EAL.save_asset(skin_mesh.get_path_name())
    unreal.log("-> Updated SK_Naruto_GeninTracksuit")

# Import SM_Bld_IchirakuRamen
bld_dest = "/Game/Environment/Konoha/Buildings/Meshes"
bld_name = "SM_Bld_IchirakuRamen"
bld_pkg = f"{bld_dest}/{bld_name}"

task_bld = unreal.AssetImportTask()
task_bld.filename = ichiraku_obj
task_bld.destination_path = bld_dest
task_bld.destination_name = bld_name
task_bld.replace_existing = True
task_bld.automated = True
task_bld.save = True

opts_bld = unreal.FbxImportUI()
opts_bld.import_mesh = True
opts_bld.import_as_skeletal = False
opts_bld.import_materials = False
opts_bld.import_textures = False
opts_bld.static_mesh_import_data.combine_meshes = True
task_bld.options = opts_bld

AT.import_asset_tasks([task_bld])
unreal.log(f"-> Imported {bld_pkg}")

bld_mesh = unreal.load_asset(bld_pkg)
if bld_mesh and isinstance(bld_mesh, unreal.StaticMesh):
    mi_wood  = unreal.load_asset("/Game/Environment/Konoha/Materials/MI_Bld_Ichiraku_Wood")
    mi_roof  = unreal.load_asset("/Game/Environment/Konoha/Materials/MI_Bld_Ichiraku_Roof")
    mi_sign  = unreal.load_asset("/Game/Environment/Konoha/Materials/MI_Bld_Ichiraku_Sign")
    mi_noren = unreal.load_asset("/Game/Environment/Konoha/Materials/MI_Bld_Ichiraku_Noren")
    bld_mats = [mi_wood, mi_roof, mi_sign, mi_noren]
    for i in range(bld_mesh.get_num_sections(0)):
        mat = bld_mats[i % len(bld_mats)]
        bld_mesh.set_material(i, mat)
        unreal.log(f"  Bld Slot {i}: {mat.get_name() if mat else 'None'}")
    EAL.save_asset(bld_mesh.get_path_name())

unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log("=== Phase 2 & 3 Asset Generation and Import Completed Successfully ===")
