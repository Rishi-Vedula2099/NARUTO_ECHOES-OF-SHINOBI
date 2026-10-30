import unreal

mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
mel = unreal.MaterialEditingLibrary

# Inspect expressions in mat
print("Checking mat connections...")
# Let's recreate M_AnimeCel_Textured_Master with empty string "" pin
eal = unreal.EditorAssetLibrary
at = unreal.AssetToolsHelpers.get_asset_tools()

master_path = "/Game/Materials/Master/M_AnimeCel_Textured_Master"
if eal.does_asset_exist(master_path):
    eal.delete_asset(master_path)

factory = unreal.MaterialFactoryNew()
mat = at.create_asset("M_AnimeCel_Textured_Master", "/Game/Materials/Master", unreal.Material, factory)

default_tex = unreal.load_asset("/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")

# 1. Texture Parameter
tex_param = mel.create_material_expression(mat, unreal.MaterialExpressionTextureSampleParameter2D, -800, -200)
tex_param.set_editor_property("parameter_name", "DiffuseTexture")
if default_tex:
    tex_param.set_editor_property("texture", default_tex)

# 2. BaseColor Parameter
bc_param = mel.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -800, 50)
bc_param.set_editor_property("parameter_name", "BaseColor")
bc_param.set_editor_property("default_value", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))

# 3. Multiply: tex_param ("") * bc_param ("") -> MP_BASE_COLOR
mul_bc = mel.create_material_expression(mat, unreal.MaterialExpressionMultiply, -400, -50)
c1 = mel.connect_material_expressions(tex_param, "", mul_bc, "A")
c2 = mel.connect_material_expressions(bc_param, "", mul_bc, "B")
c3 = mel.connect_material_property(mul_bc, "", unreal.MaterialProperty.MP_BASE_COLOR)
print(f"BaseColor connections: c1={c1}, c2={c2}, c3={c3}")

# 4. Roughness
rough = mel.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 150)
rough.set_editor_property("parameter_name", "Roughness")
rough.set_editor_property("default_value", 0.82)
mel.connect_material_property(rough, "", unreal.MaterialProperty.MP_ROUGHNESS)

# 5. Specular
spec = mel.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -400, 250)
spec.set_editor_property("parameter_name", "Specular")
spec.set_editor_property("default_value", 0.08)
mel.connect_material_property(spec, "", unreal.MaterialProperty.MP_SPECULAR)

# 6. Emissive Ambient Fill (0.22)
amb = mel.create_material_expression(mat, unreal.MaterialExpressionConstant, -400, 400)
amb.set_editor_property("r", 0.22)

mul_em = mel.create_material_expression(mat, unreal.MaterialExpressionMultiply, -200, 350)
c4 = mel.connect_material_expressions(mul_bc, "", mul_em, "A")
c5 = mel.connect_material_expressions(amb, "", mul_em, "B")
c6 = mel.connect_material_property(mul_em, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
print(f"Emissive connections: c4={c4}, c5={c5}, c6={c6}")

res = mel.recompile_material(mat)
print(f"Recompile result: {res}")
eal.save_asset(mat.get_path_name())
