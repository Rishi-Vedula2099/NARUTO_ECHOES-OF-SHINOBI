import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\rotate_result.txt"
lines = []

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.skeletal_mesh_component
        comp.set_simulate_physics(False)
        comp.set_enable_gravity(False)
        
        # Roll = 90 tilts Y/Z axis to stand vertically upright!
        rot = unreal.Rotator(0.0, -90.0, 90.0)
        loc = unreal.Vector(0.0, -450.0, 0.0)
        a.set_actor_location(loc, False, False)
        a.set_actor_rotation(rot, False)
        a.modify()
        lines.append(f"Set Naruto actor to {loc}, Rot={rot}")
        break

saved = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path}: {saved}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
