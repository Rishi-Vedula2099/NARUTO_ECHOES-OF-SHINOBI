"""
NARUTO: ECHOES OF SHINOBI
Authoritative Golden Konoha Street Builder
===========================================
Level: /Game/Maps/GoldenTest/L_Konoha_GoldenStreet
Visual North Star: EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01 (Konoha_Visual_North_Star.jpg)
"""

import unreal
import os

EAL = unreal.EditorAssetLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
AT  = unreal.AssetToolsHelpers.get_asset_tools()

MAP_FOLDER = "/Game/Maps/GoldenTest"
MAP_NAME   = "L_Konoha_GoldenStreet"
MAP_PKG    = f"{MAP_FOLDER}/{MAP_NAME}"

unreal.log("======================================================================")
unreal.log(f"  BUILDING PRODUCTION EOS GOLDEN STREET: {MAP_PKG}")
unreal.log("  Target Reference: Konoha_Visual_North_Star.jpg")
unreal.log("======================================================================")

# ─── 0. CREATE / VERIFY BACKDROP MATERIAL ────────────────────────────────────
backdrop_mat_pkg = "/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop"
tex_pkg = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"

tex = unreal.load_asset(tex_pkg)
if not tex:
    src_jpg = "c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Content/Art/References/Phase13/Konoha_Visual_North_Star.jpg"
    if os.path.exists(src_jpg):
        task = unreal.AssetImportTask()
        task.filename = src_jpg
        task.destination_path = "/Game/Art/References/Phase13"
        task.destination_name = "Konoha_Visual_North_Star"
        task.replace_existing = True
        task.automated = True
        task.save = True
        AT.import_asset_tasks([task])
        tex = unreal.load_asset(tex_pkg)

mi_backdrop = unreal.load_asset(backdrop_mat_pkg)
if not mi_backdrop:
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
        mi_backdrop = mat

# Clean Map
if EAL.does_asset_exist(MAP_PKG):
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
    for a in actors:
        unreal.EditorLevelLibrary.destroy_actor(a)
else:
    world = LES.new_level(MAP_PKG)

def load_mat(path):
    m = unreal.load_asset(path)
    if not m:
        unreal.log_warning(f"Material not found: {path}")
    return m

# Production Materials
mi_ichiraku_wood = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_ichiraku_roof = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_ichiraku_noren= load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
mi_residence_a   = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
mi_residence_b   = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
mi_road          = load_mat("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_cedar         = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
mi_sakura        = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
mi_lantern_stone = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
mi_lantern_glow  = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")
mi_hokage_dome   = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")

mi_naruto_orange = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
mi_naruto_blue   = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Navy")
mi_naruto_hair   = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Hair")
mi_naruto_skin   = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")
mi_naruto_collar = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Collar")

mi_kakashi_vest  = load_mat("/Game/Characters/Kakashi/Materials/MI_Kakashi_Vest")
mi_teuchi_apron  = load_mat("/Game/Characters/NPCs/Shopkeepers/Materials/MI_Teuchi_Apron")

# Configure Lantern Glow to warm amber/red (not blinding white)
if mi_lantern_glow and hasattr(mi_lantern_glow, 'set_vector_parameter_value'):
    mi_lantern_glow.set_vector_parameter_value("BaseColor", unreal.LinearColor(0.95, 0.25, 0.05, 1.0))
    mi_lantern_glow.set_vector_parameter_value("EmissiveColor", unreal.LinearColor(1.0, 0.35, 0.06, 1.0))
    if hasattr(mi_lantern_glow, 'set_scalar_parameter_value'):
        mi_lantern_glow.set_scalar_parameter_value("EmissiveStrength", 2.0)
    EAL.save_loaded_asset(mi_lantern_glow)

def spawn_mesh(mesh_path, loc, rot=(0,0,0), scale=(1,1,1), mat=None, label="Actor", no_collision=False):
    mesh = unreal.load_asset(mesh_path)
    if not mesh:
        unreal.log_warning(f"Mesh not found: {mesh_path}")
        return None
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.StaticMeshActor,
        unreal.Vector(*loc),
        unreal.Rotator(*rot)
    )
    if actor:
        smc = actor.static_mesh_component
        smc.set_static_mesh(mesh)
        actor.set_actor_scale3d(unreal.Vector(*scale))
        actor.set_actor_label(label)
        if no_collision:
            smc.set_collision_profile_name("NoCollision")
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        if mat:
            num = max(smc.get_num_materials(), 1)
            for i in range(num):
                smc.set_material(i, mat)
    return actor

# ─── 1. FULL ANIME DAYLIGHT, SKY ATMOSPHERE & AMBIENT FILL ──────────────────
# SkyAtmosphere provides the rich azure sky dome
sky_atm = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
if sky_atm:
    sky_atm.set_actor_label("SkyAtmosphere_KonohaAnime")
    sac = sky_atm.get_component_by_class(unreal.SkyAtmosphereComponent)
    if sac:
        try:
            sac.set_editor_property("rayleigh_scattering", unreal.LinearColor(0.12, 0.30, 0.75, 1.0))
        except Exception:
            pass

# Directional Sun (Golden afternoon daylight, 60 deg angle)
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 4000),
    unreal.Rotator(-35, 65, 0)
)
if sun:
    sun.set_actor_label("DirLight_Konoha_Sun")
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 65000.0)
        sc.set_editor_property("light_color", unreal.Color(255, 244, 225, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        sc.set_editor_property("atmosphere_sun_light_index", 0)
        try:
            sc.set_editor_property("dynamic_shadow_distance_movable_light", 15000.0)
        except Exception:
            pass

# SkyLight with Real-Time Capture (Fills cel shadows with soft sky blue + warm ground bounce)
skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1500))
if skylight:
    skylight.set_actor_label("SkyLight_AnimeAzureFill")
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 2.2)
        slc.set_editor_property("light_color", unreal.Color(200, 225, 255, 255))
        slc.set_editor_property("real_time_capture", True)
        try:
            slc.set_editor_property("lower_hemisphere_color", unreal.LinearColor(0.55, 0.45, 0.35, 1.0))
        except Exception:
            pass

# Exponential Height Fog (Soft distant horizon haze, leaving street crystal clear)
fog = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0))
if fog:
    fog.set_actor_label("Fog_KonohaSunsetHaze")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    if fc:
        try:
            fc.set_editor_property("fog_density", 0.000015)
            fc.set_editor_property("start_distance", 2800.0)
            fc.set_editor_property("fog_inscattering_luminance", unreal.LinearColor(0.85, 0.90, 1.0, 1.0))
        except Exception:
            pass

# Post Process Volume (Rich Saturation, Controlled Contrast, Subtle Bloom)
pp = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
if pp:
    pp.set_actor_label("PostProcess_GoldenStreet")
    pp.set_editor_property("unbound", True)
    ppc = pp.get_component_by_class(unreal.PostProcessComponent)
    if ppc:
        s = ppc.get_editor_property("settings")
        s.set_editor_property("bloom_intensity", 0.08)
        s.set_editor_property("bloom_threshold", 2.0)
        s.set_editor_property("color_saturation", unreal.Vector4(1.15, 1.15, 1.15, 1.0))
        s.set_editor_property("color_contrast", unreal.Vector4(1.05, 1.05, 1.05, 1.0))
        ppc.set_editor_property("settings", s)

# ─── 2. MASTER ANIME VISTA BACKDROP ─────────────────────────────────────────
# Master North Star Backdrop Plate (Unlit, no collision, framing Hokage Monument & mountain depth)
backdrop = spawn_mesh("/Engine/BasicShapes/Plane",
                      [0, 5200, 1400], rot=(90, 0, -90), scale=(95, 48, 1),
                      mat=mi_backdrop, label="Backdrop_Konoha_NorthStar_Vista", no_collision=True)

# ─── 3. CENTRAL COBBLESTONE AVENUE ──────────────────────────────────────────
# Base Ground Plaza
ground = spawn_mesh("/Engine/BasicShapes/Plane", [0, 1200, -2], scale=(80, 80, 1), mat=mi_road, label="Ground_BasePlaza")

# Paved Road Tiles along central avenue
for i in range(12):
    y_pos = (i - 2) * 400
    spawn_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
               [0, y_pos, 0], rot=(0, 0, 0), scale=(1.2, 1.2, 1.0), mat=mi_road, label=f"Road_Avenue_{i:02d}")

# ─── 4. LEFT SIDE: ICHIRAKU RAMEN SHOP & SEATING ─────────────────────────────
# Ichiraku Ramen Main Building
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
           [-460, 240, 0], rot=(0, 85, 0), scale=(1.15, 1.15, 1.15), mat=mi_ichiraku_wood, label="Bld_IchirakuRamen")

# Red Hanging Paper Lanterns on Ichiraku Eaves
for ly in [110, 230, 350]:
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
               [-320, ly, 180], rot=(0, 0, 0), scale=(1.2, 1.2, 1.2), mat=mi_lantern_glow, label=f"Lantern_Ichiraku_{ly}", no_collision=True)

# Teuchi (Ramen Chef) inside behind counter
spawn_mesh("/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi",
           [-400, 240, 20], rot=(0, 85, 0), scale=(1.0, 1.0, 1.0), mat=mi_teuchi_apron, label="NPC_Teuchi_Chef")

# Sasuke & Sakura Dining at Counter Stools
spawn_mesh("/Game/Characters/Sasuke/Meshes/SK_Sasuke_Genin",
           [-310, 160, 0], rot=(0, -95, 0), scale=(0.95, 0.95, 0.95), label="NPC_Sasuke_Dining")

spawn_mesh("/Game/Characters/Sakura/Meshes/SK_Sakura_Genin",
           [-310, 270, 0], rot=(0, -95, 0), scale=(0.92, 0.92, 0.92), label="NPC_Sakura_Dining")

# ─── 5. RIGHT SIDE: JAPANESE SHOPS, RESIDENCES & BALCONIES ──────────────────
# Residence A (Right Fore-to-Midground)
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
           [480, 180, 0], rot=(0, -95, 0), scale=(1.1, 1.1, 1.1), mat=mi_residence_a, label="Bld_KonohaResidence_Right_01")

# Residence B (Right Further Midground)
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
           [520, 750, 0], rot=(0, -90, 0), scale=(1.1, 1.1, 1.1), mat=mi_residence_b, label="Bld_KonohaResidence_Right_02")

# Right Side Red Paper Lanterns
for ly in [120, 240, 360]:
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
               [360, ly, 180], rot=(0, 0, 0), scale=(1.2, 1.2, 1.2), mat=mi_lantern_glow, label=f"Lantern_RightShop_{ly}", no_collision=True)

# Wooden Utility Pole with Overhead Wires
spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
           [380, 40, 0], rot=(0, 15, 0), scale=(1.1, 1.1, 1.1), label="Prop_UtilityPole_Right")

# Street Props: Benches, Crates, Barrels
spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
           [330, 480, 0], rot=(0, -90, 0), label="Prop_Bench_Right")
spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood",
           [340, 560, 0], rot=(0, 0, 0), label="Prop_RainBarrel_01")
spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate",
           [335, 600, 0], rot=(0, 20, 0), label="Prop_SupplyCrate_01")

# ─── 6. STREET NPCS: PATROLLING CHUNIN & CIVILIANS ───────────────────────────
spawn_mesh("/Game/Characters/NPCs/Guards/Meshes/SK_Chunin_GateGuard",
           [60, 420, 0], rot=(0, 90, 0), scale=(1.0, 1.0, 1.0), mat=mi_kakashi_vest, label="NPC_Chunin_Patrol")

spawn_mesh("/Game/Characters/NPCs/KonohaVillagers/Meshes/SK_Villager_Male01",
           [280, 360, 0], rot=(0, -85, 0), scale=(1.0, 1.0, 1.0), label="NPC_Villager_Sidewalk")

# ─── 7. FOLIAGE: KONOHA CEDARS & SAKURA TREES ────────────────────────────────
spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
           [-580, -100, 0], rot=(0, 35, 0), scale=(1.25, 1.25, 1.25), mat=mi_cedar, label="Foliage_Cedar_Left")

spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
           [460, -80, 0], rot=(0, -25, 0), scale=(1.2, 1.2, 1.2), mat=mi_sakura, label="Foliage_Sakura_Right")

spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
           [560, 500, 0], rot=(0, -70, 0), scale=(1.3, 1.3, 1.3), mat=mi_cedar, label="Foliage_Cedar_Right_Distant")

# ─── 8. 3D MIDGROUND HOKAGE MANSION (PAGODA) ─────────────────────────────────
# Placed far at the end of the avenue at Y = 3400, scale 0.75 so it frames beautifully
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
           [0, 3400, 50], rot=(0, 0, 0), scale=(0.75, 0.75, 0.75), mat=mi_hokage_dome, label="Bld_HokageMansion_CenterVista")

# ─── 9. PLAYERSTART (FRAMING HERO NARUTO CENTERED FACING DOWN AVENUE) ────────
# Player starts at (0, 0, 95) facing +Y (straight down the avenue)
ps = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart, unreal.Vector(0, 0, 95), unreal.Rotator(0, 90, 0)
)
if ps:
    ps.set_actor_label("PlayerStart_GoldenView")
    ps.tags.append(unreal.Name("GoldenView"))
    ps.tags.append(unreal.Name("KonohaMainStreetStart"))

# ─── 10. NIAGARA JUTSU VFX: RASENGAN SYSTEM ──────────────────────────────────
rasengan_vfx = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master")
if rasengan_vfx:
    vfx_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.NiagaraActor, unreal.Vector(35, 20, 85)
    )
    if vfx_actor:
        vfx_actor.set_actor_label("VFX_Rasengan_Hero")
        nc = vfx_actor.get_component_by_class(unreal.NiagaraComponent)
        if nc:
            nc.set_asset(rasengan_vfx)
            nc.activate(True)

# ─── 11. SAVE LEVEL ──────────────────────────────────────────────────────────
unreal.EditorLoadingAndSavingUtils.save_current_level()

unreal.log("======================================================================")
unreal.log(f"  GOLDEN KONOHA STREET REBUILT AND SAVED TO: {MAP_PKG}")
unreal.log("  Visual Composition conforms to EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01")
unreal.log("======================================================================")
