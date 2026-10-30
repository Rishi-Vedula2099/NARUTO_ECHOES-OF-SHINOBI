import unreal

def inspect_material(path):
    mat = unreal.load_asset(path)
    if not mat:
        print(f"FAILED TO LOAD: {path}")
        return
    print(f"=== MATERIAL: {path} ===")
    print(f"Class: {mat.get_class().get_name()}")
    try:
        exprs = mat.get_editor_property("expressions")
        print(f"Number of expressions: {len(exprs)}")
        for e in exprs:
            if hasattr(e, "get_editor_property"):
                name = ""
                try:
                    name = e.get_editor_property("parameter_name")
                except:
                    pass
                desc = f" ({name})" if name else ""
                print(f"  Expression: {e.get_class().get_name()}{desc}")
    except Exception as ex:
        print(f"Could not read expressions: {ex}")

inspect_material("/Game/Materials/Master/M_AnimeCel_Master")
inspect_material("/Game/Materials/CelShading/M_AnimeCel_Master")
