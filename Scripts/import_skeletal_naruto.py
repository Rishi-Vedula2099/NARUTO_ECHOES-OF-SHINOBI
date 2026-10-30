import unreal, os

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\import_skeletal_result.txt"
glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"
target_package = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"

lines = []
lines.append("=== SKELETAL MESH IMPORT AND BINDING ===")

# 1. Delete existing StaticMesh if present
if unreal.EditorAssetLibrary.does_asset_exist(target_package):
    old_asset = unreal.EditorAssetLibrary.load_asset(target_package)
    if old_asset and not isinstance(old_asset, unreal.SkeletalMesh):
        lines.append(f"Deleting old non-skeletal asset: {target_package} ({old_asset.get_class().get_name()})")
        unreal.EditorAssetLibrary.delete_asset(target_package)
    else:
        lines.append(f"Existing asset is {old_asset.get_class().get_name() if old_asset else 'None'}")

# 2. Setup Import Task
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "SK_Naruto_Genin"
task.replace_existing = True
task.automated = True
task.save = True

asset_tools.import_asset_tasks([task])

imported_paths = task.get_editor_property("imported_object_paths")
lines.append(f"Imported Objects: {imported_paths}")

skel_mesh = None
skeleton = None

for p in imported_paths:
    obj = unreal.EditorAssetLibrary.load_asset(p)
    if obj:
        cname = obj.get_class().get_name()
        lines.append(f"Loaded: {p} -> Class: {cname}")
        if isinstance(obj, unreal.SkeletalMesh):
            skel_mesh = obj
            skeleton = obj.get_editor_property("skeleton")
            lines.append(f"   SkeletalMesh Skeleton: {skeleton.get_path_name() if skeleton else 'None'}")
        elif isinstance(obj, unreal.Skeleton):
            skeleton = obj

# Also check if skeleton was saved in destination path
if not skeleton:
    skel_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton"
    if unreal.EditorAssetLibrary.does_asset_exist(skel_path):
        skeleton = unreal.EditorAssetLibrary.load_asset(skel_path)
        lines.append(f"Found Skeleton at {skel_path}")

# Check destination directly
dest_asset = unreal.EditorAssetLibrary.load_asset(target_package)
if dest_asset:
    lines.append(f"Destination asset {target_package} -> Class: {dest_asset.get_class().get_name()}")
    if isinstance(dest_asset, unreal.SkeletalMesh):
        skel_mesh = dest_asset
        skeleton = dest_asset.get_editor_property("skeleton")
        lines.append(f"   Destination Skeleton: {skeleton.get_path_name() if skeleton else 'None'}")

# 3. Bind to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"SUCCESS: Bound {skeleton.get_name()} to {abp_path}!")
elif abp:
    lines.append(f"ABP found but skeleton is None")
else:
    lines.append(f"ABP not found at {abp_path}")

# 4. Assign Production Materials to SK_Naruto_Genin
if skel_mesh:
    # Production anime materials:
    # MI_Naruto_Skin, MI_Naruto_Hair, MI_Naruto_Tracksuit, MI_Naruto_Collar, MI_Naruto_Navy
    mat_tracksuit = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
    if mat_tracksuit:
        skel_mesh.set_material(0, mat_tracksuit)
        lines.append("Assigned MI_Naruto_Tracksuit to slot 0")
    unreal.EditorLoadingAndSavingUtils.save_packages([skel_mesh.get_package()], True)

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("=== SKELETAL IMPORT PROCESS COMPLETE ===")
