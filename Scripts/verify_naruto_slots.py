import unreal

mesh = unreal.load_asset("/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit")
print(f"Mesh: {mesh}")
if mesh:
    num_sections = mesh.get_num_sections(0)
    print(f"Num sections: {num_sections}")
    p = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\naruto_verified_slots.txt"
    with open(p, "w") as f:
        f.write(f"Num sections: {num_sections}\n")
        for i in range(num_sections):
            sm = mesh.static_materials[i]
            mat = mesh.get_material(i)
            mat_name = mat.get_name() if mat else "None"
            f.write(f"Slot {i} [{sm.material_slot_name}]: {mat_name}\n")
            print(f"Slot {i} [{sm.material_slot_name}]: {mat_name}")
