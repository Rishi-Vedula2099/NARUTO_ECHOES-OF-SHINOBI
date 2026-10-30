import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_abp_content.txt"
lines = []

abp = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")
lines.append(f"ABP: {abp}")

if abp:
    skel = abp.get_editor_property("target_skeleton")
    lines.append(f"Target Skeleton: {skel}")
    # Inspect anim nodes / graphs
    lines.append("Properties of ABP:")
    for p in dir(abp):
        if not p.startswith("_"):
            try:
                v = getattr(abp, p)
                if not callable(v):
                    lines.append(f"  {p} = {v}")
            except:
                pass

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
