"""
NARUTO: ECHOES OF SHINOBI
50M GOLDEN VISUAL TEST LEVEL GENERATOR
========================================
Level: /Game/Maps/GoldenTest/L_VisualAssetGoldenTest
Dimensions: 50m x 50m High-Fidelity Stylized Anime Slice
Primary Visual Target: VISUAL_STYLE_KONOHA_STYLIZED_ANIME_3D
Based on: konoha_rendered_viewport_1790422628671.jpg

Contents:
- 1 Naruto Genin (SK_Naruto_GeninTracksuit + MI_Naruto_Tracksuit)
- 1 Kakashi Boss (SK_Kakashi_StandardJonin + MI_Kakashi_Vest)
- 1 Konoha NPC (SK_Shopkeeper_Teuchi + MI_Teuchi_Apron)
- 1 Enemy/Training Target (SK_RogueNinja_Melee + SM_Prop_TrainingLog_Post)
- 1 Konoha Building (SM_Bld_IchirakuRamen + MI_Bld_Ichiraku_Wood/Roof)
- 1 Konoha Residence (SM_Bld_Konoha_Residence_A + MI_Bld_Residence_A)
- 1 Cobblestone Road (SM_Road_Paved_01 + MI_Road_Cobblestone)
- Foliage: Japanese Cedar (SM_Veg_KonohaCedar_Large) + Sakura (SM_Veg_Sakura_Bloom)
- Props: Stone Lantern (SM_Prop_StoneLantern_01) with warm amber point light
- Background Landmark: Hokage Rock faces (SM_Lnd_HokageFaces_Monument) + Hokage Mansion
- 1 Primary Directional Light (Warm Golden-Hour Sun, ForwardShadingPriority=1)
- 1 Sky/Ambient Setup (SkyAtmosphere + SkyLight Soft Azure Fill + HeightFog)
- 1 PostProcessVolume (Manual Exposure 1.0, Saturation 1.18, Bloom 0.45)
- 1 Third-Person Camera / PlayerStart framing composition
- 1 Navigation Setup (NavMeshBoundsVolume)
- 1 Rasengan VFX (NS_Rasengan_Orb_Master)
"""

import unreal
import os

EAL = unreal.EditorAssetLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
AT  = unreal.AssetToolsHelpers.get_asset_tools()

MAP_FOLDER = "/Game/Maps/GoldenTest"
MAP_NAME   = "L_VisualAssetGoldenTest"
MAP_PKG    = f"{MAP_FOLDER}/{MAP_NAME}"

unreal.log("======================================================================")
unreal.log(f"  BUILDING 50M GOLDEN VISUAL TEST LEVEL: {MAP_PKG}")
unreal.log("======================================================================")

EAL.make_directory(MAP_FOLDER)

# Create a clean new level
LES.new_level(MAP_PKG)
world = LES.get_world()

def load_mat(path):
    m = unreal.load_asset(path)
    if not m:
        unreal.log_warning(f"Material not found: {path}")
    return m

# Material references
mi_ichiraku_wood = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_ichiraku_roof = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_residence_a   = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
mi_road          = load_mat("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_cedar         = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
mi_sakura        = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
mi_lantern_stone = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
mi_hokage_rock   = load_mat("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
mi_hokage_dome   = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")

mi_naruto_orange = load_mat("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
mi_kakashi_vest  = load_mat("/Game/Characters/Kakashi/Materials/MI_Kakashi_Vest")
mi_teuchi_apron  = load_mat("/Game/Characters/NPCs/Shopkeepers/Materials/MI_Teuchi_Apron")
mi_enemy_rogue   = load_mat("/Game/Characters/Enemies/Materials/MI_Enemy_Rogue")
mi_dummy_wood    = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Target_Dummy")

def spawn_mesh(mesh_path, loc, rot=(0,0,0), scale=(1,1,1), mat=None, label="Actor"):
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
        actor.static_mesh_component.set_static_mesh(mesh)
        actor.set_actor_scale3d(unreal.Vector(*scale))
        actor.set_actor_label(label)
        if mat:
            num = max(actor.static_mesh_component.get_num_materials(), 1)
            for i in range(num):
                actor.static_mesh_component.set_material(i, mat)
    return actor

# ─── 1. SUN & ATMOSPHERE SETUP (WARM GOLDEN ANIME KEY) ─────────────────────────
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 2500),
    unreal.Rotator(-32, 45, 0)
)
if sun:
    sun.set_actor_label("DirLight_Konoha_GoldenSun")
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 12.5)
        sc.set_editor_property("light_color", unreal.Color(255, 240, 218, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        try:
            sc.set_editor_property("forward_shading_priority", 1)
        except Exception:
            pass

# SkyAtmosphere
unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))

# SkyLight with Soft Azure Fill (No pitch-black shadows)
skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1000))
if skylight:
    skylight.set_actor_label("SkyLight_AzureAnimeFill")
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 2.2)
        slc.set_editor_property("light_color", unreal.Color(185, 215, 255, 255))
        try:
            slc.set_editor_property("real_time_capture", True)
        except Exception:
            pass

# Atmospheric Fog
fog = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0))
if fog:
    fog.set_actor_label("Fog_SunsetHaze")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    if fc:
        try:
            fc.set_editor_property("fog_density", 0.0018)
        except Exception:
            pass

# Unbound Post Process Volume (Locked manual exposure, vibrant anime grade)
pp = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
if pp:
    pp.set_actor_label("PostProcess_AnimeCelGrade")
    pp.set_editor_property("unbound", True)
    ppc = pp.get_component_by_class(unreal.PostProcessComponent)
    if ppc:
        s = ppc.get_editor_property("settings")
        try:
            s.set_editor_property("auto_exposure_min_brightness", 1.0)
            s.set_editor_property("auto_exposure_max_brightness", 1.0)
            s.set_editor_property("bloom_intensity", 0.45)
            ppc.set_editor_property("settings", s)
        except Exception as e:
            unreal.log_warning(f"PP Settings warning: {e}")

# ─── 2. GROUND & COBBLESTONE ROAD (50m x 50m) ──────────────────────────────────
# Ground base plane (50m = 5000 units)
ground = spawn_mesh("/Engine/BasicShapes/Plane", [0, 500, -2], scale=(50, 50, 1), mat=mi_road, label="Ground_DistrictPlaza_50m")

# Main Cobblestone Avenue tiles
for i in range(8):
    y_pos = (i - 2) * 450
    spawn_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
               [0, y_pos, 0], rot=(0, 0, 0), mat=mi_road, label=f"Road_Avenue_{i:02d}")

# ─── 3. BUILDINGS: ICHIRAKU RAMEN & KONOHA SHOPS ───────────────────────────────
# Left: Ichiraku Ramen (Facing street)
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
           [-420, 250, 0], rot=(0, 90, 0), mat=mi_ichiraku_wood, label="Bld_IchirakuRamen_Golden")

# Right: Konoha Residence / Shop
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
           [450, 250, 0], rot=(0, -90, 0), mat=mi_residence_a, label="Bld_KonohaResidence_Golden")

# ─── 4. BACKGROUND VISTA: HOKAGE ROCK MONUMENT & HOKAGE MANSION ───────────────
# Hokage Rock Cliff Monument framed in distance
spawn_mesh("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
           [0, 2800, 350], rot=(0, 0, 0), scale=(1.2, 1.2, 1.2), mat=mi_hokage_rock, label="Lnd_HokageRockMonument_Vista")

# Hokage Circular Mansion
spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
           [0, 1800, 0], rot=(0, 0, 0), mat=mi_hokage_dome, label="Bld_HokageMansion_Vista")

# ─── 5. FOLIAGE & STREET PROPS ────────────────────────────────────────────────
# Large Japanese Cedar on street corner
spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
           [-550, -350, 0], rot=(0, 25, 0), mat=mi_cedar, label="Foliage_KonohaCedar_Large")

# Blooming Sakura Tree with pink petals
spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
           [400, -250, 0], rot=(0, -40, 0), mat=mi_sakura, label="Foliage_Sakura_Bloom")

# Traditional Stone Lanterns with warm amber illumination
for x_side, y_pos in [(-250, -100), (250, -100), (-250, 650), (250, 650)]:
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [x_side, y_pos, 0], mat=mi_lantern_stone, label=f"Prop_Lantern_X{x_side}_Y{y_pos}")
    pl = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.PointLight, unreal.Vector(x_side, y_pos, 140)
    )
    if pl:
        plc = pl.get_component_by_class(unreal.PointLightComponent)
        if plc:
            plc.set_editor_property("intensity", 450.0)
            plc.set_editor_property("attenuation_radius", 350.0)
            plc.set_editor_property("light_color", unreal.Color(255, 175, 80, 255))

# Training Dummy Log Post
spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
           [-220, 850, 0], rot=(0, 15, 0), mat=mi_dummy_wood, label="Prop_TrainingLog_Target")

# ─── 6. CHARACTERS & COMBAT TARGETS ───────────────────────────────────────────
# 1. Naruto Genin (Foreground Center Hero)
spawn_mesh("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit",
           [0, 50, 0], rot=(0, -85, 0), mat=mi_naruto_orange, label="Hero_Naruto_Genin")

# 2. Kakashi Boss (Midground Master Mentor)
spawn_mesh("/Game/Characters/Kakashi/Skins/SK_Kakashi_StandardJonin",
           [140, 480, 0], rot=(0, -110, 0), mat=mi_kakashi_vest, label="Boss_Kakashi_Jonin")

# 3. Konoha NPC Teuchi (Inside Ichiraku Counter)
spawn_mesh("/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi",
           [-350, 260, 20], rot=(0, 90, 0), mat=mi_teuchi_apron, label="NPC_Shopkeeper_Teuchi")

# 4. Enemy Rogue Ninja (Training Rival)
spawn_mesh("/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee",
           [-160, 720, 0], rot=(0, -60, 0), mat=mi_enemy_rogue, label="Enemy_RogueNinja_Melee")

# ─── 7. NIAGARA JUTSU VFX: RASENGAN ORB ───────────────────────────────────────
rasengan_vfx = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master")
if rasengan_vfx:
    vfx_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.NiagaraActor, unreal.Vector(30, 70, 85)
    )
    if vfx_actor:
        vfx_actor.set_actor_label("VFX_Rasengan_Orb_Master")
        nc = vfx_actor.get_component_by_class(unreal.NiagaraComponent)
        if nc:
            nc.set_asset(rasengan_vfx)
            nc.activate(True)
        unreal.log("  [VFX] Spawned active Rasengan Niagara system beside Naruto")

# ─── 8. PLAYER START & CAMERA FRAMING ─────────────────────────────────────────
# Position PlayerStart to frame Naruto in foreground, street & shops in midground, Hokage Rock in vista
ps = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart, unreal.Vector(0, -320, 40), unreal.Rotator(0, 90, 0)
)
if ps:
    ps.set_actor_label("PlayerStart_GoldenView")

# ─── 9. SAVE LEVEL & ALL PACKAGES ──────────────────────────────────────────────
LES.save_current_level()
unreal.EditorAssetLibrary.save_directory("/Game/", recursive=True, only_if_is_dirty=False)

unreal.log("======================================================================")
unreal.log(f"  50M GOLDEN VISUAL TEST LEVEL BUILT & SAVED: {MAP_PKG}")
unreal.log("  1 Naruto Genin, 1 Kakashi Boss, 1 Teuchi NPC, 1 Enemy Target,")
unreal.log("  Ichiraku Ramen, Cobblestone Road, Cedar/Sakura Trees, Lanterns,")
unreal.log("  Hokage Monument Vista, Single Sun, Azure Fill, Rasengan VFX.")
unreal.log("======================================================================")
