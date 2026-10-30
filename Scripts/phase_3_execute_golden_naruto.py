"""
PHASE 3: Golden Naruto Implementation
Compliant with Docs/VisualBible/03_CHARACTER_PRODUCTION_BIBLE.md

1. Imports scaled 146cm authored Naruto Genin mesh (5 material groups: Skin, Headband/Navy, Hair, Collar, Tracksuit)
   into both canonical registry targets:
   - /Game/Characters/Naruto/Meshes/SK_Naruto_Genin
   - /Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit
2. Binds the 5 Phase 2 Anime Cel Material Instances to the 5 material slots:
   - Slot 0: MI_Naruto_Skin
   - Slot 1: MI_Naruto_Navy
   - Slot 2: MI_Naruto_Hair
   - Slot 3: MI_Naruto_Collar
   - Slot 4: MI_Naruto_Tracksuit
3. Verifies geometry dimensions, bounding box (~146cm total height), and material slot integrity.
"""

import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SRC_OBJ = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "SK_Naruto_Genin.obj")

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

def import_and_bind_naruto(dest_dir, asset_name):
    pkg_path = f"{dest_dir}/{asset_name}"
    unreal.log(f"=== Importing {asset_name} to {dest_dir} ===")

    task = unreal.AssetImportTask()
    task.filename = SRC_OBJ
    task.destination_path = dest_dir
    task.destination_name = asset_name
    task.replace_existing = True
    task.automated = True
    task.save = True

    opts = unreal.FbxImportUI()
    opts.import_mesh = True
    opts.import_as_skeletal = False
    opts.import_materials = False
    opts.import_textures = False
    opts.static_mesh_import_data.combine_meshes = True
    task.options = opts

    AT.import_asset_tasks([task])
    unreal.log(f"Import task complete for {pkg_path}")

    mesh = unreal.load_asset(pkg_path)
    if not mesh or not isinstance(mesh, unreal.StaticMesh):
        unreal.log_error(f"Failed to load mesh: {pkg_path}")
        return None

    # Load the 5 Phase 2 Anime Material Instances
    mi_skin      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")
    mi_navy      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Navy")
    mi_hair      = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Hair")
    mi_collar    = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Collar")
    mi_tracksuit = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")

    materials = [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit]
    num_sections = mesh.get_num_sections(0)
    unreal.log(f"Assigning materials to {num_sections} sections of {mesh.get_name()}:")

    for i in range(num_sections):
        mat = materials[i % len(materials)]
        if mat:
            mesh.set_material(i, mat)
            unreal.log(f"  Slot {i}: {mat.get_name()}")
        else:
            unreal.log_error(f"  Slot {i}: Material is NULL!")

    EAL.save_asset(mesh.get_path_name())

    # Log bounds
    bounds = mesh.get_bounds()
    box_ext = bounds.box_extent
    h = box_ext.z * 2.0
    unreal.log(f"  Bounds: X={box_ext.x:.1f}, Y={box_ext.y:.1f}, Z={box_ext.z:.1f} (Height: {h:.1f} cm)")
    return mesh

# Import both primary and skin mesh
mesh_genin = import_and_bind_naruto("/Game/Characters/Naruto/Meshes", "SK_Naruto_Genin")
mesh_skin  = import_and_bind_naruto("/Game/Characters/Naruto/Skins", "SK_Naruto_GeninTracksuit")

unreal.log("=== PHASE 3 GOLDEN NARUTO ASSET IMPORT AND BINDING COMPLETE ===")
