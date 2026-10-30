import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\diagnose_lighting.txt", "w", encoding="utf-8") as f:
    f.write("=== DIAGNOSE LIGHTING & MATERIALS IN L_Konoha_GoldenStreet ===\n")
    for a in actors:
        cname = a.get_class().get_name()
        label = a.get_actor_label()
        if "light" in cname.lower() or "fog" in cname.lower() or "postprocess" in cname.lower() or "sky" in cname.lower():
            f.write(f"\nACTOR: {label} ({cname})\n")
            for comp in a.get_components_by_class(unreal.ActorComponent):
                f.write(f"  COMP: {comp.get_name()} ({comp.get_class().get_name()})\n")
                if isinstance(comp, unreal.LightComponentBase):
                    f.write(f"    Intensity: {comp.intensity}\n")
                    f.write(f"    LightColor: ({comp.light_color.r}, {comp.light_color.g}, {comp.light_color.b})\n")
                    f.write(f"    CastShadows: {comp.cast_shadows}\n")
                    if hasattr(comp, "atmosphere_sun_light"):
                        f.write(f"    AtmosphereSunLight: {comp.atmosphere_sun_light}\n")
                if isinstance(comp, unreal.SkyLightComponent):
                    f.write(f"    RealTimeCapture: {comp.real_time_capture}\n")
                    f.write(f"    SourceType: {comp.source_type}\n")
                if isinstance(comp, unreal.PostProcessComponent):
                    settings = comp.settings
                    f.write(f"    PP Unbound: {comp.unbound}\n")
                    f.write(f"    AutoExposure: MinEV={settings.auto_exposure_min_brightness}, MaxEV={settings.auto_exposure_max_brightness}\n")
        elif "road" in label.lower() or "ichiraku" in label.lower() or "backdrop" in label.lower():
            f.write(f"\nACTOR: {label} ({cname})\n")
            for comp in a.get_components_by_class(unreal.StaticMeshComponent):
                sm = comp.static_mesh
                f.write(f"  SM: {sm.get_path_name() if sm else 'None'}\n")
                for i in range(comp.get_num_materials()):
                    mat = comp.get_material(i)
                    f.write(f"  MAT[{i}]: {mat.get_path_name() if mat else 'None'}\n")

    f.write("\n=== END DIAGNOSTIC ===\n")
