import unreal

hud = unreal.new_object(unreal.EOSShinobiHUD)
unreal.log(f"NAME BEFORE: {hud.active_target_name}")
hud.set_target_boss_info("Zabuza", 0.5)
unreal.log(f"NAME AFTER INFO: {hud.active_target_name}, has_target={hud.has_active_target}")
hud.set_target_boss_extended_info("Kakashi", "Copy Ninja", 0.75, 1, 2, 0.85, False)
unreal.log(f"NAME AFTER EXTENDED: {hud.active_target_name}, has_target={hud.has_active_target}")
