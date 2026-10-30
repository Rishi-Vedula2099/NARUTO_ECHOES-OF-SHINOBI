import unreal

p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\skel_import_test.txt"
with open(p, "w") as f:
    f.write("=== SKELETAL IMPORT TEST ===\n")
    opts = unreal.FbxImportUI()
    f.write(f"opts.import_as_skeletal: {opts.import_as_skeletal}\n")
    f.write(f"opts.mesh_type_to_import: {opts.mesh_type_to_import}\n")
    for prop in dir(opts):
        if not prop.startswith("_"):
            f.write(f"  opts.{prop}\n")
    
    im = unreal.InterchangeManager.get_interchange_manager_scripted()
    f.write(f"\nInterchangeManager: {im}\n")
    for m in dir(im):
        if not m.startswith("_"):
            f.write(f"  im.{m}\n")

print("Wrote skel_import_test.txt")
