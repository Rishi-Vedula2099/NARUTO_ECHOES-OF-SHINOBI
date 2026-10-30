import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_current_naruto_state.txt"
lines = []

asset_paths = [
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
]

for p in asset_paths:
    obj = unreal.EditorAssetLibrary.load_asset(p)
    if obj:
        lines.append(f"Asset: {p} -> Class: {obj.get_class().get_name()}")
        if isinstance(obj, unreal.SkeletalMesh):
            lines.append(f"  Skeleton: {obj.get_editor_property('skeleton')}")
            mats = obj.get_editor_property("materials")
            lines.append(f"  Materials count: {len(mats)}")
            for i, m in enumerate(mats):
                lines.append(f"    Slot {i}: {m.get_editor_property('material_slot_name')} = {m.get_editor_property('material_interface')}")
        elif isinstance(obj, unreal.StaticMesh):
            lines.append(f"  StaticMesh materials count: {obj.get_num_sections(0)}")
    else:
        lines.append(f"Asset: {p} -> NOT FOUND")

# Check actor in smoke test map
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
for a in unreal.EditorLevelLibrary.get_all_level_actors():
    if "Naruto" in a.get_actor_label():
        lines.append(f"Actor in level: {a.get_actor_label()} (Class: {a.get_class().get_name()})")
        lines.append(f"  Location: {a.get_actor_location()}")
        lines.append(f"  Rotation: {a.get_actor_rotation()}")
        for c in a.get_components_by_class(unreal.SkeletalMeshComponent):
            lines.append(f"  SkeletalMeshComponent:")
            lines.append(f"    SkeletalMeshAsset: {c.get_skeletal_mesh_asset()}")
            lines.append(f"    AnimClass: {c.get_editor_property('anim_class')}")
            lines.append(f"    SimulatePhysics: {c.is_simulating_physics()}")
            lines.append(f"    RelativeRotation: {c.get_editor_property('relative_rotation')}")
        for c in a.get_components_by_class(unreal.StaticMeshComponent):
            lines.append(f"  StaticMeshComponent:")
            lines.append(f"    StaticMesh: {c.static_mesh}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Inspection script completed.")
