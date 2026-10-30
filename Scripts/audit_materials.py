import unreal

MEL = unreal.MaterialEditingLibrary

mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
print("=== M_AnimeCel_Master Parameter Inspection ===")
if mat:
    vec_names = MEL.get_vector_parameter_names(mat)
    scalar_names = MEL.get_scalar_parameter_names(mat)
    print(f"Vector parameters ({len(vec_names)}): {[str(n) for n in vec_names]}")
    print(f"Scalar parameters ({len(scalar_names)}): {[str(n) for n in scalar_names]}")

    for vn in vec_names:
        val = MEL.get_material_default_vector_parameter_value(mat, vn)
        print(f"  VecParam '{vn}': {val}")
    for sn in scalar_names:
        val = MEL.get_material_default_scalar_parameter_value(mat, sn)
        print(f"  ScalarParam '{sn}': {val}")

mi_list = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
    "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
    "/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock",
    "/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized",
]

print("\n=== Material Instances Audit ===")
for p in mi_list:
    mi = unreal.load_asset(p)
    if mi:
        parent = mi.get_editor_property("parent")
        pname = parent.get_path_name() if parent else "None"
        print(f"[OK] {mi.get_name()} -> Parent: {pname}")
    else:
        print(f"[MISSING] {p}")
