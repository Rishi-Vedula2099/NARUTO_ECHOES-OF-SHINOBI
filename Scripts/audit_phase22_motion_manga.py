"""
NARUTO: ECHOES OF SHINOBI
Phase 22: Motion Manga & Cinematic Visuals Pipeline Audit & Verification Script
Contract Section 31 (Story / Motion Manga) & Section 43 (Phase Order)
"""
import unreal
import sys

def audit_phase22():
    unreal.log("======================================================================")
    unreal.log("  PHASE 22 AUDIT: 2.5D MOTION MANGA & CINEMATIC NARRATIVE PIPELINE")
    unreal.log("======================================================================")

    # 1. Verify Motion Manga & Narrative Subsystem Classes
    unreal.log("----------------------------------------------------------------------")
    unreal.log("1. Checking Motion Manga Subsystem Class:")
    manga_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSMotionMangaSubsystem")
    narr_sub_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSNarrativeSubsystem")

    if not manga_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSMotionMangaSubsystem class.")
        sys.exit(1)
    if not narr_sub_class:
        unreal.log_error("  [FAIL] Failed to load EOSNarrativeSubsystem class.")
        sys.exit(1)

    manga_sub = unreal.new_object(manga_sub_class)
    manga_sub.initialize_default_manga_database()
    unreal.log(f"  [OK] Motion Manga Subsystem Initialized: {manga_sub_class.get_name()}")

    # 2. Test Sequence Loading & Panel Progression
    unreal.log("----------------------------------------------------------------------")
    unreal.log("2. Testing Canonical Manga Sequence (Land of Waves Climax):")
    b_loaded = manga_sub.load_sequence("MM_SEQ_LAND_OF_WAVES_CLIMAX")
    assert b_loaded, "Failed to load MM_SEQ_LAND_OF_WAVES_CLIMAX"
    unreal.log(f"  [OK] Loaded Sequence: MM_SEQ_LAND_OF_WAVES_CLIMAX (Total Panels: {manga_sub.get_total_panel_count()})")

    b_started = manga_sub.start_playback()
    assert b_started, "Failed to start manga playback"

    # Advance through panels to verify lifecycle
    total_panels = manga_sub.get_total_panel_count()
    for idx in range(total_panels - 1):
        b_advanced = manga_sub.advance_panel()
        assert b_advanced, f"Failed to advance panel at index {idx}"
        unreal.log(f"  Advanced to Panel {manga_sub.get_current_panel_index() + 1} / {total_panels}")

    assert manga_sub.is_active_panel_final() == True, "Final panel must be recognized"
    unreal.log("  [OK] Dramatic Hold Final Panel reached successfully")

    # 3. Test 2.5D Multi-Layer Parallax Math
    unreal.log("----------------------------------------------------------------------")
    unreal.log("3. Testing 2.5D Layer Parallax Calculation:")
    cam_delta = unreal.Vector2D(100.0, 50.0)
    bg_offset = manga_sub.calculate_layer_parallax_offset(cam_delta, 0.2, False) # Background moves slower
    fg_offset = manga_sub.calculate_layer_parallax_offset(cam_delta, 1.2, False) # Foreground moves faster

    assert abs(bg_offset.x - 20.0) < 0.1, "Background parallax math mismatch"
    assert abs(fg_offset.x - 120.0) < 0.1, "Foreground parallax math mismatch"
    unreal.log(f"  [OK] Parallax Delta: BG Offset={bg_offset}, FG Offset={fg_offset}")

    unreal.log("======================================================================")
    unreal.log("  PHASE 22 MOTION MANGA AUDIT RESULT: ALL CHECKS PASSED (100% OK)")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase22()
