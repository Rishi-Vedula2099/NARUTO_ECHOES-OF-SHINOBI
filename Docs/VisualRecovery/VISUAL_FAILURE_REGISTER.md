# VISUAL FAILURE REGISTER & DEFECT ROOT CAUSE MATRIX
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Permanent Governance Anchor:** GEMINI.md 10 Defect Classifications & 18 Non-Negotiable Rules  
**Baseline Evidence:** `media_1793283768094_460b3b9b.jpg` (Runtime Viewport In-Engine)  

---

## 1. Defect Classification System (10 Formal Categories)

1. `ASSET_DEFECT` — Missing/incorrect 3D mesh geometry, improper topology, or missing mesh parts.
2. `MATERIAL_DEFECT` — Incorrect shader assignment, untextured slot, inverted normal, or missing cel band response.
3. `LIGHTING_DEFECT` — Improper Sun Lux, SkyLight intensity, bad shadows, or excessive emissive glare.
4. `CAMERA_DEFECT` — Field of View, boom arm length, socket offset, or framing ratio error.
5. `COMPOSITION_DEFECT` — Street width, facade setback, vista sightline, or vanishing point blockage.
6. `ANIMATION_DEFECT` — Rigging tear, skin weight pinching, state machine glitch, or missing transition.
7. `VFX_DEFECT` — Particle occlusion, uncalibrated Niagara emitter, blown-out emissive blast.
8. `UI_DEFECT` — HUD clipping, GAS attribute desynchronization, or unstyled UI elements.
9. `PERFORMANCE_DEFECT` — Frame time exceeding 16.67 ms (sub-60 FPS), VRAM leak, or render stall.
10. `MISSING_SOURCE_ASSET` — Asset not authored, not acquired, or blocked on production pipeline.

---

## 2. Active Defect Ledger (Observed from Baseline Runtime Evidence)

### DEFECT-001: Player Character Spawning as Monochrome Grey Proxy
- **Defect ID:** `DEF-001`
- **Classification:** `MATERIAL_DEFECT` & `ASSET_DEFECT`
- **Location:** Viewport Center / Player Pawn
- **Observed Behavior:** Character renders as flat blue-grey silhouette without assigned tracksuit orange, navy collar, blonde hair, or peach skin tones.
- **Root Cause:** In C++, `AEOSCharacterBase::SetupStylizedVisualMesh()` initialized the capsule and transform offsets but did not invoke `SetSkeletalMeshAsset()` with `SK_Naruto_Genin_Rigged`. The spawned actor fell back to engine default material on uninitialized mesh slots.
- **Permanent Prevention Rule:** Rule 11 (Runtime Verification) & Non-Negotiable Rule 1: A visible character must not be mistaken for a mannequin, robot, debug actor, or generic placeholder.
- **Remediation Task:** Explicitly bind `SK_Naruto_Genin_Rigged` and its 7 assigned Material Instances (`MI_Naruto_*`) in `AEOSCharacterBase` or a dedicated Blueprint subclass `BP_Naruto_Genin`.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-002: Naruto Facial Anatomy Lacks Anime Identity & Expressive Features
- **Defect ID:** `DEF-002`
- **Classification:** `MISSING_SOURCE_ASSET` & `ASSET_DEFECT`
- **Location:** Character Head / Face
- **Observed Behavior:** Head geometry is a conical procedural approximation with no eye sockets, pupils, catchlights, cheek whiskers, or mouth contour.
- **Root Cause:** Character authoring script generated low-polygon procedural facial vertices without UV unwrapping or texture mapping for facial features.
- **Permanent Prevention Rule:** Non-Negotiable Rule 5: Correct material colors are not proof of correct character modeling. Component A Close-Up `CAM_B` Gate mandatory.
- **Remediation Task:** Author high-resolution anime face texture `T_Naruto_Genin_Face_D` (cerulean blue eyes, specular catchlights, 3 curved cheek whiskers, determined smirk mouth line) and map to head UVs with sculpted eye socket contours.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-003: Hair Geometry Lacks Multi-Tiered Clump Volumes & Natural Rooting
- **Defect ID:** `DEF-003`
- **Classification:** `ASSET_DEFECT`
- **Location:** Character Head / Hair
- **Observed Behavior:** Spikes are arranged as a single radial ring of polygonal cones, resembling a crown rather than layered anime hair clumps.
- **Root Cause:** Procedural geometry script lacked tiered clumping logic (front bangs cascading over headband, temporal locks framing ears, crown spikes catching cel bands).
- **Permanent Prevention Rule:** Non-Negotiable Rule 15: Maintain a coherent global visual identity across the entire game (Storm benchmark hair clumping).
- **Remediation Task:** Model tiered volume clumps (front bangs rooted behind headband, side locks, crown spikes) with faceted cel-banding planes.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-004: Stand-Up Fleece Collar Lacks Ribbed Shearling Folds & Zipper Track
- **Defect ID:** `DEF-004`
- **Classification:** `ASSET_DEFECT`
- **Location:** Character Neck / Collar
- **Observed Behavior:** Neck collar is a plain cylinder without vertical compression folds, rolled rim, or brass zipper track.
- **Root Cause:** Low-poly procedural cylinder used without sculpted ribbed details or seam indentation.
- **Permanent Prevention Rule:** Supreme Directive 1: NARUTO: ECHOES OF SHINOBI is not a prototype waiting for visuals.
- **Remediation Task:** Author high-poly ribbed fleece collar funnel with rolled upper lip and central metallic zipper slider geometry.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-005: Material Calibration Swatch Spheres Exposed in Viewport
- **Defect ID:** `DEF-005`
- **Classification:** `COMPOSITION_DEFECT`
- **Location:** Map Background (`L_VisualAssetSmokeTest`)
- **Observed Behavior:** A horizontal row of 11 colored calibration spheres (`Swatch_Skin`, `Swatch_Hair`, `Swatch_Tracksuit`, etc.) floats directly behind the player character.
- **Root Cause:** The game launched into the technical calibration level `L_VisualAssetSmokeTest` instead of the production world map `L_Konoha_GoldenStreet`.
- **Permanent Prevention Rule:** Non-Negotiable Rule 2 & 14: A production environment must not contain unexplained floating spheres, test primitives, disconnected platforms or exposed blockout geometry. Keep debug content in dedicated development/QA environments.
- **Remediation Task:** Configure default game map and launcher targets strictly to `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`. Move all swatch spheres to hidden test layer.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-006: Stepped Reflective Floor Channels Causing Specular Blowout
- **Defect ID:** `DEF-006`
- **Classification:** `MATERIAL_DEFECT` & `LIGHTING_DEFECT`
- **Location:** Ground Plane Foreground
- **Observed Behavior:** High-intensity white glare and specular reflections bleeding along stepped channel rails on the floor.
- **Root Cause:** Roughness values set near 0.0 with high metallic properties on test floor meshes, combined with direct high-lux directional lighting angle ($82^\circ$).
- **Permanent Prevention Rule:** Non-Negotiable Rule 3 & Supreme Directive 2: Do not solve missing visual assets through lighting, exposure, or material tweaks. Ground must be authored cobblestone.
- **Remediation Task:** Replace test ground meshes with authored Konoha cobblestone street kit (`SM_Road_Center` with `MI_Cobblestone_Anime`).
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-007: Complete Absence of Konoha Architecture & Hokage Monument
- **Defect ID:** `DEF-007`
- **Classification:** `COMPOSITION_DEFECT` & `MISSING_SOURCE_ASSET`
- **Location:** Entire Environment Backdrop
- **Observed Behavior:** Background shows empty blue atmosphere gradient above a dark horizon line; zero village buildings, shopfronts, or landmarks.
- **Root Cause:** Scene was rendered in the smoke test arena where environment geometry is absent.
- **Permanent Prevention Rule:** Non-Negotiable Rule 3: A sky gradient is not a finished environment.
- **Remediation Task:** Validate runtime execution in `L_Konoha_GoldenStreet` featuring full timber streetscape, Ichiraku Ramen shopfront, and Hokage Monument vista.
- **Status:** `OPEN - ACTIVE REMEDIATION`

---

### DEFECT-008: Missing NPC Population and Ambient Village Life
- **Defect ID:** `DEF-008`
- **Classification:** `ASSET_DEFECT`
- **Location:** Avenue Margins
- **Observed Behavior:** Scene contains only an isolated wooden target dummy; no village civilians, merchants, or shinobi patrols.
- **Root Cause:** NPCs only exist in `L_Konoha_GoldenStreet` and were not spawned or activated in the proving testbed.
- **Permanent Prevention Rule:** Rule 3 & Visual DNA: Populate environments with authentic NPC archetypes (Teuchi, Chunin guards, academy students).
- **Remediation Task:** Ensure all 5 Konoha NPCs are active and performing idle routines in `L_Konoha_GoldenStreet`.
- **Status:** `OPEN - ACTIVE REMEDIATION`

