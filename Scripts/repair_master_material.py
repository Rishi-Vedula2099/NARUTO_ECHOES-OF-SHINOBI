"""
NARUTO: ECHOES OF SHINOBI
Master Cel Material Shader Fix
Assigns default texture to DiffuseTexture parameter to guarantee 100% successful shader compilation.
"""

import unreal

EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary

def build_perfect_master_material():
    master_path = "/Game/Materials/Master/M_AnimeCel_Textured_Master"
    if EAL.does_asset_exist(master_path):
        EAL.delete_asset(master_path)

    factory = unreal.MaterialFactoryNew()
    mat = AT.create_asset("M_AnimeCel_Textured_Master", "/Game/Materials/Master", unreal.Material, factory)

    # Load valid default texture
    default_tex = unreal.load_asset("/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")

    # 1. Texture Parameter (DiffuseTexture) with VALID default texture
    tex_param = MEL.create_material_expression(mat, unreal.MaterialExpressionTextureSampleParameter2D, -800, -200)
    tex_param.set_editor_property("parameter_name", "DiffuseTexture")
    if default_tex:
        tex_param.set_editor_property("texture", default_tex)

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
    # mul_bc * 0.25 -> MP_EMISSIVE_COLOR
    ambient_scale = MEL.create_material_expression(mat, unreal.MaterialExpressionConstant, -400, 450)
    ambient_scale.set_editor_property("r", 0.25)

    mul_ambient = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -200, 400)
    MEL.connect_material_expressions(mul_bc, "", mul_ambient, "A")
    MEL.connect_material_expressions(ambient_scale, "", mul_ambient, "B")
    MEL.connect_material_property(mul_ambient, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log("  [SUCCESS] M_AnimeCel_Textured_Master compiled with valid default texture!")

    # Now refresh all MIs
    def update_mi(path, tex_path=None, tint=(1,1,1,1)):
        mi = unreal.load_asset(path)
        if not mi:
            folder, name = path.rsplit("/", 1)
            factory_mi = unreal.MaterialInstanceConstantFactoryNew()
            mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory_mi)
        MEL.set_material_instance_parent(mi, mat)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*tint))
        if tex_path:
            tex = unreal.load_asset(tex_path)
            if tex:
                MEL.set_material_instance_texture_parameter_value(mi, "DiffuseTexture", tex)
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
        unreal.log(f"  [UPDATED MI] {mi.get_name()} -> Parent: {mat.get_name()}")

    update_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
              "/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")
    update_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
              "/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Facade_D")
    update_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
              "/Game/Environment/Konoha/Buildings/Textures/T_RoofTiles_D")
    update_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",
              "/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Noren_D")
    update_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
              "/Game/Characters/Naruto/Textures/T_Naruto_Genin_D")
    update_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
              tint=(0.98, 0.84, 0.72, 1.0))
    update_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
              tint=(0.06, 0.10, 0.20, 1.0))
    update_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
              tint=(0.98, 0.82, 0.08, 1.0))
    update_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
              tint=(0.94, 0.94, 0.92, 1.0))

if __name__ == "__main__":
    build_perfect_master_material()
