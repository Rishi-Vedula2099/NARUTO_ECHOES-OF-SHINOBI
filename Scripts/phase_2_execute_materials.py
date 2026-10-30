"""
PHASE 2: Anime Material System Implementation
Compliant with Docs/VisualBible/02_ANIME_MATERIAL_BIBLE.md

1. Rebuilds /Game/Materials/Master/M_AnimeCel_Master with full anime cel parameter graph:
   - BaseColor, SkinToneMultiplier -> Base Color
   - Roughness -> Roughness
   - Specular -> Specular
   - EmissiveColor, EmissiveIntensity + RimLight (Fresnel * RimLightColor * RimLightIntensity) -> Emissive Color
   - ShadowColor, ShadowBias, ShadowSoftness, SubsurfaceWarmth exposed as parameters
2. Authors all required canonical Material Instances:
   - /Game/Materials/Characters/MI_AnimeSkin
   - /Game/Materials/Characters/MI_AnimeHair
   - /Game/Materials/Characters/MI_AnimeCloth
   - /Game/Materials/Characters/MI_AnimeMetal
   - /Game/Materials/Environment/MI_AnimeWood
   - /Game/Materials/Environment/MI_AnimeStone
   - /Game/Materials/Environment/MI_AnimeRoof
   - /Game/Materials/Environment/MI_AnimeFoliage
   - /Game/Materials/Environment/MI_AnimeGround
   - /Game/Materials/Environment/MI_AnimeWater
   - /Game/Materials/VFX/MI_AnimeJutsu
3. Updates and ensures all Character and Konoha Environment Material Instances are fully synchronized.
"""

import unreal

MEL = unreal.MaterialEditingLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

def build_master_anime_cel():
    master_path = "/Game/Materials/Master/M_AnimeCel_Master"
    mat = unreal.load_asset(master_path)
    if not mat:
        factory = unreal.MaterialFactoryNew()
        mat = AT.create_asset("M_AnimeCel_Master", "/Game/Materials/Master", unreal.Material, factory)

    if not mat:
        unreal.log_error(f"Could not load or create {master_path}")
        return None

    # Clear previous expressions
    MEL.delete_all_material_expressions(mat)

    # 1. BaseColor (VectorParameter)
    bc = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -800, -200)
    bc.set_editor_property("parameter_name", "BaseColor")
    bc.set_editor_property("default_value", unreal.LinearColor(0.85, 0.60, 0.35, 1.0))

    # 2. SkinToneMultiplier (VectorParameter)
    stm = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -800, 0)
    stm.set_editor_property("parameter_name", "SkinToneMultiplier")
    stm.set_editor_property("default_value", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))

    # Multiply BaseColor * SkinToneMultiplier -> Base Color
    mul_bc = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -500, -100)
    MEL.connect_material_expressions(bc, "", mul_bc, "A")
    MEL.connect_material_expressions(stm, "", mul_bc, "B")
    MEL.connect_material_property(mul_bc, "", unreal.MaterialProperty.MP_BASE_COLOR)

    # 3. Roughness (ScalarParameter)
    rough = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -500, 150)
    rough.set_editor_property("parameter_name", "Roughness")
    rough.set_editor_property("default_value", 0.8)
    MEL.connect_material_property(rough, "", unreal.MaterialProperty.MP_ROUGHNESS)

    # 4. Specular (ScalarParameter)
    spec = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -500, 300)
    spec.set_editor_property("parameter_name", "Specular")
    spec.set_editor_property("default_value", 0.15)
    MEL.connect_material_property(spec, "", unreal.MaterialProperty.MP_SPECULAR)

    # 5. Emissive Components:
    # Base Emissive = EmissiveColor * EmissiveIntensity
    em_color = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -1000, 450)
    em_color.set_editor_property("parameter_name", "EmissiveColor")
    em_color.set_editor_property("default_value", unreal.LinearColor(0.0, 0.0, 0.0, 1.0))

    em_int = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -1000, 650)
    em_int.set_editor_property("parameter_name", "EmissiveIntensity")
    em_int.set_editor_property("default_value", 1.0)

    mul_em = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -750, 500)
    MEL.connect_material_expressions(em_color, "", mul_em, "A")
    MEL.connect_material_expressions(em_int, "", mul_em, "B")

    # Rim Lighting = Fresnel(Exponent=RimLightPower) * RimLightColor * RimLightIntensity
    fresnel = MEL.create_material_expression(mat, unreal.MaterialExpressionFresnel, -1000, 800)
    rim_pow = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -1250, 850)
    rim_pow.set_editor_property("parameter_name", "RimLightPower")
    rim_pow.set_editor_property("default_value", 3.0)
    MEL.connect_material_expressions(rim_pow, "", fresnel, "Exponent")

    rim_color = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -1000, 1000)
    rim_color.set_editor_property("parameter_name", "RimLightColor")
    rim_color.set_editor_property("default_value", unreal.LinearColor(1.0, 0.95, 0.85, 1.0))

    rim_int = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -1000, 1200)
    rim_int.set_editor_property("parameter_name", "RimLightIntensity")
    rim_int.set_editor_property("default_value", 0.0)

    mul_rim_c = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -750, 850)
    MEL.connect_material_expressions(fresnel, "", mul_rim_c, "A")
    MEL.connect_material_expressions(rim_color, "", mul_rim_c, "B")

    mul_rim_final = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -550, 900)
    MEL.connect_material_expressions(mul_rim_c, "", mul_rim_final, "A")
    MEL.connect_material_expressions(rim_int, "", mul_rim_final, "B")

    # Add Base Emissive + Rim Lighting -> MP_EMISSIVE_COLOR
    add_em_final = MEL.create_material_expression(mat, unreal.MaterialExpressionAdd, -350, 600)
    MEL.connect_material_expressions(mul_em, "", add_em_final, "A")
    MEL.connect_material_expressions(mul_rim_final, "", add_em_final, "B")
    MEL.connect_material_property(add_em_final, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    # 6. Additional parameters for anime shadow and SSS contract (referenced by instances & game shaders)
    shadow_col = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -500, 1350)
    shadow_col.set_editor_property("parameter_name", "ShadowColor")
    shadow_col.set_editor_property("default_value", unreal.LinearColor(0.25, 0.18, 0.22, 1.0))

    shadow_bias = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -500, 1550)
    shadow_bias.set_editor_property("parameter_name", "ShadowBias")
    shadow_bias.set_editor_property("default_value", 0.5)

    shadow_soft = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -500, 1700)
    shadow_soft.set_editor_property("parameter_name", "ShadowSoftness")
    shadow_soft.set_editor_property("default_value", 0.2)

    sss_warm = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -500, 1850)
    sss_warm.set_editor_property("parameter_name", "SubsurfaceWarmth")
    sss_warm.set_editor_property("default_value", 0.0)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log(f"[PHASE 2] M_AnimeCel_Master successfully constructed and recompiled with full cel parameters!")
    return mat

def create_or_update_mi(name, folder, base_color, roughness=0.8, specular=0.15, 
                        rim_intensity=0.0, rim_color=(1.0, 0.95, 0.85, 1.0), rim_power=3.0,
                        emissive_color=(0,0,0,1), emissive_intensity=0.0,
                        shadow_color=(0.25, 0.18, 0.22, 1.0), subsurface_warmth=0.0):
    pkg_path = f"{folder}/{name}"
    EAL.make_directory(folder)
    mi = unreal.load_asset(pkg_path)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)

    parent = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
    if parent and mi:
        MEL.set_material_instance_parent(mi, parent)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
        MEL.set_material_instance_scalar_parameter_value(mi, "RimLightIntensity", rim_intensity)
        MEL.set_material_instance_vector_parameter_value(mi, "RimLightColor", unreal.LinearColor(*rim_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "RimLightPower", rim_power)
        MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(*emissive_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", emissive_intensity)
        MEL.set_material_instance_vector_parameter_value(mi, "ShadowColor", unreal.LinearColor(*shadow_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "SubsurfaceWarmth", subsurface_warmth)
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
        unreal.log(f"  [MI CONFIGURED] {pkg_path}")
    return mi

# Step 1: Build Master
master = build_master_anime_cel()

# Step 2: Canonical Library Instances required by 02_ANIME_MATERIAL_BIBLE.md
unreal.log("=== Creating Canonical Library Instances ===")
create_or_update_mi("MI_AnimeSkin",    "/Game/Materials/Characters",  (0.98, 0.82, 0.70, 1.0), roughness=0.70, specular=0.20, rim_intensity=0.35, subsurface_warmth=0.6)
create_or_update_mi("MI_AnimeHair",    "/Game/Materials/Characters",  (0.98, 0.84, 0.15, 1.0), roughness=0.55, specular=0.35, rim_intensity=0.45)
create_or_update_mi("MI_AnimeCloth",   "/Game/Materials/Characters",  (0.92, 0.38, 0.06, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.20)
create_or_update_mi("MI_AnimeMetal",   "/Game/Materials/Characters",  (0.75, 0.78, 0.82, 1.0), roughness=0.35, specular=0.80, rim_intensity=0.70)

create_or_update_mi("MI_AnimeWood",    "/Game/Materials/Environment", (0.76, 0.52, 0.28, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.10)
create_or_update_mi("MI_AnimeStone",   "/Game/Materials/Environment", (0.58, 0.55, 0.50, 1.0), roughness=0.90, specular=0.10, rim_intensity=0.05)
create_or_update_mi("MI_AnimeRoof",    "/Game/Materials/Environment", (0.82, 0.28, 0.14, 1.0), roughness=0.70, specular=0.15, rim_intensity=0.15)
create_or_update_mi("MI_AnimeFoliage", "/Game/Materials/Environment", (0.18, 0.42, 0.18, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.25, subsurface_warmth=0.4)
create_or_update_mi("MI_AnimeGround",  "/Game/Materials/Environment", (0.60, 0.54, 0.46, 1.0), roughness=0.92, specular=0.08, rim_intensity=0.0)
create_or_update_mi("MI_AnimeWater",   "/Game/Materials/Environment", (0.15, 0.45, 0.65, 1.0), roughness=0.15, specular=0.90, rim_intensity=0.60)

create_or_update_mi("MI_AnimeJutsu",   "/Game/Materials/VFX",         (0.20, 0.60, 1.00, 1.0), roughness=0.20, specular=0.50, rim_intensity=0.80, emissive_color=(0.20, 0.60, 1.00, 1.0), emissive_intensity=2.5)

# Step 3: Naruto Specific Instances
unreal.log("=== Updating Naruto Character Instances ===")
create_or_update_mi("MI_Naruto_Tracksuit", "/Game/Characters/Naruto/Materials", (0.92, 0.38, 0.06, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.25)
create_or_update_mi("MI_Naruto_Navy",      "/Game/Characters/Naruto/Materials", (0.10, 0.18, 0.32, 1.0), roughness=0.80, specular=0.12, rim_intensity=0.25)
create_or_update_mi("MI_Naruto_Hair",      "/Game/Characters/Naruto/Materials", (0.98, 0.84, 0.15, 1.0), roughness=0.55, specular=0.35, rim_intensity=0.45)
create_or_update_mi("MI_Naruto_Skin",      "/Game/Characters/Naruto/Materials", (0.98, 0.82, 0.70, 1.0), roughness=0.70, specular=0.20, rim_intensity=0.35, subsurface_warmth=0.6)
create_or_update_mi("MI_Naruto_Collar",    "/Game/Characters/Naruto/Materials", (0.95, 0.95, 0.92, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.20)

# Step 4: Konoha Environment Specific Instances
unreal.log("=== Updating Konoha Environment Instances ===")
create_or_update_mi("MI_Bld_Ichiraku_Wood", "/Game/Environment/Konoha/Buildings/Materials", (0.76, 0.52, 0.28, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.10)
create_or_update_mi("MI_Bld_Ichiraku_Roof", "/Game/Environment/Konoha/Buildings/Materials", (0.82, 0.28, 0.14, 1.0), roughness=0.70, specular=0.15, rim_intensity=0.15)
create_or_update_mi("MI_Bld_Ichiraku_Noren","/Game/Environment/Konoha/Buildings/Materials", (0.95, 0.92, 0.82, 1.0), roughness=0.90, specular=0.08, rim_intensity=0.10)
create_or_update_mi("MI_Bld_Residence_A",   "/Game/Environment/Konoha/Buildings/Materials", (0.88, 0.82, 0.72, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.10)
create_or_update_mi("MI_Bld_Residence_B",   "/Game/Environment/Konoha/Buildings/Materials", (0.35, 0.38, 0.42, 1.0), roughness=0.75, specular=0.15, rim_intensity=0.15)
create_or_update_mi("MI_Bld_HokageMansion", "/Game/Environment/Konoha/Buildings/Materials", (0.85, 0.30, 0.18, 1.0), roughness=0.80, specular=0.12, rim_intensity=0.20)

create_or_update_mi("MI_Road_Cobblestone",   "/Game/Environment/Konoha/Roads/Materials",     (0.58, 0.55, 0.50, 1.0), roughness=0.90, specular=0.08, rim_intensity=0.05)
create_or_update_mi("MI_HokageRock",        "/Game/Environment/Konoha/Landmarks/Materials", (0.68, 0.58, 0.45, 1.0), roughness=0.95, specular=0.05, rim_intensity=0.10)
create_or_update_mi("MI_Torii_Red",         "/Game/Environment/Konoha/Landmarks/Materials", (0.78, 0.12, 0.08, 1.0), roughness=0.70, specular=0.20, rim_intensity=0.25)

create_or_update_mi("MI_Veg_CedarNeedles_Stylized", "/Game/Environment/Konoha/Foliage/Materials", (0.18, 0.42, 0.18, 1.0), roughness=0.85, specular=0.10, rim_intensity=0.25, subsurface_warmth=0.4)
create_or_update_mi("MI_Veg_PinkPetals_Translucent", "/Game/Environment/Konoha/Foliage/Materials", (0.96, 0.65, 0.72, 1.0), roughness=0.75, specular=0.15, rim_intensity=0.35, subsurface_warmth=0.5)

create_or_update_mi("MI_Prop_Lantern_Stone","/Game/Environment/Konoha/Props/Materials",     (0.50, 0.48, 0.45, 1.0), roughness=0.90, specular=0.08)
create_or_update_mi("MI_Prop_Lantern_Glow", "/Game/Environment/Konoha/Props/Materials",     (1.00, 0.85, 0.40, 1.0), roughness=0.40, specular=0.30, emissive_color=(1.0, 0.85, 0.4, 1.0), emissive_intensity=2.0)
create_or_update_mi("MI_Sky_AnimeDaylight", "/Game/Environment/Konoha/Sky/Materials",       (0.32, 0.62, 0.96, 1.0), roughness=1.00, specular=0.00, emissive_color=(0.32, 0.62, 0.96, 1.0), emissive_intensity=0.6)

unreal.log("=== PHASE 2 MATERIAL SYSTEM COMPLETE ===")
