import unreal

eal = unreal.EditorAssetLibrary

mis = [
    "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"
]

lines = []
for p in mis:
    mi = unreal.load_asset(p)
    if mi:
        parent = mi.get_editor_property("parent")
        p_name = parent.get_name() if parent else "None"
        lines.append(f"MI: {mi.get_name()} | Parent: {p_name} | Valid: {parent is not None}\n")
    else:
        lines.append(f"MI NOT FOUND: {p}\n")

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Saved\mi_status.txt", "w") as f:
    f.writelines(lines)
