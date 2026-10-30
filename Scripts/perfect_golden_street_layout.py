"""
NARUTO: ECHOES OF SHINOBI
Perfect Golden Street Avenue Layout & Vista Calibration
======================================================
1. Widens building setbacks to X = +/-760 to +/-780, creating a sweeping 1:1 avenue framing.
2. Centers the 4 Hokage Rock faces on the vanishing point, grounding the backdrop from Z=0 to sky.
3. Moves utility poles, stone lanterns, and benches to sidewalk perimeters (no vista blocking).
4. Frames sky with overhanging pink Sakura cherry blossom canopies.
5. Saves level /Game/Maps/GoldenTest/L_Konoha_GoldenStreet cleanly.
"""

import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
if not world:
    unreal.log_error("Could not load golden street map!")
    quit()

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    if not a:
        continue
    label = a.get_actor_label()

    # 1. Left Buildings (Setback line X = -760)
    if label == "Bld_IchirakuRamen":
        a.set_actor_location(unreal.Vector(-760.0, 320.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-780.0, 1100.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-780.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    # 2. Right Buildings (Setback line X = +760)
    elif label == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(760.0, 320.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    elif label == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(780.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(780.0, 1800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    # 3. Hokage Mansion (Moved far east out of street view)
    elif label == "Bld_HokageMansion_MidVista":
        a.set_actor_location(unreal.Vector(1200.0, 3600.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)

    # 4. Vista Backdrop Plane (Grounded at Z=620, Scale 85x58, Pure Hokage Monument & Sky)
    elif "northstar_vista" in label.lower():
        a.set_actor_location(unreal.Vector(0.0, 4200.0, 620.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=90.0), False)
        a.set_actor_scale3d(unreal.Vector(85.0, 58.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")

    # 5. Props: Reposition to sidewalk perimeter
    elif label == "Prop_UtilityPole_Right_01":
        a.set_actor_location(unreal.Vector(540.0, 60.0, 0.0), False, False)

    elif label == "Prop_UtilityPole_Left_01":
        a.set_actor_location(unreal.Vector(-540.0, 650.0, 0.0), False, False)

    elif label == "Prop_Bench_Right_01":
        a.set_actor_location(unreal.Vector(460.0, 480.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "Prop_Bench_Left_01":
        a.set_actor_location(unreal.Vector(-460.0, 800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "Prop_StoneLantern_Left_01":
        a.set_actor_location(unreal.Vector(-360.0, 100.0, 0.0), False, False)

    elif label == "Prop_StoneLantern_Right_01":
        a.set_actor_location(unreal.Vector(360.0, 100.0, 0.0), False, False)

    elif label == "Prop_StoneLantern_Left_02":
        a.set_actor_location(unreal.Vector(-360.0, 1500.0, 0.0), False, False)

    elif label == "Prop_StoneLantern_Right_02":
        a.set_actor_location(unreal.Vector(360.0, 1500.0, 0.0), False, False)

    elif label == "Prop_ShopBanner_Ichiraku":
        a.set_actor_location(unreal.Vector(-650.0, 240.0, 0.0), False, False)

    # 6. Sakura Cherry Blossoms
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-580.0, 360.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=65.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.6, 1.6, 1.6))

    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(580.0, 440.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.6, 1.6, 1.6))

    # 7. NPCs
    elif label == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-740.0, 320.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(440.0, 120.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-440.0, 750.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(440.0, 520.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    elif label == "NPC_Academy_Student":
        a.set_actor_location(unreal.Vector(440.0, 350.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-60.0, roll=0.0), False)

# Save map
save_res = unreal.EditorLoadingAndSavingUtils.save_map(world, "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log(f"=== Perfect Layout Applied and Saved: {save_res} ===")
