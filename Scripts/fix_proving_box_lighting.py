import unreal

print("=== CHECKING POSTPROCESS VOLUME PROPERTIES ===")
unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

ppv = None
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.PostProcessVolume) or a.get_actor_label() == "PostProcess_ProvingBox":
        ppv = a
        break

if ppv:
    print("Found PostProcessVolume:", ppv.get_actor_label())
    s = ppv.get_editor_property("settings")
    print("settings type:", type(s))
    
    # Check overrides
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
    
    ppv.set_editor_property("settings", s)
    ppv.set_editor_property("unbound", True)
    print("Successfully set PostProcessVolume settings!")

# Also check Directional Light and set intensity to 12.0
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.set_editor_property("intensity", 12.0)
            print("Set DirLight intensity to 12.0 (matching Konoha daylight)")
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.set_editor_property("intensity", 3.0)
            print("Set SkyLight intensity to 3.0")

unreal.EditorLoadingAndSavingUtils.save_dirty_packages(False, True)
print("Saved map /Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
