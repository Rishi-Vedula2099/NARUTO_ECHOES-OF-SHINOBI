"""
NARUTO: ECHOES OF SHINOBI
Phase 9: Hand Signs + QTE + Chakra Control Audit & Verification Script
"""
import unreal

def audit_phase9():
    unreal.log("======================================================================")
    unreal.log("  PHASE 9 AUDIT: HAND SIGNS + QTE + CHAKRA CONTROL VERIFICATION")
    unreal.log("======================================================================")

    # 1. Initialize Jutsu Trial Subsystem
    subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    
    gi = unreal.new_object(unreal.GameInstance)
    trial_sub = unreal.new_object(unreal.EOSJutsuTrialSubsystem, outer=gi)
    trial_sub.initialize_canon_jutsu_registry()

    canon_jutsu = [
        "JUT_FIREBALL",
        "JUT_CHIDORI",
        "JUTSU_NARUTO_RASENGAN",
        "JUTSU_NARUTO_SHADOW_CLONE",
        "JUTSU_WATER_DRAGON"
    ]

    all_registered = True
    for jid in canon_jutsu:
        record = trial_sub.get_jutsu_record(jid)
        if record and record.jutsu_id:
            num_signs = len(record.hand_sign_sequence.required_signs)
            method = record.learning_method
            window = record.hand_sign_sequence.execution_window_seconds
            gauge_min = record.chakra_model.target_gauge_min
            gauge_max = record.chakra_model.target_gauge_max
            unreal.log(f"  [CANON JUTSU] {jid}: Name='{record.jutsu_name}', Method={method}, Signs={num_signs}, Window={window:.1f}s, Gauge=[{gauge_min:.0f}-{gauge_max:.0f}]")
        else:
            unreal.log_error(f"  [MISSING] Failed to find registered jutsu {jid}")
            all_registered = False

    # 2. Test Hand Sign QTE Validation
    unreal.log("----------------------------------------------------------------------")
    unreal.log("Testing Hand-Sign QTE Validation:")

    # Chidori: Ox, Hare, Monkey (3 signs, 2.5s window)
    correct_chidori = [unreal.EOSHandSign.OX, unreal.EOSHandSign.HARE, unreal.EOSHandSign.MONKEY]
    wrong_chidori   = [unreal.EOSHandSign.OX, unreal.EOSHandSign.TIGER, unreal.EOSHandSign.MONKEY]

    pass_correct = trial_sub.validate_hand_sign_sequence("JUT_CHIDORI", correct_chidori, 1.8)
    pass_wrong   = trial_sub.validate_hand_sign_sequence("JUT_CHIDORI", wrong_chidori, 1.8)
    pass_timeout = trial_sub.validate_hand_sign_sequence("JUT_CHIDORI", correct_chidori, 3.2)

    qte_passed = pass_correct and (not pass_wrong) and (not pass_timeout)
    unreal.log(f"  Chidori Sequence QTE: Correct={pass_correct}, WrongRejected={not pass_wrong}, TimeoutRejected={not pass_timeout} -> {'PASS' if qte_passed else 'FAIL'}")

    # Fireball: Snake, Ram, Monkey, Boar, Horse, Tiger (6 signs, 3.5s window)
    correct_fireball = [
        unreal.EOSHandSign.SERPENT, unreal.EOSHandSign.RAM, unreal.EOSHandSign.MONKEY,
        unreal.EOSHandSign.BOAR, unreal.EOSHandSign.HORSE, unreal.EOSHandSign.TIGER
    ]
    wrong_fireball = [
        unreal.EOSHandSign.SERPENT, unreal.EOSHandSign.RAM, unreal.EOSHandSign.TIGER
    ]
    pass_fireball_correct = trial_sub.validate_hand_sign_sequence("JUT_FIREBALL", correct_fireball, 2.6)
    pass_fireball_wrong   = trial_sub.validate_hand_sign_sequence("JUT_FIREBALL", wrong_fireball, 2.6)

    fireball_passed = pass_fireball_correct and (not pass_fireball_wrong)
    unreal.log(f"  Fireball 6-Sign Sequence: Correct={pass_fireball_correct}, IncompleteRejected={not pass_fireball_wrong} -> {'PASS' if fireball_passed else 'FAIL'}")

    # 3. Test Chakra Control QTE
    unreal.log("----------------------------------------------------------------------")
    unreal.log("Testing Chakra Control Gauge Precision:")

    # Rasengan: Target 70-95, Hold 2.0s
    pass_chakra_stable  = trial_sub.validate_chakra_control("JUTSU_NARUTO_RASENGAN", 82.0, 2.2)
    pass_chakra_too_low = trial_sub.validate_chakra_control("JUTSU_NARUTO_RASENGAN", 50.0, 2.2)
    pass_chakra_short   = trial_sub.validate_chakra_control("JUTSU_NARUTO_RASENGAN", 82.0, 1.0)

    chakra_passed = pass_chakra_stable and (not pass_chakra_too_low) and (not pass_chakra_short)
    unreal.log(f"  Rasengan Chakra Control: StableInBand={pass_chakra_stable}, LowRejected={not pass_chakra_too_low}, ShortDurationRejected={not pass_chakra_short} -> {'PASS' if chakra_passed else 'FAIL'}")

    # 4. Test Failure Flow & Practice XP Retention
    unreal.log("----------------------------------------------------------------------")
    unreal.log("Testing Failure Flow & Practice XP Retention:")

    res_fail = trial_sub.process_trial_result("JUT_CHIDORI", False, 75.0)
    fail_rec = trial_sub.get_jutsu_record("JUT_CHIDORI")
    unreal.log(f"  Attempt 1 (Fail): State={res_fail}, RetainedPracticeXP={fail_rec.practice_xp}")

    res_pass = trial_sub.process_trial_result("JUT_CHIDORI", True, 100.0)
    pass_rec = trial_sub.get_jutsu_record("JUT_CHIDORI")
    unreal.log(f"  Attempt 2 (Retry Success): State={res_pass}, TotalPracticeXP={pass_rec.practice_xp}, Mastery={pass_rec.mastery_band}")

    progression_passed = (res_fail == unreal.EOSJutsuTrialState.PRACTICE) and (fail_rec.practice_xp == 75.0) and (res_pass == unreal.EOSJutsuTrialState.MASTERED) and (pass_rec.practice_xp == 175.0)

    # 5. Final Summary
    overall_pass = all_registered and qte_passed and fireball_passed and chakra_passed and progression_passed
    unreal.log("======================================================================")
    unreal.log(f"  PHASE 9 VERIFICATION RESULT: {'PASS' if overall_pass else 'FAIL'}")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase9()
