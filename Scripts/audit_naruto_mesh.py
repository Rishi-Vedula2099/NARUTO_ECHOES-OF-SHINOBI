import unreal

def audit_naruto():
    print("=== AUDIT NARUTO PRODUCTION ASSET (PHASE 3) ===")
    mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin"
    mesh = unreal.load_asset(mesh_path)
    if not mesh:
        print(f"[FAIL] Could not load mesh: {mesh_path}")
        return

    print(f"[OK] Loaded asset: {mesh.get_name()}")
    print(f"Class: {mesh.get_class().get_name()}")

    if isinstance(mesh, unreal.StaticMesh):
        num_sections = mesh.get_num_sections(0)
        print(f"LOD 0 section count: {num_sections}")
        for i in range(num_sections):
            mat = mesh.get_material(i)
            mat_name = mat.get_name() if mat else "None"
            mat_path = mat.get_path_name() if mat else "None"
            print(f"  Slot {i}: {mat_name} ({mat_path})")

        bounds = mesh.get_bounds()
        box_extent = bounds.box_extent
        sphere_radius = bounds.sphere_radius
        print(f"Bounding Box Extent: X={box_extent.x:.1f}, Y={box_extent.y:.1f}, Z={box_extent.z:.1f}")
        total_height = box_extent.z * 2.0
        print(f"Total Model Height: {total_height:.1f} cm (Target: ~145 cm)")
        
    skin_mesh_path = "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit"
    skin_mesh = unreal.load_asset(skin_mesh_path)
    if skin_mesh:
        print(f"\n[OK] Loaded skin asset: {skin_mesh.get_name()}")
        print(f"Class: {skin_mesh.get_class().get_name()}")
        if isinstance(skin_mesh, unreal.StaticMesh):
            print(f"LOD 0 sections: {skin_mesh.get_num_sections(0)}")
            for i in range(skin_mesh.get_num_sections(0)):
                mat = skin_mesh.get_material(i)
                mat_name = mat.get_name() if mat else "None"
                print(f"  Skin Slot {i}: {mat_name}")

audit_naruto()
