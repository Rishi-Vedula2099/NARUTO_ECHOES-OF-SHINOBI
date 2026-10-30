import unreal

glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"
out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\reimport_upright_result.txt"
lines = []

# 1. Run direct import into SK_Naruto_Genin
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "SK_Naruto_Genin"
task.replace_existing = True
task.automated = True
task.save = True

asset_tools.import_asset_tasks([task])
imported = task.get_editor_property("imported_object_paths")
lines.append(f"Imported objects: {imported}")

# Find skeletal mesh and skeleton
sk_mesh = None
skeleton = None

for p in imported:
    obj = unreal.EditorAssetLibrary.load_asset(p)
    if isinstance(obj, unreal.SkeletalMesh):
        sk_mesh = obj
    elif isinstance(obj, unreal.Skeleton):
        skeleton = obj

if not sk_mesh:
    sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
if not skeleton:
    skeleton = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton")

lines.append(f"SK Mesh: {sk_mesh}")
lines.append(f"Skeleton: {skeleton}")

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
    lines.append(f"Multi-material mesh has {len(mesh_mats)} material slots!")
    for slot_idx, slot_name, mat_path in materials_map:
        mat_asset = unreal.EditorAssetLibrary.load_asset(mat_path)
        if mat_asset and slot_idx < len(mesh_mats):
            mesh_mats[slot_idx].set_editor_property("material_interface", mat_asset)
            lines.append(f"  Slot {slot_idx} ({slot_name}) -> {mat_asset.get_name()}")
        elif mat_asset:
            lines.append(f"  Slot {slot_idx} out of range (slots count = {len(mesh_mats)})")
    sk_mesh.set_editor_property("materials", mesh_mats)
    unreal.EditorLoadingAndSavingUtils.save_packages([sk_mesh.get_package()], True)

# Bind skeleton to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"Compiled ABP with skeleton {skeleton.get_name()}!")

# Update Actor in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.skeletal_mesh_component
        comp.set_simulate_physics(False)
        comp.set_skeletal_mesh_asset(sk_mesh)
        if abp:
            comp.set_anim_instance_class(abp.generated_class())
        a.set_actor_location(unreal.Vector(0.0, -450.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)
        a.modify()
        lines.append("Configured SK_Naruto_Genin_ProvingActor upright at (0, -450, 0) facing camera!")
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path} successfully!")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("=== REIMPORT COMPLETE ===")
