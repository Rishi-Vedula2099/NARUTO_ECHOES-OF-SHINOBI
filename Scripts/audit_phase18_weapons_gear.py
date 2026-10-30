"""
NARUTO: ECHOES OF SHINOBI
Phase 18: Weapons, Gear & Loadout Collection Audit & Verification Script
Contract Section 29 (Weapons / Gear) & Section 43 (Phase Order)
"""
import unreal
import sys

def audit_phase18():
    unreal.log("======================================================================")
    unreal.log("  PHASE 18 AUDIT: SHINOBI WEAPONS, GEAR & LOADOUT COLLECTION PIPELINE")
    unreal.log("======================================================================")

    # 1. Verify Subsystem Class
    unreal.log("----------------------------------------------------------------------")
    unreal.log("1. Checking Gear Subsystem Class:")
    gear_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSGearSubsystem")
    if not gear_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSGearSubsystem class.")
        sys.exit(1)

    gear_sub = unreal.new_object(gear_sub_class)
    gear_sub.initialize_default_gear_database()
    unreal.log(f"  [OK] Gear Subsystem Initialized: {gear_sub_class.get_name()}")

    # 2. Verify Canonical Weapons & Artifacts
    unreal.log("----------------------------------------------------------------------")
    unreal.log("2. Validating Canonical Weapons & Artifacts:")

    items_to_check = [
        ("WEAPON_KUSANAGI_SWORD", "Sword of Kusanagi", unreal.EOSMetaGearType.WEAPON),
        ("WEAPON_SAMEHADA", "Samehada Greatsword", unreal.EOSMetaGearType.WEAPON),
        ("ARTIFACT_UCHIHA_FIRE_FAN", "Uchiha Crest Flame Talisman", unreal.EOSMetaGearType.ARTIFACT),
        ("EQUIP_UCHIHA_COMBAT_ROBE", "Uchiha Combat Robe", unreal.EOSMetaGearType.EQUIPMENT)
    ]

    for item_id, expected_name, expected_type in items_to_check:
        b_found, item_record = gear_sub.get_gear_item(item_id)
        assert b_found, f"Item {item_id} not found in database"
        assert item_record.gear_type == expected_type, f"Item {item_id} type mismatch"
        unreal.log(f"  [OK] Item Verified: {item_record.display_name} ({item_id}) — Level: {item_record.enhancement_level}, Tier: {item_record.tier}")

    # 3. Test Enhancement & Stat Growth
    unreal.log("----------------------------------------------------------------------")
    unreal.log("3. Testing Gear Enhancement & Stat Growth:")
    b_enhanced = gear_sub.enhance_gear_item("WEAPON_KUSANAGI_SWORD", 5)
    assert b_enhanced, "Failed to enhance Kusanagi Sword"
    b_found, kusanagi = gear_sub.get_gear_item("WEAPON_KUSANAGI_SWORD")
    assert kusanagi.enhancement_level == 5, "Enhancement level must equal 5"
    unreal.log(f"  [OK] Kusanagi Sword enhanced to Level {kusanagi.enhancement_level} (Attack Bonus: +{kusanagi.bonus_stats.attack})")

    # 4. Test Set Bonus Evaluation
    unreal.log("----------------------------------------------------------------------")
    unreal.log("4. Testing Gear Set Bonus Resolution (Uchiha Legacy 2-pc & 4-pc):")
    b_found_set, set_bonus = gear_sub.get_gear_set("SET_UCHIHA_LEGACY")
    assert b_found_set, "SET_UCHIHA_LEGACY must be registered"
    unreal.log(f"  [OK] Gear Set Verified: {set_bonus.set_name} — 2-pc Bonus: {set_bonus.piece2_description}, 4-pc Bonus: {set_bonus.piece4_description}")

    unreal.log("======================================================================")
    unreal.log("  PHASE 18 WEAPONS & GEAR AUDIT RESULT: ALL CHECKS PASSED (100% OK)")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase18()
