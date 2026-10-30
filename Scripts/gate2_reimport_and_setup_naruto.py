import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\gate2_reimport_result.txt"
lines = []

glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"

# 1. Run direct import
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "SK_Naruto_Genin_Rigged"
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
    sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin")
if not skeleton:
    skeleton = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton")

lines.append(f"SK Mesh: {sk_mesh}")
lines.append(f"Skeleton: {skeleton}")

# 2. Check and assign all 7 material slots
mat_map = {
    "Skin": "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "Navy": "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "Hair": "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "Collar": "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "Tracksuit": "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "Emblem": "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "Metal": "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
}

if sk_mesh:
    mesh_mats = sk_mesh.get_editor_property("materials")
    lines.append(f"Mesh has {len(mesh_mats)} material slots:")
    for i, slot in enumerate(mesh_mats):
        slot_name = str(slot.get_editor_property("material_slot_name"))
        lines.append(f"  Slot {i}: '{slot_name}'")
        # Try match by slot_name first, or by index order
        target_mat_path = mat_map.get(slot_name)
        if not target_mat_path:
            keys = list(mat_map.keys())
            if i < len(keys):
                target_mat_path = mat_map[keys[i]]
        
        if target_mat_path:
            m_asset = unreal.EditorAssetLibrary.load_asset(target_mat_path)
            if m_asset:
                slot.set_editor_property("material_interface", m_asset)
                lines.append(f"    Assigned -> {m_asset.get_name()}")
            else:
                lines.append(f"    Failed to load {target_mat_path}")

    sk_mesh.set_editor_property("materials", mesh_mats)
    unreal.EditorLoadingAndSavingUtils.save_packages([sk_mesh.get_package()], True)

# 3. Bind skeleton to ABP_Naruto_Genin
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.EditorAssetLibrary.load_asset(abp_path)
if abp and skeleton:
    abp.set_editor_property("target_skeleton", skeleton)
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
    lines.append(f"Compiled ABP with skeleton {skeleton.get_name()}!")

# 4. Configure Actor and Camera in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
all_actors = eas.get_all_level_actors()

naruto_actor = None
cam_actor = None

for a in all_actors:
    label = a.get_actor_label()
    if label == "SK_Naruto_Genin_ProvingActor":
        naruto_actor = a
    elif label == "Camera_Naruto_Showcase":
        cam_actor = a

if naruto_actor and sk_mesh:
    naruto_actor.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
    # Face towards the camera (camera is at -Y, looking +Y; character faces -Y)
    naruto_actor.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
    comp = naruto_actor.get_component_by_class(unreal.SkeletalMeshComponent)
    if comp:
        comp.set_simulate_physics(False)
        comp.set_enable_gravity(False)
        comp.set_skeletal_mesh_asset(sk_mesh)
        if abp:
            comp.set_anim_instance_class(abp.generated_class())
    naruto_actor.modify()
    lines.append(f"Configured SK_Naruto_Genin_ProvingActor at (0, -350, 0), Rot=(0, -90, 0)")

if not cam_actor:
    cam_actor = eas.spawn_actor_from_class(unreal.CameraActor, unreal.Vector(0.0, -650.0, 85.0))
    if cam_actor:
        cam_actor.set_actor_label("Camera_Naruto_Showcase")

if cam_actor:
    cam_actor.set_actor_location(unreal.Vector(0.0, -650.0, 85.0), False, False)
    cam_actor.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
    cam_comp = cam_actor.get_component_by_class(unreal.CameraComponent)
    if cam_comp:
        cam_comp.field_of_view = 40.0
    cam_actor.modify()
    lines.append(f"Configured Camera_Naruto_Showcase at (0, -650, 85), Rot=(0, 90, 0), FOV=40")

# Also ensure PlayerStart is aligned with this camera
for a in all_actors:
    if a.get_actor_label() == "PlayerStart_NarutoGenin":
        a.set_actor_location(unreal.Vector(0.0, -650.0, 85.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.modify()
        lines.append("Aligned PlayerStart_NarutoGenin with camera")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path} successfully!")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Script execution completed successfully.")
