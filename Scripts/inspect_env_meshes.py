import unreal

def inspect_assets():
    mesh_paths = [
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion",
        "/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
        "/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_KonohaFrontierGate",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_WaterPumpingStation",
        "/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin",
        "/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
        "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Cedar_Large",
        "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large",
        "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom",
        "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01",
        "/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized",
    ]

    unreal.log("=== INSPECTING STATIC MESH SECTIONS & MATERIALS ===")
    for p in mesh_paths:
        mesh = unreal.load_asset(p)
        if not mesh:
            unreal.log_warning(f"MISSING: {p}")
            continue
        num_mats = len(mesh.static_materials)
        mat_slots = []
        for i, sm in enumerate(mesh.static_materials):
            slot_name = sm.material_slot_name
            m_interface = sm.material_interface
            m_name = m_interface.get_name() if m_interface else "None"
            mat_slots.append(f"[{i}:{slot_name} -> {m_name}]")
        unreal.log(f"{mesh.get_name()} ({num_mats} slots): {', '.join(mat_slots)}")

inspect_assets()
