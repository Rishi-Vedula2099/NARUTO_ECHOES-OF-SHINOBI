import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\restore_calibrated_result.txt"
lines = []

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# 1. Calibrated Lighting (Gate 1 Baseline: 12.0 Lux key, 3.0 Azure fill, Clamped EV)
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        a.modify()
        # Light comes over camera's left shoulder directly illuminating Naruto's front face
        a.set_actor_rotation(unreal.Rotator(pitch=-35.0, yaw=50.0, roll=0.0), False)
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.modify()
            dlc.set_editor_property("intensity", 12.0)
            dlc.set_editor_property("use_temperature", True)
            dlc.set_editor_property("temperature", 5400.0)
            dlc.set_editor_property("light_color", unreal.Color(255, 240, 215, 255))
        lines.append("DirectionalLight: 12.0 Lux, 5400K, Pitch=-35, Yaw=50")
    elif isinstance(a, unreal.SkyLight):
        a.modify()
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.modify()
            slc.set_editor_property("intensity", 3.0)
            slc.set_editor_property("light_color", unreal.Color(185, 220, 255, 255))
        lines.append("SkyLight: 3.0 intensity, Azure fill")
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
            s.set_editor_property("bloom_intensity", 0.35)
            ppc.set_editor_property("settings", s)
        lines.append("PostProcess: Clamped AEM_HISTOGRAM min/max=1.0")

# 2. Materials
mats_info = [
    (0, "MI_Naruto_Skin",      "/Game/Characters/Naruto/Materials/MI_Naruto_Skin"),
    (1, "MI_Naruto_Navy",      "/Game/Characters/Naruto/Materials/MI_Naruto_Navy"),
    (2, "MI_Naruto_Hair",      "/Game/Characters/Naruto/Materials/MI_Naruto_Hair"),
    (3, "MI_Naruto_Collar",    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar"),
    (4, "MI_Naruto_Tracksuit", "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"),
    (5, "MI_Naruto_Emblem",    "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem"),
    (6, "MI_Naruto_Metal",     "/Game/Characters/Naruto/Materials/MI_Naruto_Metal"),
]

# 3. Setup Naruto Actor
for a in EAS.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        a.modify()
        a.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            comp.modify()
            for idx, name, path in mats_info:
                mat_asset = unreal.EditorAssetLibrary.load_asset(path)
                if mat_asset:
                    comp.set_material(idx, mat_asset)
                    lines.append(f"  Slot {idx} ({name}) -> {mat_asset.get_name()}")
        lines.append("Naruto ProvingActor: Loc=(0, -350, 0), Rot=(0, 180, 0)")
    elif a.get_actor_label() == "Camera_Naruto_Showcase":
        a.modify()
        a.set_actor_location(unreal.Vector(0.0, -520.0, 80.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        cc = a.get_component_by_class(unreal.CameraComponent)
        if cc:
            cc.field_of_view = 35.0
        lines.append("Camera_Naruto_Showcase: Loc=(0, -520, 80), FOV=35")
    elif a.get_actor_label() == "PlayerStart_NarutoGenin":
        a.modify()
        a.set_actor_location(unreal.Vector(0.0, -520.0, 80.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        lines.append("PlayerStart_NarutoGenin: Loc=(0, -520, 80)")

saved = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path}: {saved}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Restore calibrated script complete.")
