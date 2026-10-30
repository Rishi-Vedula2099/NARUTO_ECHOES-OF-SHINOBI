import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\skel_bones.txt"
lines = []

sk_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP.TutorialTPP")
if sk_mesh:
    lines.append(f"Loaded SkeletalMesh: {sk_mesh.get_name()}")
    skel = sk_mesh.get_editor_property("skeleton")
    lines.append(f"Skeleton: {skel.get_name() if skel else 'None'}")
    
    # Check bone names
    ref_skeleton = skel.get_editor_property("reference_skeleton") if hasattr(skel, "reference_skeleton") else None
    
    # Print bones from mesh
    num_bones = unreal.SkeletalMeshLibrary.get_num_bones(sk_mesh) if hasattr(unreal, "SkeletalMeshLibrary") else 0
    lines.append(f"Num Bones: {num_bones}")
    if hasattr(unreal, "SkeletalMeshLibrary"):
        for i in range(num_bones):
            bname = unreal.SkeletalMeshLibrary.get_bone_name(sk_mesh, i)
            lines.append(f"Bone {i}: {bname}")
else:
    lines.append("Failed to load /Engine/Tutorial/SubEditors/TutorialAssets/Character/TutorialTPP")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
