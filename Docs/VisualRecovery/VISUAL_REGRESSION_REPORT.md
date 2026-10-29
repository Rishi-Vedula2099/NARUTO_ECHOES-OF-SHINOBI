# VISUAL REGRESSION & CAMERA AUDIT REPORT
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Permanent Governance Anchor:** GEMINI.md Three-Tier Locking Policy & Camera Separation Protocol  
**Regression Audit Evidence:** `media_1793283768094_460b3b9b.jpg` vs Approved Gate 2 Baselines  

---

## 1. Visual Regression Incident Analysis

### A. The Observed Regression
In the baseline runtime screenshot (`media_1793283768094_460b3b9b.jpg`), the player character appeared as a monochrome grey-blue untextured proxy standing in front of calibration spheres and reflective floor rails.

### B. Root Cause Classification
1. **Level Regression:** Standalone launcher was dispatched without the explicit `-game /Game/Maps/GoldenTest/L_Konoha_GoldenStreet` argument, causing the engine to load the development testbed `L_VisualAssetSmokeTest`.
2. **Actor Spawn Regression:** In `AEOSCharacterBase`, `SetupStylizedVisualMesh()` initialized component transforms but lacked the explicit C++ code `GetMesh()->SetSkeletalMeshAsset(...)`. The dynamically spawned pawn had no skeletal mesh asset assigned, exposing fallback geometry.
3. **Material Binding Invalidation:** Procedural updates to `SK_Naruto_Genin.obj` in `Content/SourceMeshes/` created new material slot indices that did not match the 7 Material Instances in `Content/Characters/Naruto/Materials/`, temporarily causing unassigned material slots.

---

## 2. Deterministic Camera Profile Matrix (Strict Separation)

To ensure repeatable visual testing without distorting gameplay perspective, 8 locked camera profiles are enforced:

| Profile ID | Profile Name | Boom Length | FOV | Pitch / Yaw / Roll | Socket Offset (X, Y, Z) | Visual Acceptance Target |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **`CAPTURE_A`** | Konoha Gameplay | 320 cm | 48° | (-12.0°, 0.0°, 0.0°) | (0.0, 0.0, 115.0) | Naruto occupies ~28% screen height; avenue baseline at $X \approx \pm 680$; clear Hokage Rock sightline. |
| **`CAPTURE_B`** | Naruto Close-Up | 95 cm | 32° | (-4.0°, 0.0°, 0.0°) | (0.0, 5.0, 135.0) | Face audit: almond eyes, pupil depth, specular catchlights, 3 whiskers, metallic headband plate, hair roots. |
| **`CAPTURE_C`** | Architecture Detail | 180 cm | 50° | (-8.0°, 35.0°, 0.0°) | (0.0, 0.0, 120.0) | Timber post-and-beam mortise joints, noren fabric wave folds, warm paper lantern emission ($120\text{ cd/m}^2$). |
| **`CAPTURE_D`** | Street Life Density | 450 cm | 65° | (-22.0°, 15.0°, 0.0°)| (0.0, 0.0, 180.0) | NPC spacing, drifting cherry blossom petals, utility poles, benches, street clutter readability. |
| **`CAPTURE_E`** | Combat & Rasengan | 460 cm | 75° | (-14.0°, 0.0°, 0.0°) | (0.0, 0.0, 140.0) | Niagara swirling chakra flow, impact flash, dynamic frame rate stability during combo animations. |
| **`CAPTURE_F`** | Training Ground | 400 cm | 60° | (-15.0°, 45.0°, 0.0°)| (0.0, 0.0, 150.0) | 3 wooden training posts, target dummies, riverbank transition, perimeter cedar trees. |
| **`CAPTURE_G`** | Roster Switching | 200 cm | 45° | (-6.0°, 0.0°, 0.0°) | (0.0, 0.0, 125.0) | Instant party swap between Naruto, Sasuke, and Sakura with zero mesh pop, correct HUD portrait sync. |
| **`CAPTURE_H`** | Lighting Variation | 350 cm | 52° | (-10.0°, -30.0°, 0.0)| (0.0, 0.0, 130.0) | Cel shadow terminator transition under Noon (5400K), Sunset (3800K), and Night (lanterns lit). |

---

## 3. Immutable Regression Lock & Recovery Policy

1. **Three-Tier Locking Policy:**
   - **Tier 1 (Visual Identity):** Cel shading bands, timber architecture, anime proportions — *PERMANENTLY LOCKED*.
   - **Tier 2 (Approved Asset Revisions):** Once an asset passes its gates, its mesh and textures are locked in `Docs/Reports/MASTER_GATE_EXECUTION_TRACKER.md`.
   - **Tier 3 (Technical Tuning Values):** Light Lux, exposure EV, and shadow cascades remain tunable via automated scripts.
2. **Pre-Execution Checkpoint:** Before running any geometry or material script, a source-control rollback point is validated.
3. **Change Isolation:** Correcting Naruto character assets must never alter global exposure, street facade placements, or HUD styling.

