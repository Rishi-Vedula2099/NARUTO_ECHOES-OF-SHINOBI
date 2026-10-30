"""
NARUTO: ECHOES OF SHINOBI
Gate 1 — Technical Rendering Proving Box Setup
Map: /Game/Maps/VisualSmoke/L_VisualAssetSmokeTest
Sets up isolated material swatches, calibrated daylight, and test camera.
"""

import unreal

MAP_PATH = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

print("=== Setting up Gate 1 Rendering Proving Box in L_VisualAssetSmokeTest ===")

# 1. Directional Sun (5400K Golden Key)
sun = None
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.DirectionalLight) or a.get_actor_label() == "DirLight_Konoha_5400K":
        sun = a
        break

if not sun:
    sun = EAS.spawn_actor_from_class(unreal.DirectionalLight, unreal.Vector(0.0, 0.0, 2000.0))
    sun.set_actor_label("DirLight_Konoha_5400K")

sun.set_actor_rotation(unreal.Rotator(pitch=-58.0, yaw=75.0, roll=0.0), False)
dlc = sun.get_component_by_class(unreal.DirectionalLightComponent)
if dlc:
    dlc.set_editor_property("light_color", unreal.Color(r=255, g=240, b=215, a=255))
    dlc.set_editor_property("use_temperature", True)
    dlc.set_editor_property("temperature", 5400.0)
    dlc.set_editor_property("intensity", 75000.0) # Physical daylight units (Lux)
    dlc.set_editor_property("atmosphere_sun_light", True)
    dlc.set_editor_property("cast_shadows", True)
    print("Configured DirLight_Konoha_5400K: 5400K, 75000 Lux, Pitch -58°, Yaw 75°")

# 2. SkyLight (Azure Fill to prevent crushed blacks)
skylight = None
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.SkyLight) or a.get_actor_label() == "SkyLight_AnimeAzureFill":
        skylight = a
        break

if not skylight:
    skylight = EAS.spawn_actor_from_class(unreal.SkyLight, unreal.Vector(0.0, 0.0, 1500.0))
    skylight.set_actor_label("SkyLight_AnimeAzureFill")

slc = skylight.get_component_by_class(unreal.SkyLightComponent)
if slc:
    slc.set_editor_property("light_color", unreal.Color(r=185, g=220, b=255, a=255))
    slc.set_editor_property("intensity", 1.5)
    slc.set_editor_property("cast_shadows", True)
    print("Configured SkyLight_AnimeAzureFill: RGB(185, 220, 255), Intensity 1.5")

# 3. PostProcessVolume (Manual Exposure & Controlled Bloom)
ppv = None
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.PostProcessVolume) or a.get_actor_label() == "PostProcess_ProvingBox":
        ppv = a
        break

if not ppv:
    ppv = EAS.spawn_actor_from_class(unreal.PostProcessVolume, unreal.Vector(0.0, 0.0, 0.0))
    ppv.set_actor_label("PostProcess_ProvingBox")

ppc = ppv.get_component_by_class(unreal.PostProcessComponent)
if ppc:
    ppv.set_editor_property("unbound", True)
    settings = ppc.get_editor_property("settings")
    
    settings.set_editor_property("auto_exposure_method", unreal.AutoExposureMethod.AEM_MANUAL)
    settings.set_editor_property("auto_exposure_bias", 0.0)
    settings.set_editor_property("camera_iso", 100.0)
    settings.set_editor_property("camera_shutter_speed", 1.0 / 125.0)
    settings.set_editor_property("camera_aperture", 4.0)
    
    settings.set_editor_property("bloom_intensity", 0.65)
    settings.set_editor_property("bloom_threshold", 1.25)
    
    ppc.set_editor_property("settings", settings)
    print("Configured PostProcess_ProvingBox: Unbound, Manual Exposure, Controlled Bloom")

# 4. Material Swatch Display Stand (Using verified material paths)
swatches = [
    ("Swatch_Skin", "/Game/Characters/Naruto/Materials/MI_Naruto_Skin", -350.0),
    ("Swatch_Hair", "/Game/Characters/Naruto/Materials/MI_Naruto_Hair", -250.0),
    ("Swatch_Tracksuit", "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit", -150.0),
    ("Swatch_Navy", "/Game/Characters/Naruto/Materials/MI_Naruto_Navy", -50.0),
    ("Swatch_Collar", "/Game/Characters/Naruto/Materials/MI_Naruto_Collar", 50.0),
    ("Swatch_Metal", "/Game/Characters/Naruto/Materials/MI_Naruto_Metal", 150.0),
    ("Swatch_CedarWood", "/Game/Materials/Environment/MI_AnimeWood", 250.0),
    ("Swatch_RoofTile", "/Game/Materials/Environment/MI_AnimeRoof", 350.0),
    ("Swatch_Stone", "/Game/Materials/Environment/MI_AnimeStone", 450.0),
    ("Swatch_Foliage", "/Game/Materials/Environment/MI_AnimeFoliage", 550.0),
]

sphere_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Sphere.Sphere")
if not sphere_mesh:
    sphere_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube.Cube")

for name, mat_path, x_pos in swatches:
    swatch_actor = None
    for a in EAS.get_all_level_actors():
        if a.get_actor_label() == name:
            swatch_actor = a
            break
    
    if not swatch_actor and sphere_mesh:
        swatch_actor = EAS.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(x_pos, -200.0, 60.0))
        swatch_actor.set_actor_label(name)
        smc = swatch_actor.get_component_by_class(unreal.StaticMeshComponent)
        if smc:
            smc.set_editor_property("static_mesh", sphere_mesh)
            swatch_actor.set_actor_scale3d(unreal.Vector(0.7, 0.7, 0.7))
    
    if swatch_actor:
        mat = unreal.EditorAssetLibrary.load_asset(mat_path)
        if mat:
            smc = swatch_actor.get_component_by_class(unreal.StaticMeshComponent)
            if smc:
                smc.set_material(0, mat)
                print(f"Bound swatch {name} -> {mat.get_name()} at X={x_pos}")
        else:
            print(f"Warning: Could not load material at {mat_path}")

# 5. Proving Box Inspection Camera
cam_actor = None
for a in EAS.get_all_level_actors():
    if a.get_actor_label() == "Camera_ProvingBox_Swatches":
        cam_actor = a
        break

if not cam_actor:
    cam_actor = EAS.spawn_actor_from_class(unreal.CameraActor, unreal.Vector(100.0, -750.0, 140.0))
    cam_actor.set_actor_label("Camera_ProvingBox_Swatches")

cam_actor.set_actor_location(unreal.Vector(100.0, -750.0, 140.0), False, False)
cam_actor.set_actor_rotation(unreal.Rotator(pitch=-8.0, yaw=90.0, roll=0.0), False)
cam_comp = cam_actor.get_component_by_class(unreal.CameraComponent)
if cam_comp:
    cam_comp.set_editor_property("field_of_view", 65.0)

# Save Level
unreal.EditorLoadingAndSavingUtils.save_current_level()
print("=== Successfully saved Gate 1 Rendering Proving Box in L_VisualAssetSmokeTest ===")
