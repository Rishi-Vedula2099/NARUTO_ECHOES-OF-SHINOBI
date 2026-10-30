import csv
import pathlib

csv_path = 'Data/Visual/EOSVisualAssetRegistry.csv'
with open(csv_path, 'r', encoding='utf-8') as f:
    rows = list(csv.DictReader(f))

print(f"Total rows: {len(rows)}")
for i, r in enumerate(rows):
    print(f"{i+1:2d}. {r['AssetID']:<35} | {r['TargetPath']}")
