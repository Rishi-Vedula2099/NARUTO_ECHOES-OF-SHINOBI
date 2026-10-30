import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\bind_naruto_abp_result.txt"
lines = []

source_sk_mesh = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin"
source_skeleton = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton"
source_phys = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_PhysicsAsset"

dest_sk_mesh = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
dest_skeleton = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton"
dest_phys = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_PhysicsAsset"

# Rename / Move to final canonical paths
for src, dst in [(source_sk_mesh, dest_sk_mesh), (source_skeleton, dest_skeleton), (source_phys, dest_phys)]:
    if unreal.EditorAssetLibrary.does_asset_exist(src):
        if unreal.EditorAssetLibrary.does_asset_exist(dst):
            unreal.EditorAssetLibrary.delete_asset(dst)
        success = unreal.EditorAssetLibrary.rename_asset(src, dst)
        lines.append(f"Moved {src} -> {dst}: {success}")

# Load canonical skeletal mesh and skeleton
sk_mesh = unreal.EditorAssetLibrary.load_asset(dest_sk_mesh)
skeleton = unreal.EditorAssetLibrary.load_asset(dest_skeleton)

lines.append(f"Loaded SK Mesh: {sk_mesh} ({sk_mesh.get_class().get_name() if sk_mesh else 'None'})")
lines.append(f"Loaded Skeleton: {skeleton} ({skeleton.get_class().get_name() if skeleton else 'None'})")

# Assign materials to SK_Naruto_Genin
if sk_mesh:
    mat_tracksuit = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
    if mat_tracksuit:
        mats = sk_mesh.get_editor_property("materials")
        lines.append(f"Mesh has {len(mats)} material slots")
        if len(mats) > 0:
            mats[0].set_editor_property("material_interface", mat_tracksuit)
            sk_mesh.set_editor_property("materials", mats)
            lines.append(f"Assigned {mat_tracksuit.get_name()} to slot 0!")
        unreal.EditorLoadingAndSavingUtils.save_packages([sk_mesh.get_package()], True)

# Bind Skeleton to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    lines.append(f"Bound skeleton {skeleton.get_name()} to {abp_path}")
    
    # Compile the AnimBlueprint
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"Compiled and saved {abp_path} successfully!")
else:
    lines.append(f"Failed to bind: abp={abp}, skeleton={skeleton}")

# Also update BP_Naruto_Genin default mesh if it has a SkeletalMeshComponent
bp_naruto_path = "/Game/Characters/Naruto/Blueprints/BP_Naruto_Genin"
bp_naruto = unreal.EditorAssetLibrary.load_asset(bp_naruto_path)
if bp_naruto and sk_mesh:
    lines.append(f"Loaded character BP: {bp_naruto_path}")
    # Inspect or compile BP_Naruto_Genin
    unreal.BlueprintEditorLibrary.compile_blueprint(bp_naruto)
    unreal.EditorLoadingAndSavingUtils.save_packages([bp_naruto.get_package()], True)

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("=== BIND AND COMPILE SCRIPT COMPLETED ===")
