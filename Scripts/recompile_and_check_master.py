import unreal

eal = unreal.EditorAssetLibrary
mel = unreal.MaterialEditingLibrary

mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
print(f"Master Mat: {mat}")
if mat:
    print(f"Num expressions: {len(mat.get_editor_property('expressions'))}")
    # Force recompile
    res = mel.recompile_material(mat)
    print(f"Recompile result: {res}")
    eal.save_asset(mat.get_path_name())

for p in [
    "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"
]:
    mi = unreal.load_asset(p)
    if mi:
        mel.update_material_instance(mi)
        eal.save_asset(mi.get_path_name())
        print(f"Updated MI: {mi.get_name()}")
