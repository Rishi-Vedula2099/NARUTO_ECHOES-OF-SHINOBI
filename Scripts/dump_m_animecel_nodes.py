import unreal

mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
out = []
out.append(f"Material: {mat.get_name()}")
exprs = mat.get_editor_property("expressions")
out.append(f"Total Expressions: {len(exprs)}")

for i, e in enumerate(exprs):
    cls = e.get_class().get_name()
    pname = ""
    try:
        pname = e.get_editor_property("parameter_name")
    except:
        pass
    desc = f" [{pname}]" if pname else ""
    out.append(f"  Expr {i:02d}: {cls}{desc}")
    # inspect properties
    for prop in ["default_value", "const_b", "const_a"]:
        try:
            val = e.get_editor_property(prop)
            out.append(f"    {prop}: {val}")
        except:
            pass

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\m_animecel_nodes.txt", "w") as f:
    f.write("\n".join(out))
unreal.log("DUMPED m_animecel_nodes.txt")
