import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\exact_materials_report.txt"
lines = []

mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)

if sk_mesh:
    lines.append(f"Loaded SkeletalMesh: {mesh_path}")
    mats = sk_mesh.get_editor_property("materials")
    lines.append(f"Materials count: {len(mats)}")
    for i, m in enumerate(mats):
        slot_name = m.get_editor_property("material_slot_name")
        mat_interface = m.get_editor_property("material_interface")
        lines.append(f"  Slot {i}: '{slot_name}' -> {mat_interface.get_path_name() if mat_interface else 'None'}")
else:
    lines.append(f"Could not load {mesh_path}")

# Also check level actor
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        lines.append(f"Actor: {a.get_actor_label()}")
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            num_mats = comp.get_num_materials()
            lines.append(f"  Component num materials: {num_mats}")
            for i in range(num_mats):
                m = comp.get_material(i)
                lines.append(f"    Comp Material {i}: {m.get_path_name() if m else 'None'}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Report written.")
