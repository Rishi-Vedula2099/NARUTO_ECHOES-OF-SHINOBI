import unreal

print("=== STARTING COMPONENT MATERIAL FIX ===")

# 1. Update Master Material for Skeletal Mesh usage
master_mat = unreal.load_asset("/Game/Materials/Anime/M_AnimeCel_Master")
if master_mat:
    try:
        master_mat.set_editor_property("used_with_skeletal_mesh", True)
        unreal.MaterialEditingLibrary.recompile_material(master_mat)
        unreal.EditorAssetLibrary.save_loaded_asset(master_mat)
        print("Set used_with_skeletal_mesh=True on M_AnimeCel_Master")
    except Exception as e:
        print(f"Master mat skeletal flag notice: {e}")

# Load Naruto Material Instances
mat_paths = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
]

mats = []
for p in mat_paths:
    m = unreal.load_asset(p)
    if not m:
        print(f"ERROR: Failed to load material {p}")
    else:
        print(f"Loaded: {m.get_name()}")
        unreal.EditorAssetLibrary.save_loaded_asset(m)
    mats.append(m)

# 2. Fix Skeletal Mesh Asset materials
mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.load_asset(mesh_path)
if sk_mesh:
    mesh_mats = sk_mesh.get_editor_property("materials")
    for i in range(min(len(mesh_mats), len(mats))):
        if mats[i]:
            mesh_mats[i].set_editor_property("material_interface", mats[i])
    sk_mesh.set_editor_property("materials", mesh_mats)
    unreal.EditorAssetLibrary.save_loaded_asset(sk_mesh)
    print("Updated and saved SK_Naruto_Genin_Rigged asset materials.")

# 3. Fix Actor Component in L_VisualAssetSmokeTest
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            for i, m in enumerate(mats):
                if m:
                    comp.set_material(i, m)
                    print(f"Set actor component slot {i} -> {m.get_name()}")
            
            # Rebind Animation Blueprint
            abp_class = unreal.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin.ABP_Naruto_Genin_C")
            if abp_class:
                comp.set_anim_instance_class(abp_class)
                print("Re-bound ABP_Naruto_Genin to actor component.")
        a.modify()
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved map L_VisualAssetSmokeTest successfully.")
