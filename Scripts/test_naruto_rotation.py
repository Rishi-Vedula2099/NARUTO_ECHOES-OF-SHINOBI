import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\test_rotations.txt"
lines = []

naruto_actor = None
for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        naruto_actor = a
        break

if naruto_actor:
    comp = naruto_actor.skeletal_mesh_component
    lines.append(f"Actor location: {naruto_actor.get_actor_location()}")
    lines.append(f"Actor rotation: {naruto_actor.get_actor_rotation()}")
    
    # In gate2_naruto_character_upright.png, Naruto was lying on the ground with head at left, feet at right.
    # Pitch = 90 tilts the X axis up to Z!
    # Roll = 90 tilts the Y axis up to Z!
    # Let's rotate so feet are at Z=0 and head is at Z=169:
    # Set Actor rotation to Pitch = 0, Yaw = 0, Roll = 90 (or Pitch = -90)
    naruto_actor.set_actor_rotation(unreal.Rotator(90.0, 0.0, 0.0), False)
    naruto_actor.set_actor_location(unreal.Vector(0.0, -450.0, 0.0), False, False)
    naruto_actor.modify()
    lines.append(f"Set Actor rotation to Pitch=90, Yaw=0, Roll=0")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved map.")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
