import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in eas.get_all_level_actors():
    if isinstance(a, unreal.PostProcessVolume):
        unreal.log(f"PP VOLUME: {a.get_actor_label()}")
        unreal.log(f"  unbound: {a.unbound}")
        s = a.settings
        unreal.log(f"  auto_exposure_method: {s.auto_exposure_method}")
        unreal.log(f"  auto_exposure_bias: {s.auto_exposure_bias}")
        unreal.log(f"  auto_exposure_min_brightness: {s.auto_exposure_min_brightness}")
        unreal.log(f"  auto_exposure_max_brightness: {s.auto_exposure_max_brightness}")
