import unreal

boss = unreal.new_object(unreal.EOSBossCharacter)
unreal.log(f"INITIAL: {boss.get_current_fsm_state()}")

res_entrance = boss.transition_to_state(unreal.EOSBossState.ENTRANCE)
unreal.log(f"TRANSITION TO ENTRANCE RES: {res_entrance}, STATE NOW: {boss.get_current_fsm_state()}")

res_idle = boss.transition_to_state(unreal.EOSBossState.IDLE)
unreal.log(f"TRANSITION TO IDLE RES: {res_idle}, STATE NOW: {boss.get_current_fsm_state()}")
