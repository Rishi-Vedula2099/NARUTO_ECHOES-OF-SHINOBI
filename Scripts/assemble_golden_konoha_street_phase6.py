"""
NARUTO: ECHOES OF SHINOBI
PHASE 6: AUTHORITATIVE 50M KONOHA GOLDEN STREET LEVEL BUILDER
=============================================================
Map: /Game/Maps/GoldenTest/L_Konoha_GoldenStreet
Visual Authority:
  - Docs/VisualBible/06_ENVIRONMENT_WORLD_BIBLE.md
  - Docs/VisualBible/00_MASTER_VISUAL_DEVELOPMENT_CONTRACT.md
  - Docs/Reports/VISUAL_FAILURE_MEMORY.md (VF-004, VF-005: 8.0 lux sun, locked exposure, azure skylight)
"""

import unreal

EAL = unreal.EditorAssetLibrary

MAP_FOLDER = "/Game/Maps/GoldenTest"
MAP_NAME   = "L_Konoha_GoldenStreet"
MAP_PKG    = f"{MAP_FOLDER}/{MAP_NAME}"

unreal.log("======================================================================")
unreal.log(f"  ASSEMBLING PHASE 6 AUTHORITATIVE 50M GOLDEN STREET: {MAP_PKG}")
unreal.log("======================================================================")

# ─── 1. BIND AUTHORITATIVE ANIME MATERIALS TO STATIC MESH DEFAULTS ───────────
def bind_static_mesh_materials():
    unreal.log("--> Binding anime cel materials to static mesh default slots...")

    mi_wood          = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
    mi_stone         = unreal.load_asset("/Game/Materials/Environment/MI_AnimeStone")
    mi_metal         = unreal.load_asset("/Game/Materials/Characters/MI_AnimeMetal")
    mi_foliage       = unreal.load_asset("/Game/Materials/Environment/MI_AnimeFoliage")
    mi_ground        = unreal.load_asset("/Game/Materials/Environment/MI_AnimeGround")
    mi_cedar         = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
    mi_sakura        = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
    mi_lantern_stone = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
    mi_lantern_glow  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")
    mi_hokage        = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
    mi_torii         = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
    mi_noren         = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
    mi_road          = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
    mi_ichiraku_wood = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
    mi_residence_a   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
    mi_residence_b   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
    mi_hokage_mansion= unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")

    if mi_lantern_glow and hasattr(mi_lantern_glow, 'set_vector_parameter_value'):
        mi_lantern_glow.set_vector_parameter_value("BaseColor", unreal.LinearColor(0.95, 0.28, 0.05, 1.0))
        mi_lantern_glow.set_vector_parameter_value("EmissiveColor", unreal.LinearColor(1.0, 0.38, 0.08, 1.0))
        if hasattr(mi_lantern_glow, 'set_scalar_parameter_value'):
            mi_lantern_glow.set_scalar_parameter_value("EmissiveStrength", 2.0)
        EAL.save_loaded_asset(mi_lantern_glow)

    bindings = [
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin", mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku", mi_noren or mi_wood),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WaterPumpingStation", mi_metal),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01", mi_lantern_stone or mi_stone),
        ("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post", mi_wood),
        ("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01", mi_road),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large", mi_cedar or mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large", mi_cedar or mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom", mi_sakura or mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01", mi_foliage),
        ("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized", mi_ground or mi_foliage),
        ("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument", mi_hokage or mi_stone),
        ("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate", mi_torii or mi_wood),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen", mi_ichiraku_wood),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A", mi_residence_a),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B", mi_residence_b),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop", mi_ichiraku_wood),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse", mi_ichiraku_wood),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice", mi_residence_a),
        ("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion", mi_hokage_mansion),
    ]

    for p, mat in bindings:
        if not mat:
            continue
        mesh = unreal.load_asset(p)
        if mesh:
            num = len(mesh.static_materials)
            for idx in range(max(num, 1)):
                mesh.set_material(idx, mat)
            EAL.save_loaded_asset(mesh)

bind_static_mesh_materials()

# ─── 2. LOAD & INITIALIZE GOLDEN STREET LEVEL ───────────────────────────────
if EAL.does_asset_exist(MAP_PKG):
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in actors:
        eas.destroy_actor(a)
else:
    LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    world = LES.new_level(MAP_PKG)

def spawn_any_mesh(mesh_path, loc, rot=(0,0,0), scale=(1,1,1), mat=None, label="Actor", no_collision=False):
    asset = unreal.load_asset(mesh_path)
    if not asset:
        unreal.log_warning(f"Asset missing: {mesh_path}")
        return None

    if isinstance(asset, unreal.SkeletalMesh):
        actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.SkeletalMeshActor,
            unreal.Vector(*loc),
            unreal.Rotator(*rot)
        )
        if actor:
            skmc = actor.skeletal_mesh_component
            skmc.set_skeletal_mesh_asset(asset)
            actor.set_actor_scale3d(unreal.Vector(*scale))
            actor.set_actor_label(label)
            if no_collision:
                skmc.set_collision_profile_name("NoCollision")
                skmc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            if mat:
                num = max(skmc.get_num_materials(), 1)
                for i in range(num):
                    skmc.set_material(i, mat)
        return actor
    elif isinstance(asset, unreal.StaticMesh):
        actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,
            unreal.Vector(*loc),
            unreal.Rotator(*rot)
        )
        if actor:
            smc = actor.static_mesh_component
            smc.set_static_mesh(asset)
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
    else:
        unreal.log_warning(f"Unsupported mesh class for {mesh_path}: {asset.get_class().get_name()}")
        return None

# Load production materials
mi_ichiraku_wood = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_residence_a   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
mi_residence_b   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
mi_hokage_mansion= unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")
mi_road          = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_cedar         = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
mi_sakura        = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
mi_lantern_stone = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
mi_lantern_glow  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")
mi_hokage_rock   = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
mi_torii_red     = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
mi_backdrop      = unreal.load_asset("/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop")
mi_wood          = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
mi_metal         = unreal.load_asset("/Game/Materials/Characters/MI_AnimeMetal")
mi_foliage       = unreal.load_asset("/Game/Materials/Environment/MI_AnimeFoliage")
mi_ground        = unreal.load_asset("/Game/Materials/Environment/MI_AnimeGround")
mi_noren         = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")

# ─── 3. AUTHORITATIVE LIGHTING & ATMOSPHERE (STRICT VF-004 & VF-005 COMPLIANCE)
# SkyAtmosphere: Azure anime sky
sky_atm = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
if sky_atm:
    sky_atm.set_actor_label("SkyAtmosphere_KonohaAnime")
    sac = sky_atm.get_component_by_class(unreal.SkyAtmosphereComponent)
    if sac:
        try:
            sac.set_editor_property("rayleigh_scattering", unreal.LinearColor(0.12, 0.30, 0.75, 1.0))
        except Exception:
            pass

# Directional Sunlight: Warm ~5400K afternoon sunlight, INTENSITY = 8.0 LUX (NEVER 65000 LUX per VF-005!)
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 4000),
    unreal.Rotator(-40, 55, 0)
)
if sun:
    sun.set_actor_label("DirLight_Konoha_Sun")
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 8.0)
        sc.set_editor_property("light_color", unreal.Color(255, 244, 226, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        sc.set_editor_property("atmosphere_sun_light_index", 0)
        try:
            sc.set_editor_property("dynamic_shadow_distance_movable_light", 15000.0)
        except Exception:
            pass

# SkyLight with Real-Time Capture (Soft azure shadow fill to prevent crushed black silhouettes)
skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1500))
if skylight:
    skylight.set_actor_label("SkyLight_AnimeAzureFill")
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 2.0)
        slc.set_editor_property("light_color", unreal.Color(195, 225, 255, 255))
        slc.set_editor_property("real_time_capture", True)
        try:
            slc.set_editor_property("lower_hemisphere_color", unreal.LinearColor(0.60, 0.50, 0.40, 1.0))
        except Exception:
            pass

# Exponential Height Fog (Soft distant horizon haze, keeping the 50m street clear)
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

# Post Process Volume: Locked manual exposure (EV 10.0), rich anime saturation, controlled bloom
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

# ─── 4. GROUND & CENTRAL COBBLESTONE AVENUE ──────────────────────────────────
ground = spawn_any_mesh("/Engine/BasicShapes/Plane", [0, 1800, -2], scale=(80, 80, 1), mat=mi_road, label="Ground_BasePlaza")

# Paved Road Tiles along Central Avenue (50m span from Y = -800 to Y = 4000)
for i in range(13):
    y_pos = (i - 2) * 400
    spawn_any_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [0, y_pos, 0], rot=(0, 0, 0), scale=(1.25, 1.25, 1.0), mat=mi_road, label=f"Road_Avenue_{i:02d}")

# Sidewalk cobblestone margins (Left & Right curbs)
for i in range(13):
    y_pos = (i - 2) * 400
    spawn_any_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [-340, y_pos, 4], rot=(0, 0, 0), scale=(0.7, 1.25, 1.0), mat=mi_road, label=f"Sidewalk_Left_{i:02d}")
    spawn_any_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [340, y_pos, 4], rot=(0, 0, 0), scale=(0.7, 1.25, 1.0), mat=mi_road, label=f"Sidewalk_Right_{i:02d}")

# ─── 5. BUILDINGS (7 AUTHORITATIVE PRODUCTION BUILDINGS) ─────────────────────
# [1] Ichiraku Ramen (Left Fore-to-Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
               [-480, 320, 0], rot=(0, 85, 0), scale=(1.15, 1.15, 1.15), mat=mi_ichiraku_wood, label="Bld_IchirakuRamen")

# [2] Konoha Residence A (Right Fore-to-Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
               [490, 240, 0], rot=(0, -95, 0), scale=(1.1, 1.1, 1.1), mat=mi_residence_a, label="Bld_KonohaResidence_A")

# [3] Konoha Residence B (Right Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
               [520, 950, 0], rot=(0, -90, 0), scale=(1.1, 1.1, 1.1), mat=mi_residence_b, label="Bld_KonohaResidence_B")

# [4] Konoha Weapon Shop (Left Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",
               [-500, 1050, 0], rot=(0, 90, 0), scale=(1.1, 1.1, 1.1), mat=mi_ichiraku_wood, label="Bld_KonohaWeaponShop")

# [5] Konoha Mission Office (Left Further Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice",
               [-500, 1800, 0], rot=(0, 90, 0), scale=(1.1, 1.1, 1.1), mat=mi_residence_a, label="Bld_KonohaMissionOffice")

# [6] Konoha Tea House (Right Further Midground)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",
               [510, 1750, 0], rot=(0, -90, 0), scale=(1.1, 1.1, 1.1), mat=mi_ichiraku_wood, label="Bld_KonohaTeaHouse")

# [7] Hokage Mansion Pagoda (Mid-Background Center Framing)
spawn_any_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
               [0, 3500, 50], rot=(0, 0, 0), scale=(1.35, 1.35, 1.35), mat=mi_hokage_mansion, label="Bld_HokageMansion_MidVista")

# ─── 6. LANDMARKS (HOKAGE MONUMENT & FRONTIER GATE) ──────────────────────────
# [1] 3D Hokage Monument Cliff Faces (Towering on Central Horizon)
spawn_any_mesh("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
               [0, 4800, 500], rot=(0, 0, 0), scale=(4.5, 4.5, 4.5), mat=mi_hokage_rock, label="Lnd_HokageFaces_3DMonument")

# [2] Master North Star Vista Backdrop Plate (Behind Hokage Monument, zero black void)
spawn_any_mesh("/Engine/BasicShapes/Plane",
               [0, 5300, 1500], rot=(90, 0, -90), scale=(110, 50, 1), mat=mi_backdrop,
               label="Backdrop_Konoha_NorthStar_Vista", no_collision=True)

# [3] Konoha Frontier Gate (Grand Red Torii at South Entrance)
spawn_any_mesh("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate",
               [0, -900, 0], rot=(0, 0, 0), scale=(1.4, 1.4, 1.4), mat=mi_torii_red, label="Lnd_KonohaFrontierGate")

# ─── 7. FOLIAGE & VEGETATION (5 TYPES) ───────────────────────────────────────
# [1] Large Konoha Cedars
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [-620, -50, 0], rot=(0, 25, 0), scale=(1.3, 1.3, 1.3), mat=mi_cedar, label="Veg_KonohaCedar_Left_01")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [-640, 1450, 0], rot=(0, 60, 0), scale=(1.4, 1.4, 1.4), mat=mi_cedar, label="Veg_KonohaCedar_Left_02")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [620, 600, 0], rot=(0, 110, 0), scale=(1.35, 1.35, 1.35), mat=mi_cedar, label="Veg_KonohaCedar_Right_01")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [640, 2200, 0], rot=(0, 45, 0), scale=(1.5, 1.5, 1.5), mat=mi_cedar, label="Veg_KonohaCedar_Right_02")

# [2] Japanese Cedar Trees (Deep perimeter)
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large",
               [-680, 700, 0], rot=(0, 80, 0), scale=(1.2, 1.2, 1.2), mat=mi_cedar, label="Veg_Cedar_Left_Deep")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large",
               [680, 1400, 0], rot=(0, 15, 0), scale=(1.25, 1.25, 1.25), mat=mi_cedar, label="Veg_Cedar_Right_Deep")

# [3] Sakura Cherry Blossom Trees (Vibrant pink accents)
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [460, -60, 0], rot=(0, 30, 0), scale=(1.15, 1.15, 1.15), mat=mi_sakura, label="Veg_Sakura_Bloom_FrontRight")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [-440, -120, 0], rot=(0, 75, 0), scale=(1.1, 1.1, 1.1), mat=mi_sakura, label="Veg_Sakura_Bloom_FrontLeft")
spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [490, 2400, 0], rot=(0, 150, 0), scale=(1.2, 1.2, 1.2), mat=mi_sakura, label="Veg_Sakura_Bloom_RearRight")

# [4] Ground Shrubs (Along building margins)
for sy in [-50, 200, 700, 1300, 2000]:
    spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01",
                   [-380, sy, 0], rot=(0, (sy * 17) % 360, 0), scale=(1.1, 1.1, 1.0), mat=mi_foliage, label=f"Veg_Shrub_Left_{sy}")
    spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01",
                   [380, sy, 0], rot=(0, (sy * 23) % 360, 0), scale=(1.1, 1.1, 1.0), mat=mi_foliage, label=f"Veg_Shrub_Right_{sy}")

# [5] Grass Tufts (Stylized patches near trees and curbs)
for gy in [50, 450, 900, 1600]:
    spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized",
                   [-360, gy, 0], rot=(0, gy % 180, 0), scale=(1.2, 1.2, 1.2), mat=mi_ground, label=f"Veg_GrassTuft_Left_{gy}")
    spawn_any_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized",
                   [360, gy, 0], rot=(0, (gy * 2) % 180, 0), scale=(1.2, 1.2, 1.2), mat=mi_ground, label=f"Veg_GrassTuft_Right_{gy}")

# ─── 8. PROPS & STREET LIFE (11 AUTHORITATIVE PROP TYPES) ────────────────────
# [1] Hanging Red Paper Lanterns (Warm amber glow)
for ly in [180, 320, 460]:
    spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
                   [-335, ly, 185], rot=(0, 0, 0), scale=(1.2, 1.2, 1.2), mat=mi_lantern_glow,
                   label=f"Lantern_Ichiraku_{ly}", no_collision=True)
for ly in [160, 300, 440]:
    spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
                   [360, ly, 185], rot=(0, 0, 0), scale=(1.2, 1.2, 1.2), mat=mi_lantern_glow,
                   label=f"Lantern_Residence_{ly}", no_collision=True)

# [2] Traditional Stone Garden Lanterns
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [-240, 100, 0], rot=(0, 35, 0), scale=(1.1, 1.1, 1.1), mat=mi_lantern_stone, label="Prop_StoneLantern_Left_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [240, 100, 0], rot=(0, -35, 0), scale=(1.1, 1.1, 1.1), mat=mi_lantern_stone, label="Prop_StoneLantern_Right_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [-240, 1500, 0], rot=(0, 45, 0), scale=(1.1, 1.1, 1.1), mat=mi_lantern_stone, label="Prop_StoneLantern_Left_02")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [240, 1500, 0], rot=(0, -45, 0), scale=(1.1, 1.1, 1.1), mat=mi_lantern_stone, label="Prop_StoneLantern_Right_02")

# [3] Wooden Benches
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
               [280, 450, 0], rot=(0, -90, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_Bench_Right_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
               [-280, 800, 0], rot=(0, 90, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_Bench_Left_01")

# [4] Wooden Rain Barrels
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood",
               [320, 560, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_RainBarrel_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood",
               [-330, 580, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_RainBarrel_02")

# [5] Shinobi Supply Crates
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate",
               [320, 620, 0], rot=(0, 15, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_SupplyCrate_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate",
               [320, 620, 50], rot=(0, -10, 0), scale=(0.85, 0.85, 0.85), mat=mi_wood, label="Prop_SupplyCrate_02")

# [6] Ichiraku Shop Fabric Banner
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku",
               [-300, 200, 0], rot=(0, 85, 0), scale=(1.2, 1.2, 1.2), mat=mi_noren, label="Prop_ShopBanner_Ichiraku")

# [7] Wooden Utility Poles with Overhead Wires
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
               [340, 50, 0], rot=(0, 0, 0), scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_UtilityPole_Right_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
               [-340, 650, 0], rot=(0, 0, 0), scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_UtilityPole_Left_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
               [340, 1300, 0], rot=(0, 0, 0), scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_UtilityPole_Right_02")

# [8] Water Pumping Station
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WaterPumpingStation",
               [-320, 900, 0], rot=(0, 90, 0), scale=(1.0, 1.0, 1.0), mat=mi_metal, label="Prop_WaterPumpingStation")

# [9] Village Trash Bins
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin",
               [310, 380, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_TrashBin_Right")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin",
               [-310, 750, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_TrashBin_Left")

# [10] Training Ground Log Posts
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
               [260, 2100, 0], rot=(0, 20, 0), scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_TrainingLog_01")
spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
               [310, 2140, 0], rot=(0, -15, 0), scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_TrainingLog_02")

# [11] Training Dummy (Target post / practice dummy)
dummy_bp_class = unreal.load_class(None, "/Game/Environment/Konoha/Props/Meshes/BP_TrainingDummy.BP_TrainingDummy_C")
if dummy_bp_class:
    actor_dummy = unreal.EditorLevelLibrary.spawn_actor_from_class(dummy_bp_class, unreal.Vector(285, 2120, 0), unreal.Rotator(0, 180, 0))
    if actor_dummy:
        actor_dummy.set_actor_label("BP_TrainingDummy_Practice")
else:
    spawn_any_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
                   [285, 2120, 0], rot=(0, 180, 0), scale=(1.2, 1.2, 1.2), mat=mi_wood, label="Prop_TrainingDummy_Post")

# ─── 9. VILLAGE LIFE & NPCS ──────────────────────────────────────────────────
mi_teuchi_apron  = unreal.load_asset("/Game/Characters/NPCs/Shopkeepers/Materials/MI_Teuchi_Apron")
mi_kakashi_vest  = unreal.load_asset("/Game/Characters/Kakashi/Materials/MI_Kakashi_Vest")

# Chef Teuchi inside Ichiraku behind counter
spawn_any_mesh("/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi",
               [-420, 320, 20], rot=(0, 85, 0), scale=(1.0, 1.0, 1.0), mat=mi_teuchi_apron, label="NPC_Teuchi_Chef")

# Chunin Patrol Guard
spawn_any_mesh("/Game/Characters/NPCs/Guards/Meshes/SK_Chunin_GateGuard",
               [80, 600, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0), mat=mi_kakashi_vest, label="NPC_Chunin_Patrol")

# ─── 10. PLAYER START ────────────────────────────────────────────────────────
ps = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0, 0, 95), unreal.Rotator(0, 0, 0))
if ps:
    ps.set_actor_label("PlayerStart_GoldenView")

# Save Level
unreal.EditorLoadingAndSavingUtils.save_current_level()
unreal.log("======================================================================")
unreal.log("  PHASE 6: 50M KONOHA GOLDEN STREET SUCCESSFULLY ASSEMBLED & SAVED!")
unreal.log("======================================================================")
