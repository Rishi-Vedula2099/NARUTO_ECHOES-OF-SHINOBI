import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
skeleton = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton")
abp = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")

actor_label = "SK_Naruto_Genin_ProvingActor"
naruto_actor = None
player_start = None
cam_actor = None

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    label = a.get_actor_label()
    if label == actor_label:
        naruto_actor = a
    elif isinstance(a, unreal.PlayerStart):
        player_start = a
    elif label == "Camera_Naruto_Showcase":
        cam_actor = a

# 1. Naruto position: In front of the swatches, centered on avenue
naruto_loc = unreal.Vector(0.0, -480.0, 0.0)
naruto_rot = unreal.Rotator(0.0, -90.0, 0.0) # Facing camera (-Y)

if not naruto_actor:
    naruto_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkeletalMeshActor, naruto_loc, naruto_rot)
    naruto_actor.set_actor_label(actor_label)

naruto_actor.set_actor_location(naruto_loc, False, False)
naruto_actor.set_actor_rotation(naruto_rot, False)
naruto_actor.skeletal_mesh_component.set_skeletal_mesh_asset(sk_mesh)
if abp:
    naruto_actor.skeletal_mesh_component.set_anim_instance_class(abp.generated_class())
naruto_actor.modify()

# 2. PlayerStart position: Behind camera looking at Naruto
if player_start:
    player_start.set_actor_location(unreal.Vector(0.0, -780.0, 90.0), False, False)
    player_start.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
    player_start.modify()

# 3. Camera position
if cam_actor:
    cam_actor.set_actor_location(unreal.Vector(0.0, -750.0, 80.0), False, False)
    cam_actor.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
    cam_actor.camera_component.field_of_view = 45.0
    cam_actor.modify()

# Save level persistently
saved = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print(f"Positioned Naruto at {naruto_loc}, PlayerStart at (0, -780, 90). Saved: {saved}")
