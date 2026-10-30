"""
NARUTO: ECHOES OF SHINOBI
Phase 20 & 21: Global World Factory, Lands, Villages & Special Realms Audit Script
Contract Section 30 (World Factory) & Section 43 (Phase Order)
"""
import unreal
import sys

def audit_phase20_21():
    unreal.log("======================================================================")
    unreal.log("  PHASE 20 & 21 AUDIT: GLOBAL WORLD FACTORY, LANDS, VILLAGES & REALMS")
    unreal.log("======================================================================")

    # 1. Verify World Subsystem Class
    unreal.log("----------------------------------------------------------------------")
    unreal.log("1. Checking World Subsystem Database Initialization:")
    world_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSWorldSubsystem")
    if not world_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSWorldSubsystem class.")
        sys.exit(1)

    world_sub = unreal.new_object(world_sub_class)
    world_sub.initialize_full_world_database()
    unreal.log(f"  [OK] World Subsystem Initialized: {world_sub_class.get_name()}")

    # 2. Verify Multi-Region Coverage (11 Distinct Regions)
    unreal.log("----------------------------------------------------------------------")
    unreal.log("2. Auditing Multi-Region & Hidden Village Coverage across Ninja World:")

    regions = [
        (unreal.EOSWorldRegion.KONOHA, "Konohagakure (Hidden Leaf)", 10),
        (unreal.EOSWorldRegion.LAND_OF_FIRE, "Land of Fire Outskirts", 3),
        (unreal.EOSWorldRegion.LAND_OF_WIND, "Land of Wind & Sunagakure", 3),
        (unreal.EOSWorldRegion.LAND_OF_WATER, "Land of Water & Kirigakure", 3),
        (unreal.EOSWorldRegion.LAND_OF_LIGHTNING, "Land of Lightning & Kumogakure", 3),
        (unreal.EOSWorldRegion.LAND_OF_EARTH, "Land of Earth & Iwagakure", 2),
        (unreal.EOSWorldRegion.AMEGAKURE, "Amegakure (Hidden Rain)", 2),
        (unreal.EOSWorldRegion.MOUNT_MYOBOKU, "Mount Myōboku Sage Sanctuary", 1),
        (unreal.EOSWorldRegion.VALLEY_OF_THE_END, "Valley of the End", 1),
        (unreal.EOSWorldRegion.WAR_FRONTS, "Fourth Shinobi War Fronts", 1),
        (unreal.EOSWorldRegion.KAMUI_DIMENSION, "Kamui Dimension Void", 1),
        (unreal.EOSWorldRegion.DIVINE_TREE_REALMS, "Divine Tree Core & Roots", 1),
        (unreal.EOSWorldRegion.ASTRAL_ECLIPSE_REALM, "Eclipse Astral Core", 1)
    ]

    total_zones = 0
    for reg_enum, reg_name, min_expected in regions:
        zones = world_sub.get_zones_by_region(reg_enum)
        count = len(zones)
        total_zones += count
        assert count >= min_expected, f"Region {reg_name} has {count} zones, expected at least {min_expected}"
        unreal.log(f"  [OK] Region: {reg_name:<36} | Zones: {count} verified")

    unreal.log(f"  Total World Zones Registered: {total_zones} / 32 Canonical Zones")
    assert total_zones >= 32, f"Expected 32 zones, found {total_zones}"

    # 3. Test Cross-Region Zone Transition & Fast Travel
    unreal.log("----------------------------------------------------------------------")
    unreal.log("3. Testing Dynamic Zone Transition & Fast Travel:")
    # Transition from Konoha Market to Valley of the End
    b_transition = world_sub.transition_to_zone("ZONE_VALLEY_OF_THE_END")
    assert b_transition, "Failed to transition to ZONE_VALLEY_OF_THE_END"
    assert world_sub.get_active_zone_id() == "ZONE_VALLEY_OF_THE_END", "Active zone ID mismatch"
    unreal.log("  [OK] Transition to ZONE_VALLEY_OF_THE_END successful")

    # Transition to Mount Myoboku
    b_myoboku = world_sub.transition_to_zone("ZONE_MYOBOKU_SAGE_SANCTUARY")
    assert b_myoboku, "Failed to transition to Mount Myoboku"
    unreal.log("  [OK] Transition to ZONE_MYOBOKU_SAGE_SANCTUARY successful")

    unreal.log("======================================================================")
    unreal.log("  PHASE 20 & 21 WORLD FACTORY AUDIT RESULT: ALL CHECKS PASSED (100% OK)")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase20_21()
