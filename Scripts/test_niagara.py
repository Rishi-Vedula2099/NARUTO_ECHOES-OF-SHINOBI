
import unreal, os
p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'niagara_test.txt')
os.makedirs(os.path.dirname(p), exist_ok=True)
with open(p, 'w') as f:
    f.write('Niagara in unreal:\n')
    for m in dir(unreal):
        if 'niagara' in m.lower():
            f.write(f'  {m}\n')
