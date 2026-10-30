"""
NARUTO: ECHOES OF SHINOBI
Master Import, Material Binding & Golden Street Assembly Script
===============================================================
1. Compiles/verifies all Material Instances:
   - Naruto: Skin, Hair, Tracksuit, Navy, Collar, Emblem, Metal
   - Foliage: Cedar Foliage (Pine Green), Sakura Bloom (Pink)
   - Architecture: Timber Facade, Roof Tiles, Noren, Road Cobblestones, Lantern Glow
2. Imports all authored 3D production meshes into UE5.
3. Binds the production materials to static mesh sections.
4. Duplicates SK_Naruto_Genin into SK_Naruto_GeninTracksuit with identical material bindings.
5. Calibrates all level transforms in /Game/Maps/GoldenTest/L_Konoha_GoldenStreet.
6. Saves the level.
"""

import os
import unreal

PROJECT_DIR = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI"
MESH_DIR    = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
MAP_PKG     = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

print("======================================================================")
print("  STEP 1: CREATING & VERIFYING ALL PRODUCTION MATERIAL INSTANCES")
print("======================================================================")

master_mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
tex_white  = unreal.load_asset("/Game/Materials/Textures/T_Solid_White")
tex_road   = unreal.load_asset("/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")
tex_wood   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Facade_D")
tex_roof   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_RoofTiles_D")
tex_noren  = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Noren_D")
tex_lantern= unreal.load_asset("/Game/Environment/Konoha/Props/Textures/T_Lantern_Paper_D")

def configure_mi(pkg_path, base_color_rgba, diffuse_tex=None, roughness=0.82, specular=0.08):
    folder, name = pkg_path.rsplit("/", 1)
    if not EAL.does_directory_exist(folder):
        EAL.make_directory(folder)
        
    mi = unreal.load_asset(pkg_path)
    if not mi:
        factory_mi = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory_mi)
        
    if master_mat:
        MEL.set_material_instance_parent(mi, master_mat)
    MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color_rgba))
    
    # Use specified texture or solid white fallback (guaranteeing zero texture noise)
    target_tex = diffuse_tex if diffuse_tex else tex_white
    if target_tex:
        MEL.set_material_instance_texture_parameter_value(mi, "DiffuseTexture", target_tex)
        
    MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
    MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
    MEL.update_material_instance(mi)
    EAL.save_asset(mi.get_path_name())
    print(f"  [MI CONFIGURED] {pkg_path} -> RGB: {base_color_rgba[:3]}")
    return mi

# Character Materials (Zero micro-tiling: uses T_Solid_White)
mi_skin     = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), tex_white, roughness=0.65, specular=0.10)
mi_hair     = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",      (1.00, 0.84, 0.10, 1.0), tex_white, roughness=0.55, specular=0.35)
mi_tracksuit= configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit", (1.00, 0.45, 0.04, 1.0), tex_white, roughness=0.75, specular=0.08)
mi_navy     = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",      (0.06, 0.10, 0.22, 1.0), tex_white, roughness=0.80, specular=0.15)
mi_collar   = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",    (0.95, 0.95, 0.95, 1.0), tex_white, roughness=0.70, specular=0.05)
mi_emblem   = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",    (0.85, 0.08, 0.08, 1.0), tex_white, roughness=0.60, specular=0.10) # Crimson Uzumaki Crest!
mi_metal    = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Metal",     (0.85, 0.88, 0.92, 1.0), tex_white, roughness=0.25, specular=0.90) # Leaf Metal Plate!

# Environment & Foliage Materials
mi_cedar    = configure_mi("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarFoliage", (0.08, 0.26, 0.10, 1.0), tex_white, roughness=0.85, specular=0.05) # Pine Green!
mi_sakura   = configure_mi("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_Sakura_Bloom",  (1.00, 0.72, 0.80, 1.0), tex_white, roughness=0.75, specular=0.10) # Pink Cherry Blossom!
mi_wood     = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood", (1.0, 1.0, 1.0, 1.0), tex_wood, roughness=0.85, specular=0.05)
mi_roof     = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof", (1.0, 1.0, 1.0, 1.0), tex_roof, roughness=0.80, specular=0.08)
mi_noren    = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren", (1.0, 1.0, 1.0, 1.0), tex_noren, roughness=0.85, specular=0.05)
mi_road     = configure_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",       (1.0, 1.0, 1.0, 1.0), tex_road, roughness=0.88, specular=0.05)
mi_lantern  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")

print("======================================================================")
print("  STEP 2: IMPORTING 3D PRODUCTION MESHES INTO UE5")
print("======================================================================")

def import_obj(filename, dest_path, asset_name):
    src_file = os.path.join(MESH_DIR, filename)
    if not os.path.exists(src_file):
        print(f"ERROR: Mesh file {src_file} does not exist!")
        return None
        
    full_asset_path = f"{dest_path}/{asset_name}"
    
    task = unreal.AssetImportTask()
    task.filename = src_file
    task.destination_path = dest_path
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
    opts.static_mesh_import_data.generate_lightmap_u_vs = False
    task.options = opts

    AT.import_asset_tasks([task])
    imported = unreal.load_asset(full_asset_path)
    if imported:
        print(f"  [IMPORTED] {full_asset_path} (Sections: {imported.get_num_sections(0)})")
    else:
        print(f"  [ERROR] Failed to import {full_asset_path}")
    return imported

# 1. Naruto Character
mesh_naruto = import_obj("SK_Naruto_Genin.obj", "/Game/Characters/Naruto/Meshes", "SK_Naruto_Genin")

# 2. Ichiraku Ramen
mesh_ichiraku = import_obj("SM_Bld_IchirakuRamen.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_IchirakuRamen")

# 3. Residences A & B
mesh_res_a = import_obj("SM_Bld_Konoha_Residence_A.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_A")
mesh_res_b = import_obj("SM_Bld_Konoha_Residence_B.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_B")

# 4. Cedar Tree
mesh_cedar = import_obj("SM_Veg_KonohaCedar_Large.obj", "/Game/Environment/Konoha/Foliage/Meshes", "SM_Veg_KonohaCedar_Large")

print("======================================================================")
print("  STEP 3: BINDING PRODUCTION MATERIALS TO MESH SECTIONS")
print("======================================================================")

def bind_slots(mesh_path, mat_list):
    mesh = unreal.load_asset(mesh_path)
    if not mesh or not isinstance(mesh, unreal.StaticMesh):
        print(f"  [WARN] Cannot bind materials, mesh not found: {mesh_path}")
        return
    num_sections = mesh.get_num_sections(0)
    for i in range(num_sections):
        m = mat_list[i % len(mat_list)]
        if m:
            mesh.set_material(i, m)
    EAL.save_asset(mesh_path)
    print(f"  [BOUND] {mesh_path} ({num_sections} sections)")

# Bind Naruto Mesh (Skin, Hair, Tracksuit, Navy, Collar, Emblem, Metal)
bind_slots("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", [
    mi_skin, mi_hair, mi_tracksuit, mi_navy, mi_collar, mi_emblem, mi_metal
])

# Duplicate to Skin path SK_Naruto_GeninTracksuit
skin_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
if EAL.does_asset_exist(skin_path):
    EAL.delete_asset(skin_path)
EAL.duplicate_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", skin_path)
bind_slots(skin_path, [
    mi_skin, mi_hair, mi_tracksuit, mi_navy, mi_collar, mi_emblem, mi_metal
])

# Bind Ichiraku Ramen (Timber, Roof, Sign, Noren, Lantern)
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", [
    mi_wood, mi_roof, mi_noren, mi_noren, mi_lantern
])

# Bind Residences
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", [mi_wood, mi_roof])
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", [mi_wood, mi_roof])
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",    [mi_wood, mi_roof])
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",  [mi_wood, mi_roof])
bind_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice", [mi_wood, mi_roof])

# Bind Cedar Foliage to both meshes
bind_slots("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", [mi_cedar, mi_wood])
cedar_copy_path = "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large"
if EAL.does_asset_exist(cedar_copy_path):
    EAL.delete_asset(cedar_copy_path)
EAL.duplicate_asset("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", cedar_copy_path)
bind_slots(cedar_copy_path, [mi_cedar, mi_wood])

# Bind Sakura Bloom
bind_slots("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom", [mi_sakura])

print("======================================================================")
print("  STEP 4: CALIBRATING LEVEL LAYOUT & HORIZON COMPOSITION")
print("======================================================================")

world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
if not world:
    print(f"ERROR: Could not load map {MAP_PKG}")
    quit()

for a in EAS.get_all_level_actors():
    if not a:
        continue
    label = a.get_actor_label()

    # 1. FOREGROUND LEFT: Ichiraku Ramen (Prominent open stall, 32 deg angle to street)
    if label == "Bld_IchirakuRamen":
        a.set_actor_location(unreal.Vector(-480.0, 180.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=32.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_ichiraku:
            smc.set_static_mesh(mesh_ichiraku)
            for si, m in enumerate([mi_wood, mi_roof, mi_noren, mi_noren, mi_lantern]):
                smc.set_material(si, m)
        print("  [CALIBRATED] Bld_IchirakuRamen at (-480, 180, 0) yaw=32.0")

    # 2. FOREGROUND RIGHT: Konoha Residence A (2-story with balcony, -32 deg angle)
    elif label == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(520.0, 220.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-32.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_res_a:
            smc.set_static_mesh(mesh_res_a)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
        print("  [CALIBRATED] Bld_KonohaResidence_A at (520, 220, 0) yaw=-32.0")

    # 3. MIDGROUND BUILDINGS (Stepped back to widen avenue perspective & create breathing room)
    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-660.0, 950.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=35.0, roll=0.0), False)

    elif label == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(680.0, 900.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-35.0, roll=0.0), False)

    elif label == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-720.0, 1700.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=25.0, roll=0.0), False)

    elif label == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(740.0, 1650.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-25.0, roll=0.0), False)

    # 4. BACKGROUND VISTA PLANE (Pure 16:9 ratio, base touches Z=0 at Y=4600)
    elif "northstar_vista" in label.lower():
        a.set_actor_location(unreal.Vector(0.0, 4600.0, 1700.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=90.0), False)
        a.set_actor_scale3d(unreal.Vector(60.44, 34.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")
        print("  [CALIBRATED] Backdrop_Konoha_NorthStar_Vista at Y=4600 Z=1700 (Scale 60.44 x 34.0)")

    # 5. SAKURA CHERRY BLOSSOMS (Overhanging canopy framing upper sky)
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-460.0, 460.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=60.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_sakura)

    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(500.0, 480.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-50.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_sakura)

    # 6. CEDAR TREES (Lush Pine Green Anime Foliage, No Black Geometry)
    elif "cedar" in label.lower():
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_cedar)
            if smc.get_num_materials() > 1:
                smc.set_material(1, mi_wood)

    # 7. NPCS & STREET LIFE
    elif label == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-490.0, 180.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=32.0, roll=0.0), False)

    elif label == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(400.0, 120.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-380.0, 680.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(400.0, 560.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    # 8. PROPS: Sidewalk perimeter placement
    elif label == "Prop_StoneLantern_Left_01":
        a.set_actor_location(unreal.Vector(-340.0, 80.0, 0.0), False, False)

    elif label == "Prop_StoneLantern_Right_01":
        a.set_actor_location(unreal.Vector(340.0, 80.0, 0.0), False, False)

    elif label == "Prop_Bench_Left_01":
        a.set_actor_location(unreal.Vector(-420.0, 600.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "Prop_Bench_Right_01":
        a.set_actor_location(unreal.Vector(420.0, 360.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "Prop_UtilityPole_Right_01":
        a.set_actor_location(unreal.Vector(460.0, 60.0, 0.0), False, False)

    elif label == "Prop_UtilityPole_Left_01":
        a.set_actor_location(unreal.Vector(-460.0, 650.0, 0.0), False, False)

# Save level and dirty packages cleanly
save_res = unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
print(f"======================================================================")
print(f"  >>> PERFECT REPLICATION SETUP COMPLETE & SAVED: {save_res} <<<")
print(f"======================================================================")
