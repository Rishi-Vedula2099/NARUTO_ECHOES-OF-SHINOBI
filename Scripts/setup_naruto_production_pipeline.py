import unreal
import os

out_log = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\setup_naruto_pipeline_result.txt"
lines = []

EAL = unreal.EditorAssetLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary

lines.append("=== STARTING NARUTO PRODUCTION PIPELINE SETUP ===")

# 1. Master Material & 7 Material Instances
master_path = "/Game/Materials/Master/M_AnimeCel_Master"
master_mat = unreal.load_asset(master_path)
lines.append(f"Master Material M_AnimeCel_Master: {master_mat.get_name() if master_mat else 'NOT FOUND'}")

mat_configs = [
    ("MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), 0.85, 0.08),
    ("MI_Naruto_Navy",      (0.06, 0.10, 0.22, 1.0), 0.80, 0.12),
    ("MI_Naruto_Hair",      (1.00, 0.84, 0.10, 1.0), 0.60, 0.25),
    ("MI_Naruto_Collar",    (0.95, 0.95, 0.95, 1.0), 0.85, 0.05),
    ("MI_Naruto_Tracksuit", (1.00, 0.45, 0.04, 1.0), 0.82, 0.08),
    ("MI_Naruto_Emblem",    (0.85, 0.08, 0.08, 1.0), 0.70, 0.10),
    ("MI_Naruto_Metal",     (0.85, 0.88, 0.92, 1.0), 0.35, 0.75),
]

folder_mats = "/Game/Characters/Naruto/Materials"
EAL.make_directory(folder_mats)

loaded_mis = {}
for name, color, rough, spec in mat_configs:
    pkg = f"{folder_mats}/{name}"
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder_mats, unreal.MaterialInstanceConstant, factory)
        lines.append(f"Created MaterialInstance: {name}")

    if mi and master_mat:
        MEL.set_material_instance_parent(mi, master_mat)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*color))
        MEL.set_material_instance_vector_parameter_value(mi, "SkinToneMultiplier", unreal.LinearColor(1, 1, 1, 1))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", rough)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", spec)
        MEL.update_material_instance(mi)
        EAL.save_asset(pkg)
        lines.append(f"Configured {name} with M_AnimeCel_Master parent.")
    loaded_mis[name] = mi

# Also ensure MI_Naruto_Genin exists for compatibility
mi_genin_pkg = f"{folder_mats}/MI_Naruto_Genin"
mi_genin = unreal.load_asset(mi_genin_pkg)
if not mi_genin:
    factory = unreal.MaterialInstanceConstantFactoryNew()
    mi_genin = AT.create_asset("MI_Naruto_Genin", folder_mats, unreal.MaterialInstanceConstant, factory)
if mi_genin and master_mat:
    MEL.set_material_instance_parent(mi_genin, master_mat)
    MEL.set_material_instance_vector_parameter_value(mi_genin, "BaseColor", unreal.LinearColor(1.0, 0.45, 0.04, 1.0))
    MEL.update_material_instance(mi_genin)
    EAL.save_asset(mi_genin_pkg)

# 2. Re-import latest GLB if needed and inspect skeletal meshes
glb_source = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"
if os.path.exists(glb_source):
    lines.append(f"GLB source found: {glb_source} ({os.path.getsize(glb_source)} bytes)")

sk_mesh_paths = [
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged",
]

primary_sk_mesh = None
primary_skeleton = None

for path in sk_mesh_paths:
    sk = unreal.load_asset(path)
    if sk and isinstance(sk, unreal.SkeletalMesh):
        lines.append(f"Loaded SkeletalMesh: {path}")
        if not primary_sk_mesh:
            primary_sk_mesh = sk
            primary_skeleton = sk.get_editor_property("skeleton")

        # Assign the 7 material slots
        mats = list(sk.get_editor_property("materials"))
        lines.append(f"  Slot count: {len(mats)}")
        slot_names = ["Skin", "Navy", "Hair", "Collar", "Tracksuit", "Emblem", "Metal"]
        for idx, slot in enumerate(mats):
            slot_name_str = str(slot.get_editor_property("material_slot_name"))
            # Match by slot name or fallback by index
            chosen_mi = None
            for key in slot_names:
                if key.lower() in slot_name_str.lower():
                    chosen_mi = loaded_mis.get(f"MI_Naruto_{key}")
                    break
            if not chosen_mi and idx < len(slot_names):
                chosen_mi = loaded_mis.get(f"MI_Naruto_{slot_names[idx]}")

            if chosen_mi:
                slot.set_editor_property("material_interface", chosen_mi)
                lines.append(f"    Slot {idx} ('{slot_name_str}') -> {chosen_mi.get_name()}")

        sk.set_editor_property("materials", mats)
        EAL.save_loaded_asset(sk)
        lines.append(f"Saved {path} with updated materials.")

# Ensure /Game/Characters/Naruto/Meshes/SK_Naruto_Genin exists by copying if not present
canonical_dest = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
if primary_sk_mesh and not EAL.does_asset_exist(canonical_dest):
    EAL.duplicate_asset(primary_sk_mesh.get_path_name(), canonical_dest)
    lines.append(f"Duplicated primary SK mesh to {canonical_dest}")

# 3. ABP Binding
abp_path = "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin"
abp = unreal.load_asset(abp_path)
if abp and primary_skeleton:
    abp.set_editor_property("target_skeleton", primary_skeleton)
    unreal.BlueprintEditorLibrary.compile_blueprint(abp)
    EAL.save_loaded_asset(abp)
    lines.append(f"Compiled ABP_Naruto_Genin with Skeleton {primary_skeleton.get_name()}")

# 4. Update L_VisualAssetSmokeTest Proving Actor
smoke_map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world_smoke = unreal.EditorLoadingAndSavingUtils.load_map(smoke_map_path)
if world_smoke and primary_sk_mesh:
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in eas.get_all_level_actors():
        if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
            comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
            if comp:
                comp.set_skeletal_mesh_asset(primary_sk_mesh)
                if abp:
                    comp.set_anim_instance_class(abp.generated_class())
                for idx, slot_key in enumerate(["Skin", "Navy", "Hair", "Collar", "Tracksuit", "Emblem", "Metal"]):
                    mi = loaded_mis.get(f"MI_Naruto_{slot_key}")
                    if mi:
                        comp.set_material(idx, mi)
            a.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
            a.modify()
            lines.append("Updated SK_Naruto_Genin_ProvingActor in L_VisualAssetSmokeTest with production mesh & materials")
    unreal.EditorLoadingAndSavingUtils.save_map(world_smoke, smoke_map_path)
    lines.append("Saved L_VisualAssetSmokeTest")

# 5. Inspect and save L_Konoha_GoldenStreet
golden_map_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world_golden = unreal.EditorLoadingAndSavingUtils.load_map(golden_map_path)
if world_golden:
    lines.append(f"Loaded primary map: {golden_map_path}")
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = eas.get_all_level_actors()
    lines.append(f"Primary map actor count: {len(actors)}")
    unreal.EditorLoadingAndSavingUtils.save_map(world_golden, golden_map_path)

with open(out_log, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("SETUP SCRIPT FINISHED SUCCESSFULLY")

