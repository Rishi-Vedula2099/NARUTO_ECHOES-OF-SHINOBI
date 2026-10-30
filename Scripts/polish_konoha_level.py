"""
NARUTO: ECHOES OF SHINOBI - Konoha District Level Polishing Script
1. Loads L_Konoha_Phase13_District.umap
2. Removes all conflicting / duplicate directional lights
3. Establishes a single golden-hour sun (ForwardShadingPriority=1)
4. Adds / configures SkyAtmosphere, Azure SkyLight, and Height Fog
5. Adds unbound PostProcessVolume (Manual Exposure 1.0, Anime Saturation 1.2)
6. Assigns authored anime cel-shading material instances to all actors
7. Saves the polished level
"""
import unreal

EAL = unreal.EditorAssetLibrary
LEVEL_PKG = "/Game/Maps/Konoha/L_Konoha_Phase13_District"

unreal.log(f"Loading map: {LEVEL_PKG} ...")
world = unreal.EditorLoadingAndSavingUtils.load_map(LEVEL_PKG)
if not world:
    unreal.log_error(f"Failed to load map: {LEVEL_PKG}")
    exit(1)

unreal.log(f"Successfully loaded world: {world.get_name()}")

# ─── Load Material Instances ──────────────────────────────────────────────────
def get_mat(path):
    m = unreal.load_asset(path)
    if not m:
        unreal.log_warning(f"Could not load material: {path}")
    return m

mi_ichiraku_wood  = get_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood")
mi_ichiraku_roof  = get_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof")
mi_residence_a    = get_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A")
mi_residence_b    = get_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B")
mi_hokage_mansion = get_mat("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion")
mi_road           = get_mat("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
mi_hokage_rock    = get_mat("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")
mi_torii_red      = get_mat("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red")
mi_cedar          = get_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized")
mi_sakura         = get_mat("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent")
mi_lantern_stone  = get_mat("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone")

# ─── Clean up Duplicate Directional Lights & Setup Master Sun ────────────────
all_actors = unreal.EditorLevelLibrary.get_all_level_actors()
dir_lights = [a for a in all_actors if isinstance(a, unreal.DirectionalLight)]

sun = None
if dir_lights:
    sun = dir_lights[0]
    # Delete any duplicate directional lights to avoid forward shading warnings
    for extra in dir_lights[1:]:
        unreal.log(f"Deleting duplicate directional light: {extra.get_actor_label()}")
        unreal.EditorLevelLibrary.destroy_actor(extra)
else:
    sun = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.DirectionalLight,
        unreal.Vector(0, 0, 3000),
        unreal.Rotator(-32, 40, 0)
    )

if sun:
    sun.set_actor_label("DirLight_Konoha_Sun")
    sun.set_actor_rotation(unreal.Rotator(-32, 40, 0), False)
    sc = sun.get_component_by_class(unreal.DirectionalLightComponent)
    if sc:
        sc.set_editor_property("intensity", 12.0)
        sc.set_editor_property("light_color", unreal.Color(255, 238, 210, 255))
        sc.set_editor_property("cast_shadows", True)
        sc.set_editor_property("atmosphere_sun_light", True)
        try:
            sc.set_editor_property("forward_shading_priority", 1)
        except Exception:
            pass

# ─── Ensure SkyAtmosphere Exists ──────────────────────────────────────────────
sky_atmos = [a for a in all_actors if isinstance(a, unreal.SkyAtmosphere)]
if not sky_atmos:
    unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SkyAtmosphere, unreal.Vector(0, 0, 0))

# ─── Ensure SkyLight Exists with Soft Azure Fill ──────────────────────────────
sky_lights = [a for a in all_actors if isinstance(a, unreal.SkyLight)]
skylight = sky_lights[0] if sky_lights else unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.SkyLight, unreal.Vector(0, 0, 1000)
)
if skylight:
    skylight.set_actor_label("SkyLight_AnimeFill")
    slc = skylight.get_component_by_class(unreal.SkyLightComponent)
    if slc:
        slc.set_editor_property("intensity", 2.0)
        slc.set_editor_property("light_color", unreal.Color(185, 215, 255, 255))
        try:
            slc.set_editor_property("real_time_capture", True)
        except Exception:
            pass

# ─── Ensure Exponential Height Fog Exists ─────────────────────────────────────
fogs = [a for a in all_actors if isinstance(a, unreal.ExponentialHeightFog)]
fog = fogs[0] if fogs else unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.ExponentialHeightFog, unreal.Vector(0, 0, 0)
)
if fog:
    fog.set_actor_label("Fog_SunsetHaze")
    fc = fog.get_component_by_class(unreal.ExponentialHeightFogComponent)
    if fc:
        try:
            fc.set_editor_property("fog_density", 0.002)
        except Exception:
            pass

# ─── Ensure Unbound PostProcessVolume Exists ───────────────────────────────────
pps = [a for a in all_actors if isinstance(a, unreal.PostProcessVolume)]
pp = pps[0] if pps else unreal.EditorLevelLibrary.spawn_actor_from_class(
    unreal.PostProcessVolume, unreal.Vector(0, 0, 0)
)
if pp:
    pp.set_actor_label("PostProcess_AnimeCelGrade")
    pp.set_editor_property("unbound", True)
    pp_comp = pp.get_component_by_class(unreal.PostProcessComponent)
    if pp_comp:
        s = pp_comp.get_editor_property("settings")
        try:
            s.set_editor_property("auto_exposure_min_brightness", 1.0)
            s.set_editor_property("auto_exposure_max_brightness", 1.0)
            s.set_editor_property("bloom_intensity", 0.6)
            pp_comp.set_editor_property("settings", s)
        except Exception as e:
            unreal.log_warning(f"PP Settings warning: {e}")

# ─── Assign Materials to All StaticMeshActors ─────────────────────────────────
# Refresh all actors list
all_actors = unreal.EditorLevelLibrary.get_all_level_actors()

for a in all_actors:
    if not isinstance(a, unreal.StaticMeshActor):
        continue
    label = a.get_actor_label()
    comp = a.static_mesh_component
    if not comp or not comp.static_mesh:
        continue

    assigned_mat = None
    if "Ichiraku" in label:
        assigned_mat = mi_ichiraku_wood
    elif "HokageMansion" in label:
        assigned_mat = mi_hokage_mansion
    elif "HokageRock" in label or "HokageFaces" in label:
        assigned_mat = mi_hokage_rock
    elif "Gate" in label or "Torii" in label:
        assigned_mat = mi_torii_red
    elif "Residence_A" in label or "MissionOffice" in label:
        assigned_mat = mi_residence_a
    elif "Residence_B" in label:
        assigned_mat = mi_residence_b
    elif "Weapon" in label or "TeaHouse" in label or "Dango" in label:
        assigned_mat = mi_ichiraku_wood
    elif "Road" in label or "Ground" in label or "Plaza" in label or "Street" in label:
        assigned_mat = mi_road
    elif "Cedar" in label:
        assigned_mat = mi_cedar
    elif "Sakura" in label:
        assigned_mat = mi_sakura
    elif "Lantern" in label:
        assigned_mat = mi_lantern_stone

    if assigned_mat:
        num = max(comp.get_num_materials(), 1)
        for i in range(num):
            comp.set_material(i, assigned_mat)
        unreal.log(f"  [MATERIAL APPLIED] {label} -> {assigned_mat.get_name()}")

# ─── Save Map ─────────────────────────────────────────────────────────────────
unreal.EditorLoadingAndSavingUtils.save_map(world, LEVEL_PKG)
unreal.log("=== LEVEL L_Konoha_Phase13_District POLISHED & SAVED SUCCESSFULLY ===")
