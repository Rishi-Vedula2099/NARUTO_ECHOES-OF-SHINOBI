# AGENTS.md — PERSISTENT REPOSITORY RULES & VISUAL CONTRACT
## Project: NARUTO: ECHOES OF SHINOBI (Unreal Engine 5.8)

### Supreme Directives

1. **NARUTO: ECHOES OF SHINOBI is not a prototype waiting for visuals.** The visual identity IS the product. Every character, form, skin, jutsu, animation, environment, NPC, boss, UI, VFX, card, gacha, Motion Manga, and expansion must use one unified stylized 3D anime production language.

2. **Do not solve missing visual assets through lighting, exposure, fog, emissive, post-processing, or material changes.** Missing assets are an asset-production problem. Incorrect rendering is a rendering problem. Diagnose and fix the correct layer.

### Non-Negotiable Core Laws
1. **Never Declare Visual Completion on Scaffolding:**
   - A C++ actor, Blueprint actor, Outliner entry, HUD widget, or raw light is NOT a completed visual asset.
   - A visual feature is ONLY complete when the authored 3D production model with valid anime materials and textures is rendering in the running game.
2. **Never Use Placeholder Geometry as Production:**
   - `/Engine/BasicShapes`, primitive cubes, cylinders, cones, planes, and default mannequins are STRICT FAILURES.
   - Scenes with pitch-black voids, crushed-black silhouettes, or blinding white overexposure are STRICT FAILURES.
3. **Do Not Solve Missing Art with Lighting:**
   - If an asset is missing, author and import the asset. Never crank point lights, bloom, or emissive colors to compensate.
4. **Do Not Iterate Randomly on Visual Problems:**
   - Follow: OBSERVE → IDENTIFY FAILURE CATEGORY → CLASSIFY ROOT CAUSE → FIX ROOT CAUSE → TEST → EVIDENCE → PASS/FAIL.
5. **Strict Phase Gate Enforcement:**
   - Do NOT proceed to the next phase until the current visual gate PASSES.
   - Phase order: 0→1→2→3→4→5→6→7→8→9→10→11→12→13→14→15→16→17→18→19→20→21→22→23→24→25.
6. **Mandatory Visual Failure Memory:**
   - Log all visual failures in `Docs/Reports/VISUAL_FAILURE_MEMORY.md`.
   - Log all regressions in `Docs/Reports/VISUAL_REGRESSION_LOG.md`.
   - Read both logs before touching any visual system.
7. **Always Enforce Rules in `.agents/rules/`:**
   - `00_project_authority.md`
   - `02_visual_dna.md`
   - `05_no_placeholder_completion.md`
   - `10_anti_random_iteration.md`
   - `11_phase_gate_enforcement.md`
   - `12_visual_diagnostic_mandate.md`

### Master Documents
- `Docs/VisualBible/00_MASTER_VISUAL_DEVELOPMENT_CONTRACT.md` — The supreme visual authority
- `Docs/VisualBible/01_VISUAL_NORTH_STAR.md` — Visual target characteristics
- `Docs/VisualBible/02_ANIME_MATERIAL_BIBLE.md` — Material system specification
- `Docs/VisualBible/03_CHARACTER_PRODUCTION_BIBLE.md` — Character pipeline
- `Docs/VisualBible/04_MOVEMENT_ANIMATION_BIBLE.md` — Movement & animation
- `Docs/VisualBible/05_CAMERA_CINEMATIC_BIBLE.md` — Camera system
- `Docs/VisualBible/06_ENVIRONMENT_WORLD_BIBLE.md` — World production
- `Docs/VisualBible/07_JUTSU_VFX_BIBLE.md` — Jutsu & VFX
- `Docs/VisualBible/08_UI_VISUAL_BIBLE.md` — UI/HUD
- `Docs/VisualBible/09_SKIN_BIBLE.md` — Skin system
- `Docs/VisualBible/10_GACHA_CARD_COLLECTION_BIBLE.md` — Collection & gacha
- `Docs/VisualBible/11_MOTION_MANGA_BIBLE.md` — Story presentation
