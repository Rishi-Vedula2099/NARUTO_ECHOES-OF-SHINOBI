

# PHASE GATE ENFORCEMENT RULE

Project: NARUTO: ECHOES OF SHINOBI
Engine: Unreal Engine 5.8

## PHASE EXECUTION ORDER (IMMUTABLE)

```
PHASE 0   Visual Authority Freeze         ← CURRENT
PHASE 1   Rendering Baseline
PHASE 2   Anime Material System
PHASE 3   Golden Naruto
PHASE 4   Animation + Shinobi Movement
PHASE 5   Third-Person Camera
PHASE 6   50m Konoha Golden Street
PHASE 7   NPC + Props + World Life
PHASE 8   Combat + Jutsu + VFX
PHASE 9   Hand Signs + QTE + Chakra Control
PHASE 10  Final HUD + Minimap + UI
PHASE 11  Golden Boss Encounter
PHASE 12  100m Konoha
PHASE 13  250m Konoha
PHASE 14  500m Konoha
PHASE 15  Character Factory
PHASE 16  Forms + Transformations
PHASE 17  Skin Factory
PHASE 18  Weapons + Gear + Collection
PHASE 19  Card Collection + Gacha Presentation
PHASE 20  Global World Factory
PHASE 21  Lands + Villages + Special Realms
PHASE 22  Motion Manga + Cinematic Visuals
PHASE 23  25 Arcs / 100 Chapters / 500 Stages
PHASE 24  Global Visual QA
PHASE 25  Packaged Golden Build
```

## GATE ENFORCEMENT

Each phase follows this exact flow:

```
SPECIFICATION
    ↓
AUDIT (what exists vs what's needed)
    ↓
ASSET REQUIREMENTS (list all needed assets)
    ↓
IMPLEMENTATION
    ↓
RUNTIME ACTIVATION
    ↓
VISUAL REVIEW (against reference)
    ↓
AUTOMATED TEST (if applicable)
    ↓
PERFORMANCE TEST (FPS, memory)
    ↓
EVIDENCE (screenshots, test results)
    ↓
GATE VERDICT: PASS / CONDITIONAL_PASS / FAIL / BLOCKED
```

## ABSOLUTE RULES

1. If a gate returns FAIL: **STOP.** Fix the current phase. Do NOT proceed.
2. If a gate returns BLOCKED: Identify the blocking dependency. Fix it first.
3. If a gate returns CONDITIONAL_PASS: Document conditions. May proceed with caution.
4. PASS: Proceed to next phase.

## REQUIRED PHASE REPORT

After every phase, return:

```
PHASE:              [Phase number and name]
STATUS:             [PASS / CONDITIONAL_PASS / FAIL / BLOCKED]
IMPLEMENTED:        [List of implementations]
REUSED:             [List of reused existing systems]
CREATED:            [New files/assets created]
MODIFIED:           [Existing files/assets modified]
BLOCKED:            [Blockers preventing progress]
ASSETS REQUIRED:    [Total assets needed]
ASSETS AVAILABLE:   [Assets that exist on disk]
ASSETS MISSING:     [Assets that still need authoring]
TESTS:              [Tests performed]
TEST RESULTS:       [Results of tests]
SCREENSHOT EVIDENCE:[File paths to screenshots]
PERFORMANCE:        [FPS, memory metrics if applicable]
KNOWN ISSUES:       [Outstanding issues]
NEXT STEP:          [What comes next]
```

## NEVER FABRICATE EVIDENCE

- Never claim a test passed without running it
- Never claim a screenshot looks correct without capturing it
- Never claim an asset exists without verifying it on disk
- Never claim performance is acceptable without profiling
