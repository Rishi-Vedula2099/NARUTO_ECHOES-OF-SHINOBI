import unreal
import os

p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'asset_classes.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    for pkg in [
        '/Game/Characters/Naruto/Meshes/SK_Naruto_Genin',
        '/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen',
        '/Game/Materials/CelShading/M_AnimeCel_Master',
        '/Game/Characters/Naruto/Materials/MI_Naruto_Genin',
    ]:
        obj = unreal.load_asset(pkg)
        cls_name = obj.get_class().get_name() if obj else "None"
        f.write(f'{pkg}: {cls_name}\n')
