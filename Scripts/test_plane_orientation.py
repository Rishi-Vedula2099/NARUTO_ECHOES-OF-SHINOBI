import unreal

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet")

for a in EAS.get_all_level_actors():
    if not a: continue
    if "northstar_vista" in a.get_actor_label().lower():
        fwd = a.get_actor_forward_vector()
        up  = a.get_actor_up_vector()
        rgt = a.get_actor_right_vector()
        rot = a.get_actor_rotation()
        unreal.log(f"Backdrop Vista Plane:")
        unreal.log(f"  Current Rot: pitch={rot.pitch}, yaw={rot.yaw}, roll={rot.roll}")
        unreal.log(f"  Forward (+X): ({fwd.x:.2f}, {fwd.y:.2f}, {fwd.z:.2f})")
        unreal.log(f"  Right   (+Y): ({rgt.x:.2f}, {rgt.y:.2f}, {rgt.z:.2f})")
        unreal.log(f"  Up      (+Z): ({up.x:.2f}, {up.y:.2f}, {up.z:.2f})")
