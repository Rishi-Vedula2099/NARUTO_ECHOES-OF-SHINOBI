import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap")
hud = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.EOSShinobiHUD, unreal.Vector(0,0,0))
unreal.log(f"SPAWNED HUD: {hud}")

hud.set_target_boss_extended_info("Kakashi Hatake", "Copy Ninja", 0.75, 1, 2, 0.85, False)
unreal.log(f"AFTER EXTENDED: has_target={hud.has_active_target}, hp={hud.active_target_health_percent}")

hud.register_hit(8, 800.0)
unreal.log(f"AFTER HIT 8: hits={hud.current_combo_hits}, rank={hud.style_rank}")

hud.register_hit(7, 700.0)
unreal.log(f"AFTER HIT 15: hits={hud.current_combo_hits}, rank={hud.style_rank}")
