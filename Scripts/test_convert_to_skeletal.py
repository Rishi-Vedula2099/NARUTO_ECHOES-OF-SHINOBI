import unreal

print("=== TESTING SkeletalMeshFromStaticMeshFactory ===")
factory = unreal.SkeletalMeshFromStaticMeshFactory()
for prop in dir(factory):
    if not prop.startswith("_"):
        print(f"factory.{prop}")

p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\skel_factory_methods.txt"
with open(p, "w") as f:
    for prop in dir(factory):
        if not prop.startswith("_"):
            f.write(f"factory.{prop}\n")
