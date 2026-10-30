import unreal

eal = unreal.EditorAssetLibrary

m1 = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
m2 = unreal.load_asset("/Game/Materials/CelShading/M_AnimeCel_Master")

out = []
out.append(f"M1 (/Game/Materials/Master/M_AnimeCel_Master): {m1.get_class().get_name() if m1 else 'None'}")
out.append(f"M2 (/Game/Materials/CelShading/M_AnimeCel_Master): {m2.get_class().get_name() if m2 else 'None'}")

# Check which parent is used by the Material Instances
mis = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone",
    "/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood"
]

for p in mis:
    inst = unreal.load_asset(p)
    if inst:
        out.append(f"{inst.get_name()}: Parent = {inst.parent.get_path_name() if inst.parent else 'None'}")
    else:
        out.append(f"{p}: NOT FOUND")

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\two_masters_check.txt", "w") as f:
    f.write("\n".join(out))
unreal.log("Wrote two_masters_check.txt")
