import unreal
import sys

# Angle argument passed via python or default
yaw_val = float(sys.argv[1]) if len(sys.argv) > 1 else 45.0

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        rot = a.get_actor_rotation()
        a.set_actor_rotation(unreal.Rotator(pitch=rot.pitch, yaw=yaw_val, roll=rot.roll), True)
        print(f"Set Naruto Yaw to {yaw_val}")
        a.modify()
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved map.")
