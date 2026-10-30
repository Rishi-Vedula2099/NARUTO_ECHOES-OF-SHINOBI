import unreal
for p in ['/Game/Characters/Naruto/Meshes/SK_Naruto_Genin',
          '/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit',
          '/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen',
          '/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A',
          '/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large']:
    m = unreal.load_asset(p)
    if m:
        unreal.log(f'=== {p} ({m.get_num_sections(0)} sections) ===')
        for i in range(m.get_num_sections(0)):
            mat = m.get_material(i)
            mat_name = mat.get_name() if mat else 'None'
            slot_name = m.get_section_material_slot_name(0, i) if hasattr(m, 'get_section_material_slot_name') else ''
            unreal.log(f'  Slot {i}: slot={slot_name}, mat={mat_name}')
