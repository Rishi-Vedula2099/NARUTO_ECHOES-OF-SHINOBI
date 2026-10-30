import unreal

out = []
def log_line(s):
    out.append(s)
    unreal.log(s)

def inspect_asset_materials(asset_path):
    asset = unreal.load_asset(asset_path)
    if not asset:
        log_line(f"FAILED TO LOAD: {asset_path}")
        return
    log_line(f"\n=== ASSET: {asset_path} ({type(asset).__name__}) ===")
    if isinstance(asset, unreal.StaticMesh):
        for i in range(asset.get_num_sections(0)):
            mat = asset.get_material(i)
            log_line(f"  Slot {i}: {mat.get_path_name() if mat else 'None'}")
    
def inspect_mi(mi_path):
    mi = unreal.load_asset(mi_path)
    if not mi:
        log_line(f"FAILED TO LOAD MI: {mi_path}")
        return
    log_line(f"\n=== MATERIAL INSTANCE: {mi_path} ===")
    log_line(f"  Parent: {mi.parent.get_path_name() if mi.parent else 'None'}")
    mel = unreal.MaterialEditingLibrary
    for param in ["BaseColor", "EmissiveColor", "RimLightColor", "ShadowColor", "SkinToneMultiplier"]:
        try:
            val = mel.get_material_instance_vector_parameter_value(mi, param)
            log_line(f"  VectorParam '{param}': {val}")
        except Exception as e:
            log_line(f"  VectorParam '{param}': err {e}")
    for param in ["Roughness", "Specular", "EmissiveIntensity", "RimLightIntensity", "RimLightPower", "ShadowBias"]:
        try:
            val = mel.get_material_instance_scalar_parameter_value(mi, param)
            log_line(f"  ScalarParam '{param}': {val}")
        except Exception as e:
            log_line(f"  ScalarParam '{param}': err {e}")

inspect_asset_materials("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
inspect_asset_materials("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01")
inspect_asset_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")

inspect_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")
inspect_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
inspect_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair")
inspect_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
inspect_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_materials_live.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(out))
log_line("Wrote inspect_materials_live.txt successfully.")
