"""
NARUTO: ECHOES OF SHINOBI
Import Authored Naruto Genin Mesh & Setup Anime Materials
=========================================================
Imports SK_Naruto_Genin.obj into UE5 and binds the 5 production anime materials:
- Slot 0: MI_Naruto_Skin
- Slot 1: MI_Naruto_Navy (Headband fabric & sandals)
- Slot 2: MI_Naruto_Hair (Signature spiky blonde)
- Slot 3: MI_Naruto_Collar (High popped white collar)
- Slot 4: MI_Naruto_Tracksuit (Vibrant Genin orange)
"""

import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SRC_OBJ = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "SK_Naruto_Genin.obj")
DEST_DIR = "/Game/Characters/Naruto/Meshes"
ASSET_NAME = "SK_Naruto_Genin"
PKG_PATH = f"{DEST_DIR}/{ASSET_NAME}"

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

unreal.log(f"=== Importing Authored Naruto Mesh: {SRC_OBJ} ===")

if not os.path.exists(SRC_OBJ):
    unreal.log_error(f"Source OBJ does not exist: {SRC_OBJ}")
else:
    task = unreal.AssetImportTask()
    task.filename = SRC_OBJ
    task.destination_path = DEST_DIR
    task.destination_name = ASSET_NAME
    task.replace_existing = True
    task.automated = True
    task.save = True

    opts = unreal.FbxImportUI()
    opts.import_mesh = True
    opts.import_as_skeletal = False
    opts.import_materials = False
    opts.import_textures = False
    opts.static_mesh_import_data.combine_meshes = True
    try:
        opts.static_mesh_import_data.normal_import_method = unreal.FBXNormalImportMethod.FBXNIM_IMPORT_NORMALS
    except Exception:
        pass
    task.options = opts

    AT.import_asset_tasks([task])
    unreal.log(f"Import task completed for {PKG_PATH}")


mesh = unreal.load_asset(PKG_PATH)
if not mesh:
    unreal.log_error(f"Failed to load imported mesh: {PKG_PATH}")
else:
    unreal.log(f"Loaded {mesh.get_name()} with {mesh.get_num_sections(0)} sections")

# Helper to create/update Material Instance
def get_or_create_mi(name, base_color, roughness=0.85):
    folder = "/Game/Characters/Naruto/Materials"
    pkg = f"{folder}/{name}"
    EAL.make_directory(folder)
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)

    parent = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
    if parent and mi:
        MEL.set_material_instance_parent(mi, parent)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
        MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(0,0,0,1))
        MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", 0.0)
        MEL.set_material_instance_scalar_parameter_value(mi, "RimLightIntensity", 0.0)
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
    return mi

mi_skin      = get_or_create_mi("MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), roughness=0.85)
mi_navy      = get_or_create_mi("MI_Naruto_Navy",      (0.10, 0.16, 0.28, 1.0), roughness=0.80)
mi_hair      = get_or_create_mi("MI_Naruto_Hair",      (0.98, 0.85, 0.15, 1.0), roughness=0.65)
mi_collar    = get_or_create_mi("MI_Naruto_Collar",    (0.95, 0.95, 0.92, 1.0), roughness=0.85)
mi_tracksuit = get_or_create_mi("MI_Naruto_Tracksuit", (0.96, 0.42, 0.05, 1.0), roughness=0.85)

materials = [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit]

if mesh and isinstance(mesh, unreal.StaticMesh):
    num_sections = mesh.get_num_sections(0)
    unreal.log(f"Assigning materials to {num_sections} sections of {mesh.get_name()}...")
    for i in range(num_sections):
        mat_to_use = materials[i % len(materials)]
        mesh.set_material(i, mat_to_use)
        unreal.log(f"  Slot {i}: {mat_to_use.get_name()}")
    skin_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
    if EAL.does_asset_exist(skin_path):
        EAL.delete_asset(skin_path)
    EAL.duplicate_asset(PKG_PATH, skin_path)
    skin_mesh = unreal.load_asset(skin_path)
    if skin_mesh and isinstance(skin_mesh, unreal.StaticMesh):
        for i in range(skin_mesh.get_num_sections(0)):
            skin_mesh.set_material(i, materials[i % len(materials)])
        EAL.save_asset(skin_mesh.get_path_name())
    unreal.log("=== Naruto Mesh & Materials Successfully Configured and Saved ===")

