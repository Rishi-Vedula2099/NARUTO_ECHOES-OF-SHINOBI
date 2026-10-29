# OBJECTIVE DISCREPANCY REPORT: KONOHA GOLDEN STREET REFERENCE MATCH
## NARUTO: ECHOES OF SHINOBI | Unreal Engine 5.8
**Document:** `Docs/VisualBible/Reports/GOLDEN_STREET_REFERENCE_MATCH_REPORT.md`  
**Target Authority:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01` (`Konoha_Visual_North_Star.jpg`)  
**Current Audit Subject:** Runtime Capture `GoldenStreet_RuntimeCapture_Beauty_16.png`  
**Audit Date:** 2026-10-21  

---

### 1. Executive Summary

````carousel
![Konoha Visual North Star Target (Authoritative Standard)](/c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Docs/Screenshots/Konoha_Visual_North_Star.jpg)
<!-- slide -->
![Current Runtime Capture (ScreenShot00016.png)](/c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Docs/Screenshots/ScreenShot00016.png)
````

While recent technical rectifications resolved critical shader compilation crashes and unblocked engine execution, the visual output of `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet` remains in an unacceptable prototype state when evaluated against the Konoha Visual North Star target. 

Per supreme project mandates: **NARUTO: ECHOES OF SHINOBI is not a prototype waiting for visuals. The visual identity IS the product.** The current scene exhibits severe defects across all 12 visual dimensions, particularly in character anatomy, architectural fidelity, vista composition, and environmental life.

---

### 2. Systematic 12-Category Discrepancy Matrix

| Category | Target Reference Standard (`Konoha_Visual_North_Star.jpg`) | Current Runtime State (`ScreenShot00016.png`) | Discrepancy Severity | Root Cause Asset / Setting | Required Production Correction | Validation Method |
| :--- | :--- | :--- | :---: | :--- | :--- | :--- |
| **1. Camera & Composition** | Naruto centered in lower-middle third (~28% height); wide street angle framing vanishing point directly to Hokage Monument. | Camera set too low and steep; building walls create a narrow dark corridor; vanishing point blocked. | **CRITICAL (P0)** | `EOSCameraProfile` / `CameraBoom` in `EOSCharacterBase`: Arm length, SocketOffset, and pitch misaligned. | Calibrate `TargetArmLength = 460.0`, `SocketOffset = (0, 0, 75)`, `Pitch = -8.0°`, `FOV = 68.0°`. Remove central blocking tower. | In-engine camera view comparison. |
| **2. Naruto Mesh & Silhouette** | Curving, organic anime proportions (1:5.5); spiky blonde hair; headband with metal plate; orange tracksuit with navy accents, Uzumaki back spiral, thigh bandage, kunai holster, sandals. | Rigid prototype blocky mannequin composed of primitive cuboids; head is flat stacked boxes; stiff posture. | **CRITICAL (P0)** | `SK_Naruto_Genin.obj` was authored with rigid geometric box primitives instead of stylized anatomical meshes. | Author/import high-fidelity 3D anime character mesh with organic limbs, spiky hair clusters, headband, folds, and sandals. | Full-body turnaround & rear gameplay view. |
| **3. Building Architecture** | Post-and-beam Japanese timber framing, distinct 1st and 2nd stories, individual balconies, railings, window frames, multi-tiered sloped roofs with curved terracotta tiles and eaves. | Monolithic flat brown rectangular slabs with primitive triangular roof prisms; no windows, beams, or depth. | **CRITICAL (P0)** | `SM_Bld_Konoha_Residence_A/B` lacks modular timber sub-elements; flat box extrusions. | Author multi-layered modular timber facades with separate window mullions, roof ridges, and balconies. | Wireframe and close-up architectural inspection. |
| **4. Shopfronts & Signage** | Ichiraku Ramen shopfront with split noren curtains, Japanese calligraphy ("一楽ラーメン"), counter with 4 stools, hanging paper lanterns, dining props. | Generic brown box with a flat sign board; noren is flat textured strip; stools are primitive cylinders. | **CRITICAL (P0)** | `SM_Bld_IchirakuRamen` lacks interior counter depth, separate fabric geometry, and prop layout. | Rebuild Ichiraku Ramen with recessed dining alcove, fabric wave noren, 3D hanging lanterns, and ramen bowls. | Eye-level sidewalk inspection. |
| **5. Road & Pavements** | Light grey cobblestone avenue with natural stone segmentation, dirt edges, stone kerbs, drainage channels, and scattered pink petals. | Central road is uniform washed-out white; side strips are flat repeated pavers without variation or dirt edges. | **HIGH (P1)** | `SM_Road_Paved_01` lacks multi-tone stone vertex color / dirt decal blend; flat diffuse response. | Implement dual-frequency stone paver material with dirt gutter blends, raised kerb stones, and petal decals. | Surface detail inspection. |
| **6. Vista & Horizon** | Towering Hokage Rock mountain cliff (Hashirama, Tobirama, Hiruzen, Minato faces) on central axis; distant blue peaks; open azure sky with anime cumulus clouds. | Artificial tiered pagoda cylinder (`Bld_HokageMansion_MidVista`) blocks center street; Hokage faces occluded; sky is narrow slit. | **CRITICAL (P0)** | Central pagoda actor placed at `(0, 2800, 0)` directly on street axis; backdrop plane occluded. | Move/remove central pagoda from road center; position towering Hokage Rock cliff backdrop at horizon `(0, 4200, 1100)`. | Horizon sightline audit. |
| **7. Foliage & Environmental Props** | Flowering pink Sakura cherry blossoms branching between buildings; Japanese cedars framing background; utility poles with cables. | No visible cherry blossom canopy; cedars hidden behind giant walls; utility poles lack wires. | **HIGH (P1)** | Foliage actors placed too far behind building walls; missing spline wires between poles. | Reposition Sakura trees to overhang sidewalks; add overhead utility cables and hanging wires. | Sidewalk canopy inspection. |
| **8. NPC Life & Activity** | 8+ visible villagers, Genin/Chunin, pedestrians walking/standing, Teuchi behind counter, animated lively street life. | 0 NPCs visible in viewport. Desolate empty ghost town. | **HIGH (P1)** | `AEOSVillageNPC` actors not positioned along Golden Street sidewalks in `L_Konoha_GoldenStreet.umap`. | Populate 8 interactive NPCs (Teuchi, Genin squad, shopkeepers) with anime idle animations along sidewalks. | Runtime population audit. |
| **9. Lighting & Color Grading** | Warm 5400K golden key sunlight; soft azure sky bounce fill; rich warm wood tones, vermilion roofs, natural greens, pink blossoms; zero crushed blacks. | Overly uniform exposure; wood walls appear flat mud-brown; road is slightly overexposed; lacks warmth. | **HIGH (P1)** | SkyLight intensity and DirectionalLight sun color/pitch need harmonized warmth (5400K) with tonemapper curve. | Set Sun light color `(255, 240, 215, 255)`, pitch `-58°`, yaw `75°`; SkyLight azure fill `(185, 220, 255)`; calibrate exposure. | Color histogram & EV verification. |
| **10. Anime RPG HUD** | Gold circular medallion with Naruto portrait; curved red HP and cyan Chakra bars; circular parchment minimap with ribbon; fiery combo; 4-way diamond jutsu cross. | Custom HUD rendered, but elements are slightly oversized/blocky and lack refined bevels and target styling. | **MEDIUM (P1)** | `EOSShinobiHUD.cpp` procedural canvas primitives need styling refinement to match reference graphics 1:1. | Polish HUD drawing math: exact circular portrait clip, rounded pill bars with gloss highlight, parchment texture minimap. | Pixel comparison against target HUD. |
| **11. Materials & Textures** | Painterly stylized 3D anime cel textures with clean line work, wood grain, tile bevels, fabric weave, and controlled cel bands. | Textures are low-frequency tiled procedural maps without layered edge wear or painterly line accents. | **HIGH (P1)** | Texture maps are basic synthetic patterns without anime line art and cel shadow thresholds. | Upgrade texture atlas suite with authored timber grain, tiled roof normal bevels, and crisp anime cel bands. | Material slot audit. |
| **12. Scene Depth & Traversal** | Deep, inviting 500m village thoroughfare with clear layers: Foreground (Naruto) -> Midground (Shops/Life) -> Vista (Cliff/Sky). | Scene feels like an enclosed narrow alley between two monolithic walls; zero atmospheric depth. | **CRITICAL (P0)** | Building sidewalk clearance too narrow (`X = ±540`); lack of height fog layering. | Widen street profile (`X = ±680`), vary building heights (1-story vs 2-story), add subtle warm sunset atmospheric haze. | Depth plane pass/fail check. |

---

### 3. Classification of Fixes

#### A. Configuration & Level Layout (Immediate In-Engine Fixes)
- Remove the central cylindrical pagoda (`Bld_HokageMansion_MidVista`) from `(0, 2800, 0)` to clear the street axis.
- Widen building sidewalk setbacks from `X = ±540` to `X = ±680` to eliminate the tunnel claustrophobia.
- Re-align camera boom to `TargetArmLength = 460.0`, `SocketOffset = (0, 0, 75)`, `Pitch = -8.0°` for exact target framing.
- Calibrate lighting color temperature to 5400K golden key and azure fill with manual exposure EV 9.8.
- Reposition Sakura cherry blossom trees to overhang sidewalks with drifting petal particle emitters.

#### B. Asset Authoring & Replacement (P0 Geometry & Shaders)
- **Naruto Hero Mesh (`SK_Naruto_Genin`):** Replace blocky prototype geometry with a watertight, anatomically proportioned stylized anime 3D mesh featuring spiky hair clumps, metal headband, high collar, Uzumaki back spiral, thigh holster, and ninja sandals.
- **Ichiraku Ramen Shop (`SM_Bld_IchirakuRamen`):** Model authentic post-and-beam timber alcove, hanging fabric noren curtains with wave folds, 3D counter, and red paper lanterns.
- **Modular Konoha Residences (`SM_Bld_Konoha_Residence_A/B`):** Multi-tiered timber facades with framed sliding windows, second-floor balconies, balustrades, and layered terracotta curved roof tiles.
- **Hokage Rock Vista Backdrop (`SM_Backdrop_HokageRock`):** Monumental mountain cliff with the four Hokage face sculptures (Hashirama, Tobirama, Hiruzen, Minato) standing towering at the horizon under an azure anime sky.
- **Village NPCs (`BP_EOSVillageNPC`):** Place 8 animated NPCs along the sidewalks (Teuchi, Konoha Genin, pedestrian villagers).

---

### 4. Progression Gate Status

> **CURRENT STATUS: GATE 1 (GOLDEN STREET) = FAIL**  
> Reason: Runtime screenshot evidence confirms persistent prototype geometry, blocky character mesh, narrow claustrophobic street tunnel, and occluded background vista.  
> Advancement to full village expansion or secondary regions is strictly FROZEN until all P0 and P1 deficiencies are resolved and verified in runtime gameplay.
