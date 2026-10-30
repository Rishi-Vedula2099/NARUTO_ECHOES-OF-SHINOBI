import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\mesh_checkerboard_debug.txt"
lines = []

mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.load_asset(mesh_path)

if sk_mesh:
    lines.append(f"Loaded {mesh_path}")
    mats = sk_mesh.get_editor_property("materials")
    lines.append(f"Num mesh materials: {len(mats)}")
    for i, m in enumerate(mats):
        slot_name = m.get_editor_property("material_slot_name")
        mat_iface = m.get_editor_property("material_interface")
        lines.append(f"  Slot {i}: '{slot_name}' -> {mat_iface.get_path_name() if mat_iface else 'None'}")
        
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            lines.append("Checking SK_Naruto_Genin_ProvingActor Component:")
            lines.append(f"  SkeletalMeshAsset: {comp.get_skeletal_mesh_asset().get_name() if comp.get_skeletal_mesh_asset() else 'None'}")
            lines.append(f"  AnimClass: {comp.get_editor_property('anim_class')}")
            lines.append(f"  Num comp materials: {comp.get_num_materials()}")
            for i in range(comp.get_num_materials()):
                m = comp.get_material(i)
                lines.append(f"    Comp Mat {i}: {m.get_path_name() if m else 'None'}")
                
            # TEST: Remove AnimClass (set to None) to see if Animation Blueprint was overriding materials or failing!
            comp.set_anim_instance_class(None)
            lines.append("Set AnimClass to None for test")
        a.modify()
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved map.")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Mesh checkerboard debug script complete.")
