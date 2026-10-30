import unreal

out_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\diagnose_output.txt"
lines = []
lines.append("=== DIAGNOSING L_VisualAssetSmokeTest ===")
world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

actors = EAS.get_all_level_actors()
lines.append(f"Total Actors: {len(actors)}")

for a in actors:
    cname = a.get_class().get_name()
    label = a.get_actor_label()
    loc = a.get_actor_location()
    
    if isinstance(a, unreal.DirectionalLight):
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        intensity = dlc.get_editor_property("intensity") if dlc else None
        color = dlc.get_editor_property("light_color") if dlc else None
        lines.append(f"[DirectionalLight] {label} at {loc} | Intensity={intensity} | Color={color}")
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        intensity = slc.get_editor_property("intensity") if slc else None
        lines.append(f"[SkyLight] {label} at {loc} | Intensity={intensity}")
    elif isinstance(a, unreal.PostProcessVolume):
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        unbound = a.get_editor_property("unbound")
        lines.append(f"[PostProcessVolume] {label} at {loc} | Unbound={unbound}")
        if ppc:
            s = ppc.get_editor_property("settings")
            meth = s.get_editor_property("auto_exposure_method")
            min_b = s.get_editor_property("auto_exposure_min_brightness")
            max_b = s.get_editor_property("auto_exposure_max_brightness")
            bias = s.get_editor_property("auto_exposure_bias")
            lines.append(f"   Settings: method={meth}, min={min_b}, max={max_b}, bias={bias}")
    elif "Camera" in cname or "Camera" in label:
        lines.append(f"[Camera] {label} ({cname}) at {loc}")
    elif "PlayerStart" in cname or "PlayerStart" in label:
        lines.append(f"[PlayerStart] {label} at {loc}")
    elif "Swatch" in label:
        lines.append(f"[Swatch] {label} at {loc}")

lines.append("=== DIAGNOSIS COMPLETE ===")

with open(out_path, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Wrote output to", out_path)
