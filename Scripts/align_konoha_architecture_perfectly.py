import unreal

level_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world = unreal.EditorLoadingAndSavingUtils.load_map(level_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in eas.get_all_level_actors():
    label = a.get_actor_label()

    # 1. Ichiraku Ramen on Left Foreground
    if label == "Bld_IchirakuRamen":
        # Front (-Y in local) faces East (+X) and slightly South (-Y): Yaw = 102.0
        a.set_actor_location(unreal.Vector(-530.0, 340.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=102.0, roll=0.0), False)
        print("Aligned Bld_IchirakuRamen to face avenue.")

    # 2. Chef Teuchi inside the counter
    elif label == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-610.0, 350.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=102.0, roll=0.0), False)

    # 3. Residence A on Right Foreground (2-Story Timber with Balcony)
    elif label == "Bld_KonohaResidence_A":
        # Front (-Y in local) faces West (-X) and slightly South (-Y): Yaw = -102.0
        a.set_actor_location(unreal.Vector(530.0, 340.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-102.0, roll=0.0), False)
        print("Aligned Bld_KonohaResidence_A to face avenue.")

    # 4. Residence B on Midground Right
    elif label == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(680.0, 950.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    # 5. Weapon Shop on Midground Left
    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-680.0, 950.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    # 6. Sakura Blooming Tree on Foreground Left and Right
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-680.0, 750.0, 0.0), False, False)
    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(640.0, 520.0, 0.0), False, False)

    # 7. PlayerStart positioned at (0, 0, 95)
    elif label == "PlayerStart_GoldenView":
        a.set_actor_location(unreal.Vector(0.0, -50.0, 95.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

unreal.EditorLoadingAndSavingUtils.save_current_level()
print("Saved perfectly aligned Konoha Golden Street architecture.")
