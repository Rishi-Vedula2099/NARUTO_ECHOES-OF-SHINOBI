"""
NARUTO: ECHOES OF SHINOBI
Master Golden Street Perfect Replication & Visual Rescue Script
===============================================================
1. Binds SK_Naruto_Genin and SK_Naruto_GeninTracksuit by EXACT slot name:
   - Slot 0 ('Skin')     -> MI_Naruto_Skin      (Peach)
   - Slot 1 ('Navy')     -> MI_Naruto_Navy      (Navy blue cuffs/sandals/ribbons)
   - Slot 2 ('Hair')     -> MI_Naruto_Hair      (Spiky blonde yellow)
   - Slot 3 ('Collar')   -> MI_Naruto_Collar    (Pure white collar & bandages)
   - Slot 4 ('Tracksuit')-> MI_Naruto_Tracksuit (Cadmium orange tracksuit)
   - Slot 5 ('Emblem')   -> MI_Naruto_Emblem    (Crimson red Uzumaki back crest)
   - Slot 6 ('Metal')    -> MI_Naruto_Metal     (Silver metallic Leaf plate)

2. Binds SM_Veg_KonohaCedar_Large & SM_Veg_Cedar_Large:
   - Slot 0 ('M_Veg_CedarFoliage') -> MI_Veg_CedarFoliage (Rich pine green)
   - Slot 1 ('M_Timber_Facade')    -> MI_Bld_Ichiraku_Wood

3. Binds SM_Veg_Sakura_Bloom:
   - Slot 0 -> MI_Veg_Sakura_Bloom (Soft pink cherry blossom)

4. Calibrates level /Game/Maps/GoldenTest/L_Konoha_GoldenStreet:
   - Eliminates corridor crowding: Sets building setbacks to X = +/-720 to +/-780.
   - Rotates Ichiraku Ramen to yaw=45.0 (perfect 3/4 perspective showing open counter, stools, noren, and signboard).
   - Rotates Residence A to yaw=-45.0 (showing timber balcony and gabled tiled roof).
   - Removes old standalone orphan lantern actors that were blocking the avenue.
   - Calibrates distant Hokage backdrop at Y=4800, Z=1600 (scale 56.88 x 32.0).
   - Places Chef Teuchi at the Ichiraku counter, guard and villagers along sidewalks.
   - Saves level cleanly.
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

print("======================================================================")
print("  STEP 1: PRECISE MATERIAL BINDING BY EXACT SLOT NAME")
print("======================================================================")

# Load Material Instances
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
mi_lantern  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")

def bind_by_slot_name(mesh_path, slot_map):
    mesh = unreal.load_asset(mesh_path)
    if not mesh or not isinstance(mesh, unreal.StaticMesh):
        print(f"ERROR: Mesh not found {mesh_path}")
        return
    for i, sm in enumerate(mesh.static_materials):
        name = str(sm.material_slot_name)
        if name in slot_map and slot_map[name]:
            mesh.set_material(i, slot_map[name])
            print(f"  [{mesh.get_name()}] Slot {i} ('{name}') -> {slot_map[name].get_name()}")
        else:
            print(f"  [WARN] [{mesh.get_name()}] No mapping for slot {i} ('{name}')")
    EAL.save_asset(mesh_path)

naruto_slot_map = {
    "Skin":      mi_skin,
    "Navy":      mi_navy,
    "Hair":      mi_hair,
    "Collar":    mi_collar,
    "Tracksuit": mi_tracksuit,
    "Emblem":    mi_emblem,
    "Metal":     mi_metal
}

# Bind both SK_Naruto_Genin and SK_Naruto_GeninTracksuit
bind_by_slot_name("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", naruto_slot_map)
bind_by_slot_name("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit", naruto_slot_map)

# Bind Cedar Foliage
cedar_slot_map = {
    "M_Veg_CedarFoliage": mi_cedar,
    "M_Timber_Facade":    mi_wood
}
bind_by_slot_name("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", cedar_slot_map)
bind_by_slot_name("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large", cedar_slot_map)

# Bind Sakura Bloom
mesh_sakura = unreal.load_asset("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom")
if mesh_sakura and isinstance(mesh_sakura, unreal.StaticMesh):
    for i in range(mesh_sakura.get_num_sections(0)):
        mesh_sakura.set_material(i, mi_sakura)
    EAL.save_asset("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom")
    print("  [BOUND] SM_Veg_Sakura_Bloom -> MI_Veg_Sakura_Bloom")

print("======================================================================")
print("  STEP 2: CALIBRATING LEVEL LAYOUT & ELIMINATING CORRIDOR CROWDING")
print("======================================================================")

world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
if not world:
    print(f"ERROR: Could not load {MAP_PKG}")
    quit()

# Destroy orphan hanging lantern actors that were cluttering the avenue
actors_to_destroy = []
for a in EAS.get_all_level_actors():
    if not a: continue
    lbl = a.get_actor_label()
    if "lantern_ichiraku_" in lbl.lower() or "lantern_residence_" in lbl.lower():
        actors_to_destroy.append(a)

for a in actors_to_destroy:
    lbl = a.get_actor_label()
    EAS.destroy_actor(a)
    print(f"  [DESTROYED] Orphan lantern actor: {lbl}")

# Calibrate remaining level actors
for a in EAS.get_all_level_actors():
    if not a: continue
    lbl = a.get_actor_label()

    # 1. FOREGROUND LEFT: Ichiraku Ramen (Setback X=-720, yaw=45.0)
    # The counter edge reaches X=-560, dining area & stools in full 3/4 view!
    if lbl == "Bld_IchirakuRamen":
        a.set_actor_location(unreal.Vector(-720.0, 260.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        print("  [CALIBRATED] Bld_IchirakuRamen at (-720, 260, 0) yaw=45.0")

    # 2. FOREGROUND RIGHT: Konoha Residence A (Setback X=720, yaw=-45.0)
    # Balcony and gabled tiled roof frame the right side in 3/4 view!
    elif lbl == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(720.0, 260.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        print("  [CALIBRATED] Bld_KonohaResidence_A at (720, 260, 0) yaw=-45.0")

    # 3. MIDGROUND BUILDINGS (Avenue perspective breathing room)
    elif lbl == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-780.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=45.0, roll=0.0), False)

    elif lbl == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(780.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)

    elif lbl == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-820.0, 1750.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=30.0, roll=0.0), False)

    elif lbl == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(820.0, 1750.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-30.0, roll=0.0), False)

    # 4. BACKGROUND VISTA PLANE (Pure 16:9 ratio, base at Z=0, Y=4800)
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

    # 5. SAKURA CHERRY BLOSSOMS (Overhanging canopy framing upper sky)
    elif lbl == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-620.0, 360.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_sakura)

    elif lbl == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(620.0, 380.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.8, 1.8, 1.8))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_sakura)

    # 6. CEDAR TREES (Lush Pine Green Foliage)
    elif "cedar" in lbl.lower():
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_cedar)
            if smc.get_num_materials() > 1:
                smc.set_material(1, mi_wood)

    # 7. NPCS
    elif lbl == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-700.0, 260.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=45.0, roll=0.0), False)

    elif lbl == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(450.0, 120.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif lbl == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-450.0, 680.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif lbl == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(450.0, 560.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    # 8. PROPS
    elif lbl == "Prop_StoneLantern_Left_01":
        a.set_actor_location(unreal.Vector(-360.0, 80.0, 0.0), False, False)

    elif lbl == "Prop_StoneLantern_Right_01":
        a.set_actor_location(unreal.Vector(360.0, 80.0, 0.0), False, False)

    elif lbl == "Prop_Bench_Left_01":
        a.set_actor_location(unreal.Vector(-460.0, 600.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif lbl == "Prop_Bench_Right_01":
        a.set_actor_location(unreal.Vector(460.0, 360.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif lbl == "Prop_UtilityPole_Right_01":
        a.set_actor_location(unreal.Vector(500.0, 60.0, 0.0), False, False)

    elif lbl == "Prop_UtilityPole_Left_01":
        a.set_actor_location(unreal.Vector(-500.0, 650.0, 0.0), False, False)

# Save level and packages
save_res = unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
print(f"======================================================================")
print(f"  >>> PERFECT REPLICATION SETUP COMPLETE & SAVED: {save_res} <<<")
print(f"======================================================================")
