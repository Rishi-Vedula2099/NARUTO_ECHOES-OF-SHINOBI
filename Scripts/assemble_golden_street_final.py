"""
NARUTO: ECHOES OF SHINOBI
Final Assembly of L_Konoha_GoldenStreet with Modular 3D Kits and Anime Shading
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def assemble_level():
    unreal.log("======================================================================")
    unreal.log("  FINAL ASSEMBLY OF L_KONOHA_GOLDENSTREET")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    # 1. Meshes & Materials
    sm_ichiraku  = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
    sm_res_a     = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A")
    sm_res_b     = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B")
    sm_road      = unreal.load_asset("/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01")
    sm_naruto    = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
    mat_backdrop = unreal.load_asset("/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop")

    for a in eas.get_all_level_actors():
        if not a:
            continue
        label_lower = a.get_actor_label().lower()

        # Sun: Warm 5400K from overhead-left
        if isinstance(a, unreal.DirectionalLight):
            a.set_actor_location(unreal.Vector(-800.0, -400.0, 2500.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=-52.0, yaw=55.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 6.5)
                dlc.set_editor_property("light_color", unreal.Color(255, 245, 230, 255))
                dlc.set_editor_property("cast_shadows", True)
                unreal.log("  [SUN] Aligned Directional Sun: pitch=-52, yaw=55, int=6.5 lux")

        # SkyLight: Soft Azure Fill
        elif isinstance(a, unreal.SkyLight):
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 8.5)
                slc.set_editor_property("light_color", unreal.Color(195, 228, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", False)
                unreal.log("  [SKYLIGHT] Set soft azure fill: int=8.5 lux, real_time=False")

        # PostProcess: Exposure & Saturation
        elif isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.8)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.2)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.5)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.25, 1.25, 1.25, 1.0))
            a.settings = s

        # Vista Backdrop: Frame horizon and face camera along -Y
        elif "backdrop" in label_lower:
            a.set_actor_location(unreal.Vector(0.0, 3500.0, 750.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=-90.0), False)
            a.set_actor_scale3d(unreal.Vector(65.0, 32.0, 1.0))
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and mat_backdrop:
                smc.set_material(0, mat_backdrop)
            unreal.log("  [VISTA] Aligned Backdrop_Konoha_NorthStar_Vista facing -Y at Y=3500")

        # Ichiraku Ramen Shop
        elif "bld_ichirakuramen" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_ichiraku:
                smc.set_static_mesh(sm_ichiraku)
            a.set_actor_location(unreal.Vector(-540.0, 320.0, 0.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)
            unreal.log("  [ICHI] Assigned SM_Bld_IchirakuRamen facing avenue")

        # Konoha Residence A (Right sidewalk)
        elif "bld_konoharesidence_a" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_res_a:
                smc.set_static_mesh(sm_res_a)
            a.set_actor_location(unreal.Vector(540.0, 260.0, 0.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
            unreal.log("  [RES A] Assigned SM_Bld_Konoha_Residence_A facing avenue")

        # Konoha Residence B (Right sidewalk)
        elif "bld_konoharesidence_b" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_res_b:
                smc.set_static_mesh(sm_res_b)
            a.set_actor_location(unreal.Vector(560.0, 980.0, 0.0), False, False)
            a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
            unreal.log("  [RES B] Assigned SM_Bld_Konoha_Residence_B facing avenue")

        # Road segments
        elif "road_avenue" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_road:
                smc.set_static_mesh(sm_road)

    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> LEVEL ASSEMBLED AND SAVED CLEANLY <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    assemble_level()
