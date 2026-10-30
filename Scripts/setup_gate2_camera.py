import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

# 1. Ensure Camera_ProvingBox_Swatches or Camera_Naruto_Showcase is perfectly framed on Naruto
cam_label = "Camera_Naruto_Showcase"
found_cam = None
found_naruto = None

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == cam_label:
        found_cam = a
    elif a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        found_naruto = a

if not found_cam:
    cam_loc = unreal.Vector(0.0, -520.0, 75.0)
    cam_rot = unreal.Rotator(0.0, 90.0, 0.0) # looking towards +Y (at Y=-300)
    found_cam = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.CameraActor, cam_loc, cam_rot)
    if found_cam:
        found_cam.set_actor_label(cam_label)

if found_cam:
    found_cam.set_actor_location(unreal.Vector(0.0, -520.0, 75.0), False, False)
    found_cam.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
    cam_comp = found_cam.camera_component
    cam_comp.field_of_view = 45.0
    found_cam.modify()

if found_naruto:
    found_naruto.set_actor_location(unreal.Vector(0.0, -300.0, 0.0), False, False)
    found_naruto.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False) # Face the camera (facing -Y)
    found_naruto.modify()

# Make this camera active or primary for PlayerStart
unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Successfully framed Camera_Naruto_Showcase directly on Naruto!")
