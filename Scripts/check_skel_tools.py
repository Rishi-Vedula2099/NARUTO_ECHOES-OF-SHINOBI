import unreal
import os

p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'skel_tools.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    for m in dir(unreal):
        if 'skeletal' in m.lower():
            f.write(f'{m}\n')
