import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\character_cel_result.txt"
lines = []

EAL = unreal.EditorAssetLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary

# 1. Ensure M_AnimeCel_Master exists and is compiled
master_path = "/Game/Materials/Master/M_AnimeCel_Master"
master_mat = unreal.load_asset(master_path)
lines.append(f"Master material: {master_mat.get_name() if master_mat else 'None'}")

# 2. Configure 7 Naruto Material Instances to use M_AnimeCel_Master
mats_info = [
    ("MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), 0.85, 0.08),
    ("MI_Naruto_Navy",      (0.06, 0.10, 0.22, 1.0), 0.80, 0.12),
    ("MI_Naruto_Hair",      (1.00, 0.84, 0.10, 1.0), 0.60, 0.25),
    ("MI_Naruto_Collar",    (0.95, 0.95, 0.95, 1.0), 0.85, 0.05),
    ("MI_Naruto_Tracksuit", (1.00, 0.45, 0.04, 1.0), 0.82, 0.08),
    ("MI_Naruto_Emblem",    (0.85, 0.08, 0.08, 1.0), 0.70, 0.10),
    ("MI_Naruto_Metal",     (0.85, 0.88, 0.92, 1.0), 0.35, 0.75),
]

folder = "/Game/Characters/Naruto/Materials"
EAL.make_directory(folder)

configured_mats = []
for name, color, rough, spec in mats_info:
    pkg = f"{folder}/{name}"
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)
    
    if mi and master_mat:
        MEL.set_material_instance_parent(mi, master_mat)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*color))
        MEL.set_material_instance_vector_parameter_value(mi, "SkinToneMultiplier", unreal.LinearColor(1, 1, 1, 1))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", rough)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", spec)
        MEL.update_material_instance(mi)
        EAL.save_asset(pkg)
        lines.append(f"Configured {name} -> BaseColor=({color[0]}, {color[1]}, {color[2]}) with parent M_AnimeCel_Master")
    configured_mats.append(mi)

# 3. Assign to SK_Naruto_Genin_Rigged
mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.load_asset(mesh_path)
if sk_mesh:
    mesh_mats = list(sk_mesh.get_editor_property("materials"))
    for i, mi in enumerate(configured_mats):
        if i < len(mesh_mats) and mi:
            mesh_mats[i].set_editor_property("material_interface", mi)
            lines.append(f"  Assigned slot {i} -> {mi.get_name()}")
    sk_mesh.set_editor_property("materials", mesh_mats)
    EAL.save_loaded_asset(sk_mesh)
    lines.append("Saved SK_Naruto_Genin_Rigged")

# 4. Update L_VisualAssetSmokeTest lighting & actor
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    label = a.get_actor_label()
    if label == "SK_Naruto_Genin_ProvingActor":
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            for i, mi in enumerate(configured_mats):
                if mi:
                    comp.set_material(i, mi)
        # Face camera (Yaw = 180)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
        a.modify()
        lines.append("Updated SK_Naruto_Genin_ProvingActor materials and rotation (Yaw=180)")
    elif isinstance(a, unreal.DirectionalLight):
        # Rotate key light to illuminate front of Naruto warmly
        a.set_actor_rotation(unreal.Rotator(pitch=-35.0, yaw=-120.0, roll=0.0), False)
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.set_editor_property("intensity", 10.0)
            dlc.set_editor_property("light_color", unreal.Color(255, 245, 230, 255)) # 5400K warm
        a.modify()
        lines.append("Adjusted DirectionalLight for direct front-hero illumination")
    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.set_editor_property("intensity", 3.0)
            slc.set_editor_property("light_color", unreal.Color(190, 220, 255, 255)) # Azure fill
        a.modify()
        lines.append("Adjusted SkyLight for clean azure shadow fill")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved L_VisualAssetSmokeTest successfully")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Completed apply_true_anime_cel_character_materials.py")
