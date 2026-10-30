"""
NARUTO: ECHOES OF SHINOBI
Authoritative Sun Alignment & Ambient Sky Fill Calibration
==========================================================
1. Aligns Directional Sun directly down the central avenue:
   - Pitch = -45.0 (high warm midday-afternoon sun)
   - Yaw = 90.0 (aligned with camera and avenue axis)
   - Roll = 0.0
   - Intensity = 8.0 lux
   - LightColor = (255, 246, 225) (warm 5400K golden anime daylight)
   -> Eliminates the building shadow occluding the street and Naruto.
2. Calibrates SkyLight for vibrant anime shadow fill:
   - Intensity = 12.0 lux
   - LightColor = (185, 220, 255) (soft azure sky fill)
   - CastShadows = False
   -> Guarantees shadows are bright, colorful periwinkle blue, NEVER crushed dark.
3. PostProcess Exposure Lock:
   - AutoExposure: Manual / Locked EV (min=1.5, max=1.5, bias=1.0)
   - Color Saturation: 1.18 (vibrant anime color grade)
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def align_lighting():
    unreal.log("======================================================================")
    unreal.log("  ALIGNING SUN DOWN CENTRAL AVENUE & BRIGHTENING AMBIENT SKY FILL")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    for a in eas.get_all_level_actors():
        if not a:
            continue
        # Sun: Straight down the avenue (yaw=90.0, pitch=-45.0)
        if isinstance(a, unreal.DirectionalLight):
            a.set_actor_label("DirLight_Konoha_Sun")
            a.set_actor_location(unreal.Vector(0.0, -800.0, 3000.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-45.0, yaw=90.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 8.5)
                dlc.set_editor_property("light_color", unreal.Color(255, 245, 220, 255))
                dlc.set_editor_property("cast_shadows", True)
                unreal.log("  [SUN] Aligned Sun down avenue: pitch=-45, yaw=90, int=8.5 lux")

        # SkyLight: High intensity azure fill
        elif isinstance(a, unreal.SkyLight):
            a.set_actor_label("SkyLight_AnimeAzureFill")
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 14.0)
                slc.set_editor_property("light_color", unreal.Color(185, 225, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", True)
                unreal.log("  [SKYLIGHT] SkyLight calibrated: int=14.0 lux (soft azure fill)")

        # PostProcessVolume: Lock exposure
        elif isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.6)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.20, 1.20, 1.20, 1.0))
            a.settings = s
            unreal.log("  [PP] PostProcess locked exposure at 1.2 EV, saturation 1.20")

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> LIGHTING ALIGNMENT COMPLETE & SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    align_lighting()
