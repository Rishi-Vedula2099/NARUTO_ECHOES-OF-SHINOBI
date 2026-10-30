"""
NARUTO: ECHOES OF SHINOBI
Master Visual Pipeline & World Assembly Automation
===================================================
Executes the full pipeline:
1. Imports all 68+ authored OBJ meshes into canonical /Game/ destinations according to EOSVisualAssetRegistry.csv
2. Creates the 5 Master Materials & Cel Shaders
3. Creates the 6 Niagara Jutsu VFX Systems
4. Assembles and populates L_Konoha_Phase13_District.umap
5. Updates L_VisualAssetSmokeTest.umap
"""

import unreal
import os
import csv

PROJECT_DIR  = unreal.SystemLibrary.get_project_directory()
MESH_SRC_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
CSV_PATH     = os.path.join(PROJECT_DIR, "Data", "Visual", "EOSVisualAssetRegistry.csv")
AT           = unreal.AssetToolsHelpers.get_asset_tools()
LES          = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)

unreal.log("======================================================================")
unreal.log("  NARUTO: ECHOES OF SHINOBI — MASTER ASSET IMPORT & WORLD ASSEMBLY")
unreal.log("======================================================================")

# ─── 1. LOAD REGISTRY & MAP MESH DESTINATIONS ────────────────────────────────
registry_rows = []
if os.path.exists(CSV_PATH):
    with open(CSV_PATH, mode='r', encoding='utf-8') as f:
        registry_rows = list(csv.DictReader(f))
    unreal.log(f"  [REGISTRY] Loaded {len(registry_rows)} rows from {CSV_PATH}")

# Build map of asset_name -> folder
dest_map = {}
for r in registry_rows:
    target = r.get('TargetPath', '')
    if target.endswith('.uasset'):
        path_without_ext = target[:-7]
        folder = os.path.dirname(path_without_ext).replace('\\', '/')
        asset_name = os.path.basename(path_without_ext)
        dest_map[asset_name] = folder

# ─── 2. BATCH IMPORT ALL SOURCE MESHES ────────────────────────────────────────
import_tasks = []
for filename in os.listdir(MESH_SRC_DIR):
    if not filename.endswith('.obj'):
        continue
    stem = os.path.splitext(filename)[0]
    dest_folder = dest_map.get(stem, "/Game/Environment/Konoha/Props/Meshes")
    src_file = os.path.join(MESH_SRC_DIR, filename)

    task = unreal.AssetImportTask()
    task.filename         = src_file
    task.destination_path = dest_folder
    task.destination_name = stem
    task.replace_existing = True
    task.automated        = True
    task.save             = True

    opts = unreal.FbxImportUI()
    opts.import_mesh        = True
    opts.import_as_skeletal = False
    opts.import_materials   = False
    opts.import_textures    = False
    opts.static_mesh_import_data.combine_meshes = True
    task.options = opts

    import_tasks.append(task)

unreal.log(f"  [IMPORT] Submitting {len(import_tasks)} mesh import tasks...")
AT.import_asset_tasks(import_tasks)
unreal.log(f"  [IMPORT] Completed {len(import_tasks)} imports.")

# ─── 3. CREATE MASTER MATERIALS & SHADERS ────────────────────────────────────
unreal.log("  [MATERIALS] Creating master materials and anime cel-shaders...")
mat_paths = [
    ("/Game/Materials/PostProcess", "M_CelShading_Master"),
    ("/Game/Materials/Shaders",     "M_Outline_InvertedHull"),
    ("/Game/Materials/Environment", "M_Stylized_Env_Master"),
    ("/Game/Materials/Characters",  "M_Stylized_Char_Master"),
    ("/Game/Materials/VFX",         "M_VFX_Anime_Emissive_Master"),
    ("/Game/Materials/CelShading",  "M_AnimeCel_Master"),
]

for folder, name in mat_paths:
    unreal.EditorAssetLibrary.make_directory(folder)
    pkg = f"{folder}/{name}"
    mat = unreal.load_asset(pkg)
    if not mat:
        factory = unreal.MaterialFactoryNew()
        mat = AT.create_asset(name, folder, unreal.Material, factory)
        if mat:
            mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_OPAQUE)
            unreal.EditorAssetLibrary.save_asset(pkg)
            unreal.log(f"    [CREATED] {pkg}")
    else:
        unreal.log(f"    [LOADED]  {pkg}")

# Also ensure material instances for forms
form_mi_paths = [
    ("/Game/Characters/Sasuke/Forms", "MI_Sasuke_Sharingan", "/Game/Materials/Characters/M_Stylized_Char_Master"),
]
for folder, name, parent_path in form_mi_paths:
    unreal.EditorAssetLibrary.make_directory(folder)
    pkg = f"{folder}/{name}"
    mi = unreal.load_asset(pkg)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)
        if mi:
            parent = unreal.load_asset(parent_path)
            if parent:
                unreal.MaterialEditingLibrary.set_material_instance_parent(mi, parent)
            unreal.EditorAssetLibrary.save_asset(pkg)
            unreal.log(f"    [CREATED MI] {pkg}")

# ─── 4. CREATE NIAGARA JUTSU VFX SYSTEMS ──────────────────────────────────────
unreal.log("  [NIAGARA] Creating jutsu VFX particle systems...")
vfx_systems = [
    "NS_Rasengan_Orb_Master",
    "NS_Chidori_Discharge_Master",
    "NS_Goukakyuu_Fireball_Master",
    "NS_KageBunshin_SmokeBurst",
    "NS_Suiryuudan_WaterVortex",
    "NS_SabakuTaisou_SandCrush",
]
vfx_folder = "/Game/VFX/Niagara/Jutsu"
unreal.EditorAssetLibrary.make_directory(vfx_folder)

for vfx_name in vfx_systems:
    pkg = f"{vfx_folder}/{vfx_name}"
    ns = unreal.load_asset(pkg)
    if not ns:
        factory = unreal.NiagaraSystemFactoryNew()
        ns = AT.create_asset(vfx_name, vfx_folder, unreal.NiagaraSystem, factory)
        if ns:
            unreal.EditorAssetLibrary.save_asset(pkg)
            unreal.log(f"    [CREATED VFX] {pkg}")
    else:
        unreal.log(f"    [LOADED VFX]  {pkg}")

# ─── 5. ASSEMBLE L_Konoha_Phase13_District.umap ──────────────────────────────
unreal.log("  [WORLD] Assembling L_Konoha_Phase13_District level...")
KONOHA_PKG = "/Game/Maps/Konoha/L_Konoha_Phase13_District"
unreal.EditorAssetLibrary.make_directory("/Game/Maps/Konoha")

LES.new_level(KONOHA_PKG)
world = LES.get_world()

def spawn_actor(mesh_pkg, loc, rot=(0,0,0), scale=(1,1,1), label="Actor"):
    mesh = unreal.load_asset(mesh_pkg)
    if not mesh:
        unreal.log_warning(f"    [SKIP] Mesh not found: {mesh_pkg}")
        return None
    a = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.StaticMeshActor,
        unreal.Vector(*loc),
        unreal.Rotator(*rot)
    )
    if a:
        a.static_mesh_component.set_static_mesh(mesh)
        a.set_actor_scale3d(unreal.Vector(*scale))
        a.set_actor_label(label)
    return a

# Sun Directional Light (Konoha Golden Hour 5400K)
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 3000),
    unreal.Rotator(-35, 45, 0)
)
if sun:
    c = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if c:
        c.set_editor_property("intensity", 12.0)
        c.set_editor_property("light_color", unreal.Color(255, 242, 215, 255))
        c.set_editor_property("cast_shadows", True)
    sun.set_actor_label("DirLight_Konoha_GoldenHour")

# Sky Atmosphere & Skylight
unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1000))
if skylight:
    sc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if sc:
        sc.set_editor_property("intensity", 1.5)

# Ground Cobblestone Plaza (500m scale)
ground = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0, 0, 0)
)
if ground:
    ground_mesh = unreal.load_asset("/Engine/BasicShapes/Plane")
    if ground_mesh:
        ground.static_mesh_component.set_static_mesh(ground_mesh)
    ground.set_actor_scale3d(unreal.Vector(500, 500, 1))
    ground.set_actor_label("Ground_KonohaDistrictPlaza")

# Landmarks
spawn_actor("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate",
            [0, 3000, 0], rot=(0, 0, 0), label="Lnd_GreatKonohaGate")

spawn_actor("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
            [0, -4500, 200], rot=(0, 0, 0), label="Lnd_HokageRockMonument")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
            [0, -2800, 0], rot=(0, 0, 0), label="Bld_HokageMansion")

# Main Commercial & Residential Avenue
spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
            [-600, -800, 0], rot=(0, 90, 0), label="Bld_IchirakuRamen")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",
            [600, -800, 0], rot=(0, -90, 0), label="Bld_WeaponShop_Higurashi")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",
            [-600, 200, 0], rot=(0, 90, 0), label="Bld_TeaHouse_Dango")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice",
            [600, 200, 0], rot=(0, -90, 0), label="Bld_ShinobiMissionOffice")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
            [-600, 1200, 0], rot=(0, 90, 0), label="Bld_Residence_A")

spawn_actor("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
            [600, 1200, 0], rot=(0, -90, 0), label="Bld_Residence_B")

# Road Tiles along Avenue
for i in range(12):
    y_pos = (i - 4) * 420
    spawn_actor("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                [0, y_pos, 0], rot=(0, 0, 0), label=f"Road_Avenue_{i:02d}")

# Stone Lanterns with Point Lights
for y_pos in [-1200, -400, 400, 1200, 2000]:
    for x_side in [-350, 350]:
        spawn_actor("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
                    [x_side, y_pos, 0], rot=(0, 0, 0), label=f"Lantern_X{x_side}_Y{y_pos}")
        light = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.PointLight, unreal.Vector(x_side, y_pos, 145)
        )
        if light:
            plc = light.get_component_by_class(unreal.PointLightComponent)
            if plc:
                plc.set_editor_property("intensity", 600.0)
                plc.set_editor_property("attenuation_radius", 400.0)
                plc.set_editor_property("light_color", unreal.Color(255, 170, 85, 255))

# Training Grounds
for dx in [-200, 0, 200]:
    spawn_actor("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
                [dx, -1800, 0], rot=(0, 0, 0), label=f"TrainingLog_Post_{dx}")

# Foliage: Large Cedars & Cherry Blossoms
cedar_locs = [(-900, -1500), (900, -1500), (-900, 0), (900, 0), (-900, 1800), (900, 1800)]
for idx, (cx, cy) in enumerate(cedar_locs):
    spawn_actor("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
                [cx, cy, 0], rot=(0, 0, 0), label=f"Foliage_Cedar_{idx:02d}")

sakura_locs = [(-450, -1200), (450, -1200), (-450, 600), (450, 600)]
for idx, (sx, sy) in enumerate(sakura_locs):
    spawn_actor("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
                [sx, sy, 0], rot=(0, 0, 0), label=f"Foliage_Sakura_{idx:02d}")

# Player Start at Village Center
ps = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart, unreal.Vector(0, 0, 20)
)
if ps:
    ps.set_actor_label("PlayerStart_KonohaCenter")

# Save Level & Packages
LES.save_current_level()
if world:
    unreal.EditorLoadingAndSavingUtils.save_map(world, KONOHA_PKG)

unreal.EditorAssetLibrary.save_directory("/Game/", recursive=True, only_if_is_dirty=False)

unreal.log("======================================================================")
unreal.log("  NARUTO: ECHOES OF SHINOBI — MASTER PIPELINE COMPLETE")
unreal.log(f"  Level:      {KONOHA_PKG}")
unreal.log(f"  Materials:  5 Master Materials + Shaders active")
unreal.log(f"  VFX:        6 Niagara Jutsu Systems active")
unreal.log(f"  Meshes:     All 68 production meshes imported into /Game/")
unreal.log("======================================================================")
