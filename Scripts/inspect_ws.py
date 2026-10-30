import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
ws = unreal.GameplayStatics.get_world_settings(world)
unreal.log(f"WORLD_SETTINGS: {ws}")

# Check if there is any LevelScriptActor
for a in unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors():
    if "LevelScript" in a.get_class().get_name():
        unreal.log(f"LEVEL_SCRIPT_ACTOR: {a.get_name()} class={a.get_class().get_name()}")

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\ws_inspect.txt", "w") as f:
    f.write(f"WorldSettings: {ws}\n")
    if ws:
        try:
            f.write(f"bEnableWorldBoundsChecks: {ws.get_editor_property('bEnableWorldBoundsChecks')}\n")
        except Exception as e:
            f.write(f"err: {e}\n")
