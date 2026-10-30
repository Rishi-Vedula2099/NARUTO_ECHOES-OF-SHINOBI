import unreal

def inspect():
    paths = [
        "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin",
        "/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit",
        "/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen",
        "/Game/Environment/Konoha/Roads/Meshes/SM_Road_Paved_01",
        "/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument",
        "/Game/Materials/Master/M_AnimeCel_Master",
        "/Game/Maps/GoldenTest/L_VisualAssetGoldenTest",
    ]
    for p in paths:
        a = unreal.load_asset(p)
        if a:
            unreal.log(f"[INSPECT] {p} -> Class: {a.get_class().get_name()}")
        else:
            unreal.log_warning(f"[INSPECT] FAILED TO LOAD: {p}")

inspect()
