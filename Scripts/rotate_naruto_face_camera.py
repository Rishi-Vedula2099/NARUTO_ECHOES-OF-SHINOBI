import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        # Rotate 180 so his front faces the camera
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
        a.modify()
        print("Set Naruto Yaw=180 (Front facing camera)")
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved map.")
