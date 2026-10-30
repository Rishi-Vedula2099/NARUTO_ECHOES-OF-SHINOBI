import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\diagnose_mi_issue.txt"
lines = []

EAL = unreal.EditorAssetLibrary

mi_paths = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Materials/Environment/MI_AnimeWood",
    "/Game/Materials/Environment/MI_AnimeRoof"
]

for p in mi_paths:
    mi = unreal.load_asset(p)
    if mi:
        lines.append(f"=== {p} ===")
        parent = mi.get_editor_property("parent")
        lines.append(f"  Parent: {parent.get_path_name() if parent else 'None'}")
        
        vecs = mi.get_editor_property("vector_parameter_values")
        for v in vecs:
            lines.append(f"  VectorParam '{v.parameter_info.name}': {v.parameter_value}")
            
        scalars = mi.get_editor_property("scalar_parameter_values")
        for s in scalars:
            lines.append(f"  ScalarParam '{s.parameter_info.name}': {s.parameter_value}")
    else:
        lines.append(f"{p}: NOT FOUND")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Diagnose script done.")
