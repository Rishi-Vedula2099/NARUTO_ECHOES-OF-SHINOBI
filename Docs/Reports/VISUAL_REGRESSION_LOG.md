# NARUTO: ECHOES OF SHINOBI
# VISUAL REGRESSION LOG
## *Permanent Engineering State Record*

> **Purpose:** Record all visual failures, incorrect fixes, and permanent prevention rules so that future agent sessions learn from previous mistakes.

---

## How To Use This Document

Before touching any visual system, the agent MUST:
1. Read this document
2. Check if the proposed change matches a known failure pattern
3. If it does: follow the CORRECT FIX, not the INCORRECT FIX
4. After any visual change: add a new entry if the change caused a regression

---

## REGRESSION #001

| Field | Value |
|---|---|
| Date | 2026-10-01 |
| Problem | Scene became black/cyan void |
| Expected | Warm stylized anime daylight |
| Root Cause | Incorrect lighting/exposure configuration. Lumen enabled without mesh distance fields. |
| Secondary Cause | Missing production materials on environment meshes |
| INCORRECT Fix | Increasing emissive intensity on materials |
| CORRECT Fix | 1. Restore lighting baseline (DirectionalLight + SkyAtmosphere + SkyLight). 2. Replace placeholder materials with M_AnimeCel_Master instances. 3. Validate exposure (EV 9.5-10.0). 4. Validate sky (blue/cyan, NOT black). 5. Validate directional light (warm 5400K). |
| DO NOT REPEAT | YES |
| Status | RESOLVED |

---

## REGRESSION #002

| Field | Value |
|---|---|
| Date | 2026-10-01 |
| Problem | White mannequin/capsule appeared instead of Naruto |
| Expected | Production Naruto Genin character mesh |
| Root Cause | `CharacterID` uninitialized in `AEOSCharacterBase` constructor, soft reference to SK_Naruto_Genin was null |
| INCORRECT Fix | Adding more lights to make the mannequin visible |
| CORRECT Fix | Initialize `CharacterID = "CHAR_NARUTO"` in constructor. Validate soft reference loads SK_Naruto_Genin at BeginPlay. |
| DO NOT REPEAT | YES |
| Status | RESOLVED |

---

## REGRESSION #003

| Field | Value |
|---|---|
| Date | 2026-10-01 |
| Problem | Crushed dark void + blinding white emissive wall |
| Expected | Evenly lit warm daylight scene |
| Root Cause | 1. DirectionalLight not properly illuminating scene. 2. Point lights placed directly against wall surface. 3. SkyAtmosphere not coupled with directional sun. 4. No ambient fill light. |
| INCORRECT Fix | Adding more point lights, increasing material emissive |
| CORRECT Fix | 1. Couple SkyAtmosphere with DirectionalLight. 2. Add SkyLight for ambient fill. 3. Remove point lights from directly against surfaces. 4. Set minimum ambient level > 0 (never crushed black). |
| DO NOT REPEAT | YES |
| Status | RESOLVED |

---

## REGRESSION #004

| Field | Value |
|---|---|
| Date | 2026-10-01 |
| Problem | Game launched into procedural graybox testbed instead of authored Golden Street |
| Expected | Launch into L_Konoha_GoldenStreet |
| Root Cause | RunGame.bat defaulted to wrong map |
| INCORRECT Fix | Adding visual effects to make the graybox look presentable |
| CORRECT Fix | Update launcher default to `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet` |
| DO NOT REPEAT | YES |
| Status | RESOLVED |

---

## REGRESSION #005

| Field | Value |
|---|---|
| Date | 2026-10-23 |
| Problem | Procedural box mannequin, monolithic brown boxes, and cone trees rendering in standalone game |
| Expected | Stylized 3D anime Naruto model, modular Japanese timber architecture, and natural foliage matching Visual North Star |
| Root Cause | Generating primitive OBJs using `make_box` / `make_cylinder` procedural scripts and treating technical execution as visual pass |
| INCORRECT Fix | Modifying lighting, camera boom, or adding emissive materials to procedural boxes |
| CORRECT Fix | Author/import authentic 3D hero assets (Naruto, Ichiraku Ramen, Konoha Residences, Sakura/Cedar foliage), apply world-unit UV scaling, calibrate building setbacks, and enforce visual gate review against North Star |
| DO NOT REPEAT | YES |
| Status | RESOLVED (Verified via ScreenShot00026.png) |

---

## PATTERN RECOGNITION


### If you see BLACK SCREEN:
```
CHECK: SkyAtmosphere exists
CHECK: DirectionalLight exists and is active
CHECK: SkyLight exists
CHECK: PostProcess exposure is not crushing to black
DO NOT: Add emissive materials
DO NOT: Add point lights randomly
```

### If you see WHITE SCREEN:
```
CHECK: Exposure settings (AutoExposure range)
CHECK: DirectionalLight intensity (not > 10 lux)
CHECK: Emissive materials (none should be > 5.0)
CHECK: Bloom intensity (should be ≤ 0.45)
DO NOT: Reduce all material colors to compensate
DO NOT: Add fog to dim the scene
```

### If you see MANNEQUIN/BASICSHAPE:
```
CHECK: Character mesh soft reference is valid
CHECK: SK_Naruto_Genin.uasset exists on disk
CHECK: Material slots are assigned
CHECK: AnimBP is assigned
DO NOT: Change lighting to hide the mannequin
DO NOT: Declare the scene "acceptable"
```

### If you see NO VEGETATION/PROPS:
```
CHECK: Foliage/prop meshes exist in Content
CHECK: Meshes are placed in the level
CHECK: Materials are assigned to meshes
DO NOT: Add fog to hide empty areas
DO NOT: Declare the environment complete
```
