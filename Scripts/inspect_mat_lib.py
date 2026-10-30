import unreal, os
p = os.path.join(unreal.SystemLibrary.get_project_directory(), 'Saved', 'mat_lib_dump.txt')
with open(p, 'w') as f:
    f.write('MaterialEditingLibrary:\n')
    for m in sorted(dir(unreal.MaterialEditingLibrary)):
        f.write(f'  {m}\n')
    f.write('\nMaterialProperty:\n')
    for m in sorted(dir(unreal.MaterialProperty)):
        f.write(f'  {m}\n')
