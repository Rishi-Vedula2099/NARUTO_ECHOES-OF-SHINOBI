import unreal

MAP_PATH = "/Game/Maps/GoldenTest/L_Naruto_Showcase"
world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# Clean up all existing actors in the showcase level
actors = list(EAS.get_all_level_actors())
for a in actors:
    EAS.destroy_actor(a)

# 1. Floor: 1000x1000 neutral dark slate studio floor
floor = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
floor.set_actor_label("StudioFloor")
floor.set_actor_scale3d(unreal.Vector(15.0, 15.0, 0.1))
smc_f = floor.get_component_by_class(unreal.StaticMeshComponent)
plane = unreal.load_asset("/Engine/BasicShapes/Plane")
if plane and smc_f:
    smc_f.set_static_mesh(plane)

# 2. Studio Key Light (Directional, pitch=-25, yaw=45, intensity=6.0, warm 5400K)
sun = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0, 0, 500))
sun.set_actor_label("StudioKeyLight")
sun.set_actor_rotation(unreal.Rotator(pitch=-25.0, yaw=45.0, roll=0.0), False)
dlc = sun.get_component_by_class(unreal.DirectionalLightComponent)
if dlc:
    dlc.set_editor_property("intensity", 6.0)
    dlc.set_editor_property("light_color", unreal.Color(255, 245, 230, 255))
    dlc.set_editor_property("cast_shadows", True)

# 3. Studio SkyLight Fill (intensity=3.5, azure fill)
sky = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0, 0, 600))
sky.set_actor_label("StudioSkyFill")
slc = sky.get_component_by_class(unreal.SkyLightComponent)
if slc:
    slc.set_editor_property("intensity", 3.5)
    slc.set_editor_property("light_color", unreal.Color(190, 220, 255, 255))
    slc.set_editor_property("cast_shadows", False)
    slc.set_editor_property("real_time_capture", True)

# 4. Post Process Volume
pp = EAS.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0, 0, 0))
pp.set_actor_label("StudioPP")
pp.set_editor_property("unbound", True)
pps = pp.settings
pps.set_editor_property("override_auto_exposure_method", True)
pps.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_MANUAL)
pps.set_editor_property("override_auto_exposure_apply_physical_camera_exposure", True)
pps.set_editor_property("auto_exposure_apply_physical_camera_exposure", False)
pps.set_editor_property("override_auto_exposure_bias", True)
pps.set_editor_property("auto_exposure_bias", 0.0)

# 5. Spawn Naruto Masterpiece Showcase Actor
naruto = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0, 0, 0))
naruto.set_actor_label("HeroNaruto_Showcase")
# Face towards camera at (0, -260): Naruto looks along -Y axis, so Yaw = -90
naruto.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
smc_n = naruto.get_component_by_class(unreal.StaticMeshComponent)
mesh = unreal.load_asset("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit")
if smc_n and mesh:
    smc_n.set_static_mesh(mesh)
    print(f"Naruto Showcase Mesh loaded with {mesh.get_num_sections(0)} sections.")

# 6. PlayerStart placed at camera location (0, -260, 85) looking at Naruto at (0, 0, 80)
# Looking along +Y axis -> Yaw = 90
pstart = EAS.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, -260.0, 85.0))
pstart.set_actor_label("PlayerStart_FrontView")
pstart.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

# Save Showcase Level
unreal.EditorLoadingAndSavingUtils.save_current_level()
print("Saved clean studio showcase setup in L_Naruto_Showcase.")
