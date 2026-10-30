import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\diagnose_actor_mesh.txt"
lines = []

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.skeletal_mesh_component
        lines.append(f"Actor class: {a.get_class().get_name()}")
        lines.append(f"Actor location: {a.get_actor_location()}")
        lines.append(f"Actor rotation: {a.get_actor_rotation()}")
        
        # Check bone names and bone world locations
        num_bones = comp.get_num_bones()
        lines.append(f"Num bones: {num_bones}")
        for b_idx in range(min(num_bones, 8)):
            b_name = comp.get_bone_name(b_idx)
            b_loc = comp.get_bone_location(b_name)
            lines.append(f"  Bone {b_idx} ('{b_name}'): world location = {b_loc}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
