import unreal

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")

out_lines = []
out_lines.append("=== CAMERA FRUSTUM ACTOR AUDIT ===")

for a in EAS.get_all_level_actors():
    if not a:
        continue
    loc = a.get_actor_location()
    rot = a.get_actor_rotation()
    scale = a.get_actor_scale3d()
    smc = a.get_component_by_class(unreal.StaticMeshComponent)
    mesh_name = smc.static_mesh.get_name() if (smc and smc.static_mesh) else "NO_MESH"
    
    # Check all actors in level
    line = f"{a.get_actor_label():<32} | Loc: ({loc.x:7.1f}, {loc.y:7.1f}, {loc.z:7.1f}) | Rot: ({rot.pitch:5.1f}, {rot.yaw:5.1f}, {rot.roll:5.1f}) | Scale: ({scale.x:4.1f}, {scale.y:4.1f}, {scale.z:4.1f}) | Mesh: {mesh_name}"
    out_lines.append(line)

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\street_audit_results.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(out_lines))

unreal.log("Saved street audit results to street_audit_results.txt")
