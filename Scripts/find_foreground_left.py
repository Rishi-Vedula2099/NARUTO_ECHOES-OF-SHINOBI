import unreal

level_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world = unreal.EditorLoadingAndSavingUtils.load_map(level_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\foreground_left.txt"
with open(p, "w") as f:
    f.write("=== ACTORS NEAR LEFT FOREGROUND (X < -200, Y < 600) ===\n")
    for a in actors:
        loc = a.get_actor_location()
        if loc.x < -200 and loc.y < 600:
            f.write(f"  [{a.get_class().get_name()}] '{a.get_actor_label()}': Loc=({loc.x}, {loc.y}, {loc.z}) Rot={a.get_actor_rotation()}\n")

print("Wrote foreground left actors.")
