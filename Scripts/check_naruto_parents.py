import unreal

paths = [
    "/Game/Characters/Naruto/Materials/MI_Naruto_Skin",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Hair",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Emblem",
    "/Game/Characters/Naruto/Materials/MI_Naruto_Metal",
]

for p in paths:
    mi = unreal.load_asset(p)
    if mi:
        parent = mi.get_editor_property("parent")
        print(f"Material: {mi.get_name()} -> Parent: {parent.get_path_name() if parent else 'None'}")
        if parent:
            try:
                parent.set_editor_property("used_with_skeletal_mesh", True)
                unreal.MaterialEditingLibrary.recompile_material(parent)
                unreal.EditorAssetLibrary.save_loaded_asset(parent)
                print(f"Set used_with_skeletal_mesh=True on parent: {parent.get_name()}")
            except Exception as e:
                print(f"Parent flag exception: {e}")

# Check also both M_AnimeCel_Master files
for m_path in ["/Game/Materials/Master/M_AnimeCel_Master", "/Game/Materials/CelShading/M_AnimeCel_Master"]:
    m = unreal.load_asset(m_path)
    if m:
        try:
            m.set_editor_property("used_with_skeletal_mesh", True)
            unreal.MaterialEditingLibrary.recompile_material(m)
            unreal.EditorAssetLibrary.save_loaded_asset(m)
            print(f"Set used_with_skeletal_mesh=True on {m_path}")
        except Exception as e:
            print(f"Exception on {m_path}: {e}")
