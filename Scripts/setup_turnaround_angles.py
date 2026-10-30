import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

naruto_actor = None
camera_actor = None

for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        naruto_actor = a
    elif a.get_actor_label() == "Gate2_Naruto_Camera":
        camera_actor = a

if naruto_actor and camera_actor:
    loc = naruto_actor.get_actor_location()
    print(f"Naruto Loc: {loc}")
    print(f"Camera Loc: {camera_actor.get_actor_location()}, Rot: {camera_actor.get_actor_rotation()}")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
