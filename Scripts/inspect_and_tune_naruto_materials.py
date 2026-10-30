import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\naruto_materials_inspect.txt"
lines = []

mats = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
]

for p in mats:
    mi = unreal.EditorAssetLibrary.load_asset(p)
    if mi:
        lines.append(f"Material: {p} ({mi.get_class().get_name()})")
        parent = mi.get_editor_property("parent")
        lines.append(f"  Parent: {parent.get_name() if parent else 'None'}")
        
        # Check scalar and vector parameter values
        scalars = mi.get_editor_property("scalar_parameter_values")
        for s in scalars:
            param_name = str(s.parameter_info.name)
            val = s.parameter_value
            lines.append(f"  Scalar '{param_name}' = {val}")
            
        vectors = mi.get_editor_property("vector_parameter_values")
        for v in vectors:
            param_name = str(v.parameter_info.name)
            val = v.parameter_value
            lines.append(f"  Vector '{param_name}' = R={val.r:.3f}, G={val.g:.3f}, B={val.b:.3f}")
    else:
        lines.append(f"Material: {p} NOT FOUND")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Inspection completed.")
