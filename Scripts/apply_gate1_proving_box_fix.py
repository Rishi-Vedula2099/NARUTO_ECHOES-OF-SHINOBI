import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\save_check.txt"
world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.modify()
            dlc.set_editor_property("intensity", 12.0)
            a.modify()
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.modify()
            slc.set_editor_property("intensity", 3.0)
            a.modify()
    elif isinstance(a, unreal.PostProcessVolume):
        a.modify()
        a.set_editor_property("unbound", True)
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        if ppc:
            ppc.modify()
            s = ppc.get_editor_property("settings")
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.0)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 1.0)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.0)
            s.set_editor_property("override_bloom_intensity", True)
            s.set_editor_property("bloom_intensity", 0.4)
            ppc.set_editor_property("settings", s)
    elif "PlayerStart" in a.get_actor_label():
        a.modify()
        a.set_actor_location(unreal.Vector(100.0, -750.0, 140.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=-8.0, yaw=90.0, roll=0.0), False)

# Save with explicit save_map
saved = unreal.EditorLoadingAndSavingUtils.save_map(world, "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
with open(out_file, "w", encoding="utf-8") as f:
    f.write(f"SAVE RESULT: {saved}\n")

print("SAVE MAP EXPLICIT:", saved)
