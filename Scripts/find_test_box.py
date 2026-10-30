import unreal

EAL = unreal.EditorAssetLibrary
assets = EAL.list_assets("/Game", recursive=True, include_folder=False)
for a in assets:
    if "test_box" in a.lower():
        print(f"FOUND ASSET: {a}")
