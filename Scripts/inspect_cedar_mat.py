import unreal
mesh = unreal.EditorAssetLibrary.load_asset('/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large')
if mesh:
    for i, mat in enumerate(mesh.static_materials):
        unreal.log(f'Slot {i}: slot_name={mat.material_slot_name}, mat={mat.material_interface}')
else:
    unreal.log_error('Could not load SM_Veg_KonohaCedar_Large')
