import unreal

MEL = unreal.MaterialEditingLibrary

funcs = [f for f in dir(MEL) if not f.startswith("_")]
print("=== MaterialEditingLibrary Functions ===")
for f in funcs:
    print(f"  {f}")
