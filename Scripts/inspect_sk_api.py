import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\skel_transforms.txt"
lines = []

sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin")

# In UE5 Python, we can inspect bone transforms from USkeletalMesh:
# sk_mesh has get_ref_pose_matrix(bone_index) or similar
lines.append(f"SK_Naruto_Genin: {sk_mesh}")
for prop in dir(sk_mesh):
    if "bone" in prop.lower() or "socket" in prop.lower() or "ref" in prop.lower():
        lines.append(f"  {prop}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
