import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\calibrate_lighting_result.txt"
lines = []

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        # Position sun to illuminate Naruto's front with golden 5400K daylight
        # Camera is at (0, -500, 85) looking +Y. Naruto is at (0, -350, 0) looking -Y.
        # Yaw = 45 means light is shining along (+X, +Y), coming over camera's left shoulder directly onto Naruto's front!
        a.set_actor_rotation(unreal.Rotator(pitch=-38.0, yaw=45.0, roll=0.0), False)
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.set_editor_property("intensity", 75000.0) # 75,000 Lux physical daylight
            dlc.set_editor_property("use_temperature", True)
            dlc.set_editor_property("temperature", 5400.0)
            dlc.set_editor_property("light_color", unreal.Color(255, 240, 215, 255))
        a.modify()
        lines.append("Set DirectionalLight: 75000 Lux, 5400K, Pitch=-38, Yaw=45")
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.set_editor_property("intensity", 1.5)
            slc.set_editor_property("light_color", unreal.Color(185, 220, 255, 255))
        a.modify()
        lines.append("Set SkyLight: 1.5 intensity, Azure fill")
    elif a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        a.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
        a.modify()
        lines.append("Set Naruto Actor: Loc=(0, -350, 0), Yaw=180 (facing camera)")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved L_VisualAssetSmokeTest")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Calibrate lighting script complete.")
