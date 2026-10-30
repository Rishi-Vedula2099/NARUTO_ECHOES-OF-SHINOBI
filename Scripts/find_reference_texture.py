import unreal

eal = unreal.EditorAssetLibrary

p1 = "/Game/Art/References/Phase13/Konoha_Visual_North_Star"
p2 = "/Game/Textures/References/Konoha_Visual_North_Star"
p3 = "/Game/Art/References/Konoha_Visual_North_Star"

print(f"p1 exists: {eal.does_asset_exist(p1)}")
print(f"p2 exists: {eal.does_asset_exist(p2)}")
print(f"p3 exists: {eal.does_asset_exist(p3)}")

# Search for any asset with North_Star in its name
reg = unreal.AssetRegistryHelpers.get_asset_registry()
filter = unreal.ARFilter(package_paths=["/Game"], recursive_paths=True)
assets = reg.get_assets(filter)
for a in assets:
    if "north_star" in str(a.package_name).lower() or "reference" in str(a.package_name).lower():
        print(f"FOUND ASSET: {a.package_name} ({a.asset_class_path.asset_name})")
