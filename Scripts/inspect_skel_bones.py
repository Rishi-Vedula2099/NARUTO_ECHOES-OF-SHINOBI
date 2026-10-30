import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\inspect_skel_bones.txt"
lines = []

skel = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Skeleton")
sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")

lines.append(f"Skeleton: {skel}")
lines.append(f"SkeletalMesh: {sk_mesh}")

if sk_mesh:
    ref_skel = sk_mesh.get_editor_property("skeleton")
    lines.append(f"Mesh ref skeleton: {ref_skel}")
    # Inspect bounds
    bounds = sk_mesh.get_bounds()
    lines.append(f"Mesh bounds box extent: {bounds.box_extent}")
    lines.append(f"Mesh bounds origin: {bounds.origin}")
    lines.append(f"Mesh bounds sphere radius: {bounds.sphere_radius}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
