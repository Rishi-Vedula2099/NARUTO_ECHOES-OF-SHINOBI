import unreal

AT = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

mat_pkg = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
tex_pkg = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"

tex = unreal.load_asset(tex_pkg)
unreal.log(f"TEX LOADED: {tex}")

mat_factory = unreal.MaterialFactoryNew()
mat = AT.create_asset("M_Konoha_NorthStar_Backdrop", "/Game/Environment/Konoha/Materials", unreal.Material, mat_factory)
if mat:
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property("two_sided", True)
    tex_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -300, 0)
    if tex:
        tex_node.texture = tex
    unreal.MaterialEditingLibrary.connect_material_expressions(tex_node, "RGB", None, "EmissiveColor")
    unreal.MaterialEditingLibrary.recompile_material(mat)
    EAL.save_loaded_asset(mat)
    unreal.log("SUCCESS: M_Konoha_NorthStar_Backdrop compiled and saved!")
else:
    unreal.log_error("FAILED to create mat")
