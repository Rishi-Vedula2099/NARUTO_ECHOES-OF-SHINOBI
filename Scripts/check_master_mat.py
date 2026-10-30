import unreal

mat = unreal.load_asset("/Game/Materials/Master/M_AnimeCel_Master")
blend = mat.get_editor_property("blend_mode")
shading = mat.get_editor_property("shading_model")
two_sided = mat.get_editor_property("two_sided")

unreal.log(f">>> MAT_CHECK: BlendMode={blend}, ShadingModel={shading}, TwoSided={two_sided} <<<")
with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\mat_props.txt", "w") as f:
    f.write(f"BlendMode={blend}\nShadingModel={shading}\nTwoSided={two_sided}\n")
