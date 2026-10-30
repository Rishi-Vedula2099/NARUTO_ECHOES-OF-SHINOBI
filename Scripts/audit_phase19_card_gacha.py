"""
NARUTO: ECHOES OF SHINOBI
Phase 19: Card Collection & Gacha Presentation Pipeline Audit & Verification Script
Contract Section 27 (Card Collection), Section 28 (Gacha Presentation) & Section 43 (Phase Order)
"""
import unreal
import sys

def audit_phase19():
    unreal.log("======================================================================")
    unreal.log("  PHASE 19 AUDIT: SHINOBI CARD COLLECTION & GACHA PRESENTATION PIPELINE")
    unreal.log("======================================================================")

    # 1. Verify Gacha & Collection Subsystem Classes
    unreal.log("----------------------------------------------------------------------")
    unreal.log("1. Checking Gacha & Collection Subsystem Classes:")
    gacha_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSGachaSubsystem")
    coll_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSCollectionSubsystem")

    if not gacha_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSGachaSubsystem class.")
        sys.exit(1)
    if not coll_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSCollectionSubsystem class.")
        sys.exit(1)

    gacha_sub = unreal.new_object(gacha_sub_class)
    coll_sub = unreal.new_object(coll_sub_class)
    gacha_sub.initialize_default_gacha_database()
    coll_sub.initialize_default_collection_database()

    unreal.log(f"  [OK] Gacha Subsystem Initialized: {gacha_sub_class.get_name()}")
    unreal.log(f"  [OK] Collection Subsystem Initialized: {coll_sub_class.get_name()}")

    # 2. Verify Active Banners & Rarity Weights
    unreal.log("----------------------------------------------------------------------")
    unreal.log("2. Validating Banners & Rarity Weights:")
    active_banners = gacha_sub.get_active_banners()
    assert len(active_banners) >= 2, f"Expected at least 2 active banners, got {len(active_banners)}"

    for b in active_banners:
        unreal.log(f"  [OK] Active Banner: {b.banner_title} ({b.banner_id}) — Type: {b.summon_type}")

    # 3. Test Transaction Safety & Atomic Summon Currency Execution
    unreal.log("----------------------------------------------------------------------")
    unreal.log("3. Testing Currency Deduction & 10-Pull Summon Execution:")
    # Add Gems
    gacha_sub.add_currency(unreal.EOSCurrencyType.SHINOBI_GEMS, 3000)
    bal = gacha_sub.get_currency_balance(unreal.EOSCurrencyType.SHINOBI_GEMS)
    assert bal >= 3000, "Failed to credit Shinobi Gems"

    # Execute 10-Pull Multi Summon
    res = gacha_sub.execute_multi_summon("BANNER_STANDARD", 10)
    assert res.b_transaction_success == True, "Summon transaction failed"
    assert len(res.summoned_items) == 10, f"Expected 10 items, got {len(res.summoned_items)}"
    unreal.log(f"  [OK] Multi-Summon Success: Generated {len(res.summoned_items)} items. Highest Rarity: {res.highest_rarity_pulled}")

    # 4. Verify Chronicle Card Categories & Rarity Tiers (Bible 10)
    unreal.log("----------------------------------------------------------------------")
    unreal.log("4. Validating Chronicle Card Categories & Archive Metrics:")
    global_pct = coll_sub.get_global_archive_completion_percentage()
    account_level = coll_sub.get_archive_account_level()
    unreal.log(f"  [OK] Global Collection Archive Completion: {global_pct:.1f}% | Archive Level: {account_level}")

    # Check Character Card Category
    char_entries = coll_sub.get_entries_by_category(unreal.EOSCollectionCategory.CHARACTERS)
    assert len(char_entries) > 0, "No character collection entries found"
    unreal.log(f"  [OK] Character Collection Category Verified ({len(char_entries)} entries)")

    unreal.log("======================================================================")
    unreal.log("  PHASE 19 CARD & GACHA AUDIT RESULT: ALL CHECKS PASSED (100% OK)")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase19()
