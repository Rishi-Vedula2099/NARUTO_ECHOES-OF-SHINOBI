import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_actor_state.txt"
lines = []

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.skeletal_mesh_component
        lines.append(f"Actor location: {a.get_actor_location()}")
        lines.append(f"Actor rotation: {a.get_actor_rotation()}")
        lines.append(f"Component relative location: {comp.get_relative_location()}")
        lines.append(f"Component relative rotation: {comp.get_relative_rotation()}")
        lines.append(f"Simulate physics: {comp.is_simulating_physics()}")
        lines.append(f"Collision enabled: {comp.get_collision_enabled()}")
        lines.append(f"Physics asset: {comp.get_physics_asset()}")
        lines.append(f"Animation mode: {comp.get_editor_property('animation_mode')}")
        lines.append(f"Anim class: {comp.get_editor_property('anim_class')}")
        
        # Turn off physics simulation, lock to upright standing posture
        comp.set_simulate_physics(False)
        comp.set_enable_gravity(False)
        # In glTF, if Z was forward and Y was up:
        # Check bone orientations
        sk_mesh = comp.skeletal_mesh_asset
        if sk_mesh:
            lines.append(f"Mesh materials count: {len(sk_mesh.get_editor_property('materials'))}")
            for idx, m in enumerate(sk_mesh.get_editor_property('materials')):
                lines.append(f"  Slot {idx}: {m.material_slot_name} -> {m.material_interface.get_name() if m.material_interface else 'None'}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
