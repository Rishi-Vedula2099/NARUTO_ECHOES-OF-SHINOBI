"""
NARUTO: ECHOES OF SHINOBI
Finalize Masterpiece Golden Street Level & Pure Anime Vista
===========================================================
1. Imports T_Konoha_HokageRock_Backdrop.jpg into /Game/Environment/Konoha/Textures.
2. Configures M_Konoha_NorthStar_Backdrop to use pure Hokage Monument matte painting.
3. Corrects all building orientations (pitch=0, yaw=+/-90, roll=0) using keyword arguments.
4. Clears street axis completely so Hokage rock monument towers on the central vanishing point.
5. Reliably saves the map package /Game/Maps/GoldenTest/L_Konoha_GoldenStreet.
"""

import os
import unreal

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

unreal.log("=== STEP 1: IMPORT PURE HOKAGE ROCK BACKDROP TEXTURE ===")

src_backdrop = os.path.join(PROJECT_DIR, "Content", "SourceTextures", "T_Konoha_HokageRock_Backdrop.jpg")
if os.path.exists(src_backdrop):
    task = unreal.AssetImportTask()
    task.filename = src_backdrop
    task.destination_path = "/Game/Environment/Konoha/Textures"
    task.destination_name = "T_Konoha_HokageRock_Backdrop"
    task.replace_existing = True
    task.automated = True
    task.save = True
    AT.import_asset_tasks([task])
    unreal.log("  [OK] Imported T_Konoha_HokageRock_Backdrop texture")

tex_backdrop = unreal.load_asset("/Game/Environment/Konoha/Textures/T_Konoha_HokageRock_Backdrop")

unreal.log("=== STEP 2: UPDATE M_Konoha_NorthStar_Backdrop AS UNLIT EMISSIVE ===")

mat_path = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
mat = unreal.load_asset(mat_path)
if not mat:
    mat_factory = unreal.MaterialFactoryNew()
    mat = AT.create_asset("M_Konoha_NorthStar_Backdrop", "/Game/Environment/Konoha/Materials", unreal.Material, mat_factory)

if mat:
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MSM_UNLIT)
    mat.set_editor_property("two_sided", True)

    MEL.delete_all_material_expressions(mat)
    tex_node = MEL.create_material_expression(mat, unreal.MaterialExpressionTextureSample, -400, 0)
    if tex_backdrop:
        tex_node.texture = tex_backdrop

    MEL.connect_material_property(tex_node, "RGB", unreal.MaterialProperty.MP_EMISSIVE_COLOR)
    MEL.recompile_material(mat)
    EAL.save_loaded_asset(mat)
    unreal.log("  [OK] M_Konoha_NorthStar_Backdrop updated with pure Hokage backdrop!")

unreal.log("=== STEP 3: ASSEMBLE AND RECTIFY L_Konoha_GoldenStreet ===")

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
if not world:
    unreal.log_error("Failed to load L_Konoha_GoldenStreet!")
    quit()

# Load canonical meshes & materials
mesh_ramen    = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
mesh_res_a    = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A")
mesh_res_b    = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B")
mesh_rd_ctr   = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Center")
mesh_sw_l     = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Left")
mesh_sw_r     = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Right")
mi_road       = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_wood       = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_roof       = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_sign       = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Sign")
mi_noren      = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
mi_lantern    = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_PaperLantern")

for a in EAS.get_all_level_actors():
    if not a:
        continue
    label = a.get_actor_label()
    loc = a.get_actor_location()

    # 1. Road & Sidewalks
    if label.startswith("Road_Avenue_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_rd_ctr:
            smc.set_static_mesh(mesh_rd_ctr)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(0.0, loc.y, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)

    elif label.startswith("Sidewalk_Left_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sw_l:
            smc.set_static_mesh(mesh_sw_l)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(-410.0, loc.y, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)

    elif label.startswith("Sidewalk_Right_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sw_r:
            smc.set_static_mesh(mesh_sw_r)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(410.0, loc.y, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)

    # 2. Ichiraku Ramen (Left Sidewalk: open counter, stools, noren facing street +X)
    elif label == "Bld_IchirakuRamen":
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_ramen:
            smc.set_static_mesh(mesh_ramen)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
            smc.set_material(2, mi_sign)
            smc.set_material(3, mi_noren)
            smc.set_material(4, mi_lantern)
        a.set_actor_location(unreal.Vector(-560.0, 300.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_IchirakuRamen positioned facing street at (-560, 300, 0)")

    # 3. Konoha Residence A (Right Sidewalk: 2-story timber, balcony facing street -X)
    elif label == "Bld_KonohaResidence_A":
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_res_a:
            smc.set_static_mesh(mesh_res_a)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
        a.set_actor_location(unreal.Vector(560.0, 300.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_KonohaResidence_A positioned facing street at (560, 300, 0)")

    # 4. Residence B & Other Shops
    elif label == "Bld_KonohaResidence_B":
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_res_b:
            smc.set_static_mesh(mesh_res_b)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
        a.set_actor_location(unreal.Vector(580.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-580.0, 1100.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-580.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(580.0, 1800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    # 5. Clear Street Axis: Move Hokage Mansion off to side in tree line
    elif label == "Bld_HokageMansion_MidVista":
        a.set_actor_location(unreal.Vector(850.0, 3600.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
        unreal.log("-> Bld_HokageMansion_MidVista moved to (850, 3600, 0)")

    # 6. Vista Backdrop Plane (Pure Hokage Rock & Sky)
    elif "northstar_vista" in label.lower():
        a.set_actor_location(unreal.Vector(0.0, 4200.0, 850.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=0.0, roll=90.0), False)
        a.set_actor_scale3d(unreal.Vector(75.0, 45.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mat)
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")
        unreal.log("-> Backdrop_Konoha_NorthStar_Vista calibrated at (0, 4200, 850)")

    # 7. Overhanging Sakura Cherry Blossom Trees
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-460.0, 360.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=65.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.4, 1.4, 1.4))

    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(460.0, 440.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-45.0, roll=0.0), False)
        a.set_actor_scale3d(unreal.Vector(1.4, 1.4, 1.4))

    # 8. Interactive Living NPCs
    elif label == "NPC_Teuchi_IchirakuChef":
        a.set_actor_location(unreal.Vector(-560.0, 300.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

    elif label == "NPC_Chunin_GateGuard":
        a.set_actor_location(unreal.Vector(390.0, 120.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_01":
        a.set_actor_location(unreal.Vector(-390.0, 750.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=75.0, roll=0.0), False)

    elif label == "NPC_Leaf_Villager_02":
        a.set_actor_location(unreal.Vector(390.0, 520.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-80.0, roll=0.0), False)

    elif label == "NPC_Academy_Student":
        a.set_actor_location(unreal.Vector(390.0, 350.0, 15.0), False, False)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-60.0, roll=0.0), False)

    # 9. Clean occluding shop banners
    elif label == "Prop_ShopBanner_Ichiraku":
        a.set_actor_location(unreal.Vector(-650.0, 220.0, 0.0), False, False)

# EXPLICITLY SAVE MAP AND DIRTY PACKAGES
save_success = unreal.EditorLoadingAndSavingUtils.save_map(world, "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log(f"=== Level Saved Successfully: {save_success} ===")
