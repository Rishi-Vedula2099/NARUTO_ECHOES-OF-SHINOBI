import unreal

paths = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood"
]

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\mat_inspect_out.txt", "w") as f:
    for p in paths:
        m = unreal.load_asset(p)
        if not m:
            f.write(f"NOT FOUND: {p}\n")
            continue
        f.write(f"MAT: {p} Class={m.get_class().get_name()}\n")
        if hasattr(m, 'parent'):
            f.write(f"  Parent: {m.parent}\n")
        if hasattr(m, 'vector_parameter_values'):
            for v in m.vector_parameter_values:
                f.write(f"  VecParam: {v.parameter_info.name} = {v.parameter_value}\n")
        if hasattr(m, 'scalar_parameter_values'):
            for s in m.scalar_parameter_values:
                f.write(f"  ScalarParam: {s.parameter_info.name} = {s.parameter_value}\n")
