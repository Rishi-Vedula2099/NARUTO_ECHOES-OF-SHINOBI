
import unreal, os
p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'api_dump.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    les = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)
    f.write(f'LevelEditorSubsystem: {les}\n')
    for m in sorted(dir(les)):
        f.write(f'  les.{m}\n')
    f.write('\nEditorLoadingAndSavingUtils:\n')
    for m in sorted(dir(unreal.EditorLoadingAndSavingUtils)):
        f.write(f'  utils.{m}\n')
