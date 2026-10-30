import unreal

im = unreal.InterchangeManager.get_interchange_manager_scripted()
formats = im.get_supported_formats()
p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\supported_formats.txt"
with open(p, "w") as f:
    for fmt in formats:
        f.write(f"{fmt}\n")
print("Formats written.")
