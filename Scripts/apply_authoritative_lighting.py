import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

actors = eas.get_all_level_actors()

# 1. Authoritative Directional Sun (Controlled 10.0 lux warm 5400K daylight key)
sun = None
for a in actors:
    if "dirlight" in a.get_actor_label().lower():
        sun = a
        break

if not sun:
    sun = eas.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 3000))
    sun.set_actor_label("DirLight_Konoha_Sun")

if sun:
    sun.set_actor_location(unreal.Vector(0, 0, 3000), False, False)
    # Pitch -35 shines down onto the street and buildings, Yaw 45 casts directional shadows
    sun.set_actor_rotation(unreal.Rotator(pitch=-35.0, yaw=45.0, roll=0.0), False)
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 10.0)
        sc.set_editor_property("light_color", unreal.Color(255, 242, 218, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        sc.set_editor_property("atmosphere_sun_light_index", 0)
        unreal.log("  [SUN] Configured DirLight_Konoha_Sun (pitch=-35, yaw=45, warm golden 5400K, intensity=10.0)")

# 2. SkyLight with Soft Azure Fill (Guarantees bright, colorful shadows — no black void!)
skylight = None
for a in actors:
    if "skylight" in a.get_actor_label().lower():
        skylight = a
        break

if not skylight:
    skylight = eas.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 1500))
    skylight.set_actor_label("SkyLight_AnimeAzureFill")

if skylight:
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 4.0)
        slc.set_editor_property("light_color", unreal.Color(180, 220, 255, 255))
        slc.set_editor_property("cast_shadows", True)
        slc.set_editor_property("real_time_capture", True)
        unreal.log("  [SKYLIGHT] Configured SkyLight_AnimeAzureFill (intensity=4.0, azure fill)")

# 3. SkyAtmosphere setup (Vibrant blue anime sky)
sky_atmo = None
for a in actors:
    if isinstance(a, unreal.SkyAtmosphere):
        sky_atmo = a
        break

if not sky_atmo:
    sky_atmo = eas.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))
    sky_atmo.set_actor_label("SkyAtmosphere_KonohaAnime")

if sky_atmo:
    sac = sky_atmo.get_component_by_class(unreal.SkyAtmosphereComponent)
    if sac:
        try:
            sac.set_editor_property("rayleigh_scattering", unreal.LinearColor(0.15, 0.35, 0.85, 1.0))
        except Exception:
            pass
        unreal.log("  [SKYATMO] Configured SkyAtmosphere_KonohaAnime")

# 4. Fog: Controlled subtle warm haze (no blinding milky blowout)
for a in actors:
    if isinstance(a, unreal.ExponentialHeightFog):
        fc = a.get_component_by_class(unreal.ExponentialHeightFogComponent)
        if fc:
            try:
                fc.set_editor_property("fog_density", 0.001)
            except Exception:
                pass
            unreal.log("  [FOG] Configured subtle anime haze (density=0.001)")

# 5. Clean up any leftover blocking geometry
for a in actors:
    label = a.get_actor_label().lower()
    if "skysphere" in label or "backdrop" in label:
        eas.destroy_actor(a)
        unreal.log(f"  [CLEANUP] Destroyed {a.get_actor_label()}")

# 6. Production Hokage Faces Monument Backdrop
monument_mesh = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument")
monument_mat = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")

monument = None
for a in actors:
    if "hokagefaces" in a.get_actor_label().lower():
        monument = a
        break

if not monument and monument_mesh:
    monument = eas.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 4200, 350))
    monument.set_actor_label("Lnd_HokageFaces_Monument_Vista")

if monument:
    monument.set_actor_location(unreal.Vector(0, 4200, 350), False, False)
    monument.set_actor_rotation(unreal.Rotator(pitch=0, yaw=-90, roll=0), False)
    monument.set_actor_scale3d(unreal.Vector(12.0, 12.0, 9.0))
    if monument_mat:
        for idx in range(monument.static_mesh_component.get_num_materials()):
            monument.static_mesh_component.set_material(idx, monument_mat)
    unreal.log("  [MONUMENT] Configured Hokage Monument at (0, 4200, 350)")

# 7. Hokage Mansion Pagoda scaling and alignment
mansion_mat = unreal.load_asset("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")
for a in actors:
    if "hokagemansion" in a.get_actor_label().lower():
        a.set_actor_location(unreal.Vector(0, 2600, 0), False, False)
        a.set_actor_scale3d(unreal.Vector(3.2, 3.2, 3.2))
        if mansion_mat:
            for idx in range(a.static_mesh_component.get_num_materials()):
                a.static_mesh_component.set_material(idx, mansion_mat)
        unreal.log("  [MANSION] Aligned Hokage Mansion pagoda at (0, 2600, 0)")

# 8. PostProcess Volume settings (Controlled Histogram Exposure & Anime Bloom)
pp_vol = None
for a in actors:
    if isinstance(a, unreal.PostProcessVolume):
        pp_vol = a
        break

if pp_vol:
    pp_vol.unbound = True
    s = pp_vol.settings
    s.set_editor_property("override_auto_exposure_method", True)
    s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
    s.set_editor_property("override_auto_exposure_bias", True)
    s.set_editor_property("auto_exposure_bias", 0.2)
    s.set_editor_property("override_auto_exposure_min_brightness", True)
    s.set_editor_property("auto_exposure_min_brightness", 0.05)
    s.set_editor_property("override_auto_exposure_max_brightness", True)
    s.set_editor_property("auto_exposure_max_brightness", 2.0)
    s.set_editor_property("override_bloom_intensity", True)
    s.set_editor_property("bloom_intensity", 0.2)
    unreal.log("  [POSTPROCESS] Configured PostProcess_GoldenStreet: Histogram EV, Bloom 0.2")

# Save level
unreal.EditorLoadingAndSavingUtils.save_map(world, '/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
unreal.log(">>> L_Konoha_GoldenStreet AUTHORITATIVE LIGHTING & VISTA SAVED SUCCESSFULLY <<<")
