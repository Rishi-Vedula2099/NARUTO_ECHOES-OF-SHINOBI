import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
AT = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

def import_obj(obj_name, dest_dir, asset_name):
    obj_path = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", obj_name)
    print(f"Importing {obj_path} -> {dest_dir}/{asset_name}...")
    task = unreal.AssetImportTask()
    task.filename = obj_path
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
    opts.static_mesh_import_data.generate_lightmap_u_vs = False
    task.options = opts

    AT.import_asset_tasks([task])
    imported = unreal.load_asset(f"{dest_dir}/{asset_name}")
    if imported:
        print(f"  [SUCCESS] {dest_dir}/{asset_name} ({imported.get_num_sections(0)} sections)")
    else:
        print(f"  [FAILED] {dest_dir}/{asset_name}")
    return imported

# 1. Import meshes
mesh_naruto = import_obj("SK_Naruto_Genin.obj", "/Game/Characters/Naruto/Meshes", "SK_Naruto_Genin")
mesh_skin   = import_obj("SK_Naruto_GeninTracksuit.obj", "/Game/Characters/Naruto/Skins", "SK_Naruto_GeninTracksuit")

# 2. Material mapping
naruto_materials = {
    "skin":      unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Skin") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Skin"),
    "navy":      unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Navy") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Navy"),
    "hair":      unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Hair") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Hair"),
    "collar":    unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Collar") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Collar"),
    "tracksuit": unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Tracksuit"),
    "emblem":    unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Emblem") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Genin_Emblem"),
    "metal":     unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Metal") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Headband"),
}

print("\nResolved Material Instances:")
for k, v in naruto_materials.items():
    print(f"  {k}: {v.get_path_name() if v else 'MISSING'}")

def bind_slots(mesh_path, slot_map):
    mesh = unreal.load_asset(mesh_path)
    if not mesh or not isinstance(mesh, unreal.StaticMesh):
        print(f"Could not load mesh: {mesh_path}")
        return
    num_sections = mesh.get_num_sections(0)
    print(f"\nBinding {mesh_path} ({num_sections} sections):")
    for i, sm in enumerate(mesh.static_materials):
        slot_name = str(sm.material_slot_name).lower()
        matched = False
        for k, mat in slot_map.items():
            if k in slot_name and mat:
                mesh.set_material(i, mat)
                print(f"  Slot {i} [{sm.material_slot_name}] -> {mat.get_name()}")
                matched = True
                break
        if not matched:
            print(f"  Slot {i} [{sm.material_slot_name}] -> NOT MATCHED")
    EAL.save_loaded_asset(mesh)

bind_slots("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", naruto_materials)
bind_slots("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit", naruto_materials)

print("\n=== NARUTO MASTERPIECE IMPORT & BINDING COMPLETE ===")
