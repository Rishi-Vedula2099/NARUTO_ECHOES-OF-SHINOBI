import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap")
hud = unreal.new_object(unreal.EOSShinobiHUD, world)

unreal.log(f"BEFORE: target_name={hud.active_target_name}, has_target={hud.has_active_target}")
try:
    hud.set_target_boss_extended_info("Kakashi", "Copy Ninja", 0.75, 1, 2, 0.85, False)
    unreal.log(f"AFTER FUNCTION CALL: target_name={hud.active_target_name}, has_target={hud.has_active_target}")
except Exception as e:
    unreal.log(f"FUNCTION ERROR: {e}")

try:
    hud.set_editor_property("b_has_active_target", True)
    unreal.log(f"SET EDITOR PROPERTY b_has_active_target SUCCESS: {hud.has_active_target}")
except Exception as e:
    unreal.log(f"SET EDITOR PROPERTY b_has_active_target ERROR: {e}")

try:
    hud.set_editor_property("has_active_target", True)
    unreal.log(f"SET EDITOR PROPERTY has_active_target SUCCESS: {hud.has_active_target}")
except Exception as e:
    unreal.log(f"SET EDITOR PROPERTY has_active_target ERROR: {e}")
