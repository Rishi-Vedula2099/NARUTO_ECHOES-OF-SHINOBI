import unreal

eal = unreal.EditorAssetLibrary

m1 = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
m2 = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Textured_Master")

print(f"M_AnimeCel_Master: {m1 is not None}")
print(f"M_AnimeCel_Textured_Master: {m2 is not None}")

# Check MI_Road_Cobblestone
mi_road = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
if mi_road:
    print(f"MI_Road_Cobblestone parent: {mi_road.get_editor_property('parent')}")

# Check MI_Naruto_Tracksuit
mi_naruto = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
if mi_naruto:
    print(f"MI_Naruto_Tracksuit parent: {mi_naruto.get_editor_property('parent')}")
