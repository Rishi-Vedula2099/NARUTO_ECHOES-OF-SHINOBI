import unreal

AT = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

mat_path = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
tex_path = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"

tex = unreal.load_asset(tex_path)
if not tex:
    # Try importing from source jpg if not loaded
    src_jpg = "c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Content/Art/References/Phase13/Konoha_Visual_North_Star.jpg"
    task = unreal.AssetImportTask()
    task.filename = src_jpg
    task.destination_path = "/Game/Art/References/Phase13"
    task.destination_name = "Konoha_Visual_North_Star"
    task.replace_existing = True
    task.automated = True
    task.save = True
    AT.import_asset_tasks([task])
    tex = unreal.load_asset(tex_path)

unreal.log(f"Backdrop Texture: {tex}")

mat_factory = unreal.MaterialFactoryNew()
mat = AT.create_asset("M_Konoha_NorthStar_Backdrop", "/Game/Environment/Konoha/Materials", unreal.Material, mat_factory)
if mat:
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property("two_sided", True)

    # Add TextureSample node
    tex_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -300, 0)
    if tex:
        tex_node.texture = tex
    
    # Connect to EmissiveColor (Unlit base color)
    unreal.MaterialEditingLibrary.connect_material_expressions(tex_node, "RGB", None, "EmissiveColor")
    
    unreal.MaterialEditingLibrary.update_material(mat)
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    unreal.log("SUCCESS: Created and saved M_Konoha_NorthStar_Backdrop")
else:
    unreal.log_error("Failed to create M_Konoha_NorthStar_Backdrop")
