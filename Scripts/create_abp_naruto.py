import unreal

AT = unreal.AssetToolsHelpers.get_asset_tools()
EAL = unreal.EditorAssetLibrary

folder = "/Game/Characters/Naruto/Anims"
abp_name = "ABP_Naruto_Genin"
pkg_path = f"{folder}/{abp_name}"

EAL.make_directory(folder)

abp = unreal.load_asset(pkg_path)
if not abp:
    factory = unreal.AnimBlueprintFactory()
    try:
        anim_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSAnimInstance")
        if anim_class:
            factory.set_editor_property("parent_class", anim_class)
            print(f"[OK] Set parent_class to {anim_class.get_name()}")
    except Exception as e:
        print(f"[WARN] Error setting parent_class: {e}")

    try:
        abp = AT.create_asset(abp_name, folder, unreal.AnimBlueprint, factory)
    except Exception as e:
        print(f"[INFO] create_asset AnimBlueprint: {e}")

if abp:
    print(f"[SUCCESS] Created/Loaded AnimBlueprint: {abp.get_path_name()}")
    EAL.save_asset(abp.get_path_name())
else:
    print(f"[INFO] AnimBlueprint requires skeletal rig; UEOSAnimInstance is the authoritative C++ movement state machine backend.")
