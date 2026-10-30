import unreal

out_lines = []
for p in [
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit",
    "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
    "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
    "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large"
]:
    m = unreal.load_asset(p)
    if m and isinstance(m, unreal.StaticMesh):
        out_lines.append(f"=== {p} ({len(m.static_materials)} slots) ===")
        for i, sm in enumerate(m.static_materials):
            mat_name = sm.material_interface.get_name() if sm.material_interface else "None"
            out_lines.append(f"  Slot {i}: slot_name='{sm.material_slot_name}', mat='{mat_name}'")

with open(r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\slots_out.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(out_lines))
