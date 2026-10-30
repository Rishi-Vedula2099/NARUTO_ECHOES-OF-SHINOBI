"""
NARUTO: ECHOES OF SHINOBI
Master Visual Rectification: Lighting, Anime Cel Material & Backdrop Vista
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

def fix_master_material_cel_shadows():
    unreal.log("--- UPDATING M_AnimeCel_Textured_Master WITH ANIME SHADOW AMBIENT ---")
    master_path = "/Game/Materials/Master/M_AnimeCel_Textured_Master"
    if EAL.does_asset_exist(master_path):
        EAL.delete_asset(master_path)

    factory = unreal.MaterialFactoryNew()
    mat = AT.create_asset("M_AnimeCel_Textured_Master", "/Game/Materials/Master", unreal.Material, factory)

    # 1. Texture Parameter (DiffuseTexture)
    tex_param = MEL.create_material_expression(mat, unreal.MaterialExpressionTextureSampleParameter2D, -800, -200)
    tex_param.set_editor_property("parameter_name", "DiffuseTexture")

    # 2. BaseColor Tint (VectorParameter)
    bc_param = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -800, 50)
    bc_param.set_editor_property("parameter_name", "BaseColor")
    bc_param.set_editor_property("default_value", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))

    # Multiply Texture * BaseColor -> MP_BASE_COLOR
    mul_bc = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -400, -50)
    MEL.connect_material_expressions(tex_param, "RGB", mul_bc, "A")
    MEL.connect_material_expressions(bc_param, "", mul_bc, "B")
    MEL.connect_material_property(mul_bc, "", unreal.MaterialProperty.MP_BASE_COLOR)

    # 3. Roughness
    rough = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 150)
    rough.set_editor_property("parameter_name", "Roughness")
    rough.set_editor_property("default_value", 0.82)
    MEL.connect_material_property(rough, "", unreal.MaterialProperty.MP_ROUGHNESS)

    # 4. Specular
    spec = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 250)
    spec.set_editor_property("parameter_name", "Specular")
    spec.set_editor_property("default_value", 0.08)
    MEL.connect_material_property(spec, "", unreal.MaterialProperty.MP_SPECULAR)

    # 5. Stylized Anime Shadow Ambient (Emissive Fill)
    # mul_bc * 0.22 -> MP_EMISSIVE_COLOR
    # This guarantees anime cel readability in occluded zones (no pitch black crushed shadows!)
    ambient_scale = MEL.create_material_expression(mat, unreal.MaterialExpressionConstant, -400, 450)
    ambient_scale.set_editor_property("r", 0.22)

    mul_ambient = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -200, 400)
    MEL.connect_material_expressions(mul_bc, "", mul_ambient, "A")
    MEL.connect_material_expressions(ambient_scale, "", mul_ambient, "B")
    MEL.connect_material_property(mul_ambient, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log("  [OK] M_AnimeCel_Textured_Master updated with 0.22 ambient floor!")
    return mat

def apply_fixes():
    unreal.log("======================================================================")
    unreal.log("  APPLYING MASTER LIGHTING, BACKDROP & LEVEL HARMONIZATION")
    unreal.log("======================================================================")

    # 1. Update Master Cel Material
    fix_master_material_cel_shadows()

    # 2. Load Level
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)

    for a in EAS.get_all_level_actors():
        if not a:
            continue
        label = a.get_actor_label()
        label_lower = label.lower()

        # Directional Sun: Overhead shining down the street (+Y)
        if isinstance(a, unreal.DirectionalLight):
            a.set_actor_location(unreal.Vector(0.0, -500.0, 3500.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-65.0, yaw=90.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 8.5)
                dlc.set_editor_property("light_color", unreal.Color(255, 248, 235, 255))
                dlc.set_editor_property("cast_shadows", True)
                unreal.log("  [SUN] Re-aligned Directional Sun: pitch=-65, yaw=90, int=8.5 lux")

        # SkyLight: RealTime capture + Azure fill
        elif isinstance(a, unreal.SkyLight):
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 9.0)
                slc.set_editor_property("light_color", unreal.Color(195, 228, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", True)
                slc.set_editor_property("lower_hemisphere_is_black", False)
                slc.set_editor_property("lower_hemisphere_color", unreal.LinearColor(0.25, 0.35, 0.50, 1.0))
                unreal.log("  [SKYLIGHT] Set SkyLight: int=9.0 lux, real_time=True, azure lower hemisphere")

        # PostProcessVolume
        elif isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.6)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.4)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.20, 1.20, 1.20, 1.0))
            a.settings = s

        # Backdrop Vista: Must face -Y toward camera at Y=0!
        elif "backdrop" in label_lower:
            a.set_actor_location(unreal.Vector(0.0, 3600.0, 800.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=90.0, yaw=0.0, roll=0.0), False)
            a.set_actor_scale3d(unreal.Vector(65.0, 32.0, 1.0))
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                bd_mat = unreal.load_asset("/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop")
                if bd_mat:
                    smc.set_material(0, bd_mat)
            unreal.log(f"  [BACKDROP] Re-aligned '{label}' to Y=3600, Rot=(90,0,0), Scale=(65,32,1)")

        # Clear any rogue shop banner that was at (0, 0, ...)
        elif "shopbanner" in label_lower:
            a.set_actor_location(unreal.Vector(-380.0, 220.0, 0.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
            unreal.log(f"  [BANNER] Aligned '{label}' to sidewalk X=-380")

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> LEVEL HARMONIZATION SAVED SUCCESSFULLY <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    apply_fixes()
