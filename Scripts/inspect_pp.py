import unreal

unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.PostProcessVolume):
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        s = ppc.get_editor_property("settings")
        print("PP ACTOR:", a.get_actor_label())
        print("  auto_exposure_method:", s.get_editor_property("auto_exposure_method"))
        print("  auto_exposure_bias:", s.get_editor_property("auto_exposure_bias"))
        print("  auto_exposure_min_brightness:", s.get_editor_property("auto_exposure_min_brightness"))
        print("  auto_exposure_max_brightness:", s.get_editor_property("auto_exposure_max_brightness"))
        print("  bloom_intensity:", s.get_editor_property("bloom_intensity"))
        print("  bloom_threshold:", s.get_editor_property("bloom_threshold"))
