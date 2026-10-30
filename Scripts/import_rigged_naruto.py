import unreal, os

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\import_result.txt"
glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"

print(f"=== IMPORTING RIGGED NARUTO GLB INTO UNREAL ENGINE ===")
print(f"Source GLB exists: {os.path.exists(glb_path)} ({os.path.getsize(glb_path)} bytes)")

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
task = unreal.AssetImportTask()
task.filename = glb_path
task.destination_path = "/Game/Characters/Naruto/Meshes"
task.destination_name = "SK_Naruto_Genin"
task.replace_existing = True
task.automated = True
task.save = True

asset_tools.import_asset_tasks([task])

imported_paths = task.get_editor_property("imported_object_paths")
lines = [f"Imported Objects: {imported_paths}"]

for p in imported_paths:
    obj = unreal.EditorAssetLibrary.load_asset(p)
    if obj:
        cname = obj.get_class().get_name()
        lines.append(f"Loaded: {p} ({cname})")
        if isinstance(obj, unreal.SkeletalMesh):
            skel = obj.get_editor_property("skeleton")
            lines.append(f"   SkeletalMesh Skeleton: {skel.get_name() if skel else 'None'}")
            # Bind to ABP_Naruto_Genin!
            abp = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")
            if abp and skel:
                abp.set_editor_property("target_skeleton", skel)
                unreal.EditorLoadingAndSavingUtils.save_packages([abp.get_package()], True)
                lines.append(f"   Bound skeleton to ABP_Naruto_Genin!")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("=== IMPORT PROCESS COMPLETE ===")
