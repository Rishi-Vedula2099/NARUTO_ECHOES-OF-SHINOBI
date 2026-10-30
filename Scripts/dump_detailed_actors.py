import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

out = []
for a in actors:
    if not a: continue
    loc = a.get_actor_location()
    rot = a.get_actor_rotation()
    scale = a.get_actor_scale3d()
    sm_name = ""
    if isinstance(a, unreal.StaticMeshActor):
        sm = a.static_mesh_component.static_mesh
        sm_name = sm.get_path_name() if sm else "None"
    out.append(f"{a.get_actor_label():<35} | {a.get_class().get_name():<20} | Loc: ({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}) | Rot: ({rot.pitch:.1f}, {rot.yaw:.1f}, {rot.roll:.1f}) | Mesh: {sm_name}")

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\detailed_actors.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(out))
unreal.log("Wrote detailed_actors.txt")
