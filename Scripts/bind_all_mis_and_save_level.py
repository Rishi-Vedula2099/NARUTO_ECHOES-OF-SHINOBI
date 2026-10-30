"""
NARUTO: ECHOES OF SHINOBI
Bind All Material Instances to Static Meshes & Actors in L_Konoha_GoldenStreet
"""

import unreal

EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def bind_and_save():
    master = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
    
    # 1. Update/Ensure all MIs
    def setup_mi(path, tex_path=None, tint=(1,1,1,1), rough=0.82):
        mi = unreal.load_asset(path)
        if not mi:
            folder, name = path.rsplit("/", 1)
            factory_mi = unreal.MaterialInstanceConstantFactoryNew()
            mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory_mi)
        MEL.set_material_instance_parent(mi, master)
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*tint))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", rough)
        if tex_path:
            tex = unreal.load_asset(tex_path)
            if tex:
                MEL.set_material_instance_texture_parameter_value(mi, "DiffuseTexture", tex)
        MEL.update_material_instance(mi)
        EAL.save_asset(mi.get_path_name())
        return mi

    mi_road    = setup_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
                          "/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")
    mi_wood    = setup_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",
                          "/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Facade_D")
    mi_roof    = setup_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",
                          "/Game/Environment/Konoha/Buildings/Textures/T_RoofTiles_D")
    mi_noren   = setup_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",
                          "/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Noren_D")
    mi_naruto  = setup_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
                          "/Game/Characters/Naruto/Textures/T_Naruto_Genin_D")
    mi_skin    = setup_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
                          tint=(0.98, 0.84, 0.72, 1.0))
    mi_navy    = setup_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
                          tint=(0.06, 0.10, 0.20, 1.0))
    mi_hair    = setup_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
                          tint=(0.98, 0.82, 0.08, 1.0))
    mi_collar  = setup_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
                          tint=(0.94, 0.94, 0.92, 1.0))

    # 2. Assign to Static Meshes
    def assign_slots(mesh_path, slot_list):
        m = unreal.load_asset(mesh_path)
        if m:
            for idx, mat in enumerate(slot_list):
                m.set_material(idx, mat)
            EAL.save_asset(m.get_path_name())
            unreal.log(f"  [MESH SLOTS] Saved {mesh_path}")

    assign_slots("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01", [mi_road, mi_road, mi_road])
    assign_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", [mi_wood, mi_roof, mi_wood, mi_wood, mi_noren, mi_noren])
    assign_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", [mi_wood, mi_roof, mi_roof])
    assign_slots("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", [mi_wood, mi_roof, mi_wood, mi_wood, mi_wood, mi_roof])
    assign_slots("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", [
        mi_naruto, mi_collar, mi_skin, mi_navy, mi_hair, mi_hair, mi_hair, mi_hair, mi_hair, mi_hair,
        mi_naruto, mi_naruto, mi_naruto, mi_navy, mi_naruto, mi_navy, mi_navy, mi_skin
    ])

    # 3. Load map and assign materials to actor components
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    for a in EAS.get_all_level_actors():
        if not a:
            continue
        label = a.get_actor_label().lower()
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if not smc:
            continue

        if "road" in label or "sidewalk" in label or "ground" in label:
            for i in range(smc.get_num_materials()):
                smc.set_material(i, mi_road)
        elif "ichirakuramen" in label:
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
            if smc.get_num_materials() > 2:
                smc.set_material(2, mi_wood)
                smc.set_material(3, mi_wood)
            if smc.get_num_materials() > 4:
                smc.set_material(4, mi_noren)
        elif "residence_a" in label or "residence_b" in label or "weaponshop" in label or "missionoffice" in label:
            smc.set_material(0, mi_wood)
            if smc.get_num_materials() > 1:
                smc.set_material(1, mi_roof)

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> ALL MATERIALS FULLY BOUND & LEVEL SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    bind_and_save()
