"""
NARUTO: ECHOES OF SHINOBI
Phase 23: 25 Arcs / 100 Chapters / 500 Stages Campaign Audit & Verification Script
Contract Section 32 (Future Game Expansion) & Section 43 (Phase Order)
"""
import unreal
import sys

def audit_phase23():
    unreal.log("======================================================================")
    unreal.log("  PHASE 23 AUDIT: COMPLETE 25 ARCS / 100 CHAPTERS / 500 STAGES CAMPAIGN")
    unreal.log("======================================================================")

    # 1. Verify Story Subsystem Class
    unreal.log("----------------------------------------------------------------------")
    unreal.log("1. Checking Story Subsystem Database Initialization:")
    story_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSStorySubsystem")
    if not story_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSStorySubsystem class.")
        sys.exit(1)

    story_sub = unreal.new_object(story_sub_class)
    story_sub.initialize_full_campaign_database()
    unreal.log(f"  [OK] Story Subsystem Initialized: {story_sub_class.get_name()}")

    # 2. Verify Total Counts (25 Arcs, 100 Chapters, 500 Stages)
    unreal.log("----------------------------------------------------------------------")
    unreal.log("2. Validating Campaign Database Metric Integrity:")
    arc_count = story_sub.get_total_registered_arc_count()
    ch_count = story_sub.get_total_registered_chapter_count()
    stg_count = story_sub.get_total_registered_stage_count()

    unreal.log(f"  Arcs Registered:     {arc_count} / 25 Canonical Arcs")
    unreal.log(f"  Chapters Registered: {ch_count} / 100 Canonical Chapters")
    unreal.log(f"  Stages Registered:   {stg_count} / 500 Canonical Stages")

    assert arc_count == 25, f"Expected 25 Arcs, got {arc_count}"
    assert ch_count == 100, f"Expected 100 Chapters, got {ch_count}"
    assert stg_count == 500, f"Expected 500 Stages, got {stg_count}"
    unreal.log("  [OK] Exact 25/100/500 Campaign Scale Verified")

    # 3. Test Stage Loading & Objective Progression
    unreal.log("----------------------------------------------------------------------")
    unreal.log("3. Testing Stage Loading & Completion Lifecycle (Stage 01_01_01):")
    b_loaded, stage_rec = story_sub.load_stage("STG_01_01_01")
    assert b_loaded, "Failed to load stage STG_01_01_01"
    unreal.log(f"  [OK] Loaded Stage: {stage_rec.display_name} — Boss: {stage_rec.boss_encounter_id}")

    # Complete Stage
    b_completed, world_delta, mm_id = story_sub.complete_stage("STG_01_01_01")
    assert b_completed, "Failed to complete stage STG_01_01_01"
    unreal.log(f"  [OK] Stage Completed: WorldDelta={world_delta}, MotionMangaID={mm_id}")

    unreal.log("======================================================================")
    unreal.log("  PHASE 23 CAMPAIGN AUDIT RESULT: ALL CHECKS PASSED (100% OK)")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase23()
