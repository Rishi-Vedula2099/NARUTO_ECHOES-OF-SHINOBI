import unreal
import os

p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'factory_info.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    factory = unreal.SkeletalMeshFromStaticMeshFactory()
    f.write(f'Factory: {factory}\n')
    for m in dir(factory):
        f.write(f'  {m}\n')
