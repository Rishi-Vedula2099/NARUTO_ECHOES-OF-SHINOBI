import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_lights_output.txt"
lines = []

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        val = dlc.get_editor_property("intensity") if dlc else "NO DLC"
        lines.append(f"CURRENT DIRLIGHT INTENSITY: {val}")
    elif isinstance(a, unreal.PostProcessVolume):
        lines.append(f"CURRENT PPV UNBOUND: {a.get_editor_property('unbound')}")
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        if ppc:
            s = ppc.get_editor_property("settings")
            lines.append(f"CURRENT AUTO EXPOSURE METHOD: {s.get_editor_property('auto_exposure_method')}")
            lines.append(f"CURRENT MIN BRIGHTNESS: {s.get_editor_property('auto_exposure_min_brightness')}")
            lines.append(f"CURRENT MAX BRIGHTNESS: {s.get_editor_property('auto_exposure_max_brightness')}")
    elif "PlayerStart" in a.get_actor_label():
        lines.append(f"CURRENT PLAYERSTART LOC: {a.get_actor_location()}")
        lines.append(f"CURRENT PLAYERSTART ROT: {a.get_actor_rotation()}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Wrote output to", out_file)
