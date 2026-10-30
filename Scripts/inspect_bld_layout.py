import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

lines = []
for a in eas.get_all_level_actors():
    if not a: continue
    lbl = a.get_actor_label()
    if any(k in lbl.lower() for k in ["bld", "ichiraku", "residence", "camera", "player", "cedar", "sakura"]):
        loc = a.get_actor_location()
        rot = a.get_actor_rotation()
        smc = a.get_component_by_class(unreal.StaticMeshComponent)
        sm_name = smc.static_mesh.get_name() if (smc and smc.static_mesh) else "None"
        lines.append(f"{lbl}: Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}), Rot=(P={rot.pitch:.1f}, Y={rot.yaw:.1f}, R={rot.roll:.1f}), Mesh={sm_name}")

with open(r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\bld_layout_out.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
