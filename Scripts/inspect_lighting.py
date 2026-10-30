import unreal

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for map_path in ["/Game/Maps/GoldenTest/L_Konoha_GoldenStreet", "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"]:
    print(f"\n=======================================================")
    print(f"=== INSPECTING: {map_path} ===")
    print(f"=======================================================")
    unreal.EditorLoadingAndSavingUtils.load_map(map_path)
    
    for a in EAS.get_all_level_actors():
        cname = a.get_class().get_name()
        label = a.get_actor_label()
        if "Light" in cname or "PostProcess" in cname or "Sky" in cname or "Camera" in cname:
            print(f"\nActor: {label} ({cname})")
            if isinstance(a, unreal.DirectionalLight):
                dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
                if dlc:
                    print(f"  Intensity: {dlc.get_editor_property('intensity')}")
                    print(f"  Color: {dlc.get_editor_property('light_color')}")
                    print(f"  Rot: {a.get_actor_rotation()}")
            elif isinstance(a, unreal.SkyLight):
                slc = a.get_component_by_class(unreal.SkyLightComponent)
                if slc:
                    print(f"  Intensity: {slc.get_editor_property('intensity')}")
                    print(f"  Color: {slc.get_editor_property('light_color')}")
            elif isinstance(a, unreal.PostProcessVolume):
                ppc = a.get_component_by_class(unreal.PostProcessComponent)
                if ppc:
                    s = ppc.get_editor_property("settings")
                    print(f"  Unbound: {a.get_editor_property('unbound')}")
                    for prop in [
                        "auto_exposure_method", "override_auto_exposure_method",
                        "auto_exposure_bias", "override_auto_exposure_bias",
                        "auto_exposure_min_brightness", "override_auto_exposure_min_brightness",
                        "auto_exposure_max_brightness", "override_auto_exposure_max_brightness",
                        "camera_iso", "override_camera_iso",
                        "camera_shutter_speed", "override_camera_shutter_speed",
                        "camera_aperture", "override_camera_aperture",
                        "bloom_intensity", "override_bloom_intensity"
                    ]:
                        if hasattr(s, prop):
                            print(f"  {prop}: {getattr(s, prop)}")
