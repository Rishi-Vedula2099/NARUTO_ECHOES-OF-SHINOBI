"""
NARUTO: ECHOES OF SHINOBI
Master Unreal Engine Pipeline: Asset Import, Material Setup & Golden Street Reconstruction
"""

import unreal
import os

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

SRC_TEX = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceTextures"
SRC_MSH = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes"
MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def import_texture(filename, dest_path):
    src_file = os.path.join(SRC_TEX, filename)
    name = os.path.splitext(filename)[0]
    task = unreal.AssetImportTask()
    task.filename = src_file
    task.destination_path = dest_path
    task.destination_name = name
    task.replace_existing = True
    task.automated = True
    task.save = True
    AT.import_asset_tasks([task])
    tex = unreal.load_asset(f"{dest_path}/{name}")
    unreal.log(f"  [TEXTURE] Imported: {dest_path}/{name} -> {tex is not None}")
    return tex

def import_mesh(filename, dest_path):
    src_file = os.path.join(SRC_MSH, filename)
    name = os.path.splitext(filename)[0]
    task = unreal.AssetImportTask()
    task.filename = src_file
    task.destination_path = dest_path
    task.destination_name = name
    task.replace_existing = True
    task.automated = True
    task.save = True

    opt = unreal.FbxImportUI()
    opt.set_editor_property("import_mesh", True)
    opt.set_editor_property("import_textures", False)
    opt.set_editor_property("import_materials", False)
    task.options = opt

    AT.import_asset_tasks([task])
    mesh = unreal.load_asset(f"{dest_path}/{name}")
    unreal.log(f"  [STATIC MESH] Imported: {dest_path}/{name} -> {mesh is not None}")
    return mesh

def rebuild_master_material_with_textures():
    unreal.log("--- CREATING M_AnimeCel_Textured_Master WITH TEXTURE SUPPORT ---")
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

    # 5. Controlled Emissive
    em_color = MEL.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, -800, 400)
    em_color.set_editor_property("parameter_name", "EmissiveColor")
    em_color.set_editor_property("default_value", unreal.LinearColor(0.0, 0.0, 0.0, 1.0))

    em_int = MEL.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, -800, 600)
    em_int.set_editor_property("parameter_name", "EmissiveIntensity")
    em_int.set_editor_property("default_value", 0.0)

    mul_em = MEL.create_material_expression(mat, unreal.MaterialExpressionMultiply, -400, 450)
    MEL.connect_material_expressions(em_color, "", mul_em, "A")
    MEL.connect_material_expressions(em_int, "", mul_em, "B")
    MEL.connect_material_property(mul_em, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log("  [OK] M_AnimeCel_Textured_Master compiled with Texture & Tint support!")
    return mat

def setup_unlit_backdrop_material():
    unreal.log("--- CONFIGURING M_Konoha_NorthStar_Backdrop ---")
    tex_path = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"
    tex = unreal.load_asset(tex_path)

    mat_path = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
    if EAL.does_asset_exist(mat_path):
        EAL.delete_asset(mat_path)

    mat_factory = unreal.MaterialFactoryNew()
    mat = AT.create_asset("M_Konoha_NorthStar_Backdrop", "/Game/Environment/Konoha/Materials", unreal.Material, mat_factory)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property("two_sided", True)

    tex_node = MEL.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -400, 0)
    if tex:
        tex_node.set_editor_property("texture", tex)

    MEL.connect_material_property(tex_node, "RGB", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())
    unreal.log("  [OK] M_Konoha_NorthStar_Backdrop configured as UNLIT connected to MP_EMISSIVE_COLOR")
    return mat

def create_or_update_mi(name, folder, texture_path=None, tint_color=(1,1,1,1), roughness=0.82, specular=0.08, emissive_color=(0,0,0,1), emissive_int=0.0):
    pkg = f"{folder}/{name}"
    EAL.make_directory(folder)
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)

    parent = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
    if parent and mi:
        MEL.set_material_instance_parent(mi, parent)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*tint_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
        MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(*emissive_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", emissive_int)

        if texture_path:
            tex = unreal.load_asset(texture_path)
            if tex:
                MEL.set_material_instance_texture_parameter_value(mi, "DiffuseTexture", tex)
                unreal.log(f"  [MI] Bound texture {texture_path} -> {name}")

        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
    return mi

def assign_mesh_materials(mesh_path, mat_slots):
    mesh = unreal.load_asset(mesh_path)
    if not mesh:
        unreal.log_warning(f"Could not load mesh: {mesh_path}")
        return

    for slot_idx, mat_path in enumerate(mat_slots):
        mat = unreal.load_asset(mat_path)
        if mat:
            mesh.set_material(slot_idx, mat)
            unreal.log(f"  [SLOT] {mesh.get_name()}[{slot_idx}] -> {mat.get_name()}")
    EAL.save_asset(mesh.get_path_name())

def execute_pipeline():
    unreal.log("======================================================================")
    unreal.log("  EXECUTING STEP 1: ASSET IMPORT & MATERIAL SETUP")
    unreal.log("======================================================================")

    # 1. Import Textures
    import_texture("T_Cobblestones_D.tga",   "/Game/Environment/Konoha/Roads/Textures")
    import_texture("T_RoofTiles_D.tga",      "/Game/Environment/Konoha/Buildings/Textures")
    import_texture("T_Ichiraku_Facade_D.tga","/Game/Environment/Konoha/Buildings/Textures")
    import_texture("T_Ichiraku_Noren_D.tga", "/Game/Environment/Konoha/Buildings/Textures")
    import_texture("T_Naruto_Genin_D.tga",   "/Game/Characters/Naruto/Textures")

    # 2. Import Meshes
    import_mesh("SM_Bld_IchirakuRamen.obj",    "/Game/Environment/Konoha/Buildings/Meshes")
    import_mesh("SM_Bld_Konoha_Residence_A.obj","/Game/Environment/Konoha/Buildings/Meshes")
    import_mesh("SM_Bld_Konoha_Residence_B.obj","/Game/Environment/Konoha/Buildings/Meshes")
    import_mesh("SM_Road_Paved_01.obj",         "/Game/Environment/Konoha/Roads/Meshes")
    import_mesh("SK_Naruto_Genin.obj",          "/Game/Characters/Naruto/Meshes")

    # 3. Rebuild Master Materials
    rebuild_master_material_with_textures()
    setup_unlit_backdrop_material()

    # 4. Create Material Instances with Textures
    create_or_update_mi("MI_Road_Cobblestone",  "/Game/Environment/Konoha/Roads/Materials",
                        texture_path="/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D",
                        tint_color=(1.0, 1.0, 1.0, 1.0), roughness=0.88)

    create_or_update_mi("MI_Bld_Ichiraku_Wood", "/Game/Environment/Konoha/Buildings/Materials",
                        texture_path="/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Facade_D",
                        tint_color=(1.0, 1.0, 1.0, 1.0), roughness=0.82)

    create_or_update_mi("MI_Bld_Ichiraku_Noren","/Game/Environment/Konoha/Buildings/Materials",
                        texture_path="/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Noren_D",
                        tint_color=(1.0, 1.0, 1.0, 1.0), roughness=0.90)

    create_or_update_mi("MI_Bld_Ichiraku_Roof", "/Game/Environment/Konoha/Buildings/Materials",
                        texture_path="/Game/Environment/Konoha/Buildings/Textures/T_RoofTiles_D",
                        tint_color=(1.0, 1.0, 1.0, 1.0), roughness=0.75)

    create_or_update_mi("MI_Naruto_Tracksuit",  "/Game/Characters/Naruto/Materials",
                        texture_path="/Game/Characters/Naruto/Textures/T_Naruto_Genin_D",
                        tint_color=(1.0, 1.0, 1.0, 1.0), roughness=0.82)

    create_or_update_mi("MI_Naruto_Skin",       "/Game/Characters/Naruto/Materials",
                        tint_color=(0.98, 0.84, 0.72, 1.0), roughness=0.85)

    create_or_update_mi("MI_Naruto_Navy",       "/Game/Characters/Naruto/Materials",
                        tint_color=(0.06, 0.10, 0.20, 1.0), roughness=0.78)

    create_or_update_mi("MI_Naruto_Hair",       "/Game/Characters/Naruto/Materials",
                        tint_color=(0.98, 0.82, 0.08, 1.0), roughness=0.60, specular=0.25)

    create_or_update_mi("MI_Naruto_Collar",     "/Game/Characters/Naruto/Materials",
                        tint_color=(0.94, 0.94, 0.92, 1.0), roughness=0.85)

    # 5. Bind Material Slots on Imported Meshes
    # SM_Bld_IchirakuRamen: 0: Wood, 1: Roof, 2: Sign, 3: Noren
    assign_mesh_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", [
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren"
    ])

    # SM_Bld_Konoha_Residence_A: 0: Timber, 1: Roof
    assign_mesh_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", [
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof"
    ])

    # SM_Bld_Konoha_Residence_B: 0: Timber, 1: Roof
    assign_mesh_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", [
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
        "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof"
    ])

    # SM_Road_Paved_01: 0: Cobblestone, 1: Curb L, 2: Curb R
    assign_mesh_materials("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01", [
        "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
        "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
        "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone"
    ])

    # SK_Naruto_Genin:
    assign_mesh_materials("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", [
        "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Skin"
    ])

    # 6. Re-align Level Geometry & Lighting in L_Konoha_GoldenStreet
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)

    for a in EAS.get_all_level_actors():
        if not a:
            continue

        label_lower = a.get_actor_label().lower()

        # Directional Sun: Overhead-left (pitch=-52.0, yaw=55.0)
        if isinstance(a, unreal.DirectionalLight):
            a.set_actor_location(unreal.Vector(-800.0, -400.0, 2500.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-52.0, yaw=55.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 6.5)
                dlc.set_editor_property("light_color", unreal.Color(255, 245, 230, 255))
                dlc.set_editor_property("cast_shadows", True)
                unreal.log("  [SUN] Re-aligned Directional Sun: pitch=-52, yaw=55, int=6.5 lux")

        # SkyLight: Soft Azure Fill
        elif isinstance(a, unreal.SkyLight):
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 8.5)
                slc.set_editor_property("light_color", unreal.Color(195, 228, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", False)
                unreal.log("  [SKYLIGHT] Set soft azure fill: int=8.5 lux, real_time=False")

        # PostProcess: Exposure & Saturation
        elif isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.8)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.5)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.25, 1.25, 1.25, 1.0))
            a.settings = s

        # Backdrop Vista: Frame horizon and face camera along -Y
        elif "backdrop" in label_lower:
            a.set_actor_location(unreal.Vector(0.0, 3500.0, 750.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=-90.0), False)
            a.set_actor_scale3d(unreal.Vector(65.0, 32.0, 1.0))
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                bd_mat = unreal.load_asset("/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop")
                if bd_mat:
                    smc.set_material(0, bd_mat)
            unreal.log("  [VISTA] Aligned Backdrop_Konoha_NorthStar_Vista facing -Y at Y=3500")

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> PIPELINE EXECUTION COMPLETE & MAP SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    execute_pipeline()
