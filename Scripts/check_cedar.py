import unreal
mesh = unreal.load_asset(/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large)
if mesh:
    for i in range(mesh.get_num_sections(0)):
        mat = mesh.get_material(i)
        unreal.log(fCedar Section {i}: {mat.get_name() if mat else 'None'})
