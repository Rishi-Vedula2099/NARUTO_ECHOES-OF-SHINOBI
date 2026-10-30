import csv
import pathlib

csv_path = 'Data/Visual/EOSVisualAssetRegistry.csv'
with open(csv_path, 'r', encoding='utf-8') as f:
    rows = list(csv.DictReader(f))

content = pathlib.Path('Content')
found = []
missing = []

for r in rows:
    rel_path = r['TargetPath'].replace('/Game/', '').replace('\\', '/')
    full_path = content / rel_path
    if full_path.exists():
        found.append((r['AssetID'], r['TargetPath'], full_path.stat().st_size))
    else:
        missing.append((r['AssetID'], r['TargetPath']))

print(f"=== REGISTRY AUDIT STATUS ===")
print(f"Total Registry Entries: {len(rows)}")
print(f"Authored & Imported Assets (PASS): {len(found)}")
print(f"Pending Assets (MISSING_PACKAGE): {len(missing)}")
print("\n--- PASSED ASSETS ---")
for asset_id, target_path, sz in found:
    print(f"  [PASS] {asset_id:<32} {target_path} ({sz} bytes)")
