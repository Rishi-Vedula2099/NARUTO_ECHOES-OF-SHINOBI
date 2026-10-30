import unreal

EAL = unreal.EditorAssetLibrary

source_7slot = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
dest_sk1 = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin"
dest_sk2 = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"

mesh_7slot = unreal.load_asset(source_7slot)
if mesh_7slot:
    mats = list(mesh_7slot.get_editor_property("materials"))
    print(f"Source 7-slot mesh has {len(mats)} material slots:")
    for i, m in enumerate(mats):
        print(f"  Slot {i}: {m.get_editor_property('material_slot_name')} -> {m.get_editor_property('material_interface').get_name() if m.get_editor_property('material_interface') else 'None'}")

    # Delete destination if exists and duplicate
    for dest in [dest_sk1, dest_sk2]:
        if EAL.does_asset_exist(dest):
            EAL.delete_asset(dest)
        EAL.duplicate_asset(source_7slot, dest)
        d_asset = unreal.load_asset(dest)
        d_mats = list(d_asset.get_editor_property("materials"))
        print(f"Duplicated to {dest} (Slot count: {len(d_mats)})")

    # Update Proving Actor in L_VisualAssetSmokeTest
    world_smoke = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in eas.get_all_level_actors():
        if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
            comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
            if comp:
                comp.set_skeletal_mesh_asset(mesh_7slot)
                for i, m in enumerate(mats):
                    mi = m.get_editor_property("material_interface")
                    if mi:
                        comp.set_material(i, mi)
            a.modify()
            print("Updated SK_Naruto_Genin_ProvingActor with 7-slot mesh & materials!")
    unreal.EditorLoadingAndSavingUtils.save_map(world_smoke, "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
    print("Saved L_VisualAssetSmokeTest!")

with open(r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\sync_mesh_result.txt", "w") as f:
    f.write("SYNC COMPLETE")

