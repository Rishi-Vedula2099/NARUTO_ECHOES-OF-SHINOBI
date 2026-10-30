import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in eas.get_all_level_actors():
    if "dirlight" in a.get_actor_label().lower():
        rot = a.get_actor_rotation()
        loc = a.get_actor_location()
        unreal.log(f"SUN: loc=({loc.x}, {loc.y}, {loc.z}), rot=(pitch={rot.pitch}, yaw={rot.yaw}, roll={rot.roll})")
        for comp in a.get_components_by_class(unreal.DirectionalLightComponent):
            unreal.log(f"  Intensity={comp.intensity}, Color=({comp.light_color.r}, {comp.light_color.g}, {comp.light_color.b})")
    elif "skylight" in a.get_actor_label().lower():
        for comp in a.get_components_by_class(unreal.SkyLightComponent):
            unreal.log(f"SKYLIGHT: Intensity={comp.intensity}, SourceType={comp.source_type}, RealTimeCapture={comp.real_time_capture}")
    elif "postprocess" in a.get_actor_label().lower():
        for comp in a.get_components_by_class(unreal.PostProcessComponent):
            s = comp.settings
            unreal.log(f"POSTPROCESS: Unbound={comp.unbound}")
            unreal.log(f"  AutoExposure: Method={s.auto_exposure_method}, Bias={s.auto_exposure_bias}")
            unreal.log(f"  MinEV={s.auto_exposure_min_brightness}, MaxEV={s.auto_exposure_max_brightness}")
