import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

unreal.log("=== CHECKING POST PROCESS SETTINGS ===")
for a in eas.get_all_level_actors():
    if isinstance(a, unreal.PostProcessVolume):
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        s = ppc.settings
        unreal.log(f"PP Actor: {a.get_actor_label()}")
        unreal.log(f"  unbound: {a.get_editor_property('unbound')}")
        unreal.log(f"  override_color_saturation: {s.override_color_saturation}")
        unreal.log(f"  color_saturation: {s.color_saturation}")
        unreal.log(f"  override_color_contrast: {s.override_color_contrast}")
        unreal.log(f"  override_color_gamma: {s.override_color_gamma}")
        unreal.log(f"  override_scene_color_tint: {s.override_scene_color_tint}")
        unreal.log(f"  scene_color_tint: {s.scene_color_tint}")
        unreal.log(f"  auto_exposure_method: {s.auto_exposure_method}")
        unreal.log(f"  min_ev: {s.auto_exposure_min_brightness}, max_ev: {s.auto_exposure_max_brightness}, bias: {s.auto_exposure_bias}")

unreal.log("=== CHECKING MATERIAL INSTANCES ===")
for path in [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood"
]:
    mi = unreal.load_asset(path)
    if mi:
        bc = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(mi, "BaseColor")
        unreal.log(f"MI: {mi.get_name()} -> BaseColor: ({bc.r:.3f}, {bc.g:.3f}, {bc.b:.3f})")
    else:
        unreal.log_error(f"Missing MI: {path}")

unreal.log("=== CHECKING LIGHTS ===")
for a in eas.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        unreal.log(f"Sun: {a.get_actor_label()} Intensity={dlc.intensity} Color={dlc.light_color}")
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        unreal.log(f"SkyLight: {a.get_actor_label()} Intensity={slc.intensity} Color={slc.light_color}")
