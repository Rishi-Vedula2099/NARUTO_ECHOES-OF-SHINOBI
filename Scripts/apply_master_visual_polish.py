"""
NARUTO: ECHOES OF SHINOBI
Authoritative Master Visual Polish & Color Harmony
=================================================
1. Duplicates the 5-slot SK_Naruto_Genin into SK_Naruto_GeninTracksuit.
2. Calibrates authentic Anime Cel colors matching Konoha Visual North Star:
   - Naruto:
     * Hair: Vibrant Blonde Gold (0.98, 0.85, 0.15)
     * Tracksuit: Authentic Genin Orange (0.94, 0.40, 0.05)
     * Navy: Headband fabric, shinobi sandals (0.08, 0.14, 0.26)
     * Collar: High popped jacket collar (0.94, 0.94, 0.90)
     * Skin: Warm anime peachy tone (0.98, 0.82, 0.70)
   - Environment:
     * Road: Warm light sandstone cobblestones (0.72, 0.68, 0.62)
     * Ichiraku Ramen Wood: Rich Japanese cedar timber (0.56, 0.34, 0.18)
     * Ichiraku Noren: Traditional anime indigo blue fabric (0.10, 0.22, 0.46)
     * Residence Wood: Warm cedar timber (0.58, 0.36, 0.20)
     * Hokage Mansion: Iconic vermilion red pagoda (0.85, 0.18, 0.10)
     * Foliage: Cedar deep pine (0.12, 0.42, 0.18), Sakura flowering pink (0.98, 0.62, 0.76)
     * Lanterns: Stone granite (0.60, 0.58, 0.55), Paper amber glow (1.0, 0.80, 0.40, Emissive 0.8)
3. Sun lighting: DirectionalLight at pitch=-35, yaw=55, intensity=9.0 lux, warm 5400K.
4. Sky fill: SkyLight at intensity=4.5 lux, soft azure (190, 225, 255).
5. PostProcess: Locked exposure EV, saturation 1.15 for vibrant anime presentation.
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
EAL = unreal.EditorAssetLibrary
MEL = unreal.MaterialEditingLibrary

def run_polish():
    unreal.log("======================================================================")
    unreal.log("  APPLYING MASTER VISUAL POLISH & COLOR HARMONY")
    unreal.log("======================================================================")

    # 1. Update Material Parameters
    def set_mi(path, base_color, roughness=0.8, specular=0.1, emissive_color=(0,0,0,1), emissive_intensity=0.0):
        mi = unreal.load_asset(path)
        if not mi:
            unreal.log_warning(f"Missing MI: {path}")
            return
        MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color))
        MEL.set_material_instance_vector_parameter_value(mi, "SkinToneMultiplier", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))
        MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
        MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
        MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(*emissive_color))
        MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", emissive_intensity)
        MEL.set_material_instance_scalar_parameter_value(mi, "RimLightIntensity", 0.0)
        MEL.update_material_instance(mi)
        EAL.save_loaded_asset(mi)
        unreal.log(f"  [OK] {mi.get_name()}: BaseColor={base_color[:3]}")

    # Character
    set_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), roughness=0.85, specular=0.05)
    set_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",      (0.08, 0.14, 0.28, 1.0), roughness=0.80, specular=0.08)
    set_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",      (0.98, 0.85, 0.14, 1.0), roughness=0.65, specular=0.20)
    set_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",    (0.94, 0.94, 0.90, 1.0), roughness=0.85, specular=0.05)
    set_mi("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit", (0.94, 0.38, 0.05, 1.0), roughness=0.82, specular=0.08)

    # Environment - Roads
    set_mi("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone", (0.72, 0.68, 0.62, 1.0), roughness=0.88, specular=0.06)

    # Environment - Architecture
    set_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood", (0.56, 0.34, 0.18, 1.0), roughness=0.80, specular=0.06)
    set_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A",   (0.58, 0.36, 0.20, 1.0), roughness=0.80, specular=0.06)
    set_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B",   (0.52, 0.30, 0.15, 1.0), roughness=0.80, specular=0.06)
    set_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",(0.10, 0.22, 0.46, 1.0), roughness=0.90, specular=0.04)
    set_mi("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion", (0.85, 0.18, 0.10, 1.0), roughness=0.70, specular=0.15)

    # Foliage & Props
    set_mi("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized", (0.12, 0.42, 0.18, 1.0), roughness=0.80, specular=0.05)
    set_mi("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent", (0.98, 0.62, 0.76, 1.0), roughness=0.85, specular=0.08)
    set_mi("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone",     (0.60, 0.58, 0.55, 1.0), roughness=0.85, specular=0.05)
    set_mi("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow",      (1.0, 0.80, 0.40, 1.0), roughness=0.50, specular=0.20,
           emissive_color=(1.0, 0.75, 0.35, 1.0), emissive_intensity=0.8)

    # 2. Sync SK_Naruto_GeninTracksuit with SK_Naruto_Genin
    genin_mesh = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")
    skin_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
    if genin_mesh:
        if EAL.does_asset_exist(skin_path):
            EAL.delete_asset(skin_path)
        EAL.duplicate_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin", skin_path)
        unreal.log(f"  [SYNC] Duplicated SK_Naruto_Genin -> {skin_path}")

    # 3. Load Map and update Lighting / Camera / PP
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    for a in eas.get_all_level_actors():
        if not a:
            continue
        # Sun
        if isinstance(a, unreal.DirectionalLight):
            a.set_actor_label("DirLight_Konoha_Sun")
            a.set_actor_rotation(unreal.Rotator(pitch=-35.0, yaw=55.0, roll=0.0), False)
            dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
            if dlc:
                dlc.set_editor_property("intensity", 9.0)
                dlc.set_editor_property("light_color", unreal.Color(255, 246, 228, 255))
                dlc.set_editor_property("cast_shadows", True)
                unreal.log("  [LIGHT] Sun updated: pitch=-35, yaw=55, int=9.0")
        # SkyLight
        elif isinstance(a, unreal.SkyLight):
            a.set_actor_label("SkyLight_AnimeAzureFill")
            slc = a.get_component_by_class(unreal.SkyLightComponent)
            if slc:
                slc.set_editor_property("intensity", 4.5)
                slc.set_editor_property("light_color", unreal.Color(190, 225, 255, 255))
                slc.set_editor_property("cast_shadows", False)
                slc.set_editor_property("real_time_capture", True)
                unreal.log("  [LIGHT] SkyLight updated: int=4.5, azure fill")
        # PostProcessVolume
        elif isinstance(a, unreal.PostProcessVolume):
            s = a.settings
            s.set_editor_property("override_auto_exposure_method", True)
            s.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_HISTOGRAM)
            s.set_editor_property("override_auto_exposure_min_brightness", True)
            s.set_editor_property("auto_exposure_min_brightness", 0.5)
            s.set_editor_property("override_auto_exposure_max_brightness", True)
            s.set_editor_property("auto_exposure_max_brightness", 1.5)
            s.set_editor_property("override_auto_exposure_bias", True)
            s.set_editor_property("auto_exposure_bias", 0.5)
            s.set_editor_property("override_bloom_intensity", True)
            s.set_editor_property("bloom_intensity", 0.10)
            s.set_editor_property("override_color_saturation", True)
            s.set_editor_property("color_saturation", unreal.Vector4(1.15, 1.15, 1.15, 1.0))
            a.settings = s
            unreal.log("  [PP] PostProcessVolume settings applied directly to actor.settings")

    # Save
    unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PKG)
    unreal.log("======================================================================")
    unreal.log("  >>> MASTER VISUAL POLISH COMPLETE & LEVEL SAVED <<<")
    unreal.log("======================================================================")

if __name__ == "__main__":
    run_polish()
