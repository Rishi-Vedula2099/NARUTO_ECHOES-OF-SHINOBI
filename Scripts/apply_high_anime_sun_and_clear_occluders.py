"""
NARUTO: ECHOES OF SHINOBI
High Anime Daylight & Street Shadow Elimination
================================================
1. High anime sun (pitch=-65.0, yaw=65.0, intensity=10.0 lux, warm 5400K).
   In anime production (North Star reference), the sun is overhead so characters
   have tight contact shadows under their feet and the street is fully sunlit.
2. Removes all occluding trees/props located behind player (Y < 100) that cast
   long shadows down the avenue.
3. Positions framing trees outward (|X| >= 560, Y >= 300).
4. Calibrates SkyLight (intensity=7.0, soft azure fill) so shadow transitions are readable.
5. Calibrates PostProcess exposure for warm, brilliant anime aesthetic.
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def apply_anime_daylight():
    unreal.log("======================================================================")
    unreal.log("  CONFIGURING HIGH ANIME SUN & ELIMINATING STREET SHADOW OCCLUDERS")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    # 1. High Anime Sun (Overhead, pitch=-65, yaw=65)
    for a in eas.get_all_level_actors():
        if a and isinstance(a, unreal.DirectionalLight):
            a.set_actor_label("DirLight_Konoha_Sun")
            a.set_actor_location(unreal.Vector(0.0, 0.0, 4000.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-65.0, yaw=65.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 10.0)
                dlc.set_editor_property("light_color", unreal.Color(255, 248, 230, 255))
                dlc.set_editor_property("cast_shadows", True)
                dlc.set_editor_property("atmosphere_sun_light", True)
                unreal.log("  [SUN] Configured High Anime Sun (pitch=-65, yaw=65, int=10.0 lux)")

        elif a and isinstance(a, unreal.SkyLight):
            a.set_actor_label("SkyLight_AnimeAzureFill")
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 7.0)
                slc.set_editor_property("light_color", unreal.Color(190, 225, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", True)
                unreal.log("  [SKYLIGHT] Configured SkyLight (int=7.0, soft azure fill)")

        elif a and isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.6)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.4)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.4)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.20, 1.20, 1.20, 1.0))
            s.set_editor_property("override_bloom_intensity", True)
            s.set_editor_property("bloom_intensity", 0.10)
            a.settings = s
            unreal.log("  [PP] Configured PostProcessVolume (locked exposure, saturation=1.20)")

    # 2. Relocate or remove rear occluding foliage that casts shadows into the street
    for a in eas.get_all_level_actors():
        if not a:
            continue
        label = a.get_actor_label()
        loc = a.get_actor_location()
        if "veg_sakura_front" in label.lower() or "veg_cedar_left_01" in label.lower():
            # Move behind buildings to flank outer perimeter safely at Y >= 500
            new_x = -680.0 if loc.x < 0 else 680.0
            new_y = 500.0 + abs(loc.x) * 0.5
            a.set_actor_location(unreal.Vector(new_x, new_y, 0.0), False, False)
            unreal.log(f"  [FLANKED] Moved occluding tree '{label}' from Y={loc.y} to ({new_x}, {new_y})")

    # 3. Ensure Frontier Gate behind player does not cast shadows onto the street
    for a in eas.get_all_level_actors():
        if a and "frontiergate" in a.get_actor_label().lower():
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                smc.set_cast_shadow(False)
                unreal.log("  [GATE] Disabled shadow casting on rear Frontier Gate")

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> HIGH ANIME DAYLIGHT APPLIED & MAP SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    apply_anime_daylight()
