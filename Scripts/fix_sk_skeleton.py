import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\fix_sk_skeleton_result.txt"
lines = []

# Move the newly imported multi-material mesh and skeleton
src_base = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes"
src_sk = f"{src_base}/SK_Naruto_Genin_Rigged"
src_skel = f"{src_base}/SK_Naruto_Genin_Rigged_Skeleton"
src_phys = f"{src_base}/SK_Naruto_Genin_Rigged_PhysicsAsset"

dst_sk = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
dst_skel = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton"
dst_phys = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_PhysicsAsset"

for src, dst in [(src_sk, dst_sk), (src_skel, dst_skel), (src_phys, dst_phys)]:
    if unreal.EditorAssetLibrary.does_asset_exist(src):
        if unreal.EditorAssetLibrary.does_asset_exist(dst):
            unreal.EditorAssetLibrary.delete_asset(dst)
        ok = unreal.EditorAssetLibrary.rename_asset(src, dst)
        lines.append(f"Moved {src} -> {dst}: {ok}")

sk_mesh = unreal.EditorAssetLibrary.load_asset(dst_sk)
skeleton = unreal.EditorAssetLibrary.load_asset(dst_skel)

lines.append(f"SK Mesh: {sk_mesh}")
lines.append(f"Skeleton: {skeleton}")

if sk_mesh and skeleton:
    lines.append(f"Mesh skeleton is: {sk_mesh.get_editor_property('skeleton')}")
    
    # Inspect material slots on the newly moved multi-material mesh
    mesh_mats = sk_mesh.get_editor_property("materials")
    lines.append(f"Multi-material mesh has {len(mesh_mats)} material slots!")
    
    materials_map = [
        (0, "Skin", "/Game/Characters/Naruto/Materials/MI_Naruto_Skin"),
        (1, "Navy", "/Game/Characters/Naruto/Materials/MI_Naruto_Navy"),
        (2, "Hair", "/Game/Characters/Naruto/Materials/MI_Naruto_Hair"),
        (3, "Collar", "/Game/Characters/Naruto/Materials/MI_Naruto_Collar"),
        (4, "Tracksuit", "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"),
        (5, "Emblem", "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem"),
        (6, "Metal", "/Game/Characters/Naruto/Materials/MI_Naruto_Metal"),
    ]
    for slot_idx, slot_name, mat_path in materials_map:
        mat_asset = unreal.EditorAssetLibrary.load_asset(mat_path)
        if mat_asset and slot_idx < len(mesh_mats):
            mesh_mats[slot_idx].set_editor_property("material_interface", mat_asset)
            lines.append(f"  Slot {slot_idx} ({slot_name}) -> {mat_asset.get_name()}")
        elif mat_asset:
            lines.append(f"  Slot {slot_idx} out of range (slots count = {len(mesh_mats)})")
            
    sk_mesh.set_editor_property("materials", mesh_mats)
    unreal.EditorLoadingAndSavingUtils.save_packages([sk_mesh.get_package(), skeleton.get_package()], True)

# Re-bind skeleton to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"Compiled ABP with skeleton {skeleton.get_name()}!")

# Reconnect to the level actor in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
actors = unreal.EditorLevelLibrary.get_all_level_actors()
for a in actors:
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        a.skeletal_mesh_component.set_skeletal_mesh_asset(sk_mesh)
        if abp:
            a.skeletal_mesh_component.set_anim_instance_class(abp.generated_class())
        a.modify()
        lines.append("Reconnected SK_Naruto_Genin_ProvingActor to SK_Naruto_Genin & ABP!")
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path} successfully!")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
