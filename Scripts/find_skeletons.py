import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\skeletons_found.txt"
lines = []

ar = unreal.AssetRegistryHelpers.get_asset_registry()
# Find all Skeleton assets
assets = ar.get_assets_by_class("Skeleton", True)
lines.append(f"Found {len(assets)} Skeleton assets:")
for a in assets:
    lines.append(f"  {a.package_name}.{a.asset_name}")

# Also find all SkeletalMesh assets
sk_assets = ar.get_assets_by_class("SkeletalMesh", True)
lines.append(f"\nFound {len(sk_assets)} SkeletalMesh assets:")
for a in sk_assets:
    lines.append(f"  {a.package_name}.{a.asset_name}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
