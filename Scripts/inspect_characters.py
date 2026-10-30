import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\character_inspection.txt"
lines = []

chars = [
    "/Game/Characters/Sasuke/Meshes/SK_Sasuke_Genin",
    "/Game/Characters/Kakashi/Meshes/SK_Kakashi_Jonin",
    "/Game/Characters/RockLee/Meshes/SK_RockLee_Genin",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
]

for c in chars:
    obj = unreal.EditorAssetLibrary.load_asset(c)
    if obj:
        cname = obj.get_class().get_name()
        lines.append(f"Asset: {c} -> Class: {cname}")
        if isinstance(obj, unreal.SkeletalMesh):
            skel = obj.get_editor_property("skeleton")
            skel_name = skel.get_path_name() if skel else "None"
            lines.append(f"  Skeleton: {skel_name}")
            # Materials
            mats = obj.get_editor_property("materials")
            lines.append(f"  Materials count: {len(mats)}")
            for idx, m in enumerate(mats):
                slot_name = m.get_editor_property("material_slot_name")
                mat_interface = m.get_editor_property("material_interface")
                mat_name = mat_interface.get_name() if mat_interface else "None"
                lines.append(f"    Slot {idx}: '{slot_name}' -> {mat_name}")
    else:
        lines.append(f"Asset: {c} -> NOT FOUND")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
