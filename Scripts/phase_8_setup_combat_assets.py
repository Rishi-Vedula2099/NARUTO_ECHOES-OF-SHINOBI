"""
NARUTO: ECHOES OF SHINOBI
Phase 8: Combat & VFX Asset Setup Script
"""
import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SOURCE_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
PROPS_DEST = "/Game/Environment/Konoha/Props/Meshes"

def setup_combat_assets():
    unreal.log("======================================================================")
    unreal.log("  PHASE 8: COMBAT + JUTSU + VFX ASSET SETUP")
    unreal.log("======================================================================")

    # 1. Check / Import SM_Prop_TargetBoard
    tb_path = f"{PROPS_DEST}/SM_Prop_TargetBoard"
    tb_mesh = unreal.load_asset(tb_path)
    if not tb_mesh:
        unreal.log("  Importing SM_Prop_TargetBoard from BP_TrainingDummy.obj...")
        src_obj = os.path.join(SOURCE_DIR, "BP_TrainingDummy.obj")
        if os.path.exists(src_obj):
            at = unreal.AssetToolsHelpers.get_asset_tools()
            task = unreal.AssetImportTask()
            task.filename = src_obj
            task.destination_path = PROPS_DEST
            task.destination_name = "SM_Prop_TargetBoard"
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
            at.import_asset_tasks([task])
            tb_mesh = unreal.load_asset(tb_path)
            unreal.log(f"  [OK] Imported SM_Prop_TargetBoard: {tb_mesh}")

    # 2. Check / Setup materials for SM_Prop_TrainingLog_Post and SM_Prop_TargetBoard
    mat_wood = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
    mat_cloth = unreal.load_asset("/Game/Materials/Characters/MI_AnimeCloth")

    post_path = f"{PROPS_DEST}/SM_Prop_TrainingLog_Post"
    post_mesh = unreal.load_asset(post_path)
    if post_mesh and mat_wood:
        for idx in range(max(1, len(post_mesh.static_materials))):
            post_mesh.set_material(idx, mat_wood)
        unreal.EditorAssetLibrary.save_loaded_asset(post_mesh)
        unreal.log(f"  [OK] Assigned MI_AnimeWood to {post_path}")

    if tb_mesh and (mat_cloth or mat_wood):
        target_mat = mat_cloth if mat_cloth else mat_wood
        for idx in range(max(1, len(tb_mesh.static_materials))):
            tb_mesh.set_material(idx, target_mat)
        unreal.EditorAssetLibrary.save_loaded_asset(tb_mesh)
        unreal.log(f"  [OK] Assigned Material to {tb_path}")

    # 3. Check Niagara Systems
    vfx_rasengan = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master")
    vfx_smoke = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst")
    vfx_chidori = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Chidori_Discharge_Master")
    vfx_fire = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Goukakyuu_Fireball_Master")

    unreal.log(f"  [VFX] NS_Rasengan_Orb_Master:        {'OK' if vfx_rasengan else 'MISSING'}")
    unreal.log(f"  [VFX] NS_KageBunshin_SmokeBurst:     {'OK' if vfx_smoke else 'MISSING'}")
    unreal.log(f"  [VFX] NS_Chidori_Discharge_Master:   {'OK' if vfx_chidori else 'MISSING'}")
    unreal.log(f"  [VFX] NS_Goukakyuu_Fireball_Master: {'OK' if vfx_fire else 'MISSING'}")

    unreal.log("======================================================================")

if __name__ == "__main__":
    setup_combat_assets()
