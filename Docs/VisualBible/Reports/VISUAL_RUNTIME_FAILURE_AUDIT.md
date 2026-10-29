# NARUTO: ECHOES OF SHINOBI
## VISUAL RUNTIME FAILURE AUDIT & PIPELINE CORRECTION
**Document Version:** 1.0.0-AUDIT  
**Status:** MANDATORY VISUAL PIPELINE HALT  
**Target Golden Slice:** `L_Konoha_GoldenStreet`  
**Target Reference:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`  

---

### EXECUTIVE SUMMARY: WHY THE RUNTIME TURNED OUT THIS WAY

The screenshot captured from the live Unreal Engine 5.8 runtime proves a critical architectural diagnosis:

> **The gameplay, input, GAS, and C++ actor framework are functioning; the visual production pipeline has completely failed to reach the screen.**

The IDE mistakenly substituted **C++ actor creation** and **developer debug canvas rendering** for **actual 3D art production**. In addition, three severe configuration failures occurred simultaneously when `RunGame.bat` was executed:

1. **Incorrect Map Launched:** `RunGame.bat` Option 1 was hardcoded to execute `/Game/Maps/Konoha/L_Konoha_Phase13_District`, an early Phase 12 procedural greybox testbed that procedurally spawns primitive shapes (`/Engine/BasicShapes/Cylinder`, `Cube`, and checkerboards). It did **not** execute the authored street level.
2. **Broken Global Illumination & Lumen Warning:** In `Config/DefaultEngine.ini`, Lumen was enabled (`r.DynamicGlobalIlluminationMethod=1`), but mesh distance fields (`r.GenerateMeshDistanceFields=True`) were **disabled**. Without distance fields, Lumen cannot calculate ambient occlusion or software ray-traced shadows. This produced the red on-screen warning banner, crushed scene depth, and blown-out ambient exposure (a blinding white-blue bloom).
3. **Placeholder Mannequin & Debug Geometry:** `AEOSCharacterBase` and the training dummy fell back to untextured white cylinder/capsule primitives because production skeletal meshes and textures were not bound and loaded in that level.
4. **Developer Debug Canvas HUD:** `AEOSShinobiHUD` drew raw developer 2D canvas rectangles (`DrawRect`) and system debug strings (`[Lv. 12 Genin] Form: FORM_BASE`, `[PHASE 1/2]`, etc.) directly on top of the screen instead of displaying stylized anime RPG UI widgets.

---

### PHASE 1: RUNTIME VISUAL ASSET AUDIT TABLE

Below is the definitive audit of every object appearing in the live build:

| Runtime Entity | Expected Production Asset | What Was Actually Rendering | Package Path & Asset Class | Mesh Type | Material / Shader | Skeleton / Anim BP | VFX Status | Runtime Load Status | Visual Validation Verdict |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **Player Hero (Naruto)** | Authored Naruto Genin (orange tracksuit, blue collar, blonde spiky hair, Konoha headband, sandals) | Generic untextured white mannequin / cylinder dummy with visor | `Engine/BasicShapes/Cylinder` / default mannequin | Primitive Static / Mannequin | Default White / Blown-out Emissive | None (T-pose / unrigged dummy) | None | Fallback to primitive | **FAIL: FORBIDDEN_MANNEQUIN** |
| **Boss (Kakashi)** | Authored Kakashi Jonin (Anbu vest, mask, silver hair, Sharingan eye) | Missing / Not visually present (HUD text only) | Missing from viewport | None | None | None | None | Not rendering in scene | **FAIL: MISSING_PRODUCTION_ASSET** |
| **Ichiraku Ramen Shop** | Authored Japanese timber shopfront, fabric noren curtain, tiled roof, bar counter, stools | Missing / White geometric blocks | Procedural graybox primitives | Primitive Cubes | Default checkerboard / white | N/A | None | Graybox fallback | **FAIL: PRIMITIVE_GEOMETRY** |
| **Konoha Residences** | 2-3 Japanese wooden townhouses with balconies, awnings, sliding doors | Primitive rectangular solids | Procedural graybox primitives | Primitive Cubes | Default white | N/A | None | Graybox fallback | **FAIL: PRIMITIVE_GEOMETRY** |
| **Main Road & Sidewalks** | Stone cobblestone pavement with raised stone curbs and drainage gutters | Blown-out flat plane / checkerboard surface | `/Engine/BasicShapes/Plane` | Primitive Plane | Unshadowed blown-out white | N/A | None | Graybox fallback | **FAIL: UNTEXTURED_PRIMITIVE** |
| **Vegetation (Trees)** | Stylized Japanese Cedars & pink flowering Sakura cherry blossom trees | White cylindrical posts / billboard crosses | Procedural graybox trees | Primitive Cylinders | Untextured white | N/A | None | Graybox fallback | **FAIL: PRIMITIVE_GEOMETRY** |
| **Street Props (Lanterns, Signs)** | Red paper lanterns, carved stone toro lanterns, wooden utility poles, market crates | Primitive vertical poles / cylinders | Procedural graybox props | Primitive Cylinders | Untextured white | N/A | None | Graybox fallback | **FAIL: PRIMITIVE_GEOMETRY** |
| **Background (Hokage Monument)** | Cliffside stone carving of the 4 Hokage faces overlooking Konoha + Hokage Dome | Empty black void / infinite horizon | Missing actor | None | None | N/A | None | Completely absent | **FAIL: MISSING_ENVIRONMENT_DEPTH** |
| **Atmosphere & Sky** | Stylized blue anime sky, distant mountain silhouettes, warm sunset/afternoon haze | Pitch black sky / void | Missing SkyAtmosphere / Dome | SkyAtmosphere absent | None | N/A | None | Disabled / broken | **FAIL: CRUSHED_BLACK_SKY** |
| **Lighting & Exposure** | Warm 5400K sunlight, soft azure sky fill, crisp cel shadow bands, controlled exposure | Blinding blue-white overexposure, blown-out highlights, no ambient occlusion | Unbound post-process / default directional | DirectionalLight | Overexposed default | N/A | None | Broken Lumen config | **FAIL: OVEREXPOSED_BLOWN_OUT** |
| **In-Game HUD** | Stylized anime RPG HUD: circular portrait, angled HP/Chakra bars, jutsu wheel | Developer 2D debug canvas boxes & raw system text | `AEOSShinobiHUD` (C++ Canvas) | Canvas 2D | Raw color fills (`DrawRect`) | N/A | None | Debug Canvas active | **FAIL: DEBUG_CANVAS_PROTOTYPE** |

---

### PHASE 0 & PIPELINE HALT DIRECTIVE

1. **NO NEW SCENES OR SCALING:** All development on the 500-stage campaign, additional villages (Suna, Kiri, Kumo), 78 characters, and gacha systems is strictly **FROZEN**.
2. **NO PRIMITIVE COMPENSATION:** C++ code must **never** spawn `/Engine/BasicShapes` (cubes, cylinders, spheres, cones, planes) or default third-person mannequins to simulate visual completion.
3. **NO LIGHTING-TO-COMPENSATE-FOR-ART TRICKS:** Do not crank light intensity, sky intensity, or bloom to hide the lack of textured 3D models.
4. **ONE SINGLE TESTBED:** Every ounce of visual effort is dedicated to **`L_Konoha_GoldenStreet`**.

---

### RECTIFICATION ROADMAP

- [x] **Audit & Halt:** Complete diagnostic analysis and lock pipeline halt.
- [ ] **Renderer Configuration (GTX 1650 Scalable Profile):**
  - Fix `DefaultEngine.ini`: Either enable Mesh Distance Fields (`r.GenerateMeshDistanceFields=True`) or switch to dedicated Stylized Cascaded Shadow Maps with Screen Space GI (`r.DynamicGlobalIlluminationMethod=2`) to guarantee **0 Lumen warnings** and locked 60 FPS on GTX 1650.
  - Set PostProcess exposure clamping (`MinEV100 = MaxEV100 = 9.5`) to eliminate blinding white blowout.
- [ ] **Execution Dispatch Correction:**
  - Update `RunGame.bat` and `LaunchGame.ps1` so Option 1 (and default) boots **`/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`**.
  - Remove all runtime fallback references to the procedural graybox map `L_Konoha_Phase13_District`.
- [ ] **Naruto Character Pipeline Activation:**
  - Ensure `SK_Naruto_Genin` is loaded with stylized cel materials (`MI_Naruto_Tracksuit`, `MI_Naruto_Skin`, `MI_Naruto_Hair`, `MI_Naruto_Navy`).
  - Attach character mesh correctly to `AEOSCharacterBase` capsule with proper anime silhouette.
- [ ] **One Real Konoha Street Environment:**
  - Build `L_Konoha_GoldenStreet` with real authored buildings: Ichiraku Ramen shopfront, wooden townhouses, cobblestone street, cedar & sakura trees, stone lanterns, and Hokage Monument background backdrop.
  - Establish clear 3-layer visual depth (Foreground, Midground, Background) matching `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`.
- [ ] **HUD Styling:**
  - Suppress developer debug canvas boxes in favor of clean anime framing.
