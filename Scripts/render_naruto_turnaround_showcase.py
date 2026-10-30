"""
NARUTO: ECHOES OF SHINOBI
Priority 1 Validation: Naruto Hero Character Turnaround Showcase
================================================================
Creates an isolated studio showcase for SK_Naruto_GeninTracksuit to capture:
1. Front View (Face, Headband, Collar, Jacket, Hands, Bandages, Toes)
2. Back View (Hair spikes, Flowing ribbons, Back Uzumaki Crest, Stance)
3. Right Side View (Profile, Headband plate, Thigh Holster & Bandages)
4. 3/4 Hero Perspective (Depth, anime cel shading, volume)
"""

import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
SHOTS_DIR = os.path.join(PROJECT_DIR, "Docs", "Screenshots", "Priority1_Naruto_Showcase")
os.makedirs(SHOTS_DIR, exist_ok=True)

# 1. Create / Load Showcase Level
MAP_PATH = "/Game/Maps/GoldenTest/L_Naruto_Showcase"
world = unreal.EditorLoadingAndSavingUtils.new_blank_map(False)

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# 2. Add Neutral Studio Floor
floor = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
floor.set_actor_label("StudioFloor")
floor.set_actor_scale3d(unreal.Vector(10.0, 10.0, 0.1))
smc_floor = floor.get_component_by_class(unreal.StaticMeshComponent)
plane_mesh = unreal.load_asset("/Engine/BasicShapes/Plane")
if plane_mesh and smc_floor:
    smc_floor.set_static_mesh(plane_mesh)
    floor_mat = unreal.load_asset("/Game/Art/Materials/MI_Stone_Paved") or unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Navy")
    if floor_mat:
        smc_floor.set_material(0, floor_mat)

# 3. Add Studio Directional Light (Warm Golden Key)
dir_light = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 400))
dir_light.set_actor_label("StudioKeyLight")
dir_light.set_actor_rotation(unreal.Rotator(pitch=-30.0, yaw=45.0, roll=0.0), False)
dlc = dir_light.get_component_by_class(unreal.DirectionalLightComponent)
if dlc:
    dlc.set_editor_property("intensity", 6.0)
    dlc.set_editor_property("light_color", unreal.Color(255, 245, 230, 255))
    dlc.set_editor_property("cast_shadows", True)

# 4. Add Studio SkyLight (Azure Fill for clean anime shadows)
sky_light = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 500))
sky_light.set_actor_label("StudioSkyFill")
slc = sky_light.get_component_by_class(unreal.SkyLightComponent)
if slc:
    slc.set_editor_property("intensity", 3.0)
    slc.set_editor_property("light_color", unreal.Color(190, 220, 255, 255))
    slc.set_editor_property("cast_shadows", False)
    slc.set_editor_property("real_time_capture", True)

# 5. Add Controlled Post Process Volume
pp_actor = EAS.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
pp_actor.set_actor_label("StudioPostProcess")
pp_actor.set_editor_property("unbound", True)
pps = pp_actor.settings
pps.set_editor_property("override_auto_exposure_method", True)
pps.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_MANUAL)
pps.set_editor_property("override_auto_exposure_apply_physical_camera_exposure", True)
pps.set_editor_property("auto_exposure_apply_physical_camera_exposure", False)
pps.set_editor_property("override_auto_exposure_bias", True)
pps.set_editor_property("auto_exposure_bias", 0.0)

# 6. Spawn Naruto Character Mesh
naruto_actor = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
naruto_actor.set_actor_label("HeroNaruto_Showcase")
naruto_mesh = unreal.load_asset("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit")
smc_naruto = naruto_actor.get_component_by_class(unreal.StaticMeshComponent)
if smc_naruto and naruto_mesh:
    smc_naruto.set_static_mesh(naruto_mesh)
    print(f"Spawned Naruto Showcase Mesh: {naruto_mesh.get_path_name()} with {naruto_mesh.get_num_sections(0)} sections")

# Save Showcase Map
unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PATH)
print(f"Saved Showcase Level: {MAP_PATH}")
