import unreal

EAL = unreal.EditorAssetLibrary
assets = EAL.list_assets('/Game', recursive=True, include_folder=False)
skel_meshes = []
static_meshes = []
anims = []
skeletons = []

for a in assets:
    data = EAL.find_asset_data(a)
    asset_class = str(data.asset_class_path.asset_name)
    if 'SkeletalMesh' in asset_class:
        skel_meshes.append(a)
    elif 'StaticMesh' == asset_class:
        static_meshes.append(a)
    elif 'Anim' in asset_class:
        anims.append(f"{a} ({asset_class})")
    elif 'Skeleton' in asset_class:
        skeletons.append(a)

out_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\audit_result.txt"
with open(out_path, "w", encoding="utf-8") as f:
    f.write("=== ASSET AUDIT REPORT ===\n")
    f.write(f"Total /Game assets: {len(assets)}\n")
    f.write(f"Skeletal Meshes found: {len(skel_meshes)}\n")
    for s in skel_meshes:
        f.write(f"  [SKEL MESH] {s}\n")

    f.write(f"\nSkeletons found: {len(skeletons)}\n")
    for sk in skeletons:
        f.write(f"  [SKELETON] {sk}\n")

    f.write(f"\nAnimation Assets found: {len(anims)}\n")
    for an in anims:
        f.write(f"  [ANIM] {an}\n")

    naruto_mesh = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
    if naruto_mesh:
        f.write(f"\nNaruto mesh actual class: {naruto_mesh.get_class().get_name()}\n")
        if isinstance(naruto_mesh, unreal.StaticMesh):
            f.write(f"  StaticMesh sections: {naruto_mesh.get_num_sections(0)}\n")
            for i, sm in enumerate(naruto_mesh.static_materials):
                mat_name = sm.material_interface.get_name() if sm.material_interface else 'None'
                f.write(f"  Slot {i}: '{sm.material_slot_name}' -> {mat_name}\n")

    # Audit current level actors in Golden Street
    world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
    if world:
        EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
        f.write("\n=== GOLDEN STREET LEVEL ACTORS AUDIT ===\n")
        for act in EAS.get_all_level_actors():
            if not act: continue
            lbl = act.get_actor_label()
            cls = act.get_class().get_name()
            loc = act.get_actor_location()
            rot = act.get_actor_rotation()
            scale = act.get_actor_scale3d()
            mesh_info = "None"
            smc = act.get_component_by_class(unreal.StaticMeshComponent)
            if smc and smc.static_mesh:
                mesh_info = f"StaticMesh: {smc.static_mesh.get_path_name()}"
            skmc = act.get_component_by_class(unreal.SkeletalMeshComponent)
            if skmc and skmc.skeletal_mesh_asset:
                mesh_info = f"SkeletalMesh: {skmc.skeletal_mesh_asset.get_path_name()}"
            f.write(f"  Actor: {lbl:32s} Class: {cls:24s} Loc: ({loc.x:7.1f},{loc.y:7.1f},{loc.z:6.1f}) Mesh: {mesh_info}\n")

print("Audit written to Content/audit_result.txt")
