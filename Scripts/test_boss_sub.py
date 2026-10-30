import unreal

gi = unreal.new_object(unreal.GameInstance)
boss_sub = unreal.new_object(unreal.EOSBossEncounterSubsystem, gi)
unreal.log(f"CREATED BOSS SUB: {boss_sub}")

boss_sub.initialize_golden_boss_registry()
unreal.log(f"IS REGISTERED: {boss_sub.is_boss_registered('BOSS_ZABUZA')}")
zabuza = boss_sub.get_boss_encounter_definition('BOSS_ZABUZA')
unreal.log(f"ZABUZA NAME: {zabuza.boss_name}, PHASES: {len(zabuza.phases)}")
