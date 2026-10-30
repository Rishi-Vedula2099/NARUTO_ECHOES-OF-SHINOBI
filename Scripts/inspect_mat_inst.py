import unreal

mi = unreal.load_asset("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone")
if mi:
    unreal.log(f"MI_Road_Cobblestone: Class={mi.get_class().get_name()}")
    unreal.log(f"  Parent: {mi.parent.get_path_name() if hasattr(mi, 'parent') and mi.parent else 'None'}")
    # Inspect parameters
    for sp in unreal.MaterialEditingLibrary.get_scalar_parameter_names(mi):
        v = unreal.MaterialEditingLibrary.get_material_instance_scalar_parameter_value(mi, sp)
        unreal.log(f"  ScalarParam: {sp} = {v}")
    for vp in unreal.MaterialEditingLibrary.get_vector_parameter_names(mi):
        v = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(mi, vp)
        unreal.log(f"  VectorParam: {vp} = ({v.r}, {v.g}, {v.b}, {v.a})")
    for tp in unreal.MaterialEditingLibrary.get_texture_parameter_names(mi):
        v = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(mi, tp)
        unreal.log(f"  TextureParam: {tp} = {v.get_path_name() if v else 'None'}")
else:
    unreal.log("Could not load MI_Road_Cobblestone")

naruto_mat = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit")
if naruto_mat:
    unreal.log(f"MI_Naruto_Tracksuit: Class={naruto_mat.get_class().get_name()}")
    unreal.log(f"  Parent: {naruto_mat.parent.get_path_name() if hasattr(naruto_mat, 'parent') and naruto_mat.parent else 'None'}")
    for vp in unreal.MaterialEditingLibrary.get_vector_parameter_names(naruto_mat):
        v = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(naruto_mat, vp)
        unreal.log(f"  VectorParam: {vp} = ({v.r}, {v.g}, {v.b}, {v.a})")
