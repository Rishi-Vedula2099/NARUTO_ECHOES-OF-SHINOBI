"""
NARUTO: ECHOES OF SHINOBI
Authoritative Anime Cel Master Material & Instance Rebuilder
============================================================
Rebuilds /Game/Materials/Master/M_AnimeCel_Master:
- Clean BaseColor input (eliminating Fresnel over-emissive blowout)
- Controlled Emissive (EmissiveColor * EmissiveIntensity, default 0.0)
- Configures all Material Instances with authentic Anime colors:
  * Naruto: Skin (peachy), Navy (#101B2E), Hair (#F9D326), Collar (#EEEEEE), Tracksuit (#EE5D07)
  * Environment: Cobblestone (#756F68), Ichiraku Wood (#5A381E), Ichiraku Noren (#E8E4DA)
  * Hokage Mansion: Vermilion Red (#B8251B), Roof Tile (#2B2C30)
  * Foliage: Cedar (#1F4522), Sakura (#F29BB2)
  * Lanterns: Stone (#888682), Paper Glow Emissive (Amber 1.0, 0.75, 0.35, Intensity 3.0)
"""

import unreal

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

def rebuild_anime_cel_master():
    unreal.log("======================================================================")
    unreal.log("  REBUILDING /Game/Materials/Master/M_AnimeCel_Master")
    unreal.log("======================================================================")

    master_path = "/Game/Materials/Master/M_AnimeCel_Master"
    mat = unreal.load_asset(master_path)
    if not mat:
        factory = unreal.MaterialFactoryNew()
        mat = AT.create_asset("M_AnimeCel_Master", "/Game/Materials/Master", unreal.Material, factory)

    MEL.delete_all_material_expressions(mat)

    # 1. BaseColor (VectorParameter)
    bc = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -600, -200)
    bc.set_editor_property("parameter_name", "BaseColor")
    bc.set_editor_property("default_value", unreal.LinearColor(0.85, 0.50, 0.20, 1.0))

    # 2. SkinToneMultiplier (VectorParameter)
    stm = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -600, 0)
    stm.set_editor_property("parameter_name", "SkinToneMultiplier")
    stm.set_editor_property("default_value", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))

    # BaseColor * SkinToneMultiplier -> MP_BASE_COLOR
    mul_bc = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -300, -100)
    MEL.connect_material_expressions(bc, "", mul_bc, "A")
    MEL.connect_material_expressions(stm, "", mul_bc, "B")
    MEL.connect_material_property(mul_bc, "", unreal.MaterialProperty.MP_BASE_COLOR)

    # 3. Roughness
    rough = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -300, 100)
    rough.set_editor_property("parameter_name", "Roughness")
    rough.set_editor_property("default_value", 0.75)
    MEL.connect_material_property(rough, "", unreal.MaterialProperty.MP_ROUGHNESS)

    # 4. Specular
    spec = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -300, 250)
    spec.set_editor_property("parameter_name", "Specular")
    spec.set_editor_property("default_value", 0.10)
    MEL.connect_material_property(spec, "", unreal.MaterialProperty.MP_SPECULAR)

    # 5. Controlled Emissive: EmissiveColor * EmissiveIntensity -> MP_EMISSIVE_COLOR
    # NO raw Fresnel connected to prevent facet blowout
    em_color = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -600, 450)
    em_color.set_editor_property("parameter_name", "EmissiveColor")
    em_color.set_editor_property("default_value", unreal.LinearColor(0.0, 0.0, 0.0, 1.0))

    em_int = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -600, 650)
    em_int.set_editor_property("parameter_name", "EmissiveIntensity")
    em_int.set_editor_property("default_value", 0.0)

    mul_em = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -300, 500)
    MEL.connect_material_expressions(em_color, "", mul_em, "A")
    MEL.connect_material_expressions(em_int, "", mul_em, "B")
    MEL.connect_material_property(mul_em, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log("  [OK] M_AnimeCel_Master recompiled without Fresnel blowout!")
    return mat

def configure_mi(name, folder, base_color, roughness=0.75, specular=0.10, emissive_color=(0,0,0,1), emissive_intensity=0.0):
    pkg = f"{folder}/{name}"
    EAL.make_directory(folder)
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)

    parent = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
    if parent and mi:
        MEL.set_material_instance_parent(mi, parent)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color))
        MEL.set_material_instance_vector_parameter_value(mi, "SkinToneMultiplier", unreal.LinearColor(1, 1, 1, 1))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
        MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(*emissive_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", emissive_intensity)
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
        unreal.log(f"  [MI] Configured {name} -> BaseColor={base_color[:3]}")
    return mi

def setup_all_material_instances():
    unreal.log("======================================================================")
    unreal.log("  CONFIGURING ALL AUTHORITATIVE PRODUCTION MATERIAL INSTANCES")
    unreal.log("======================================================================")

    # 1. Naruto Genin Palette
    configure_mi("MI_Naruto_Skin",      "/Game/Characters/Naruto/Materials", (0.98, 0.82, 0.70, 1.0), roughness=0.85, specular=0.08)
    configure_mi("MI_Naruto_Navy",      "/Game/Characters/Naruto/Materials", (0.05, 0.08, 0.18, 1.0), roughness=0.80, specular=0.12)
    configure_mi("MI_Naruto_Hair",      "/Game/Characters/Naruto/Materials", (0.98, 0.82, 0.10, 1.0), roughness=0.60, specular=0.25)
    configure_mi("MI_Naruto_Collar",    "/Game/Characters/Naruto/Materials", (0.92, 0.92, 0.88, 1.0), roughness=0.85, specular=0.05)
    configure_mi("MI_Naruto_Tracksuit", "/Game/Characters/Naruto/Materials", (0.92, 0.35, 0.04, 1.0), roughness=0.82, specular=0.08)

    # 2. Environment - Roads & Sidewalks
    configure_mi("MI_Road_Cobblestone", "/Game/Environment/Konoha/Roads/Materials", (0.42, 0.39, 0.35, 1.0), roughness=0.88, specular=0.06)

    # 3. Environment - Buildings
    configure_mi("MI_Bld_Ichiraku_Wood", "/Game/Environment/Konoha/Buildings/Materials", (0.38, 0.22, 0.12, 1.0), roughness=0.82, specular=0.06)
    configure_mi("MI_Bld_Residence_A",   "/Game/Environment/Konoha/Buildings/Materials", (0.48, 0.32, 0.18, 1.0), roughness=0.80, specular=0.06)
    configure_mi("MI_Bld_Residence_B",   "/Game/Environment/Konoha/Buildings/Materials", (0.44, 0.28, 0.16, 1.0), roughness=0.80, specular=0.06)
    configure_mi("MI_Bld_Ichiraku_Noren","/Game/Environment/Konoha/Buildings/Materials", (0.88, 0.84, 0.74, 1.0), roughness=0.90, specular=0.04)
    configure_mi("MI_Bld_HokageMansion", "/Game/Environment/Konoha/Buildings/Materials", (0.75, 0.15, 0.10, 1.0), roughness=0.70, specular=0.15)
    configure_mi("MI_Bld_Roof_DarkTile", "/Game/Environment/Konoha/Buildings/Materials", (0.18, 0.19, 0.22, 1.0), roughness=0.75, specular=0.10)

    # 4. Environment - Foliage
    configure_mi("MI_Veg_CedarNeedles_Stylized", "/Game/Environment/Konoha/Foliage/Materials", (0.10, 0.26, 0.12, 1.0), roughness=0.80, specular=0.05)
    configure_mi("MI_Veg_PinkPetals_Translucent", "/Game/Environment/Konoha/Foliage/Materials", (0.95, 0.65, 0.75, 1.0), roughness=0.85, specular=0.08)

    # 5. Environment - Landmarks & Props
    configure_mi("MI_HokageRock",        "/Game/Environment/Konoha/Landmarks/Materials", (0.55, 0.49, 0.42, 1.0), roughness=0.90, specular=0.04)
    configure_mi("MI_Torii_Red",          "/Game/Environment/Konoha/Landmarks/Materials", (0.80, 0.12, 0.08, 1.0), roughness=0.70, specular=0.12)
    configure_mi("MI_Prop_Lantern_Stone", "/Game/Environment/Konoha/Props/Materials",     (0.52, 0.50, 0.46, 1.0), roughness=0.85, specular=0.05)
    configure_mi("MI_Prop_Lantern_Glow",  "/Game/Environment/Konoha/Props/Materials",     (1.0, 0.85, 0.45, 1.0), roughness=0.50, specular=0.20,
                 emissive_color=(1.0, 0.75, 0.35, 1.0), emissive_intensity=2.5)

    # 6. Generic Environment fallbacks
    configure_mi("MI_AnimeWood",    "/Game/Materials/Environment", (0.40, 0.25, 0.14, 1.0), roughness=0.80, specular=0.05)
    configure_mi("MI_AnimeGround",  "/Game/Materials/Environment", (0.45, 0.40, 0.34, 1.0), roughness=0.90, specular=0.04)
    configure_mi("MI_AnimeFoliage", "/Game/Materials/Environment", (0.12, 0.28, 0.14, 1.0), roughness=0.80, specular=0.05)
    configure_mi("MI_AnimeMetal",   "/Game/Materials/Characters",  (0.65, 0.68, 0.72, 1.0), roughness=0.40, specular=0.45)

if __name__ == "__main__":
    rebuild_anime_cel_master()
    setup_all_material_instances()
