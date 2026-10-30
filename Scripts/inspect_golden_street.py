import unreal

def inspect_street():
    pkg = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
    world = unreal.EditorLoadingAndSavingUtils.load_map(pkg)
    actors = unreal.EditorLevelLibrary.get_all_level_actors()
    unreal.log(f"=== INSPECTING {pkg}: {len(actors)} Actors Found ===")
    for a in actors:
        label = a.get_actor_label()
        loc = a.get_actor_location()
        rot = a.get_actor_rotation()
        unreal.log(f"  * [{a.get_class().get_name()}] '{label}' Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}) Rot=(P={rot.pitch:.1f}, Y={rot.yaw:.1f}, R={rot.roll:.1f})")

if __name__ == "__main__":
    inspect_street()
