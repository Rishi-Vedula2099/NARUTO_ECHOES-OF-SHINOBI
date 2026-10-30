"""
NARUTO: ECHOES OF SHINOBI
Authoritative Golden Street Reconstruction & Visual Rescue
==========================================================
Rebuilds /Game/Maps/GoldenTest/L_Konoha_GoldenStreet:
1. Strict keyword Rotators: unreal.Rotator(pitch=0.0, yaw=yaw, roll=0.0)
   ELIMINATING ALL TILT AND INVERSION DEFECTS.
2. Clears street central axis:
   - Hokage Mansion positioned at mid-vista (Y=3000, Z=0) upright at proper scale.
   - Hokage Faces Monument cliff positioned on horizon (Y=4800, Z=400) upright.
   - Paved cobblestone central avenue continuous and unobstructed.
3. Upright architecture:
   - West sidewalk: Ichiraku Ramen stand with fabric noren curtains and amber lanterns.
   - East sidewalk: Konoha Timber Residences A & B with second-floor balconies.
   - Stone Toro lanterns placed upright on curbs at normal human scale.
   - Cedar and flowering Sakura trees flanking the outer perimeter.
4. Clean Anime Cel Lighting:
   - 5400K warm golden sunlight (10.0 lux, pitch -35, yaw 45).
   - Azure SkyLight fill (4.0 lux).
   - Crisp atmosphere (fog density 0.0005, start 1500).
   - Histogram auto-exposure calibrated (min 0.05, max 2.0, bias 0.2).
5. PlayerStart_GoldenView at (0, 0, 95) facing down avenue (+Y).
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
EAL = unreal.EditorAssetLibrary

def build_golden_street():
    unreal.log("======================================================================")
    unreal.log("  REBUILDING L_Konoha_GoldenStreet — AUTHORITATIVE VISUAL RESCUE")
    unreal.log("======================================================================")

    # 1. Load map and clear all old actors
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    if not world:
        LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
        world = LES.new_level(MAP_PKG)

    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    for a in eas.get_all_level_actors():
        if a:
            eas.destroy_actor(a)
    unreal.log("  [CLEANUP] Destroyed all legacy level actors.")

    # 2. Material References
    mi_road          = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
    mi_ichiraku_wood = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
    mi_residence_a   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
    mi_residence_b   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
    mi_noren         = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren")
    mi_hokage_mansion= unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")
    mi_hokage_rock   = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
    mi_torii_red     = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
    mi_cedar         = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
    mi_sakura        = unreal.load_asset("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
    mi_lantern_stone = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")
    mi_lantern_glow  = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")
    mi_wood          = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")

    def spawn_mesh(mesh_path, loc, yaw=0.0, pitch=0.0, roll=0.0, scale=(1,1,1), mat=None, label="Actor", no_collision=False):
        asset = unreal.load_asset(mesh_path)
        if not asset:
            unreal.log_warning(f"Missing mesh: {mesh_path}")
            return None
        
        rot = unreal.Rotator(pitch=float(pitch), yaw=float(yaw), roll=float(roll))
        actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,
            unreal.Vector(float(loc[0]), float(loc[1]), float(loc[2])),
            rot
        )
        if actor:
            smc = actor.static_mesh_component
            smc.set_static_mesh(asset)
            actor.set_actor_scale3d(unreal.Vector(float(scale[0]), float(scale[1]), float(scale[2])))
            actor.set_actor_label(label)
            if no_collision:
                smc.set_collision_profile_name("NoCollision")
                smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            if mat:
                for idx in range(max(smc.get_num_materials(), 1)):
                    smc.set_material(idx, mat)
        return actor

    # 3. ATMOSPHERE & LIGHTING (Warm daylight anime baseline)
    # SkyAtmosphere
    sky_atm = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0,0,0))
    if sky_atm:
        sky_atm.set_actor_label("SkyAtmosphere_KonohaAnime")
        sac = sky_atm.get_component_by_class(unreal.SkyAtmosphereComponent)
        if sac:
            try:
                sac.set_editor_property("rayleigh_scattering", unreal.LinearColor(0.12, 0.32, 0.78, 1.0))
            except Exception:
                pass

    # Directional Sun (5400K warm daylight, down onto street at pitch -35°, yaw 45°)
    sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.DirectionalLight,
        unreal.Vector(0, 0, 4000),
        unreal.Rotator(pitch=-35.0, yaw=45.0, roll=0.0)
    )
    if sun:
        sun.set_actor_label("DirLight_Konoha_Sun")
        sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
        if sc:
            sc.set_editor_property("intensity", 10.0)
            sc.set_editor_property("light_color", unreal.Color(255, 242, 218, 255))
            sc.set_editor_property("cast_shadows", True)
            sc.set_editor_property("atmosphere_sun_light", True)
            sc.set_editor_property("atmosphere_sun_light_index", 0)

    # SkyLight (Soft azure fill)
    skylight = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1500))
    if skylight:
        skylight.set_actor_label("SkyLight_AnimeAzureFill")
        slc = skylight.get_component_by_class(unreal.SkyLightComponent)
        if slc:
            slc.set_editor_property("intensity", 4.0)
            slc.set_editor_property("light_color", unreal.Color(180, 220, 255, 255))
            slc.set_editor_property("real_time_capture", True)

    # Height Fog (Subtle distant haze, crisp avenue)
    fog = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0))
    if fog:
        fog.set_actor_label("Fog_KonohaSunsetHaze")
        fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
        if fc:
            try:
                fc.set_editor_property("fog_density", 0.0005)
                fc.set_editor_property("start_distance", 1500.0)
                fc.set_editor_property("fog_inscattering_luminance", unreal.LinearColor(0.85, 0.90, 1.0, 1.0))
            except Exception:
                pass

    # Post Process Volume (Histogram exposure, controlled bloom)
    pp = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
    if pp:
        pp.set_actor_label("PostProcess_GoldenStreet")
        pp.set_editor_property("unbound", True)
        ppc = pp.get_component_by_class(unreal.PostProcessComponent)
        if ppc:
            s = ppc.get_editor_property("settings")
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.05)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 2.0)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.2)
            s.set_editor_property("override_bloom_intensity", True)
            s.set_editor_property("bloom_intensity", 0.15)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.10, 1.10, 1.10, 1.0))
            ppc.set_editor_property("settings", s)

    # 4. GROUND & CENTRAL AVENUE COBBLESTONES (50m span)
    spawn_mesh("/Engine/BasicShapes/Plane", [0, 1800, -2], yaw=0.0, scale=(80, 80, 1), mat=mi_road, label="Ground_BasePlaza")

    for i in range(13):
        y_pos = (i - 2) * 400
        # Central avenue cobblestones
        spawn_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [0, y_pos, 0], yaw=0.0, scale=(1.25, 1.25, 1.0), mat=mi_road, label=f"Road_Avenue_{i:02d}")
        # Left sidewalk
        spawn_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [-340, y_pos, 4], yaw=0.0, scale=(0.7, 1.25, 1.0), mat=mi_road, label=f"Sidewalk_Left_{i:02d}")
        # Right sidewalk
        spawn_mesh("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
                   [340, y_pos, 4], yaw=0.0, scale=(0.7, 1.25, 1.0), mat=mi_road, label=f"Sidewalk_Right_{i:02d}")

    # 5. UPRIGHT ARCHITECTURE (Faced cleanly along sidewalks)
    # [A] Ichiraku Ramen (West Sidewalk, facing East into street, upright pitch=0, roll=0)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
               [-480, 300, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.15, 1.15, 1.15), mat=mi_ichiraku_wood, label="Bld_IchirakuRamen")

    # Ichiraku Fabric Banner (Noren)
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku",
               [-320, 200, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_noren, label="Prop_ShopBanner_Ichiraku")

    # Hanging Paper Lanterns on Ichiraku facade (warm amber glow)
    for ly in [180, 300, 420]:
        spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
                   [-335, ly, 185], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_lantern_glow,
                   label=f"Lantern_Ichiraku_{ly}", no_collision=True)

    # [B] Konoha Weapon Shop (West Sidewalk, further down at Y=1050)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",
               [-500, 1050, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_ichiraku_wood, label="Bld_KonohaWeaponShop")

    # [C] Konoha Mission Office (West Sidewalk, at Y=1800)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice",
               [-500, 1800, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_residence_a, label="Bld_KonohaMissionOffice")

    # [D] Konoha Residence A (East Sidewalk, upright pitch=0, roll=0)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
               [490, 240, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_residence_a, label="Bld_KonohaResidence_A")

    # Hanging Paper Lanterns on Residence A facade
    for ly in [180, 300]:
        spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging",
                   [355, ly, 185], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_lantern_glow,
                   label=f"Lantern_Residence_{ly}", no_collision=True)

    # [E] Konoha Residence B (East Sidewalk, at Y=950)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
               [520, 950, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_residence_b, label="Bld_KonohaResidence_B")

    # [F] Konoha Tea House (East Sidewalk, at Y=1750)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",
               [510, 1750, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_ichiraku_wood, label="Bld_KonohaTeaHouse")

    # [G] Hokage Mansion Pagoda (Mid-Vista at Y=3000, NOT blocking street, upright scale 1.25)
    spawn_mesh("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
               [0, 3000, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.25, 1.25, 1.25), mat=mi_hokage_mansion, label="Bld_HokageMansion_MidVista")

    # [H] 3D Hokage Monument Cliff Landmark (Horizon Central Vista at Y=4800, Z=400)
    spawn_mesh("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
               [0, 4800, 400], yaw=-90.0, pitch=0.0, roll=0.0, scale=(8.0, 8.0, 6.0), mat=mi_hokage_rock, label="Lnd_HokageFaces_3DMonument")

    # [I] Konoha Frontier Gate (Grand Red Torii at South entrance behind player)
    spawn_mesh("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate",
               [0, -900, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.4, 1.4, 1.4), mat=mi_torii_red, label="Lnd_KonohaFrontierGate")

    # 6. STREET PROPS & DETAILS (Upright, human scale)
    # Upright Traditional Stone Garden Lanterns (Curbside, scale 0.85, pitch=0, roll=0!)
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [-260, 100, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(0.85, 0.85, 0.85), mat=mi_lantern_stone, label="Prop_StoneLantern_Left_01")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [260, 100, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(0.85, 0.85, 0.85), mat=mi_lantern_stone, label="Prop_StoneLantern_Right_01")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [-260, 1500, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(0.85, 0.85, 0.85), mat=mi_lantern_stone, label="Prop_StoneLantern_Left_02")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
               [260, 1500, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(0.85, 0.85, 0.85), mat=mi_lantern_stone, label="Prop_StoneLantern_Right_02")

    # Wooden Benches
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
               [280, 480, 0], yaw=-90.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_Bench_Right_01")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
               [-280, 800, 0], yaw=90.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_Bench_Left_01")

    # Rain Barrels & Supply Crates
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood",
               [320, 560, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_RainBarrel_01")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate",
               [320, 620, 0], yaw=15.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_SupplyCrate_01")

    # Wooden Utility Poles
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
               [340, 50, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_UtilityPole_Right_01")
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
               [-340, 650, 0], yaw=0.0, pitch=0.0, roll=0.0, scale=(1.0, 1.0, 1.0), mat=mi_wood, label="Prop_UtilityPole_Left_01")

    # Training dummy
    spawn_mesh("/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
               [285, 2120, 0], yaw=180.0, pitch=0.0, roll=0.0, scale=(1.1, 1.1, 1.1), mat=mi_wood, label="Prop_TrainingDummy_Post")

    # 7. FOLIAGE (Upright cedars & flowering cherry blossoms framing outer avenue)
    # Cedars
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [-620, -50, 0], yaw=25.0, pitch=0.0, roll=0.0, scale=(1.3, 1.3, 1.3), mat=mi_cedar, label="Veg_Cedar_Left_01")
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [-640, 1450, 0], yaw=60.0, pitch=0.0, roll=0.0, scale=(1.4, 1.4, 1.4), mat=mi_cedar, label="Veg_Cedar_Left_02")
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [620, 600, 0], yaw=110.0, pitch=0.0, roll=0.0, scale=(1.35, 1.35, 1.35), mat=mi_cedar, label="Veg_Cedar_Right_01")
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
               [640, 2200, 0], yaw=45.0, pitch=0.0, roll=0.0, scale=(1.5, 1.5, 1.5), mat=mi_cedar, label="Veg_Cedar_Right_02")

    # Sakura Cherry Blossoms
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [460, -60, 0], yaw=30.0, pitch=0.0, roll=0.0, scale=(1.2, 1.2, 1.2), mat=mi_sakura, label="Veg_Sakura_FrontRight")
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [-440, -120, 0], yaw=75.0, pitch=0.0, roll=0.0, scale=(1.2, 1.2, 1.2), mat=mi_sakura, label="Veg_Sakura_FrontLeft")
    spawn_mesh("/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
               [490, 2400, 0], yaw=30.0, pitch=0.0, roll=0.0, scale=(1.3, 1.3, 1.3), mat=mi_sakura, label="Veg_Sakura_RearRight")

    # 8. PLAYER START (Centered on avenue, facing North +Y toward Hokage Monument)
    ps = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.PlayerStart,
        unreal.Vector(0.0, 0.0, 95.0),
        unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0)
    )
    if ps:
        ps.set_actor_label("PlayerStart_GoldenView")
        ps.tags.append("GoldenView")

    # 9. Save Map
    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> L_Konoha_GoldenStreet REBUILD COMPLETE & SAVED SUCCESSFULLY <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    build_golden_street()
