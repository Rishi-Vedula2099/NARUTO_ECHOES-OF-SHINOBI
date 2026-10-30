import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\interchange_mgr.txt"
lines = []

mgr = unreal.InterchangeManager.get_interchange_manager_scripted()
lines.append(f"InterchangeManager: {mgr}")
for m in dir(mgr):
    if not m.startswith("_"):
        lines.append(f"  {m}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
