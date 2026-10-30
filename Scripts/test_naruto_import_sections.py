import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SRC_OBJ = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "SK_Naruto_Genin.obj")
DEST_DIR = "/Game/Characters/Naruto/Meshes"
ASSET_NAME = "SK_Naruto_Genin"
PKG_PATH = f"{DEST_DIR}/{ASSET_NAME}"

AT = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

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
opts.import_materials = True
opts.import_textures = False
opts.static_mesh_import_data.combine_meshes = True
try:
    opts.static_mesh_import_data.normal_import_method = unreal.FBXNormalImportMethod.FBXNIM_IMPORT_NORMALS
except Exception:
    pass
task.options = opts

AT.import_asset_tasks([task])

mesh = unreal.load_asset(PKG_PATH)
if mesh and isinstance(mesh, unreal.StaticMesh):
    num_sections = mesh.get_num_sections(0)
    unreal.log(f"=== MESH SECTIONS: {num_sections} ===")
    for i in range(num_sections):
        mat = mesh.get_material(i)
        unreal.log(f"  Slot {i}: {mat.get_name() if mat else 'None'}")
