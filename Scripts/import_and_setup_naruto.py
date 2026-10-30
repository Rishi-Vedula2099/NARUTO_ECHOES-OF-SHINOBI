import unreal, os

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\setup_naruto_result.txt"
glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"

lines = []
lines.append("=== SETUP PRODUCTION NARUTO SKELETAL MESH & ANIMATION ===")

# Clean up previous temporary assets if needed
old_temp = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged"
if unreal.EditorAssetLibrary.does_directory_exist(old_temp):
    unreal.EditorAssetLibrary.delete_directory(old_temp)

# Run Interchange Import
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "SK_Naruto_Genin_Rigged"
task.replace_existing = True
task.automated = True
task.save = True

asset_tools.import_asset_tasks([task])
lines.append(f"Imported tasks completed. Paths: {task.get_editor_property('imported_object_paths')}")

# Move from SkeletalMeshes subdirectory to canonical paths
src_sk = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin"
src_skel = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton"
src_phys = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_PhysicsAsset"

dst_sk = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
dst_skel = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton"
dst_phys = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_PhysicsAsset"

for src, dst in [(src_sk, dst_sk), (src_skel, dst_skel), (src_phys, dst_phys)]:
    if unreal.EditorAssetLibrary.does_asset_exist(src):
        if unreal.EditorAssetLibrary.does_asset_exist(dst):
            unreal.EditorAssetLibrary.delete_asset(dst)
        ok = unreal.EditorAssetLibrary.rename_asset(src, dst)
        lines.append(f"Moved {src} -> {dst}: {ok}")

# Clean up empty directory
if unreal.EditorAssetLibrary.does_directory_exist(old_temp):
    unreal.EditorAssetLibrary.delete_directory(old_temp)

# Load canonical assets
sk_mesh = unreal.EditorAssetLibrary.load_asset(dst_sk)
skeleton = unreal.EditorAssetLibrary.load_asset(dst_skel)

lines.append(f"Canonical SK Mesh: {sk_mesh} ({sk_mesh.get_class().get_name() if sk_mesh else 'None'})")
lines.append(f"Canonical Skeleton: {skeleton} ({skeleton.get_class().get_name() if skeleton else 'None'})")

# Assign 7 anime material slots:
# 0: Skin, 1: Navy, 2: Hair, 3: Collar, 4: Tracksuit, 5: Emblem, 6: Metal
materials_map = [
    (0, "Skin", "/Game/Characters/Naruto/Materials/MI_Naruto_Skin"),
    (1, "Navy", "/Game/Characters/Naruto/Materials/MI_Naruto_Navy"),
    (2, "Hair", "/Game/Characters/Naruto/Materials/MI_Naruto_Hair"),
    (3, "Collar", "/Game/Characters/Naruto/Materials/MI_Naruto_Collar"),
    (4, "Tracksuit", "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"),
    (5, "Emblem", "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem"),
    (6, "Metal", "/Game/Characters/Naruto/Materials/MI_Naruto_Metal"),
]

if sk_mesh:
    mesh_mats = sk_mesh.get_editor_property("materials")
    lines.append(f"Mesh currently has {len(mesh_mats)} material slots")
    
    for slot_idx, slot_name, mat_path in materials_map:
        mat_asset = unreal.EditorAssetLibrary.load_asset(mat_path)
        if mat_asset and slot_idx < len(mesh_mats):
            mesh_mats[slot_idx].set_editor_property("material_interface", mat_asset)
            lines.append(f"  Slot {slot_idx} ({slot_name}) -> {mat_asset.get_name()}")
        elif mat_asset:
            lines.append(f"  Slot {slot_idx} out of range (slots count = {len(mesh_mats)})")
        else:
            lines.append(f"  Material asset not found: {mat_path}")
            
    sk_mesh.set_editor_property("materials", mesh_mats)
    unreal.EditorLoadingAndSavingUtils.save_packages([sk_mesh.get_package()], True)

# Bind Skeleton to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"Bound skeleton to {abp_path} and compiled successfully!")

# Place or update SkeletalMeshActor in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

# Look for existing actor or spawn new
actor_label = "SK_Naruto_Genin_ProvingActor"
found_actor = None
actors = unreal.EditorLevelLibrary.get_all_level_actors()
for a in actors:
    if a.get_actor_label() == actor_label:
        found_actor = a
        break

if not found_actor:
    spawn_loc = unreal.Vector(0.0, -300.0, 0.0)
    spawn_rot = unreal.Rotator(0.0, 90.0, 0.0) # facing camera
    found_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkeletalMeshActor, spawn_loc, spawn_rot)
    if found_actor:
        found_actor.set_actor_label(actor_label)
        lines.append(f"Spawned new SkeletalMeshActor at {spawn_loc}")

if found_actor and sk_mesh:
    found_actor.skeletal_mesh_component.set_skeletal_mesh_asset(sk_mesh)
    if abp:
        found_actor.skeletal_mesh_component.set_anim_instance_class(abp.generated_class())
    found_actor.set_actor_location(unreal.Vector(0.0, -300.0, 0.0), False, False)
    found_actor.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
    found_actor.modify()
    lines.append("Configured SkeletalMeshActor with SK_Naruto_Genin and ABP_Naruto_Genin!")

# Save the level persistently
saved = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path}: {saved}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("=== SETUP COMPLETE ===")
