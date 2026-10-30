import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
glb_path = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "test_box.glb")

task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "test_box"
task.replace_existing = True
task.automated = True
task.save = False

at = unreal.AssetToolsHelpers.get_asset_tools()
at.import_asset_tasks([task])

imported = unreal.load_asset("/Game/Characters/Naruto/Meshes/test_box")
print(f"=== GLB IMPORT RESULT: {imported} ===")
p_out = os.path.join(PROJECT_DIR, "Content", "glb_test_result.txt")
with open(p_out, "w") as f:
    f.write(f"Imported GLB: {imported}\n")
    if imported:
        f.write(f"Class: {imported.get_class().get_name()}\n")
