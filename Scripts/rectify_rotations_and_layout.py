"""
NARUTO: ECHOES OF SHINOBI
Master Layout & Rotation Rectification
======================================
Corrects all building, prop, and backdrop rotations using explicit keyword arguments
(pitch=..., yaw=..., roll=...) to eliminate orientation distortion.
Clears the street axis completely, opening a breathtaking vista to the Hokage Rock monument.
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

    # 1. Ichiraku Ramen (Left Sidewalk, facing street +X)
    if label == "Bld_IchirakuRamen":
        a.set_actor_location(unreal.Vector(-580.0, 280.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_IchirakuRamen: Loc=(-580, 280, 0), Rot=(pitch=0, yaw=90, roll=0)")

    # 2. Konoha Residence A (Right Sidewalk, facing street -X)
    elif label == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(580.0, 260.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_KonohaResidence_A: Loc=(580, 260, 0), Rot=(pitch=0, yaw=-90, roll=0)")

    # 3. Konoha Residence B (Right Sidewalk, midground)
    elif label == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(600.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    # 4. Weapon Shop (Left Sidewalk, midground)
    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-600.0, 1100.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    # 5. Mission Office (Left Sidewalk, background)
    elif label == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-600.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    # 6. Tea House (Right Sidewalk, background)
    elif label == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(600.0, 1800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))

    # 7. Hokage Mansion: Move off-axis into eastern cedar tree line to completely clear street vista
    elif label == "Bld_HokageMansion_MidVista":
        a.set_actor_location(unreal.Vector(780.0, 3800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_HokageMansion_MidVista moved off-axis to (780, 3800, 0)")

    # 8. Vista Backdrop Plane (Hokage Rock Monument & Anime Afternoon Sky)
    elif "northstar_vista" in label.lower():
        a.set_actor_location(unreal.Vector(0.0, 4200.0, 850.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=90.0), False)
        a.set_actor_scale3d(unreal.Vector(75.0, 45.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")
        unreal.log("-> Backdrop_Konoha_NorthStar_Vista calibrated at (0, 4200, 850)")

    # 9. Overhanging Sakura Cherry Blossoms
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-480.0, 350.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=65.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.4, 1.4, 1.4))

    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(480.0, 420.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.4, 1.4, 1.4))

    # 10. NPCs Upright Orientations
    elif label == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-580.0, 280.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(410.0, 100.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-410.0, 750.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(410.0, 520.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    elif label == "NPC_Academy_Student":
        a.set_actor_location(unreal.Vector(410.0, 350.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-60.0, roll=0.0), False)

    # 11. Clean old placeholder banners / occluders
    elif label == "Prop_ShopBanner_Ichiraku":
        # Remove or tuck neatly behind the counter so it doesn't clip
        a.set_actor_location(unreal.Vector(-650.0, 220.0, 0.0), False, False)

unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log("=== Master Layout & Rotation Rectification Succeeded! ===")
