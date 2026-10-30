import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\actors_in_smoke_test.txt"
lines = []

world_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(world_path)

actors = unreal.EditorLevelLibrary.get_all_level_actors()
lines.append(f"Actors in {world_path} ({len(actors)} total):")

for a in actors:
    cname = a.get_class().get_name()
    label = a.get_actor_label()
    loc = a.get_actor_location()
    lines.append(f"  [{cname}] '{label}' at X={loc.x:.1f}, Y={loc.y:.1f}, Z={loc.z:.1f}")
    if isinstance(a, unreal.SkeletalMeshActor):
        sk = a.skeletal_mesh_component.skeletal_mesh_asset if hasattr(a.skeletal_mesh_component, "skeletal_mesh_asset") else "N/A"
        lines.append(f"     SkeletalMesh: {sk}")
    elif isinstance(a, unreal.Character):
        mesh = a.get_editor_property("mesh")
        sk = mesh.get_editor_property("skeletal_mesh_asset") if mesh else "None"
        lines.append(f"     Character Mesh: {sk}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
