import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\interchange_props.txt"
lines = []

pipeline = unreal.InterchangeGenericMeshPipeline()
lines.append("InterchangeGenericMeshPipeline attributes:")
for p in dir(pipeline):
    if not p.startswith("_"):
        try:
            val = getattr(pipeline, p)
            lines.append(f"  {p} = {val}")
        except Exception as e:
            lines.append(f"  {p} = (error: {e})")

lines.append("\nAssetTools and Factory inspection:")
for k in dir(unreal):
    if "Interchange" in k or "Fbx" in k or "SkeletalMeshImport" in k:
        lines.append(f"  {k}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

unreal.log("Saved interchange_props.txt successfully")
