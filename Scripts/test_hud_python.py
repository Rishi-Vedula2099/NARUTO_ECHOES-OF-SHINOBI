import unreal

hud = unreal.new_object(unreal.EOSShinobiHUD)
unreal.log("TARGET ATTRS: " + str([x for x in dir(hud) if 'target' in x.lower()]))

unreal.log(f"BEFORE: has_active_target={getattr(hud, 'has_active_target', None)}")
unreal.log(f"BEFORE: b_has_active_target={getattr(hud, 'b_has_active_target', None)}")

hud.set_target_boss_extended_info("Kakashi Hatake", "Copy Ninja", 0.75, 1, 2, 0.85, False)

unreal.log(f"AFTER: has_active_target={getattr(hud, 'has_active_target', None)}")
if callable(getattr(hud, 'has_active_target', None)):
    unreal.log(f"CALLING: has_active_target()={hud.has_active_target()}")
unreal.log(f"AFTER: b_has_active_target={getattr(hud, 'b_has_active_target', None)}")
