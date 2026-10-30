"""
Sets up M_AnimeCel_Master with real parameters and creates rich, vibrant
Material Instances for all Konoha buildings, roads, vegetation, landmarks, and characters.
"""
import unreal

MEL = unreal.MaterialEditingLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

def setup_master_material():
    master_path = "/Game/Materials/Master/M_AnimeCel_Master"
    mat = unreal.load_asset(master_path)
    if not mat:
        factory = unreal.MaterialFactoryNew()
        mat = AT.create_asset("M_AnimeCel_Master", "/Game/Materials/Master", unreal.Material, factory)

    if not mat:
        unreal.log_error(f"Could not load or create {master_path}")
        return None

    # Clear old expressions to rebuild cleanly
    MEL.delete_all_material_expressions(mat)

    # 1. BaseColor Vector Parameter
    bc_expr = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -400, -100)
    bc_expr.set_editor_property("parameter_name", "BaseColor")
    bc_expr.set_editor_property("default_value", unreal.LinearColor(0.85, 0.6, 0.35, 1.0))
    MEL.connect_material_property(bc_expr, "", unreal.MaterialProperty.MP_BASE_COLOR)

    # 2. Roughness Scalar Parameter
    rough_expr = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 150)
    rough_expr.set_editor_property("parameter_name", "Roughness")
    rough_expr.set_editor_property("default_value", 0.8)
    MEL.connect_material_property(rough_expr, "", unreal.MaterialProperty.MP_ROUGHNESS)

    # 3. Specular Scalar Parameter
    spec_expr = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 300)
    spec_expr.set_editor_property("parameter_name", "Specular")
    spec_expr.set_editor_property("default_value", 0.15)
    MEL.connect_material_property(spec_expr, "", unreal.MaterialProperty.MP_SPECULAR)

    # 4. Emissive Color Vector Parameter
    em_expr = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -400, 450)
    em_expr.set_editor_property("parameter_name", "EmissiveColor")
    em_expr.set_editor_property("default_value", unreal.LinearColor(0.0, 0.0, 0.0, 1.0))
    MEL.connect_material_property(em_expr, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log(f"[MATERIAL] Successfully configured and recompiled {master_path}")
    return mat

def create_or_update_mi(name, folder, base_color, roughness=0.8, emissive=(0,0,0,1)):
    pkg_path = f"{folder}/{name}"
    EAL.make_directory(folder)
    mi = unreal.load_asset(pkg_path)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)

    parent = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
    if parent and mi:
        MEL.set_material_instance_parent(mi, parent)
        MEL.set_material_instance_vector_parameter_value(
            mi, "BaseColor", unreal.LinearColor(*base_color)
        )
        MEL.set_material_instance_scalar_parameter_value(
            mi, "Roughness", roughness
        )
        MEL.set_material_instance_vector_parameter_value(
            mi, "EmissiveColor", unreal.LinearColor(*emissive)
        )
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
        unreal.log(f"  [MI] Configured {pkg_path} with BaseColor={base_color}")
    return mi

master = setup_master_material()

# Palettes based on Naruto Anime Art Direction:
# Ichiraku Ramen: Warm amber Hinoki wood, terracotta roof, cream banner
mi_ichiraku_wood = create_or_update_mi("MI_Bld_Ichiraku_Wood", "/Game/Environment/Konoha/Buildings/Materials", (0.76, 0.52, 0.28, 1.0), 0.85)
mi_ichiraku_roof = create_or_update_mi("MI_Bld_Ichiraku_Roof", "/Game/Environment/Konoha/Buildings/Materials", (0.82, 0.28, 0.14, 1.0), 0.70)
mi_ichiraku_noren= create_or_update_mi("MI_Bld_Ichiraku_Noren","/Game/Environment/Konoha/Buildings/Materials", (0.95, 0.92, 0.82, 1.0), 0.90)

# Residences & Shops: Traditional cream plaster walls and dark slate/wood roofs
mi_residence_a   = create_or_update_mi("MI_Bld_Residence_A",   "/Game/Environment/Konoha/Buildings/Materials", (0.88, 0.82, 0.72, 1.0), 0.85)
mi_residence_b   = create_or_update_mi("MI_Bld_Residence_B",   "/Game/Environment/Konoha/Buildings/Materials", (0.35, 0.38, 0.42, 1.0), 0.75)
mi_hokage_mansion= create_or_update_mi("MI_Bld_HokageMansion", "/Game/Environment/Konoha/Buildings/Materials", (0.85, 0.30, 0.18, 1.0), 0.80)

# Roads & Plaza: Warm Konoha stone pavers
mi_road          = create_or_update_mi("MI_Road_Cobblestone",   "/Game/Environment/Konoha/Roads/Materials",     (0.58, 0.55, 0.50, 1.0), 0.90)

# Landmarks: Warm carved Hokage sandstone cliff, Vermilion Torii gate
mi_hokage_rock   = create_or_update_mi("MI_HokageRock",        "/Game/Environment/Konoha/Landmarks/Materials", (0.68, 0.58, 0.45, 1.0), 0.95)
mi_torii_red     = create_or_update_mi("MI_Torii_Red",         "/Game/Environment/Konoha/Landmarks/Materials", (0.78, 0.12, 0.08, 1.0), 0.70)

# Foliage: Vibrant Anime Cedar Green and Cherry Blossom Pink
mi_cedar         = create_or_update_mi("MI_Veg_CedarNeedles_Stylized", "/Game/Environment/Konoha/Foliage/Materials", (0.18, 0.42, 0.18, 1.0), 0.85)
mi_sakura        = create_or_update_mi("MI_Veg_PinkPetals_Translucent", "/Game/Environment/Konoha/Foliage/Materials", (0.96, 0.65, 0.72, 1.0), 0.75)

# Props: Lantern Granite and Warm Glowing Lantern Light
mi_lantern_stone = create_or_update_mi("MI_Prop_Lantern_Stone","/Game/Environment/Konoha/Props/Materials",     (0.50, 0.48, 0.45, 1.0), 0.90)
mi_lantern_glow  = create_or_update_mi("MI_Prop_Lantern_Glow", "/Game/Environment/Konoha/Props/Materials",     (1.0, 0.85, 0.40, 1.0), 0.40, emissive=(2.5, 1.6, 0.4, 1.0))

# Character: Naruto Tracksuit Orange, Navy Blue, Spiky Yellow Hair, Skin
mi_naruto_orange = create_or_update_mi("MI_Naruto_Tracksuit",  "/Game/Characters/Naruto/Materials",            (0.92, 0.38, 0.06, 1.0), 0.85)
mi_naruto_blue   = create_or_update_mi("MI_Naruto_Navy",       "/Game/Characters/Naruto/Materials",            (0.10, 0.18, 0.32, 1.0), 0.80)
mi_naruto_hair   = create_or_update_mi("MI_Naruto_Hair",       "/Game/Characters/Naruto/Materials",            (0.98, 0.84, 0.15, 1.0), 0.60)
mi_naruto_skin   = create_or_update_mi("MI_Naruto_Skin",       "/Game/Characters/Naruto/Materials",            (0.98, 0.82, 0.70, 1.0), 0.85)
mi_naruto_collar = create_or_update_mi("MI_Naruto_Collar",     "/Game/Characters/Naruto/Materials",            (0.95, 0.95, 0.92, 1.0), 0.85)
mi_sky           = create_or_update_mi("MI_Sky_AnimeDaylight",  "/Game/Environment/Konoha/Sky/Materials",       (0.32, 0.62, 0.96, 1.0), 1.0, emissive=(0.32, 0.62, 0.96, 1.0))

# Character: Kakashi Jonin Vest, Navy Undergarment, Silver Hair, Mask
mi_kakashi_vest  = create_or_update_mi("MI_Kakashi_Vest",      "/Game/Characters/Kakashi/Materials",           (0.25, 0.35, 0.20, 1.0), 0.85)
mi_kakashi_navy  = create_or_update_mi("MI_Kakashi_Navy",      "/Game/Characters/Kakashi/Materials",           (0.10, 0.14, 0.22, 1.0), 0.80)
mi_kakashi_hair  = create_or_update_mi("MI_Kakashi_Hair",      "/Game/Characters/Kakashi/Materials",           (0.88, 0.90, 0.94, 1.0), 0.65)
mi_kakashi_mask  = create_or_update_mi("MI_Kakashi_Mask",      "/Game/Characters/Kakashi/Materials",           (0.14, 0.15, 0.18, 1.0), 0.85)

# Character: Teuchi NPC Apron and Kimono Shirt
mi_teuchi_apron  = create_or_update_mi("MI_Teuchi_Apron",      "/Game/Characters/NPCs/Shopkeepers/Materials",  (0.94, 0.94, 0.90, 1.0), 0.85)
mi_teuchi_shirt  = create_or_update_mi("MI_Teuchi_Shirt",      "/Game/Characters/NPCs/Shopkeepers/Materials",  (0.38, 0.28, 0.20, 1.0), 0.85)

# Character: Rogue Ninja / Enemy
mi_enemy_rogue   = create_or_update_mi("MI_Enemy_Rogue",       "/Game/Characters/Enemies/Materials",           (0.28, 0.24, 0.32, 1.0), 0.80)
mi_target_dummy  = create_or_update_mi("MI_Target_Dummy",      "/Game/Environment/Konoha/Props/Materials",     (0.72, 0.58, 0.35, 1.0), 0.90)

# Assign materials to the primary Static Meshes
def assign_material(mesh_path, mi):
    mesh = unreal.load_asset(mesh_path)
    if mesh and mi and isinstance(mesh, unreal.StaticMesh):
        num = max(mesh.get_num_sections(0), 1)
        for i in range(num):
            mesh.set_material(i, mi)
        EAL.save_asset(mesh.get_path_name())
        unreal.log(f"  [ASSIGNED] {mesh_path} -> {mi.get_name()}")

assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", mi_ichiraku_wood)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", mi_residence_a)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", mi_residence_b)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion", mi_hokage_mansion)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse", mi_ichiraku_wood)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice", mi_residence_a)
assign_material("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop", mi_ichiraku_wood)
assign_material("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01", mi_road)
assign_material("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument", mi_hokage_rock)
assign_material("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate", mi_torii_red)
assign_material("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", mi_cedar)
assign_material("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom", mi_sakura)
assign_material("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01", mi_lantern_stone)
assign_material("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post", mi_target_dummy)

# Also assign materials to character meshes
assign_material("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit", mi_naruto_orange)
assign_material("/Game/Characters/Kakashi/Skins/SK_Kakashi_StandardJonin", mi_kakashi_vest)
assign_material("/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi", mi_teuchi_apron)
assign_material("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee", mi_enemy_rogue)

unreal.log("=== ALL ANIME CEL MATERIALS CONFIGURED AND ASSIGNED SUCCESSFULLY ===")
