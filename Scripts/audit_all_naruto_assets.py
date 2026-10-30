import unreal
import os

out_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\naruto_audit_output.txt"
lines = []

def log(msg):
    lines.append(msg)
    unreal.log(msg)

log("=================================================================")
log("       EOS COMPREHENSIVE NARUTO ASSET & MATERIAL AUDIT           ")
log("=================================================================")

EAL = unreal.EditorAssetLibrary

check_assets = [
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin",
    "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton",
    "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton",
    "/Game/Characters/Naruto/Anims/ABP_Naruto_Genin",
    "/Game/Materials/Master/M_AnimeCel_Master",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
    "/Game/Characters/Naruto/Textures/T_Naruto_Genin_D",
]

for p in check_assets:
    exists = EAL.does_asset_exist(p)
    if not exists:
        log(f"[MISSING] {p}")
        continue
    asset = unreal.load_asset(p)
    if not asset:
        log(f"[LOAD_FAIL] {p}")
        continue
    cls_name = asset.get_class().get_name()
    log(f"[FOUND] {p} | Class: {cls_name}")

    if isinstance(asset, unreal.SkeletalMesh):
        mats = asset.get_editor_property("materials")
        log(f"   -> SkeletalMesh Materials ({len(mats)} slots):")
        for i, m in enumerate(mats):
            slot_name = m.get_editor_property("material_slot_name")
            mat_interface = m.get_editor_property("material_interface")
            mi_name = mat_interface.get_name() if mat_interface else "None"
            log(f"      Slot {i} [{slot_name}]: {mi_name}")
    elif isinstance(asset, unreal.StaticMesh):
        num_sections = asset.get_num_sections(0)
        log(f"   -> StaticMesh LOD 0 ({num_sections} sections):")
        for i in range(num_sections):
            mat = asset.get_material(i)
            mat_name = mat.get_name() if mat else "None"
            log(f"      Section {i}: {mat_name}")

with open(out_path, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

log(f"Saved audit output to: {out_path}")

