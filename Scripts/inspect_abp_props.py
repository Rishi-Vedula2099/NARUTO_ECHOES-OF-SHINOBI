import unreal

factory = unreal.AnimBlueprintFactory()
print("=== AnimBlueprintFactory Properties ===")
for p in dir(factory):
    if not p.startswith("_"):
        print(" ", p)
