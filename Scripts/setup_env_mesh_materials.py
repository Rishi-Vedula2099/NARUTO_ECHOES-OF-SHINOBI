import unreal

EAL = unreal.EditorAssetLibrary

def setup_mesh_materials():
    unreal.log("=== BINDING PRODUCTION ANIME MATERIALS TO STATIC MESH ASSETS ===")

    mi_wood = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
    mi_stone = unreal.load_asset("/Game/Materials/Environment/MI_AnimeStone")
    mi_metal = unreal.load_asset("/Game/Materials/Characters/MI_AnimeMetal")
    mi_foliage = unreal.load_asset("/Game/Materials/Environment/MI_AnimeFoliage")
    mi_ground = unreal.load_asset("/Game/Materials/Environment/MI_AnimeGround")
    mi_cloth = unreal.load_asset("/Game/Materials/Characters/MI_AnimeCloth")
    mi_cedar = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
    mi_lantern_stone = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
    mi_hokage = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
    mi_torii = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
    mi_noren = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")

    mesh_bindings = [
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku", mi_noren or mi_cloth),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WaterPumpingStation", mi_metal),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01", mi_lantern_stone or mi_stone),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large", mi_cedar or mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01", mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized", mi_ground or mi_foliage),
        ("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument", mi_hokage or mi_stone),
        ("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate", mi_torii or mi_wood),
    ]

    for mesh_path, mat in mesh_bindings:
        if not mat:
            unreal.log_warning(f"Target material is null for {mesh_path}")
            continue
        mesh = unreal.load_asset(mesh_path)
        if not mesh:
            unreal.log_warning(f"StaticMesh not found: {mesh_path}")
            continue

        num = len(mesh.static_materials)
        if num == 0:
            mesh.set_material(0, mat)
        else:
            for idx in range(num):
                mesh.set_material(idx, mat)

        EAL.save_loaded_asset(mesh)
        unreal.log(f"  [OK] Saved {mesh.get_name()} with {mat.get_name()}")

    unreal.log("=== STATIC MESH MATERIAL BINDINGS COMPLETE ===")

if __name__ == "__main__":
    setup_mesh_materials()
