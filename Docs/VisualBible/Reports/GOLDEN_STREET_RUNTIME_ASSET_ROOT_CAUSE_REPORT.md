# RUNTIME ASSET ROOT CAUSE AUDIT REPORT
## NARUTO: ECHOES OF SHINOBI | Unreal Engine 5.8
**Document:** `Docs/VisualBible/Reports/GOLDEN_STREET_RUNTIME_ASSET_ROOT_CAUSE_REPORT.md`  
**Execution Context:** Phase 0 Baseline Audit & Runtime Reference Trace  
**Map:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap`  
**Target Visual Authority:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01` (`Docs/Screenshots/Konoha_Visual_North_Star.jpg`)  
**Audit Subject:** Standalone Engine Capture `ScreenShot00023.png`  
**Date:** 2026-10-23  

---

### 1. Executive Summary

A comprehensive runtime asset trace and code audit was conducted on `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap` and `AEOSCharacterBase` in Unreal Engine 5.8. 

The audit reveals why prior technical fixes improved actor positions, material assignments, and scene visibility without achieving the target visual fidelity:
1. **Procedural Geometry Defect:** The hero character (`SK_Naruto_Genin`), hero buildings (`SM_Bld_IchirakuRamen`, `SM_Bld_Konoha_Residence_A/B`), and vegetation were generated via Python scripts (`generate_master_golden_street_assets.py` / `generate_perfect_replication_assets.py`) using geometric primitives (`make_box`, `make_cylinder`, `make_cone`). While technically valid OBJ meshes, they are structurally primitive and lack anatomical topology, cloth folds, authentic Japanese carpentry, and natural foliage silhouettes.
2. **Runtime C++ Slot Override Conflict:** In `Source/EchoesOfShinobi/Private/Character/EOSCharacterBase.cpp` (`ResolveProductionVisualDefaults`), the character system hardcodes a 5-material array for `CHAR_NARUTO`. At runtime `BeginPlay`, `SetupStylizedVisualMesh` iterates over this 5-element array and overrides the static mesh component slots, resulting in slot misalignments when an imported mesh contains 7 slots or different internal OBJ alphabetical section ordering.
3. **Corridor Occlusion & Horizon Misplacement:** Buildings were positioned at insufficient street setbacks ($X = \pm 480$ to $\pm 520$) with eaves encroaching to $X = \pm 250$, closing the camera frustum into an enclosed timber tunnel rather than the wide, breathable Konoha avenue depicted in the North Star reference.

---

### 2. Runtime Asset Root Cause Inventory

#### Finding 01: Hero Player Character (Naruto Genin)
* **Runtime Actor:** Spawned Player Pawn at `PlayerStart_GoldenView` (`(0.0, 0.0, 95.0)`)
* **Actor Class:** `AEOSCharacterBase` (consuming `UEOSCharacterDefinition`)
* **Runtime Mesh Component:** `StylizedStaticMeshComp` (`UStaticMeshComponent`) attached to `UCapsuleComponent`
* **Mesh Asset Path:** `/Game/Characters/Naruto/Skins/SK_Naruto_GeninTracksuit` (duplicated from `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin`)
* **Material Paths:**
  * Slot 0: `/Game/Characters/Naruto/Materials/MI_Naruto_Skin`
  * Slot 1: `/Game/Characters/Naruto/Materials/MI_Naruto_Navy`
  * Slot 2: `/Game/Characters/Naruto/Materials/MI_Naruto_Hair`
  * Slot 3: `/Game/Characters/Naruto/Materials/MI_Naruto_Collar`
  * Slot 4: `/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit`
  * Master Material: `/Game/Materials/Master/M_AnimeCel_Textured_Master`
* **Source-Generation Method:** Procedural Python OBJ generation (`Scripts/generate_perfect_replication_assets.py` -> `Content/SourceMeshes/SK_Naruto_Genin.obj`)
* **Current Visual Defect:**
  * Mannequin-like rectangular joints and cylinder limbs.
  * Stiff, unnatural stance lacking shinobi weight and posture.
  * Flat extruded boxes for hair spikes instead of sculpted lock volumes.
  * Absence of facial sculpting, ear geometry, cloth folds, sandal straps, and defined toes.
* **Root Cause:**
  * The mesh is an extruded primitive OBJ rather than an authored 3D character asset.
  * `AEOSCharacterBase.cpp` line 128 hardcodes 5 slots, overriding materials at runtime and ignoring separate emblem/metal slots.
* **Proposed Replacement:**
  * Import an authentic sculpted 3D Naruto Genin mesh with organic anatomy, sculpted hair clump volumes, metal Leaf plate, stand-up collar with zip track, cloth folds, crimson Uzumaki back crest, and sculpted sandals.
  * Align `AEOSCharacterBase::ResolveProductionVisualDefaults` to reference all authored material slots cleanly.
* **Dependencies:** DCC authored/imported 3D Naruto model, `SK_Naruto_Genin.uasset`, `ABP_Naruto_Genin.uasset`.
* **Verification Procedure:** Front/rear/side turnarounds and standalone gameplay capture (`capture_beauty_frame.ps1`).

---

#### Finding 02: Ichiraku Ramen Shopfront
* **Runtime Actor:** `Bld_IchirakuRamen` (`Loc: (-480.0, 180.0, 0.0)`, `Rot: yaw=32.0`)
* **Actor Class:** `AStaticMeshActor`
* **Runtime Mesh:** `SM_Bld_IchirakuRamen`
* **Mesh Asset Path:** `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen`
* **Material Paths:**
  * Slot 0: `/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood`
  * Slot 1: `/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Roof`
  * Slot 2: `/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren`
  * Slot 3: `/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren`
  * Slot 4: `/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow`
* **Source-Generation Method:** Procedural Python script (`SM_Bld_IchirakuRamen.obj`)
* **Current Visual Defect:**
  * Appears as a plain brown wooden box with a flat slab roof.
  * Front dining counter, stools, and noren are occluded by flat side wall geometry from the third-person camera angle.
  * Lacks traditional Japanese timber carpentry, visible rafter ends, and roof tile silhouettes.
* **Root Cause:**
  * Geometry consists of extruded rectangular boxes without post-and-beam construction.
  * Setback ($X = -480$) and rotation ($yaw = 32^\circ$) placed the shop side wall directly in front of the camera sightline.
* **Proposed Replacement:**
  * Rebuild `SM_Bld_IchirakuRamen` with structural timber posts (`24x24cm`), lintels, recessed dining counter (`95cm`), 4 round wooden stools, authentic 4-panel noren, bold calligraphy sign, and stepped gabled tiled roof (`Irimoya-zukuri`).
  * Reposition to setback $X = -720$, $yaw = 45^\circ$, bringing the counter into clear 3/4 perspective.
* **Dependencies:** `T_Ichiraku_Facade_D`, `T_Ichiraku_Noren_D`, `T_RoofTiles_D`.
* **Verification Procedure:** Viewport inspection from player camera; verify interactive counter prompt.

---

#### Finding 03: Konoha Residences (A & B)
* **Runtime Actors:** `Bld_KonohaResidence_A` (`Loc: (520.0, 220.0, 0.0)`), `Bld_KonohaResidence_B` (`Loc: (680.0, 900.0, 0.0)`)
* **Actor Class:** `AStaticMeshActor`
* **Runtime Mesh:** `SM_Bld_Konoha_Residence_A`, `SM_Bld_Konoha_Residence_B`
* **Mesh Asset Path:** `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A`
* **Material Paths:** `MI_Bld_Ichiraku_Wood`, `MI_Bld_Ichiraku_Roof`
* **Source-Generation Method:** Procedural Python script (`SM_Bld_Konoha_Residence_A.obj`)
* **Current Visual Defect:**
  * Monolithic brown vertical blocks flanking the street.
  * Oversized flat roof eaves reaching into the center of the avenue.
  * No visible windows, sliding screens, or framed balconies.
* **Root Cause:**
  * Basic box extrusion without modular Japanese architectural articulation.
  * Setbacks were too tight ($X = 520$), crowding the street.
* **Proposed Replacement:**
  * Modular Japanese timber kit: post-and-beam timber skeleton, framed shoji windows, cantilevered timber balcony with vertical balustrades, stepped terracotta tiled gabled roofs.
  * Widen setback line to $X = +720$ to $+780$.
* **Dependencies:** Modular building meshes, `T_RoofTiles_D`, `MI_AnimeWood`.
* **Verification Procedure:** Avenue perspective check ensuring street width is 800+ units.

---

#### Finding 04: Vegetation (Cedar Pines & Sakura Blossoms)
* **Runtime Actors:** `Veg_Cedar_Left_01/02`, `Veg_Cedar_Right_01/02`, `Veg_Sakura_FrontLeft/Right`
* **Actor Class:** `AStaticMeshActor`
* **Runtime Meshes:** `SM_Veg_KonohaCedar_Large`, `SM_Veg_Sakura_Bloom`
* **Mesh Asset Path:** `/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large`
* **Material Paths:** `MI_Veg_CedarFoliage` (Pine green), `MI_Veg_Sakura_Bloom` (Pink)
* **Source-Generation Method:** Procedural cone and cylinder stacking.
* **Current Visual Defect:**
  * Cedar trees present rigid geometric conical tiers.
  * Sakura cherry blossoms lack organic branch structures and delicate flower-cluster masses.
  * Absence of falling cherry blossom petal particles.
* **Root Cause:**
  * Simplified procedural geometry without foliage card distribution or branch authoring.
* **Proposed Replacement:**
  * Authored stylized anime cedar trees with tufted needle clusters.
  * Authored Sakura blossom trees with visible timber trunks, overhanging branches, and soft pink petal clusters.
  * Add ambient Niagara cherry blossom petal drift system (`NS_Sakura_PetalFall`).
* **Dependencies:** `MI_Veg_CedarFoliage`, `MI_Veg_Sakura_Bloom`, Niagara Particle System.
* **Verification Procedure:** Silhouette check against open sky; petal particle performance validation.

---

#### Finding 05: Distant Vista Landmark (Hokage Rock & Monument)
* **Runtime Actor:** `Backdrop_Konoha_NorthStar_Vista` (`Loc: (0.0, 4600.0, 1700.0)`, `Scale: (60.44, 34.0, 1.0)`)
* **Actor Class:** `AStaticMeshActor` (Plane mesh)
* **Runtime Mesh:** `/Engine/BasicShapes/Plane`
* **Material Path:** `/Game/Environment/Konoha/Materials/M_Konoha_NorthStar_Backdrop`
* **Texture Path:** `Content/SourceTextures/T_Konoha_HokageRock_Backdrop.jpg`
* **Current Visual Defect:**
  * In earlier captures, the plane was scaled non-proportionally ($85\times 58$), squeezing faces horizontally, and center $Z=620$ buried the red Hokage Pagoda underground.
  * Currently improved in Run 23 to show the pagoda, but still flat without multi-plane depth layering.
* **Root Cause:**
  * Single 2D unlit texture plane without depth separation between mid-ground village structures and the distant Hokage cliff.
* **Proposed Replacement:**
  * Multi-plane layered backdrop: Foreground village pagoda layer, mid-ground pine ridge layer, background carved cliff faces layer, and dynamic anime sky dome.
  * Atmospheric haze fog gradient between avenue vanishing point and cliff base.
* **Dependencies:** Layered alpha planes, `ExponentialHeightFog`.
* **Verification Procedure:** Camera pitch validation verifying faces sit in upper third of frame.

---

#### Finding 06: Avenue Pavement & Street Props
* **Runtime Actors:** `Road_Center`, `Sidewalk_Left`, `Sidewalk_Right`, `Prop_StoneLantern_*`, `Prop_Bench_*`, `Prop_UtilityPole_*`
* **Actor Class:** `AStaticMeshActor`
* **Mesh Asset Paths:** `SM_Road_Center`, `SM_Sidewalk_Left/Right`, `SM_Prop_StoneLantern_01`
* **Material Paths:** `MI_Road_Cobblestone` (`T_Cobblestones_D`), `MI_AnimeStone`, `MI_Prop_Lantern_Glow`
* **Current Visual Defect:**
  * Repetitive texture tiling along central road.
  * Sharp artificial boundary between stone pavers and dirt sidewalks.
  * Props feel isolated rather than naturally integrated into village life.
* **Root Cause:**
  * Procedural box meshes with uniform UV tile factors; lack of edge decals, dirt blending, and scattered cherry blossom petals.
* **Proposed Replacement:**
  * Modular stone road kit with stone curbs, edge dirt margins, and vertex-blended blossom petal scatter.
  * Authored traditional stone Toro lanterns and wooden utility poles with electrical transformer boxes and overhead wires.
* **Dependencies:** `T_Cobblestones_D`, `MI_Road_Cobblestone`.
* **Verification Procedure:** Ground-level camera check for natural street texture transitions.

---

### 3. Trace Summary: Code, Asset & Level Pipeline

```
[Level: L_Konoha_GoldenStreet]
       │
       ├── PlayerStart_GoldenView (0, 0, 95)
       │         │
       │         └── Spawns: AEOSCharacterBase (BP_EOSCharacter)
       │                   │
       │                   ├── VisualProfile: CHAR_NARUTO
       │                   ├── C++ ResolveProductionVisualDefaults()
       │                   │     └── Assigns: SK_Naruto_GeninTracksuit
       │                   │           └── Overrides 5 Slots via C++ array
       │                   └── Mesh: StylizedStaticMeshComp
       │
       ├── Architecture (Left: X = -720, Right: X = +720)
       │         ├── Bld_IchirakuRamen (SM_Bld_IchirakuRamen)
       │         └── Bld_KonohaResidence_A (SM_Bld_Konoha_Residence_A)
       │
       ├── Foliage & Sky
       │         ├── SM_Veg_KonohaCedar_Large (MI_Veg_CedarFoliage)
       │         ├── SM_Veg_Sakura_Bloom (MI_Veg_Sakura_Bloom)
       │         └── Backdrop_Konoha_NorthStar_Vista (Y = 4800, Z = 1600)
       │
       └── Lighting & Camera
                 ├── DirLight_Konoha_Sun (5400K, 12 lux)
                 ├── SkyLight_AnimeAzureFill (azure fill, 6 lux)
                 └── PostProcess_GoldenStreet (EV locked 0.8-1.2, bloom 0.10)
```

---

### 4. Implementation Phase Order (Phase 0 -> Phase 6)

1. **Phase 0 (Baseline Audit):** Root cause established; reference specification locked (**COMPLETE**).
2. **Phase 1 (Character Asset Gate):** Secure and integrate authentic sculpted 3D Naruto Genin mesh with organic anatomy, hair clump volumes, metal Leaf plate, cloth folds, back crest, and sandals.
3. **Phase 2 (Architecture Gate):** Replace extruded boxes with authored modular Japanese timber building kit (Ichiraku Ramen open stall + residences with balconies and gabled roofs).
4. **Phase 3 (Vegetation Gate):** Replace procedural trees with authored stylized anime cedar trees and overhanging blooming Sakura canopies with petal particles.
5. **Phase 4 (Street Composition Gate):** Reassemble Golden Street layout with wide avenue setbacks ($X = \pm 780$), layered rooftops, and vista depth.
6. **Phase 5 (Lighting, Camera & Presentation Gate):** Calibrate camera boom, warm daylight, soft azure fill, and sleek translucent HUD.
7. **Phase 6 (Packaged Build Gate):** Validate standalone running build and capture beauty frame evidence.
