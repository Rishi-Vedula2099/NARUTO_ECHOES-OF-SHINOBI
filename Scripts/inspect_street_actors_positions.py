import unreal

level_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world = unreal.EditorLoadingAndSavingUtils.load_map(level_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\street_actors_dump.txt"
with open(p, "w") as f:
    f.write(f"Total level actors: {len(actors)}\n\n")
    for a in sorted(actors, key=lambda x: x.get_actor_label()):
        loc = a.get_actor_location()
        rot = a.get_actor_rotation()
        scale = a.get_actor_scale3d()
        f.write(f"[{a.get_class().get_name()}] '{a.get_actor_label()}': Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}) Rot=(P={rot.pitch:.1f}, Y={rot.yaw:.1f}, R={rot.roll:.1f}) Scale=({scale.x:.2f}, {scale.y:.2f}, {scale.z:.2f})\n")

print("Dumped street actors.")
