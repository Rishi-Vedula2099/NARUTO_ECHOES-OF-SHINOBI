import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
boss = eas.spawn_actor_from_class(unreal.EOSBossCharacter, unreal.Vector(0,0,0))

gi = unreal.new_object(unreal.GameInstance)
boss_sub = unreal.new_object(unreal.EOSBossEncounterSubsystem, gi)
boss_sub.initialize_golden_boss_registry()
zabuza_def = boss_sub.get_boss_encounter_definition('BOSS_ZABUZA')

boss.initialize_from_encounter_definition(zabuza_def)
unreal.log(f"INITIAL: {boss.get_current_fsm_state()}")

boss.trigger_entrance()
unreal.log(f"AFTER ENTRANCE: {boss.get_current_fsm_state()}")

res_idle = boss.transition_to_state(unreal.EOSBossState.IDLE)
unreal.log(f"TRANSITION TO IDLE: {res_idle}, NOW: {boss.get_current_fsm_state()}")

eas.destroy_actor(boss)
