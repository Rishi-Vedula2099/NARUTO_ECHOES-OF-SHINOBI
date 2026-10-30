import unreal

unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    print(f"Actor: {a.get_actor_label()} | Class: {a.get_class().get_name()} | Loc: {a.get_actor_location()}")
