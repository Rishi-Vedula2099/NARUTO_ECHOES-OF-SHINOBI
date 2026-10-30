import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\abp_info.txt"
lines = []

abp = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")
if abp:
    lines.append(f"Loaded ABP: {abp.get_name()} ({abp.get_class().get_name()})")
    target_skel = abp.get_editor_property("target_skeleton")
    lines.append(f"Target Skeleton: {target_skel.get_path_name() if target_skel else 'None'}")
else:
    lines.append("Failed to load ABP_Naruto_Genin")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
