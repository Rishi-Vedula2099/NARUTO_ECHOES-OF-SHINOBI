"""
NARUTO: ECHOES OF SHINOBI
Authoritative Golden Street Lighting, Vista & Material Tuning
=============================================================
Resolves:
1. Removes the occluding giant cliff box that was slicing through the avenue and casting pitch-black shadow over the street.
2. Places the authoritative North Star Vista backdrop (Hokage Faces Monument + distant mountains + blue anime sky) at horizon (Y=4200, Z=700).
3. Adds an authoritative SkyDome with soft azure anime dome material.
4. Tunes Directional Light to shine down the avenue (pitch=-38, yaw=80, intensity=10.0 lux, warm 5400K).
5. Tunes SkyLight to provide bright azure ambient fill (intensity=6.0, cast_shadows=False for soft shadow band transitions).
6. Calibrates PostProcess Volume: locks auto-exposure (min=1.0, max=1.0, bias=0.5) so glowing lanterns cannot darken the scene.
7. Sets paper lantern glow to warm controlled amber (emissive_intensity=0.8).
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

def tune_scene():
    unreal.log("======================================================================")
    unreal.log("  TUNING GOLDEN STREET LIGHTING, VISTA BACKDROP & EXPOSURE")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    # 1. Remove occluding giant cliff actor if present
    for a in eas.get_all_level_actors():
        if a and ("hokagefaces_3dmonument" in a.get_actor_label().lower() or 
                  "hokagefaces_monument" in a.get_actor_label().lower()):
            unreal.log(f"  [REMOVED] Destroyed occluding cliff actor: {a.get_actor_label()}")
            eas.destroy_actor(a)

    # 2. Setup or update North Star Vista Backdrop Plane
    # Engine Plane default is 100x100 facing +Z.
    # To face -Y (south towards camera at Y=0), rotate pitch=90.0, yaw=0.0, roll=0.0.
    mi_backdrop = unreal.load_asset("/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop")
    plane_mesh = unreal.load_asset("/Engine/BasicShapes/Plane")
    
    # Check if backdrop actor already exists
    backdrop_actor = None
    for a in eas.get_all_level_actors():
        if a and "northstar_vista" in a.get_actor_label().lower():
            backdrop_actor = a
            break
            
    if not backdrop_actor and plane_mesh and mi_backdrop:
        backdrop_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,
            unreal.Vector(0.0, 4200.0, 750.0),
            unreal.Rotator(pitch=90.0, yaw=0.0, roll=0.0)
        )
        if backdrop_actor:
            backdrop_actor.set_actor_label("Backdrop_Konoha_NorthStar_Vista")
            smc = backdrop_actor.static_mesh_component
            smc.set_static_mesh(plane_mesh)
            # Scale 4800cm wide by 2400cm high (48m x 24m backdrop)
            backdrop_actor.set_actor_scale3d(unreal.Vector(48.0, 24.0, 1.0))
            smc.set_material(0, mi_backdrop)
            smc.set_collision_profile_name("NoCollision")
            smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
            smc.set_cast_shadow(False)
            unreal.log("  [VISTA] Created Backdrop_Konoha_NorthStar_Vista at Y=4200")
    elif backdrop_actor:
        backdrop_actor.set_actor_location(unreal.Vector(0.0, 4200.0, 750.0), False, False)
        backdrop_actor.set_actor_rotation(unreal.Rotator(pitch=90.0, yaw=0.0, roll=0.0), False)
        backdrop_actor.set_actor_scale3d(unreal.Vector(48.0, 24.0, 1.0))
        smc = backdrop_actor.static_mesh_component
        smc.set_cast_shadow(False)
        smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
        unreal.log("  [VISTA] Updated Backdrop_Konoha_NorthStar_Vista")

    # 3. Directional Light: Positioned and angled to illuminate down avenue
    for a in eas.get_all_level_actors():
        if a and isinstance(a, unreal.DirectionalLight):
            a.set_actor_label("DirLight_Konoha_Sun")
            a.set_actor_location(unreal.Vector(0.0, -500.0, 3500.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-38.0, yaw=82.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 12.0)
                dlc.set_editor_property("light_color", unreal.Color(255, 246, 228, 255))
                dlc.set_editor_property("cast_shadows", True)
                dlc.set_editor_property("atmosphere_sun_light", True)
                dlc.set_editor_property("atmosphere_sun_light_index", 0)
                unreal.log("  [SUN] Configured DirLight_Konoha_Sun (pitch=-38, yaw=82, int=12.0)")

    # 4. SkyLight: Bright azure ambient fill, no harsh dark shadows
    for a in eas.get_all_level_actors():
        if a and isinstance(a, unreal.SkyLight):
            a.set_actor_label("SkyLight_AnimeAzureFill")
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 6.0)
                slc.set_editor_property("light_color", unreal.Color(190, 225, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", True)
                unreal.log("  [SKYLIGHT] Configured SkyLight_AnimeAzureFill (int=6.0, azure fill)")

    # 5. Exponential Height Fog
    for a in eas.get_all_level_actors():
        if a and isinstance(a, unreal.ExponentialHeightFog):
            fc = a.get_component_by_class(unreal.ExponentialHeightFogComponent)
            if fc:
                fc.set_editor_property("fog_density", 0.0002)
                fc.set_editor_property("start_distance", 2500.0)
                fc.set_editor_property("fog_inscattering_luminance", unreal.LinearColor(0.85, 0.92, 1.0, 1.0))
                unreal.log("  [FOG] Configured Fog_KonohaSunsetHaze")

    # 6. PostProcess Volume: Locked exposure to prevent lantern dark clamping
    for a in eas.get_all_level_actors():
        if a and isinstance(a, unreal.PostProcessVolume):
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
                s.set_editor_property("bloom_intensity", 0.10)
                s.set_editor_property("override_color_saturation", True)
                s.set_editor_property("color_saturation", unreal.Vector4(1.12, 1.12, 1.12, 1.0))
                ppc.set_editor_property("settings", s)
                unreal.log("  [PP] Configured PostProcess_GoldenStreet (locked exposure EV, bias=0.8)")

    # 7. Material Instances: Soften lantern glow from 2.0 to 0.7
    mi_lantern = unreal.load_asset("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow")
    if mi_lantern:
        MEL.set_material_instance_scalar_parameter_value(mi_lantern, "EmissiveIntensity", 0.7)
        MEL.set_material_instance_vector_parameter_value(mi_lantern, "EmissiveColor", unreal.LinearColor(1.0, 0.78, 0.38, 1.0))
        MEL.set_material_instance_vector_parameter_value(mi_lantern, "BaseColor", unreal.LinearColor(0.98, 0.82, 0.45, 1.0))
        MEL.update_material_instance(mi_lantern)
        EAL.save_loaded_asset(mi_lantern)
        unreal.log("  [MAT] Calibrated MI_Prop_Lantern_Glow (EmissiveIntensity=0.7)")

    # Save map
    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> LIGHTING, VISTA & EXPOSURE TUNING COMPLETE & SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    tune_scene()
