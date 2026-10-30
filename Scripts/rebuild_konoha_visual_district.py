"""
NARUTO: ECHOES OF SHINOBI - Master Konoha District Visual Reconstruction
Rebuilds L_Konoha_Phase13_District.umap with:
- Dedicated Golden-Hour Directional Sun (ForwardShadingPriority=1, no warnings)
- Soft Azure SkyLight (Real-time capture, bright anime shadow fill)
- Sky Atmosphere & Warm Horizon Exponential Height Fog
- Unbound PostProcessVolume with Fixed Manual Exposure & Vibrant Anime Saturation
- Warm Cobblestone Plaza & Central Avenue
- Beautifully Scaled & Textured Ichiraku Ramen Shop, Hokage Monument, and Residences
- Golden Glowing Lanterns, Cedar Pines, and Cherry Blossom Trees
- Hero PlayerStart facing down the avenue toward the Hokage Monument
"""
import unreal

EAL = unreal.EditorAssetLibrary
AT  = unreal.AssetToolsHelpers.get_asset_tools()
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)

LEVEL_PKG = "/Game/Maps/Konoha/L_Konoha_Phase13_District"
unreal.log(f"=== Reconstructing {LEVEL_PKG} ===")

EAL.make_directory("/Game/Maps/Konoha")
LES.new_level(LEVEL_PKG)
world = LES.get_world()

def load_mat(path):
    m = unreal.load_asset(path)
    if not m:
        unreal.log_warning(f"Material not found: {path}")
    return m

# Load Anime Materials
mi_ichiraku_wood  = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_ichiraku_roof  = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_ichiraku_noren = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
mi_residence_a    = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
mi_residence_b    = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
mi_hokage_mansion = load_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")
mi_road           = load_mat("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_hokage_rock    = load_mat("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
mi_torii_red      = load_mat("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
mi_cedar          = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
mi_sakura         = load_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
mi_lantern_stone  = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
mi_lantern_glow   = load_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")

# ─── 1. SUN & ATMOSPHERE LIGHTING (Golden Hour Anime Look) ────────────────────
sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.DirectionalLight,
    unreal.Vector(0, 0, 3000),
    unreal.Rotator(-32, 40, 0)
)
if sun:
    sun.set_actor_label("DirLight_Konoha_Sun")
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 12.0)
        sc.set_editor_property("light_color", unreal.Color(255, 238, 210, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        sc.set_editor_property("forward_shading_priority", 1)

# Sky Atmosphere
unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))

# Sky Light (Soft Azure Shadow Fill so shadows are colorful and bright!)
skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1000))
if skylight:
    skylight.set_actor_label("SkyLight_AnimeFill")
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 2.2)
        slc.set_editor_property("light_color", unreal.Color(185, 215, 255, 255))
        slc.set_editor_property("real_time_capture", True)

# Exponential Height Fog (Warm Sunset Horizon Haze)
fog = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0))
if fog:
    fog.set_actor_label("Fog_SunsetHaze")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    if fc:
        fc.set_editor_property("fog_density", 0.002)
        fc.set_editor_property("fog_inscattering_color", unreal.LinearColor(1.0, 0.88, 0.76, 1.0))

# Post Process Volume (Locked Manual Exposure & Vibrant Anime Saturation)
pp_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
if pp_actor:
    pp_actor.set_actor_label("PostProcess_AnimeCelGrade")
    pp_actor.set_editor_property("unbound", True)
    pp_comp = pp_actor.get_component_by_class(unreal.PostProcessComponent)
    if pp_comp:
        s = pp_comp.get_editor_property("settings")
        # Fix exposure to 1.0 to prevent camera blowout
        s.set_editor_property("auto_exposure_min_brightness", 1.0)
        s.set_editor_property("auto_exposure_max_brightness", 1.0)
        s.set_editor_property("color_saturation", unreal.Vector4(1.2, 1.2, 1.2, 1.0))
        s.set_editor_property("color_contrast", unreal.Vector4(1.06, 1.06, 1.06, 1.0))
        s.set_editor_property("bloom_intensity", 0.5)
        pp_comp.set_editor_property("settings", s)

# ─── 2. GROUND PLAZA (500m scale with Warm Stone Texture) ─────────────────────
ground = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.StaticMeshActor, unreal.Vector(0, 0, -2)
)
if ground:
    ground.set_actor_label("Ground_KonohaDistrictPlaza")
    mesh = unreal.load_asset("/Engine/BasicShapes/Plane")
    if mesh:
        ground.static_mesh_component.set_static_mesh(mesh)
    if mi_road:
        ground.static_mesh_component.set_material(0, mi_road)
    ground.set_actor_scale3d(unreal.Vector(500, 500, 1))

# Helper to spawn and texture static mesh actors
def spawn_prop(mesh_pkg, loc, rot=(0,0,0), scale=(1,1,1), label="Prop", mat=None):
    mesh = unreal.load_asset(mesh_pkg)
    if not mesh:
        unreal.log_warning(f"Mesh not found: {mesh_pkg}")
        return None
    a = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.StaticMeshActor,
        unreal.Vector(*loc),
        unreal.Rotator(*rot)
    )
    if a:
        a.set_actor_label(label)
        a.static_mesh_component.set_static_mesh(mesh)
        a.set_actor_scale3d(unreal.Vector(*scale))
        if mat:
            num = max(a.static_mesh_component.get_num_materials(), 1)
            for i in range(num):
                a.static_mesh_component.set_material(i, mat)
    return a

# ─── 3. LANDMARKS ─────────────────────────────────────────────────────────────
# Great Konoha Frontier Torii Gate (At south entrance)
spawn_prop("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate",
           [0, 3200, 0], rot=(0, 0, 0), scale=(1.8, 1.8, 1.8),
           label="Lnd_GreatKonohaGate", mat=mi_torii_red)

# Towering Hokage Rock Monument Vista (Majestic North Horizon)
spawn_prop("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
           [0, -5500, 300], rot=(0, 0, 0), scale=(16.0, 45.0, 18.0),
           label="Lnd_HokageRockMonument", mat=mi_hokage_rock)

# Hokage Administrative Mansion (Circular Red Facade at base of cliff)
spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
           [0, -3200, 0], rot=(0, 0, 0), scale=(2.2, 2.2, 2.2),
           label="Bld_HokageMansion", mat=mi_hokage_mansion)

# ─── 4. MAIN COMMERCIAL AVENUE & ICHIRAKU RAMEN ───────────────────────────────
# Ichiraku Ramen Stall (Prime position on Avenue)
ichiraku = spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
                      [-550, -800, 0], rot=(0, 90, 0), scale=(1.2, 1.2, 1.2),
                      label="Bld_IchirakuRamen", mat=mi_ichiraku_wood)

# Higurashi Weapon Shop (Across from Ichiraku)
spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",
           [550, -800, 0], rot=(0, -90, 0), scale=(1.2, 1.2, 1.2),
           label="Bld_WeaponShop_Higurashi", mat=mi_ichiraku_wood)

# Dango Tea House & Outdoor Seating
spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",
           [-550, 300, 0], rot=(0, 90, 0), scale=(1.1, 1.1, 1.1),
           label="Bld_TeaHouse_Dango", mat=mi_ichiraku_wood)

# Shinobi Mission Dispatch Office
spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice",
           [550, 300, 0], rot=(0, -90, 0), scale=(1.3, 1.3, 1.3),
           label="Bld_ShinobiMissionOffice", mat=mi_residence_a)

# Traditional Japanese Residences flanking Avenue
for idx, y_pos in enumerate([-1800, 1200, 2000]):
    spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
               [-550, y_pos, 0], rot=(0, 90, 0), scale=(1.2, 1.2, 1.2),
               label=f"Bld_Residence_West_{idx:02d}", mat=mi_residence_a)
    spawn_prop("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
               [550, y_pos, 0], rot=(0, -90, 0), scale=(1.2, 1.2, 1.2),
               label=f"Bld_Residence_East_{idx:02d}", mat=mi_residence_b)

# ─── 5. CENTRAL AVENUE COBBLESTONE PAVERS ─────────────────────────────────────
for i in range(16):
    y = (i - 6) * 400
    spawn_prop("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
               [0, y, 2], rot=(0, 0, 0), scale=(1.5, 1.0, 1.0),
               label=f"Road_Cobblestone_{i:02d}", mat=mi_road)

# ─── 6. STONE LANTERNS WITH WARM GOLDEN AMBER GLOW ────────────────────────────
for y in [-2000, -1200, -400, 400, 1200, 2000, 2800]:
    for x_side in [-320, 320]:
        spawn_prop("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
                   [x_side, y, 0], rot=(0, 0, 0), scale=(1.0, 1.0, 1.0),
                   label=f"Lantern_X{x_side}_Y{y}", mat=mi_lantern_stone)
        # Warm golden point light
        light = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.PointLight, unreal.Vector(x_side, y, 160)
        )
        if light:
            light.set_actor_label(f"Light_Lantern_X{x_side}_Y{y}")
            plc = light.get_component_by_class(unreal.PointLightComponent)
            if plc:
                plc.set_editor_property("intensity", 800.0)
                plc.set_editor_property("attenuation_radius", 450.0)
                plc.set_editor_property("light_color", unreal.Color(255, 175, 75, 255))
                plc.set_editor_property("cast_shadows", False)

# ─── 7. FOLIAGE (Lush Anime Cedars & Cherry Blossom Petals) ───────────────────
for idx, (fx, fy) in enumerate([(-750, -1000), (750, -1000), (-750, 600), (750, 600), (-800, -2200), (800, 2400)]):
    spawn_prop("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [fx, fy, 0], rot=(0, idx*45, 0), scale=(1.4, 1.4, 1.4),
               label=f"Foliage_Cedar_{idx:02d}", mat=mi_cedar)

for idx, (sx, sy) in enumerate([(-420, -600), (420, -300), (-420, 900), (420, 1600)]):
    spawn_prop("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [sx, sy, 0], rot=(0, idx*60, 0), scale=(1.2, 1.2, 1.2),
               label=f"Foliage_Sakura_{idx:02d}", mat=mi_sakura)

# ─── 8. HERO PLAYER START ─────────────────────────────────────────────────────
# Positioned right in the center of the avenue, looking north toward Ichiraku & Hokage Rock!
pstart = unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PlayerStart,
    unreal.Vector(0, -100, 60),
    unreal.Rotator(0, -90, 0)
)
if pstart:
    pstart.set_actor_label("PlayerStart_AvenueHero")

# ─── 9. SAVE LEVEL ────────────────────────────────────────────────────────────
unreal.EditorLoadingAndSavingUtils.save_current_level()
unreal.log("=== L_Konoha_Phase13_District RECONSTRUCTED & SAVED SUCCESSFULLY ===")
