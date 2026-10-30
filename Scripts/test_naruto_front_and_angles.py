import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
all_actors = eas.get_all_level_actors()

cam_actor = None
naruto_actor = None
player_start = None

for a in all_actors:
    label = a.get_actor_label()
    if label == "Camera_Naruto_Showcase":
        cam_actor = a
    elif label == "SK_Naruto_Genin_ProvingActor":
        naruto_actor = a
    elif label == "PlayerStart_NarutoGenin":
        player_start = a

# 1. Position Naruto at (0, -350, 0) facing the camera (Yaw = 0.0)
if naruto_actor:
    naruto_actor.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
    naruto_actor.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)
    naruto_actor.modify()
    print("Set Naruto location (0, -350, 0), Rot=(0, 0, 0)")

# 2. Position Camera at (0, -500, 85), looking +Y directly at Naruto
cam_loc = unreal.Vector(0.0, -500.0, 85.0)
cam_rot = unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0)

if cam_actor:
    cam_actor.set_actor_location(cam_loc, False, False)
    cam_actor.set_actor_rotation(cam_rot, False)
    cam_comp = cam_actor.get_component_by_class(unreal.CameraComponent)
    if cam_comp:
        cam_comp.field_of_view = 36.0
    cam_actor.modify()
    print(f"Set Camera at {cam_loc}, Rot={cam_rot}, FOV=36")

if player_start:
    player_start.set_actor_location(cam_loc, False, False)
    player_start.set_actor_rotation(cam_rot, False)
    player_start.modify()
    print(f"Set PlayerStart at {cam_loc}, Rot={cam_rot}")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved map successfully!")
