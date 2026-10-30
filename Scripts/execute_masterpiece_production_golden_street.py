"""
NARUTO: ECHOES OF SHINOBI
Masterpiece Production Golden Street Execution & Integration Script
===================================================================
Re-imports meshes, binds materials with world-unit UVs, and assembles the Golden Street.
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
print("  STEP 1: LOAD ALL PRODUCTION MATERIAL INSTANCES")
print("======================================================================")

mi_skin     = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")
mi_navy     = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Navy")
mi_hair     = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Hair")
mi_collar   = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Collar")
mi_tracksuit= unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
mi_emblem   = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Emblem")
mi_metal    = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Metal")

mi_cedar    = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarFoliage")
mi_sakura   = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_Sakura_Bloom")
mi_wood     = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_roof     = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_noren    = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
mi_road     = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_lantern  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")

print("======================================================================")
print("  STEP 2: IMPORT PRODUCTION 3D MESHES INTO UE5")
print("======================================================================")

def import_production_mesh(filename, dest_path, asset_name):
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
        print(f"  [IMPORTED SUCCESS] {full_asset_path} ({imported.get_num_sections(0)} sections)")
    else:
        print(f"  [IMPORT FAILED] {full_asset_path}")
    return imported

mesh_naruto      = import_production_mesh("SK_Naruto_Genin.obj", "/Game/Characters/Naruto/Meshes", "SK_Naruto_Genin")
mesh_naruto_skin = import_production_mesh("SK_Naruto_GeninTracksuit.obj", "/Game/Characters/Naruto/Skins", "SK_Naruto_GeninTracksuit")

mesh_ichiraku = import_production_mesh("SM_Bld_IchirakuRamen.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_IchirakuRamen")
mesh_res_a    = import_production_mesh("SM_Bld_Konoha_Residence_A.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_A")
mesh_res_b    = import_production_mesh("SM_Bld_Konoha_Residence_B.obj", "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_B")

mesh_cedar      = import_production_mesh("SM_Veg_KonohaCedar_Large.obj", "/Game/Environment/Konoha/Foliage/Meshes", "SM_Veg_KonohaCedar_Large")
mesh_cedar_copy = import_production_mesh("SM_Veg_Cedar_Large.obj", "/Game/Environment/Konoha/Foliage/Meshes", "SM_Veg_Cedar_Large")
mesh_sakura     = import_production_mesh("SM_Veg_Sakura_Bloom.obj", "/Game/Environment/Konoha/Foliage/Meshes", "SM_Veg_Sakura_Bloom")

print("======================================================================")
print("  STEP 3: BIND MATERIALS PRECISELY BY MATERIAL SLOT NAME")
print("======================================================================")

def bind_by_slot_name(mesh_path, slot_map, fallback_list=None):
    mesh = unreal.load_asset(mesh_path)
    if not mesh or not isinstance(mesh, unreal.StaticMesh):
        print(f"  [WARN] Mesh not found: {mesh_path}")
        return
    for i, sm in enumerate(mesh.static_materials):
        name = str(sm.material_slot_name)
        bound = False
        for k, v in slot_map.items():
            if k.lower() in name.lower() and v:
                mesh.set_material(i, v)
                print(f"  [{mesh.get_name()}] Slot {i} ('{name}') -> {v.get_name()}")
                bound = True
                break
        if not bound:
            if fallback_list and i < len(fallback_list) and fallback_list[i]:
                mesh.set_material(i, fallback_list[i])
                print(f"  [{mesh.get_name()}] Slot {i} ('{name}') [FALLBACK] -> {fallback_list[i].get_name()}")
            else:
                print(f"  [WARN] [{mesh.get_name()}] Unmapped Slot {i} ('{name}')")
    EAL.save_asset(mesh_path)

# Naruto
naruto_map = {
    "Skin":      mi_skin,
    "Navy":      mi_navy,
    "Hair":      mi_hair,
    "Collar":    mi_collar,
    "Tracksuit": mi_tracksuit,
    "Emblem":    mi_emblem,
    "Metal":     mi_metal
}
naruto_fallback = [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit, mi_emblem, mi_metal]
bind_by_slot_name("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", naruto_map, naruto_fallback)
bind_by_slot_name("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit", naruto_map, naruto_fallback)

# Ichiraku Ramen
ichiraku_map = {
    "Wood":    mi_wood,
    "Roof":    mi_roof,
    "Noren":   mi_noren,
    "Sign":    mi_wood,
    "Lantern": mi_lantern,
    "Stone":   mi_road
}
ichiraku_fallback = [mi_wood, mi_roof, mi_noren, mi_wood, mi_lantern, mi_road]
bind_by_slot_name("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", ichiraku_map, ichiraku_fallback)

# Residences
res_map = {
    "Wood":  mi_wood,
    "Roof":  mi_roof,
    "Stone": mi_road
}
res_fallback = [mi_wood, mi_roof, mi_road]
bind_by_slot_name("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", res_map, res_fallback)
bind_by_slot_name("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", res_map, res_fallback)

# Cedar Trees (Slot 0=Wood/Trunk, Slot 1=Foliage/Pine Green)
cedar_map = {
    "Wood":    mi_wood,
    "Foliage": mi_cedar
}
cedar_fallback = [mi_wood, mi_cedar]
bind_by_slot_name("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", cedar_map, cedar_fallback)
bind_by_slot_name("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large", cedar_map, cedar_fallback)

# Sakura Trees (Slot 0=Wood/Trunk, Slot 1=Foliage/Pink Blossom)
sakura_map = {
    "Wood":    mi_wood,
    "Foliage": mi_sakura
}
sakura_fallback = [mi_wood, mi_sakura]
bind_by_slot_name("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom", sakura_map, sakura_fallback)

print("======================================================================")
print("  STEP 4: ASSEMBLE GOLDEN STREET WITH HARMONIZED 3/4 COMPOSITION")
print("======================================================================")

world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
if not world:
    print(f"ERROR: Could not load map {MAP_PKG}")
    quit()

# Remove obstacle clutter from camera sightlines
actors_to_clean = []
for a in EAS.get_all_level_actors():
    if not a: continue
    lbl = a.get_actor_label().lower()
    if "traininglog" in lbl or "trashbin" in lbl or "trainingdummy" in lbl:
        actors_to_clean.append(a)

for a in actors_to_clean:
    print(f"  [REMOVED OBSTACLE] {a.get_actor_label()}")
    EAS.destroy_actor(a)

for a in EAS.get_all_level_actors():
    if not a:
        continue
    lbl = a.get_actor_label()

    # 1. Ichiraku Ramen: At (-620, 480, 0), yaw=55.0. Full shopfront, counter, stools, and noren clearly visible!
    if lbl == "Bld_IchirakuRamen":
        a.set_actor_location(unreal.Vector(-620.0, 480.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=55.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_ichiraku:
            smc.set_static_mesh(mesh_ichiraku)
            for si, m in enumerate([mi_wood, mi_roof, mi_noren, mi_wood, mi_lantern, mi_road]):
                smc.set_material(si, m)
        print("  [CALIBRATED] Bld_IchirakuRamen at (-620, 480, 0) yaw=55.0")

    # 2. Konoha Residence A: At (620, 500, 0), yaw=-55.0. Full balcony and gabled roof visible!
    elif lbl == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(620.0, 500.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-55.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_res_a:
            smc.set_static_mesh(mesh_res_a)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
            smc.set_material(2, mi_road)
        print("  [CALIBRATED] Bld_KonohaResidence_A at (620, 500, 0) yaw=-55.0")

    elif lbl == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-780.0, 1150.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=45.0, roll=0.0), False)

    elif lbl == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(780.0, 1150.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_res_b:
            smc.set_static_mesh(mesh_res_b)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)

    elif lbl == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-820.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=30.0, roll=0.0), False)

    elif lbl == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(820.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-30.0, roll=0.0), False)

    # Background Vista Plane
    elif "northstar_vista" in lbl.lower():
        a.set_actor_location(unreal.Vector(0.0, 4800.0, 1600.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=90.0), False)
        a.set_actor_scale3d(unreal.Vector(56.88, 32.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")
        print("  [CALIBRATED] Backdrop_Konoha_NorthStar_Vista at Y=4800, Z=1600 (56.88 x 32.0)")

    # Sakura Cherry Blossoms
    elif lbl == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-660.0, 680.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=50.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sakura:
            smc.set_static_mesh(mesh_sakura)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_sakura)

    elif lbl == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(680.0, 720.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-50.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sakura:
            smc.set_static_mesh(mesh_sakura)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_sakura)

    # Cedar Trees
    elif "cedar" in lbl.lower():
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_cedar:
            smc.set_static_mesh(mesh_cedar)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_cedar)

    # NPCs
    elif lbl == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-600.0, 480.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=55.0, roll=0.0), False)

    elif lbl == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(420.0, 180.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif lbl == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-420.0, 820.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif lbl == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(420.0, 780.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    # Props
    elif lbl == "Prop_StoneLantern_Left_01":
        a.set_actor_location(unreal.Vector(-380.0, 220.0, 0.0), False, False)

    elif lbl == "Prop_StoneLantern_Right_01":
        a.set_actor_location(unreal.Vector(380.0, 220.0, 0.0), False, False)

    elif lbl == "Prop_Bench_Left_01":
        a.set_actor_location(unreal.Vector(-420.0, 520.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif lbl == "Prop_Bench_Right_01":
        a.set_actor_location(unreal.Vector(420.0, 520.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

save_res = unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
print(f"======================================================================")
print(f"  >>> PRODUCTION GOLDEN STREET RECONSTRUCTION COMPLETE: {save_res} <<<")
print(f"======================================================================")
