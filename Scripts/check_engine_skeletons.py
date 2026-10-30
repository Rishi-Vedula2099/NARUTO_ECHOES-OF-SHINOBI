import unreal

EAL = unreal.EditorAssetLibrary
engine_skeletons = []
for a in EAL.list_assets('/Engine', recursive=True, include_folder=False):
    data = EAL.find_asset_data(a)
    asset_class = str(data.asset_class_path.asset_name)
    if 'Skeleton' in asset_class:
        engine_skeletons.append(a)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\engine_skeletons.txt"
with open(out_file, "w") as f:
    f.write(f"Engine skeletons found: {len(engine_skeletons)}\n")
    for s in engine_skeletons:
        f.write(f"  {s}\n")

print("Engine skeletons written")
