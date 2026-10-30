import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_output.txt", "w") as f:
    f.write(f"=== ALL ACTORS IN L_Konoha_GoldenStreet (Count: {len(actors)}) ===\n")
    for a in actors:
        loc = a.get_actor_location()
        f.write(f"ACTOR: label='{a.get_actor_label()}' name='{a.get_name()}' class='{a.get_class().get_name()}' loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f})\n")
    f.write("=== END ACTORS ===\n")
