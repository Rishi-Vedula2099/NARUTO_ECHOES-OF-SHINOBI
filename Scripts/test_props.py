import unreal

EAL = unreal.EditorAssetLibrary
LES = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
LEVEL_PKG = "/Game/Maps/Konoha/L_Konoha_Phase13_District"

# Load the existing level
LES.load_level(LEVEL_PKG)
world = LES.get_world()
unreal.log(f"Successfully loaded world: {world.get_name()}")

# Inspect ExponentialHeightFogComponent properties
fog_comp = unreal.ExponentialHeightFogComponent()
fog_props = [p for p in dir(fog_comp) if 'color' in p.lower() or 'fog' in p.lower()]
unreal.log(f"Fog properties: {fog_props}")

# Inspect DirectionalLightComponent properties
sun_comp = unreal.DirectionalLightComponent()
sun_props = [p for p in dir(sun_comp) if 'forward' in p.lower() or 'priority' in p.lower()]
unreal.log(f"Sun properties: {sun_props}")
