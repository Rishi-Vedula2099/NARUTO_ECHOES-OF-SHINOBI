"""
NARUTO: ECHOES OF SHINOBI
Unreal Engine 5.8 Python Commandlet — Golden Proof #1
=====================================================
Correct UE5.8 Python API version.
Run via:
  UnrealEditor-Cmd.exe EchoesOfShinobi.uproject
    -run=pythonscript
    -script="c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Scripts/ue5_import_and_build.py"
    -unattended -stdout
"""

import unreal
import os

PROJECT_DIR  = unreal.SystemLibrary.get_project_directory()
MESH_SRC_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
AT           = unreal.AssetToolsHelpers.get_asset_tools()

ASSET_MAP = {
    "SK_Naruto_Genin":          "/Game/Characters/Naruto/Meshes",
    "SM_Bld_IchirakuRamen":     "/Game/Environment/Konoha/Buildings/Meshes",
    "SM_Road_Paved_01":         "/Game/Environment/Konoha/Roads/Meshes",
    "SM_Prop_StoneLantern_01":  "/Game/Environment/Konoha/Props/Meshes",
    "SM_Veg_Cedar_Large":       "/Game/Environment/Konoha/Foliage/Meshes",
    "BP_TrainingDummy":         "/Game/Characters/Combat/Meshes",
}

# ─── 1. IMPORT ALL OBJ MESHES ─────────────────────────────────────────────────
unreal.log("=== EOS: Beginning Golden Proof #1 — Mesh Import ===")
import_tasks = []

for asset_name, dest_folder in ASSET_MAP.items():
    src = os.path.join(MESH_SRC_DIR, asset_name + ".obj")
    if not os.path.exists(src):
        unreal.log_warning(f"  [MISSING] {src}")
        continue

    task = unreal.AssetImportTask()
    task.filename         = src
    task.destination_path = dest_folder
    task.destination_name = asset_name
    task.replace_existing = True
    task.automated        = True
    task.save             = True

    # OBJ-specific import options
    opts = unreal.FbxImportUI()
    opts.import_mesh        = True
    opts.import_as_skeletal = False
    opts.import_materials   = False
    opts.import_textures    = False
    opts.static_mesh_import_data.combine_meshes = True
    task.options = opts

    import_tasks.append(task)

AT.import_asset_tasks(import_tasks)
unreal.log(f"  [IMPORT] Submitted {len(import_tasks)} import tasks")

# Verify each import
imported = {}
for asset_name, dest_folder in ASSET_MAP.items():
    pkg = f"{dest_folder}/{asset_name}"
    obj = unreal.load_asset(pkg)
    if obj:
        unreal.log(f"  [OK]   {pkg}")
        imported[asset_name] = obj
    else:
        unreal.log_warning(f"  [FAIL] {pkg}")

# ─── 2. CEL-SHADING MATERIAL ──────────────────────────────────────────────────
unreal.log("=== EOS: Creating M_AnimeCel_Master ===")

# Ensure folders exist
for folder in [
    "/Game/Materials/CelShading",
    "/Game/Characters/Naruto/Materials",
    "/Game/Environment/Konoha/Buildings/Materials",
    "/Game/Environment/Konoha/Roads/Materials",
]:
    unreal.EditorAssetLibrary.make_directory(folder)

cel_path = "/Game/Materials/CelShading/M_AnimeCel_Master"
cel_mat = unreal.load_asset(cel_path)
if not cel_mat:
    mat_factory = unreal.MaterialFactoryNew()
    cel_mat = AT.create_asset("M_AnimeCel_Master", "/Game/Materials/CelShading",
                              unreal.Material, mat_factory)
    if cel_mat:
        cel_mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_OPAQUE)
        unreal.EditorAssetLibrary.save_asset(cel_mat.get_path_name())
        unreal.log("  [OK Created] M_AnimeCel_Master")
else:
    unreal.log("  [OK Loaded] M_AnimeCel_Master (pre-existing)")

# ─── 3. MATERIAL INSTANCES ────────────────────────────────────────────────────
def make_mi(name, folder, parent_path):
    pkg_path = f"{folder}/{name}"
    mi = unreal.load_asset(pkg_path)
    if not mi:
        factory = unreal.MaterialInstanceConstantFactoryNew()
        mi = AT.create_asset(name, folder, unreal.MaterialInstanceConstant, factory)
        if mi:
            parent = unreal.load_asset(parent_path)
            if parent:
                unreal.MaterialEditingLibrary.set_material_instance_parent(mi, parent)
            unreal.EditorAssetLibrary.save_asset(mi.get_path_name())
            unreal.log(f"  [OK Created] {pkg_path}")
    else:
        unreal.log(f"  [OK Loaded] {pkg_path}")
    return mi

mi_naruto   = make_mi("MI_Naruto_Genin",       "/Game/Characters/Naruto/Materials",           cel_path)
mi_building = make_mi("MI_Bld_Ichiraku_Wood",  "/Game/Environment/Konoha/Buildings/Materials",cel_path)
mi_road     = make_mi("MI_Road_Cobblestone",   "/Game/Environment/Konoha/Roads/Materials",    cel_path)

# ─── 4. ASSIGN MATERIALS TO IMPORTED STATIC MESHES ───────────────────────────
def assign_mat(pkg, mi):
    mesh = unreal.load_asset(pkg)
    if mesh and mi and isinstance(mesh, unreal.StaticMesh):
        try:
            num = mesh.get_num_sections(0)
            for i in range(max(num, 1)):
                mesh.set_material(i, mi)
            unreal.EditorAssetLibrary.save_asset(mesh.get_path_name())
            unreal.log(f"  [MAT]  {pkg}")
        except Exception as e:
            unreal.log_warning(f"  [MAT WARN] {pkg}: {e}")

assign_mat("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", mi_building)
assign_mat("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",         mi_road)

# ─── 5. BUILD L_VisualAssetSmokeTest ─────────────────────────────────────────
unreal.log("=== EOS: Building L_VisualAssetSmokeTest Level ===")

LEVEL_PKG = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
unreal.EditorAssetLibrary.make_directory("/Game/Maps/VisualSmoke")

les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
created_level = les.new_level(LEVEL_PKG)
world = les.get_world()
unreal.log(f"  [OK] New level active: {LEVEL_PKG} in world: {world.get_name() if world else 'None'}")

def spawn_sma(mesh_pkg, loc, label):
    mesh = unreal.load_asset(mesh_pkg)
    if not mesh:
        unreal.log_warning(f"  [SKIP] {label} — mesh not found: {mesh_pkg}")
        return None
    a = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.StaticMeshActor, unreal.Vector(*loc)
    )
    if a:
        a.static_mesh_component.set_static_mesh(mesh)
        a.set_actor_label(label)
        unreal.log(f"  [PLACE] {label} @ {loc}")
    return a

# --- Directional Sun ---
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 2000),
    unreal.Rotator(-38, 45, 0)
)
if sun:
    c = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if c:
        c.set_editor_property("intensity",   10.0)
        c.set_editor_property("light_color", unreal.Color(255, 240, 210, 255))
        c.set_editor_property("cast_shadows", True)
    sun.set_actor_label("DirLight_Konoha_5400K")

# --- Sky Atmosphere ---
unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.SkyAtmosphere, unreal.Vector(0, 0, 0)
)

# --- Ground (plane) ---
ground = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0, 0, 0)
)
if ground:
    ground_mesh = unreal.load_asset("/Engine/BasicShapes/Plane")
    if ground_mesh:
        ground.static_mesh_component.set_static_mesh(ground_mesh)
    ground.set_actor_scale3d(unreal.Vector(50, 50, 1))
    ground.set_actor_label("Ground_KonohaStreet")

# --- Ichiraku Ramen Building ---
spawn_sma("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
          [0, -1200, 0], "SM_Bld_IchirakuRamen")

# --- Road slabs (5 tiles forming a street) ---
for i in range(5):
    spawn_sma("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
              [(i - 2) * 420, -400, 0], f"SM_Road_Paved_{i:02d}")

# --- Stone lanterns with warm point lights ---
for lx in [-350, 350]:
    spawn_sma("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
              [lx, -400, 0], f"SM_StoneLantern_X{lx}")
    pt = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.PointLight, unreal.Vector(lx, -400, 145)
    )
    if pt:
        c = pt.get_component_by_class(unreal.PointLightComponent)
        if c:
            c.set_editor_property("intensity",         600.0)
            c.set_editor_property("attenuation_radius",400.0)
            c.set_editor_property("light_color",       unreal.Color(255, 160, 80, 255))
        pt.set_actor_label(f"LanternLight_X{lx}")

# --- Cedar Tree ---
spawn_sma("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large",
          [-600, -500, 0], "SM_Veg_Cedar_01")

# --- Training Dummy ---
spawn_sma("/Game/Characters/Combat/Meshes/BP_TrainingDummy",
          [400, 100, 0], "BP_TrainingDummy_01")

# --- Player Start ---
ps = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart, unreal.Vector(0, 200, 10)
)
if ps:
    ps.set_actor_label("PlayerStart_NarutoGenin")

# --- Save level and all assets ---
les.save_current_level()
if world:
    unreal.EditorLoadingAndSavingUtils.save_map(world, LEVEL_PKG)

unreal.EditorAssetLibrary.save_directory("/Game/", recursive=True, only_if_is_dirty=False)

unreal.log("=" * 70)
unreal.log("  EOS GOLDEN PROOF #1 BUILD COMPLETE")
unreal.log(f"  Level:  {LEVEL_PKG}")
unreal.log("  Actors: Sun, Sky, Ground, Ichiraku, Road x5, Lanterns x2,")
unreal.log("          Cedar Tree, Training Dummy, Player Start")
unreal.log("  Status: Gate 1 + Gate 2 geometry in Content/")
unreal.log("=" * 70)
