"""
NARUTO: ECHOES OF SHINOBI
Import & Assemble Perfect Golden Street Proving Ground
======================================================
1. Imports all generated 24-bit TGA textures.
2. Imports all authored 3D production meshes:
   - SM_Road_Center, SM_Sidewalk_Left, SM_Sidewalk_Right
   - SM_Bld_IchirakuRamen (open counter, stools, noren, sign, gabled roof)
   - SM_Bld_Konoha_Residence_A & B
   - SK_Naruto_Genin (athletic shinobi stance, spiky hair, headband, collar, tracksuit)
3. Configures and binds all Anime Cel Material Instances (zero tiling artifacts).
4. Assembles L_Konoha_GoldenStreet layout with open vanishing point, centered Hokage Rock vista,
   open Ichiraku ramen stall on left sidewalk, residence on right sidewalk, overhanging sakura blossoms,
   and interactive NPCs.
5. Saves all packages.
"""

import os
import unreal

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SRC_MESH_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
SRC_TEX_DIR  = os.path.join(PROJECT_DIR, "Content", "SourceTextures")

AT  = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

unreal.log("=== Step 1: Importing Production TGA Textures ===")

textures_to_import = [
    ("T_Solid_White.tga",          "/Game/Materials/Textures"),
    ("T_Cobblestones_D.tga",       "/Game/Environment/Konoha/Roads/Textures"),
    ("T_Ichiraku_Facade_D.tga",    "/Game/Environment/Konoha/Buildings/Textures"),
    ("T_RoofTiles_D.tga",          "/Game/Environment/Konoha/Buildings/Textures"),
    ("T_Ichiraku_Noren_D.tga",     "/Game/Environment/Konoha/Buildings/Textures"),
    ("T_Lantern_Paper_D.tga",      "/Game/Environment/Konoha/Props/Textures"),
]

for filename, dest_path in textures_to_import:
    src_file = os.path.join(SRC_TEX_DIR, filename)
    if not os.path.exists(src_file):
        unreal.log_error(f"Missing texture file: {src_file}")
        continue
    asset_name = os.path.splitext(filename)[0]
    task = unreal.AssetImportTask()
    task.filename = src_file
    task.destination_path = dest_path
    task.destination_name = asset_name
    task.replace_existing = True
    task.automated = True
    task.save = True
    AT.import_asset_tasks([task])
    unreal.log(f"  Imported texture: {dest_path}/{asset_name}")

unreal.log("=== Step 2: Importing Production 3D Meshes ===")

meshes_to_import = [
    ("SM_Road_Center.obj",            "/Game/Environment/Konoha/Roads/Meshes",      "SM_Road_Center",            False),
    ("SM_Sidewalk_Left.obj",          "/Game/Environment/Konoha/Roads/Meshes",      "SM_Sidewalk_Left",          False),
    ("SM_Sidewalk_Right.obj",         "/Game/Environment/Konoha/Roads/Meshes",      "SM_Sidewalk_Right",         False),
    ("SM_Bld_IchirakuRamen.obj",      "/Game/Environment/Konoha/Buildings/Meshes",  "SM_Bld_IchirakuRamen",      False),
    ("SM_Bld_Konoha_Residence_A.obj", "/Game/Environment/Konoha/Buildings/Meshes",  "SM_Bld_Konoha_Residence_A", False),
    ("SM_Bld_Konoha_Residence_B.obj", "/Game/Environment/Konoha/Buildings/Meshes",  "SM_Bld_Konoha_Residence_B", False),
    ("SK_Naruto_Genin.obj",           "/Game/Characters/Naruto/Meshes",             "SK_Naruto_Genin",           False),
]

for filename, dest_path, asset_name, is_skel in meshes_to_import:
    src_file = os.path.join(SRC_MESH_DIR, filename)
    if not os.path.exists(src_file):
        unreal.log_error(f"Missing mesh file: {src_file}")
        continue
    task = unreal.AssetImportTask()
    task.filename = src_file
    task.destination_path = dest_path
    task.destination_name = asset_name
    task.replace_existing = True
    task.automated = True
    task.save = True

    opts = unreal.FbxImportUI()
    opts.import_mesh = True
    opts.import_as_skeletal = is_skel
    opts.import_materials = False
    opts.import_textures = False
    opts.static_mesh_import_data.combine_meshes = True
    task.options = opts

    AT.import_asset_tasks([task])
    unreal.log(f"  Imported mesh: {dest_path}/{asset_name}")

unreal.log("=== Step 3: Configuring Anime Cel Material Instances ===")

master_mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")
tex_white  = unreal.load_asset("/Game/Materials/Textures/T_Solid_White")
tex_road   = unreal.load_asset("/Game/Environment/Konoha/Roads/Textures/T_Cobblestones_D")
tex_wood   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Facade_D")
tex_roof   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_RoofTiles_D")
tex_noren  = unreal.load_asset("/Game/Environment/Konoha/Buildings/Textures/T_Ichiraku_Noren_D")
tex_lantern = unreal.load_asset("/Game/Environment/Konoha/Props/Textures/T_Lantern_Paper_D")

def configure_mi(pkg_path, base_color_rgba, diffuse_tex, roughness=0.82, specular=0.08):
    mi = unreal.load_asset(pkg_path)
    if not mi:
        folder, name = pkg_path.rsplit("/", 1)
        EAL.make_directory(folder)
        factory_mi = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory_mi)
    if master_mat:
        MEL.set_material_instance_parent(mi, master_mat)
    MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color_rgba))
    if diffuse_tex:
        MEL.set_material_instance_texture_parameter_value(mi, "DiffuseTexture", diffuse_tex)
    MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
    MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
    MEL.update_material_instance(mi)
    EAL.save_asset(mi.get_path_name())
    unreal.log(f"  Configured MI: {pkg_path}")
    return mi

# Character Materials (Zero micro-tiling: uses T_Solid_White)
mi_skin      = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), tex_white, roughness=0.75, specular=0.10)
mi_navy      = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",      (0.08, 0.12, 0.22, 1.0), tex_white, roughness=0.80, specular=0.15)
mi_hair      = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",      (0.98, 0.84, 0.15, 1.0), tex_white, roughness=0.55, specular=0.35)
mi_collar    = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",    (0.95, 0.95, 0.95, 1.0), tex_white, roughness=0.85, specular=0.05)
mi_tracksuit = configure_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit", (0.96, 0.42, 0.05, 1.0), tex_white, roughness=0.82, specular=0.08)

# Environment Materials
mi_road   = configure_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",       (1.0, 1.0, 1.0, 1.0), tex_road, roughness=0.90, specular=0.05)
mi_wood   = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood",   (1.0, 1.0, 1.0, 1.0), tex_wood, roughness=0.85, specular=0.08)
mi_roof   = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof",   (1.0, 1.0, 1.0, 1.0), tex_roof, roughness=0.78, specular=0.12)
mi_noren  = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",  (1.0, 1.0, 1.0, 1.0), tex_noren, roughness=0.92, specular=0.02)
mi_sign   = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Sign",   (1.0, 1.0, 1.0, 1.0), tex_noren, roughness=0.80, specular=0.06)
mi_res_a  = configure_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A",    (1.0, 1.0, 1.0, 1.0), tex_wood, roughness=0.85, specular=0.08)
mi_lantern = configure_mi("/Game/Environment/Konoha/Props/Materials/MI_Prop_PaperLantern",      (1.0, 0.85, 0.55, 1.0), tex_lantern, roughness=0.60, specular=0.20)

unreal.log("=== Step 4: Binding Materials to Mesh Sections ===")

def bind_materials(mesh_path, mat_list):
    mesh = unreal.load_asset(mesh_path)
    if mesh and isinstance(mesh, unreal.StaticMesh):
        num_sections = mesh.get_num_sections(0)
        for i in range(num_sections):
            m = mat_list[i % len(mat_list)]
            if m:
                mesh.set_material(i, m)
        EAL.save_asset(mesh.get_path_name())
        unreal.log(f"  Bound {num_sections} materials to {mesh_path}")

# Naruto character mesh
bind_materials("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit])

# Duplicate to Skin path
skin_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
if EAL.does_asset_exist(skin_path):
    EAL.delete_asset(skin_path)
EAL.duplicate_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", skin_path)
bind_materials(skin_path, [mi_skin, mi_navy, mi_hair, mi_collar, mi_tracksuit])

# Ichiraku Ramen
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", [mi_wood, mi_roof, mi_sign, mi_noren, mi_lantern])

# Residences
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", [mi_wood, mi_roof])
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", [mi_wood, mi_roof])
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",    [mi_wood, mi_roof])
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",  [mi_wood, mi_roof])
bind_materials("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice", [mi_wood, mi_roof])

# Road & Sidewalks
bind_materials("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Center", [mi_road])
bind_materials("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Left", [mi_road])
bind_materials("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Right", [mi_road])

unreal.log("=== Step 5: Assembling Level /Game/Maps/GoldenTest/L_Konoha_GoldenStreet ===")

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
if not world:
    unreal.log_error("Could not load golden street map!")
    quit()

# 1. Update Road & Sidewalk Actors
mesh_road_center   = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Center")
mesh_sidewalk_left = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Left")
mesh_sidewalk_right = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Sidewalk_Right")

for a in EAS.get_all_level_actors():
    if not a:
        continue
    label = a.get_actor_label()
    loc = a.get_actor_location()

    if label.startswith("Road_Avenue_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_road_center:
            smc.set_static_mesh(mesh_road_center)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(0.0, loc.y, 0.0), False, False)

    elif label.startswith("Sidewalk_Left_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sidewalk_left:
            smc.set_static_mesh(mesh_sidewalk_left)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(-410.0, loc.y, 0.0), False, False)

    elif label.startswith("Sidewalk_Right_"):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc and mesh_sidewalk_right:
            smc.set_static_mesh(mesh_sidewalk_right)
            smc.set_material(0, mi_road)
        a.set_actor_location(unreal.Vector(410.0, loc.y, 0.0), False, False)

    # 2. Buildings Layout & Setbacks
    elif label == "Bld_IchirakuRamen":
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        ramen_mesh = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
        if smc and ramen_mesh:
            smc.set_static_mesh(ramen_mesh)
            smc.set_material(0, mi_wood)
            smc.set_material(1, mi_roof)
            smc.set_material(2, mi_sign)
            smc.set_material(3, mi_noren)
            smc.set_material(4, mi_lantern)
        a.set_actor_location(unreal.Vector(-580.0, 280.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
        unreal.log("-> Repositioned Bld_IchirakuRamen facing street at (-580, 280, 0)")

    elif label == "Bld_KonohaResidence_A":
        a.set_actor_location(unreal.Vector(580.0, 260.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)
        unreal.log("-> Repositioned Bld_KonohaResidence_A facing street at (580, 260, 0)")

    elif label == "Bld_KonohaResidence_B":
        a.set_actor_location(unreal.Vector(600.0, 980.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)

    elif label == "Bld_KonohaWeaponShop":
        a.set_actor_location(unreal.Vector(-600.0, 1100.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)

    elif label == "Bld_KonohaMissionOffice":
        a.set_actor_location(unreal.Vector(-600.0, 1850.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)

    elif label == "Bld_KonohaTeaHouse":
        a.set_actor_location(unreal.Vector(600.0, 1800.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)

    # 3. Move Hokage Mansion away from street center axis
    elif label == "Bld_HokageMansion_MidVista":
        a.set_actor_location(unreal.Vector(520.0, 3600.0, 50.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -30.0, 0.0), False)
        unreal.log("-> Cleared street axis: Moved Bld_HokageMansion_MidVista to (520, 3600, 50)")

    # 4. Vista Backdrop Plane (Hokage Rock & Sky)
    elif "northstar_vista" in label.lower():
        a.set_actor_location(unreal.Vector(0.0, 4200.0, 950.0), False, False)
        a.set_actor_rotation(unreal.Rotator(90.0, 0.0, 0.0), False)
        a.set_actor_scale3d(unreal.Vector(75.0, 42.0, 1.0))
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_cast_shadow(False)
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_collision_profile_name("NoCollision")
        unreal.log("-> Backdrop_Konoha_NorthStar_Vista positioned at (0, 4200, 950)")

    # 5. Overhanging Sakura Blossom Trees
    elif label == "Veg_Sakura_FrontLeft":
        a.set_actor_location(unreal.Vector(-520.0, 320.0, 0.0), False, False)
        a.set_actor_scale3d(unreal.Vector(1.3, 1.3, 1.3))

    elif label == "Veg_Sakura_FrontRight":
        a.set_actor_location(unreal.Vector(520.0, 400.0, 0.0), False, False)
        a.set_actor_scale3d(unreal.Vector(1.3, 1.3, 1.3))

    # 6. Hanging Lanterns & Props
    elif "lantern" in label.lower() and isinstance(a, unreal.StaticMeshActor):
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_material(0, mi_lantern)

    # 7. Lighting & Post Process
    elif isinstance(a, unreal.DirectionalLight):
        a.set_actor_location(unreal.Vector(0.0, -500.0, 3500.0), False, False)
        a.set_actor_rotation(unreal.Rotator(-38.0, 82.0, 0.0), False)
        dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
        if dlc:
            dlc.set_editor_property("intensity", 12.0)
            dlc.set_editor_property("light_color", unreal.Color(255, 246, 228, 255))
            dlc.set_editor_property("cast_shadows", True)
            dlc.set_editor_property("atmosphere_sun_light", True)

    elif isinstance(a, unreal.SkyLight):
        slc = a.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.set_editor_property("intensity", 6.0)
            slc.set_editor_property("light_color", unreal.Color(190, 225, 255, 255))
            slc.set_editor_property("real_time_capture", True)

    elif isinstance(a, unreal.PostProcessVolume):
        ppc = a.get_component_by_class(unreal.PostProcessComponent)
        if ppc:
            s = ppc.get_editor_property("settings")
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.8)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.8)
            s.set_editor_property("override_bloom_intensity", True)
            s.set_editor_property("bloom_intensity", 0.08)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.12, 1.12, 1.12, 1.0))
            ppc.set_editor_property("settings", s)

# 8. Interactive Living Shinobi & Villager NPCs
npc_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSVillageNPC")
if npc_class:
    for a in EAS.get_all_level_actors():
        if a and (a.get_actor_label().startswith("NPC_") or a.get_class().get_name() == "EOSVillageNPC"):
            EAS.destroy_actor(a)

    # Chef Teuchi inside Ichiraku behind counter
    teuchi = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class, unreal.Vector(-580.0, 280.0, 15.0), unreal.Rotator(0.0, 90.0, 0.0)
    )
    if teuchi:
        teuchi.set_actor_label("NPC_Teuchi_IchirakuChef")
        teuchi.set_editor_property("npcid", "NPC_ICHIRAKU_TEUCHI")

    # Chunin Gate Guard on right sidewalk
    guard = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class, unreal.Vector(410.0, 100.0, 15.0), unreal.Rotator(0.0, -90.0, 0.0)
    )
    if guard:
        guard.set_actor_label("NPC_Chunin_GateGuard")
        guard.set_editor_property("npcid", "NPC_CHUNIN_GUARD_GATE")

    # Villager 1 on left sidewalk
    v1 = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class, unreal.Vector(-410.0, 750.0, 15.0), unreal.Rotator(0.0, 75.0, 0.0)
    )
    if v1:
        v1.set_actor_label("NPC_Leaf_Villager_01")

    # Villager 2 on right sidewalk
    v2 = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class, unreal.Vector(410.0, 520.0, 15.0), unreal.Rotator(0.0, -80.0, 0.0)
    )
    if v2:
        v2.set_actor_label("NPC_Leaf_Villager_02")

    # Academy Student
    student = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class, unreal.Vector(410.0, 350.0, 15.0), unreal.Rotator(0.0, -60.0, 0.0)
    )
    if student:
        student.set_actor_label("NPC_Academy_Student")

unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log("=== Golden Street Master Assembly and Save Succeeded! ===")
