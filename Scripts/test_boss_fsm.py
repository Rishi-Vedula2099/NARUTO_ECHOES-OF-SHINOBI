import unreal

gi = unreal.new_object(unreal.GameInstance)
boss_sub = unreal.new_object(unreal.EOSBossEncounterSubsystem, gi)
boss_sub.initialize_golden_boss_registry()
zabuza_def = boss_sub.get_boss_encounter_definition('BOSS_ZABUZA')

boss = unreal.new_object(unreal.EOSBossCharacter)
boss.initialize_from_encounter_definition(zabuza_def)

state0 = boss.get_current_fsm_state()
unreal.log(f"STATE 0: val={state0}, type={type(state0)}, repr={repr(state0)}")
unreal.log(f"EOSBossState.INACTIVE: val={unreal.EOSBossState.INACTIVE}, repr={repr(unreal.EOSBossState.INACTIVE)}")

boss.trigger_entrance()
state1 = boss.get_current_fsm_state()
unreal.log(f"STATE 1: val={state1}, type={type(state1)}, repr={repr(state1)}")
unreal.log(f"EOSBossState.ENTRANCE: val={unreal.EOSBossState.ENTRANCE}, repr={repr(unreal.EOSBossState.ENTRANCE)}")
unreal.log(f"EQUAL? {state1 == unreal.EOSBossState.ENTRANCE}")
