import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\fix_materials_result.txt"
lines = []

mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)

materials_map = [
    (0, "Skin", "/Game/Characters/Naruto/Materials/MI_Naruto_Skin"),
    (1, "Navy", "/Game/Characters/Naruto/Materials/MI_Naruto_Navy"),
    (2, "Hair", "/Game/Characters/Naruto/Materials/MI_Naruto_Hair"),
    (3, "Collar", "/Game/Characters/Naruto/Materials/MI_Naruto_Collar"),
    (4, "Tracksuit", "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"),
    (5, "Emblem", "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem"),
    (6, "Metal", "/Game/Characters/Naruto/Materials/MI_Naruto_Metal"),
]

# 1. Update on SkeletalMesh asset
if sk_mesh:
    mesh_mats = list(sk_mesh.get_editor_property("materials"))
    lines.append(f"Original mesh_mats length: {len(mesh_mats)}")
    
    for idx, name, path in materials_map:
        mat_asset = unreal.EditorAssetLibrary.load_asset(path)
        if mat_asset and idx < len(mesh_mats):
            mesh_mats[idx].set_editor_property("material_interface", mat_asset)
            lines.append(f"Set mesh slot {idx} ({name}) -> {mat_asset.get_name()}")
            
    sk_mesh.set_editor_property("materials", mesh_mats)
    saved = unreal.EditorAssetLibrary.save_loaded_asset(sk_mesh)
    lines.append(f"Saved sk_mesh: {saved}")

# 2. Update directly on actor in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            for idx, name, path in materials_map:
                mat_asset = unreal.EditorAssetLibrary.load_asset(path)
                if mat_asset:
                    comp.set_material(idx, mat_asset)
                    lines.append(f"Set actor comp slot {idx} ({name}) -> {mat_asset.get_name()}")
        a.modify()
        break

saved_map = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved map: {saved_map}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Completed fix_naruto_production_materials.py")
