"""
NARUTO: ECHOES OF SHINOBI
Final Polish: Connect Backdrop Emissive & Calibrate Cobblestones
"""

import unreal

EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def finalize():
    # 1. Rebuild M_Konoha_NorthStar_Backdrop with clean connection
    mat_path = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
    if EAL.does_asset_exist(mat_path):
        EAL.delete_asset(mat_path)

    factory = unreal.MaterialFactoryNew()
    mat = AT.create_asset("M_Konoha_NorthStar_Backdrop", "/Game/Environment/Konoha/Materials", unreal.Material, factory)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property("two_sided", True)

    tex_path = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"
    tex = unreal.load_asset(tex_path)
    if not tex:
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

    tex_node = MEL.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -400, 0)
    if tex:
        tex_node.set_editor_property("texture", tex)

    # Empty string connects default Color/RGB pin
    conn = MEL.connect_material_property(tex_node, "", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    unreal.log(f"  [BACKDROP MAT] Connected texture to MP_EMISSIVE_COLOR: {conn}")

    MEL.recompile_material(mat)
    EAL.save_asset(mat.get_path_name())

    # 2. Calibrate MI_Road_Cobblestone with warm sandstone tone
    mi_road = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
    if mi_road:
        MEL.set_material_instance_vector_parameter_value(mi_road, "BaseColor", unreal.LinearColor(0.72, 0.68, 0.62, 1.0))
        MEL.set_material_instance_scalar_parameter_value(mi_road, "Roughness", 0.90)
        MEL.update_material_instance(mi_road)
        EAL.save_asset(mi_road.get_path_name())
        unreal.log("  [MI_ROAD] Calibrated warm cobblestone BaseColor")

    # 3. Apply to Level
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    for a in EAS.get_all_level_actors():
        if not a:
            continue
        label = a.get_actor_label().lower()
        if "backdrop" in label:
            a.set_actor_location(unreal.Vector(0.0, 3600.0, 800.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=90.0, yaw=0.0, roll=0.0), False)
            a.set_actor_scale3d(unreal.Vector(65.0, 32.0, 1.0))
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                smc.set_material(0, mat)
            unreal.log("  [LEVEL] Assigned M_Konoha_NorthStar_Backdrop to Backdrop actor")

        elif "road" in label or "sidewalk" in label or "ground" in label:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                for i in range(smc.get_num_materials()):
                    smc.set_material(i, mi_road)

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> PERFECTION FINALIZE COMPLETE & LEVEL SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    finalize()
