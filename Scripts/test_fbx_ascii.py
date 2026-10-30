import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
fbx_path = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "test_cube.fbx")

# Minimal FBX 7.4 ASCII file
fbx_content = """; FBX 7.4.0 project file
FBXHeaderExtension:  {
	FBXHeaderVersion: 1003
	FBXVersion: 7400
}
Definitions:  {
	Version: 100
	Count: 2
	ObjectType: "Model" {
		Count: 1
	}
}
Objects:  {
	Model: 1000, "Model::TestCube", "Mesh" {
		Version: 232
		Properties70:  {
			P: "InheritType", "enum", "", "",1
		}
		Vertices: *24 {
			a: -10,-10,0, 10,-10,0, 10,10,0, -10,10,0, -10,-10,20, 10,-10,20, 10,10,20, -10,10,20
		}
		PolygonVertexIndex: *24 {
			a: 0,1,2,-4, 4,5,6,-8, 0,1,5,-5, 2,3,7,-7, 0,3,7,-5, 1,2,6,-6
		}
	}
}
Connections:  {
	C: "OO", 1000, 0
}
"""

with open(fbx_path, "w") as f:
    f.write(fbx_content)

task = unreal.AssetImportTask()
task.filename = fbx_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "test_cube"
task.replace_existing = True
task.automated = True
task.save = False

opts = unreal.FbxImportUI()
opts.import_mesh = True
opts.import_as_skeletal = False
task.options = opts

at = unreal.AssetToolsHelpers.get_asset_tools()
at.import_asset_tasks([task])

imported = unreal.load_asset("/Game/Characters/Naruto/Meshes/test_cube")
print(f"Imported FBX test asset: {imported}")
p_out = os.path.join(PROJECT_DIR, "Content", "fbx_test_result.txt")
with open(p_out, "w") as f:
    f.write(f"Imported: {imported}\n")
