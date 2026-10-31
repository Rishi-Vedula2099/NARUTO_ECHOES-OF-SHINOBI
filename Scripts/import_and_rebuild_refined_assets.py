import unreal
import os

out_log = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\refined_assets_import_result.txt"
lines = []

EAL = unreal.EditorAssetLibrary
AT = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary

lines.append("=== IMPORTING REFINED HERO CHARACTER & ARCHITECTURAL ASSETS ===")

SRC_DIR = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes"

# 1. Import SK_Naruto_Genin_Rigged.glb
glb_path = os.path.join(SRC_DIR, "SK_Naruto_Genin_Rigged.glb")
if os.path.exists(glb_path):
    lines.append(f"Importing GLB: {glb_path}")
    task = unreal.AssetImportTask()
    task.filename = glb_path
    task.destination_path = "/Game/Characters/Naruto/Meshes"
    task.destination_name = "SK_Naruto_Genin_Rigged"
    task.replace_existing = True
    task.automated = True
    task.save = True
    AT.import_asset_tasks([task])
    lines.append("Imported GLB successfully.")

# Material map for Naruto
mat_map = {
    "Skin": "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "Navy": "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "Hair": "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "Collar": "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "Tracksuit": "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "Emblem": "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "Metal": "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
}

mat_order = ["Skin", "Navy", "Hair", "Collar", "Tracksuit", "Emblem", "Metal"]

sk_paths = [
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
]

for sk_p in sk_paths:
    sk = EAL.load_asset(sk_p)
    if sk and isinstance(sk, unreal.SkeletalMesh):
        mesh_mats = sk.get_editor_property("materials")
        lines.append(f"Configuring SkeletalMesh: {sk_p} ({len(mesh_mats)} slots)")
        for i, slot in enumerate(mesh_mats):
            slot_name = str(slot.get_editor_property("material_slot_name"))
            t_path = mat_map.get(slot_name)
            if not t_path and i < len(mat_order):
                t_path = mat_map[mat_order[i]]
            if t_path:
                mi_asset = EAL.load_asset(t_path)
                if mi_asset:
                    slot.set_editor_property("material_interface", mi_asset)
                    lines.append(f"  Slot {i} '{slot_name}' -> {t_path}")
        EAL.save_asset(sk_p)

# 2. Import Refined Environmental Architecture Models
env_imports = [
    ("SM_Bld_IchirakuRamen.obj",         "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_IchirakuRamen"),
    ("SM_Bld_Konoha_Residence_A.obj",     "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_A"),
    ("SM_Bld_Konoha_Residence_B.obj",     "/Game/Environment/Konoha/Buildings/Meshes", "SM_Bld_Konoha_Residence_B"),
    ("SM_Lnd_HokageFaces_Monument.obj",   "/Game/Environment/Konoha/Landmarks/Meshes", "SM_Lnd_HokageFaces_Monument"),
    ("SM_Lnd_ValleyOfTheEnd_Statues.obj", "/Game/Environment/Konoha/Landmarks/Meshes", "SM_Lnd_ValleyOfTheEnd_Statues"),
]

for obj_file, dest_path, dest_name in env_imports:
    full_obj = os.path.join(SRC_DIR, obj_file)
    if os.path.exists(full_obj):
        EAL.make_directory(dest_path)
        task = unreal.AssetImportTask()
        task.filename = full_obj
        task.destination_path = dest_path
        task.destination_name = dest_name
        task.replace_existing = True
        task.automated = True
        task.save = True
        AT.import_asset_tasks([task])
        lines.append(f"Imported StaticMesh: {dest_path}/{dest_name}")

# 3. Bind Environmental Materials
# Ichiraku
mi_wood = EAL.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_roof = EAL.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_noren = EAL.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
mi_lantern = EAL.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_PaperLantern")

ichi_mesh = EAL.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
if ichi_mesh and isinstance(ichi_mesh, unreal.StaticMesh):
    ichi_slots = ichi_mesh.get_editor_property("static_materials")
    lines.append(f"SM_Bld_IchirakuRamen has {len(ichi_slots)} slots")
    for i, s in enumerate(ichi_slots):
        s_name = str(s.get_editor_property("material_slot_name"))
        if "Wood" in s_name or i == 0:
            ichi_mesh.set_material(i, mi_wood)
        elif "Roof" in s_name or i == 1:
            ichi_mesh.set_material(i, mi_roof)
        elif "Noren" in s_name or i == 2:
            ichi_mesh.set_material(i, mi_noren)
        elif "Lantern" in s_name or i == 3:
            ichi_mesh.set_material(i, mi_lantern)
    EAL.save_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
    lines.append("Configured materials for SM_Bld_IchirakuRamen.")

# Hokage Rock
mi_rock = EAL.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
rock_mesh = EAL.load_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument")
if rock_mesh and isinstance(rock_mesh, unreal.StaticMesh):
    rock_slots = rock_mesh.get_editor_property("static_materials")
    for i in range(len(rock_slots)):
        rock_mesh.set_material(i, mi_rock)
    EAL.save_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument")
    lines.append("Configured materials for SM_Lnd_HokageFaces_Monument.")

# Valley of the End
valley_mesh = EAL.load_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_ValleyOfTheEnd_Statues")
if valley_mesh and isinstance(valley_mesh, unreal.StaticMesh):
    v_slots = valley_mesh.get_editor_property("static_materials")
    for i in range(len(v_slots)):
        valley_mesh.set_material(i, mi_rock)
    EAL.save_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_ValleyOfTheEnd_Statues")
    lines.append("Configured materials for SM_Lnd_ValleyOfTheEnd_Statues.")

with open(out_log, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
print("Import and rebuild completed.")
