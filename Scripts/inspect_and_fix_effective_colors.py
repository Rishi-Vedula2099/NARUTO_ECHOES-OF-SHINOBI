import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\effective_colors.txt"
lines = []

EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

mi_path = "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit"
mi = unreal.load_asset(mi_path)

if mi:
    lines.append(f"Loaded {mi_path}")
    lines.append(f"Parent: {mi.get_editor_property('parent').get_name()}")
    
    # Check vector parameters
    vecs = mi.get_editor_property("vector_parameter_values")
    lines.append(f"Vector parameter values count: {len(vecs)}")
    for v in vecs:
        p_name = v.parameter_info.name
        color = v.parameter_value
        lines.append(f"  Param '{p_name}': R={color.r:.3f}, G={color.g:.3f}, B={color.b:.3f}")
        
    # Check parent master material name
    parent = mi.get_editor_property("parent")
    if parent:
        lines.append(f"Parent name: {parent.get_name()}")

# Also test: If we create a simple Unlit or DefaultLit material with pure Orange and assign it to slot 4, what happens?
# Let's check light angle:
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight):
        # Camera is at (0, -500, 85) looking +Y. Naruto is at (0, -350, 0).
        # To shine directly at Naruto's front from the camera's shoulder:
        # Pitch = -30.0, Yaw = 60.0, Roll = 0.0
        # In this rotation, light vector points towards (+X, +Y, -Z), directly illuminating Naruto's front face!
        a.set_actor_rotation(unreal.Rotator(pitch=-30.0, yaw=60.0, roll=0.0), False)
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.set_editor_property("intensity", 8.0)
            dlc.set_editor_property("light_color", unreal.Color(255, 248, 235, 255))
        a.modify()
        lines.append("Set DirectionalLight Yaw=60, Pitch=-30 (over camera shoulder into Naruto's face)")
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved map.")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Completed inspect_and_fix_effective_colors.py")
