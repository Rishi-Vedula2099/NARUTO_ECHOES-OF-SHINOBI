import unreal

level_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world = unreal.EditorLoadingAndSavingUtils.load_map(level_path)
print(f"Loaded world: {world}")

EAL = unreal.EditorAssetLibrary
actors = unreal.EditorLevelLibrary.get_all_level_actors()

char_actors = []
for a in actors:
    name = a.get_name()
    c_name = a.get_class().get_name()
    if "naruto" in name.lower() or "character" in c_name.lower() or "shinobi" in c_name.lower():
        char_actors.append(a)
        print(f"Character Actor in Level: {name} (Class: {c_name})")

p_mesh = unreal.load_asset("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit")
print(f"Masterpiece Mesh: {p_mesh}")

# Inspect the GameMode or DefaultPawn
gm_path = "/Game/Core/BP_EOSGameMode"
gm = unreal.load_asset(gm_path)
print(f"GameMode: {gm}")

# Also check BP_EOSCharacter_Naruto if it exists
bp_naruto = unreal.load_asset("/Game/Characters/Naruto/Blueprints/BP_EOSCharacter_Naruto")
print(f"BP_EOSCharacter_Naruto: {bp_naruto}")

# Check any other Naruto blueprints in /Game
for a_path in EAL.list_assets("/Game/Characters/Naruto", recursive=True):
    if "BP_" in a_path:
        print(f"  Naruto Blueprint: {a_path}")

unreal.EditorLoadingAndSavingUtils.save_current_level()
