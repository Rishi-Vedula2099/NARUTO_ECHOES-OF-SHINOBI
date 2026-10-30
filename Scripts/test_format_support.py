import unreal

p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\format_support.txt"
with open(p, "w") as f:
    f.write("=== CHECKING IMPORT FACTORIES ===\n")
    factories = [
        "FbxFactory", "FbxImportUI", "GLTFImportFactory", "InterchangeManager", 
        "SkeletalMesh", "Skeleton", "AnimationAsset", "AnimSequence", "BlendSpace"
    ]
    for m in factories:
        val = getattr(unreal, m, None)
        f.write(f"unreal.{m}: {val}\n")
    
    # Check what classes inherit from Factory
    f.write("\n=== FACTORIES IN UNREAL ===\n")
    for name in dir(unreal):
        if 'factory' in name.lower():
            f.write(f"  {name}\n")

print("Format support written.")
