import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\list_naruto_meshes.txt"
lines = []

assets = unreal.EditorAssetLibrary.list_assets("/Game/Characters/Naruto/Meshes", True, False)
lines.append(f"Assets in /Game/Characters/Naruto/Meshes ({len(assets)} total):")
for p in assets:
    obj = unreal.EditorAssetLibrary.load_asset(p)
    if obj:
        lines.append(f"  {p} -> {obj.get_class().get_name()}")
        if isinstance(obj, unreal.SkeletalMesh):
            b = obj.get_bounds()
            lines.append(f"     Bounds extent: {b.box_extent}, origin: {b.origin}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
