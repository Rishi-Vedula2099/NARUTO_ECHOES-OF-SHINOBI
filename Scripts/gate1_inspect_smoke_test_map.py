import unreal

MAP_PKG = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Saved\smoke_test_actor_audit.txt"

actors = eas.get_all_level_actors()
lines = []
lines.append("================ SMOKE TEST LEVEL ACTOR AUDIT ================\n")
lines.append(f"Total Actors: {len(actors)}\n")
for a in actors:
    if not a:
        continue
    label = a.get_actor_label()
    loc = a.get_actor_location()
    rot = a.get_actor_rotation()
    scale = a.get_actor_scale3d()
    lines.append(f"ACTOR: {label} | Class: {a.get_class().get_name()} | Loc: ({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}) | Rot: ({rot.pitch:.1f}, {rot.yaw:.1f}, {rot.roll:.1f})\n")
    
    smc = a.get_component_by_class(unreal.StaticMeshComponent)
    if smc:
        sm = smc.get_editor_property("static_mesh")
        sm_name = sm.get_name() if sm else "None"
        mats = [smc.get_material(i).get_name() if smc.get_material(i) else "None" for i in range(smc.get_num_materials())]
        lines.append(f"   -> Mesh: {sm_name} | Materials: {mats}\n")

with open(out_file, "w") as f:
    f.writelines(lines)
print(f"Wrote {len(lines)} lines to {out_file}")
