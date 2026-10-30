import unreal
import os

p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'subsystem_info.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    sub = unreal.get_editor_subsystem(unreal.SkeletalMeshEditorSubsystem)
    f.write(f'Subsystem: {sub}\n')
    for m in dir(sub):
        f.write(f'  {m}\n')
