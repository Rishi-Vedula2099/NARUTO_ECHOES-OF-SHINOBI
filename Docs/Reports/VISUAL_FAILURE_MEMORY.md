# NARUTO: ECHOES OF SHINOBI
# VISUAL FAILURE MEMORY & RECTIFICATION ARCHIVE

**Status:** ACTIVE REPOSITORY MEMORY  
**Purpose:** Permanent record of visual failures, root causes, and non-negotiable prevention rules.

---

## VF-001 — Procedural Graybox Map Execution
- **Date:** 2026-10-01
- **Screenshot Evidence:** Mannequin standing among white cylinders and cubes with red Lumen error text.
- **Problem:** Game launched into procedural graybox testbed rather than the authored street.
- **Root Cause:** `RunGame.bat` and `LaunchGame.ps1` default Option 1 was hardcoded to `/Game/Maps/Konoha/L_Konoha_Phase13_District`.
- **Permanent Lesson:** C++ actor existence does not equal visual completion. Launchers must default to authored Golden maps.
- **Prevention Rule:** Option 1 must permanently launch `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`.

---

## VF-002 — Lumen Ray Tracing Warning & Overexposed Ambient Glare
- **Date:** 2026-10-01
- **Screenshot Evidence:** Red banner: "Lumen is enabled, but has no ray tracing data and won't operate correctly." Scene washed out in blinding white-blue glare.
- **Problem:** Lumen software distance fields failed to generate, destroying indirect lighting and occlusion.
- **Root Cause:** Lumen GI was enabled in `DefaultEngine.ini` while mesh distance fields were disabled.
- **Permanent Lesson:** Visual Style > Technology. High-end GI features must not be enabled when they break the target visual aesthetic or trigger viewport error banners.
- **Prevention Rule:** Use Screen Space GI + 4-cascade high-res shadow maps with locked exposure for GTX 1650 60 FPS anime rendering.

---

## VF-003 — Missing Protagonist Model (White Mannequin Fallback)
- **Date:** 2026-10-01
- **Screenshot Evidence:** Generic cylinder/capsule mannequin with eye visor.
- **Problem:** Naruto character model did not render.
- **Root Cause:** `AEOSCharacterBase::VisualProfile.CharacterID` was uninitialized in constructor, so soft reference to `SK_Naruto_Genin` was left null.
- **Permanent Lesson:** Character runtime validation must explicitly verify `CharacterID != ""` and load production mesh + materials.
- **Prevention Rule:** `AEOSCharacterBase` constructor and `BeginPlay` must default to `CHAR_NARUTO`.

---

## VF-004 — Crushed Dark Void with Blinding White Emissive Wall
- **Date:** 2026-10-01
- **Screenshot Evidence:** Black sky/environment void on left, pitch-black character silhouette in center, and blinding 255,255,255 white glowing rectangular wall on right.
- **Problem:** The scene lacks ambient daylight; point lights on paper lanterns placed next to the shop wall are blowing out the material to pure white, while the sky and sun fail to illuminate the character.
- **Root Cause:**
  1. Directional Sun was not illuminating the street correctly (light angle or obstacle occlusion).
  2. Point lights placed at lanterns had high intensity directly against the wall surface with no light falloff exponent or bad material roughness.
  3. Sky Atmosphere was in a black void because there was no SkyDome / SkyAtmosphere directional sun coupling in the level.
  4. The character in the level was rendered unlit or shadowed in pitch black because ambient fill was missing.
- **Permanent Lesson:** Do not place point lights directly against diffuse/emissive walls. Directional daylight must illuminate both character and street evenly with soft azure sky fill.
- **Prevention Rule:** Test lighting with a dedicated SkyDome + Directional Sun + SkyLight with cast shadows, ensuring ambient light minimum level is never 0 (no crushed black silhouettes).

---

## VF-005 — Phase 1 Rendering Baseline Fix (Root Cause Resolution)
- **Date:** 2026-10-02
- **Problem:** Recurring black void and white blowout scenes across multiple sessions.
- **Root Causes Identified (3):**
  1. **DirectionalLight intensity = 65000.0 lux** in `BuildAnimeDaylightLighting()` — astronomically high, causing white blowout on all materials.
  2. **VillageBuilder disabled ALL lighting** when authored maps had ≥5 StaticMeshActors but the authored map had no dedicated DirectionalLight actors — causing BLACK VOID.
  3. **Lumen GI enabled** (`r.DynamicGlobalIlluminationMethod=1`) without mesh distance fields, causing "Lumen has no ray tracing data" red banner + visual corruption.
  4. **AutoExposure enabled** (`r.DefaultFeature.AutoExposure=True`) causing unstable brightness pumping between frames.
- **Fixes Applied:**
  1. Reduced DirectionalLight intensity from 65000.0 to 8.0 lux.
  2. VillageBuilder now checks if authored map has its own DirectionalLight before disabling builder lighting; provides fallback lighting if map has none.
  3. Disabled Lumen (`r.DynamicGlobalIlluminationMethod=0`), using Screen Space GI instead.
  4. Disabled AutoExposure (`r.DefaultFeature.AutoExposure=False`), using manual exposure control.
  5. Added SkyLight + HeightFog configuration to `BuildAnimeDaylightLighting()` so all lighting components are initialized properly.
- **Prevention Rule:** NEVER set DirectionalLight intensity above 10.0 for anime rendering. NEVER disable lighting without verifying the map has alternative lighting actors. ALWAYS use manual exposure for stable scenes.

---

## VF-006 — Village NPC Mesh Loading in BeginPlay Only (Invisible in Viewport)
- **Date:** 2026-10-19
- **Problem:** `AEOSVillageNPC` actors placed in `L_Konoha_GoldenStreet` had `Mesh=NONE` and `Mat=NONE` in editor inspection and did not visually render in the Unreal Editor viewport without starting PIE.
- **Root Cause:** `NPCVisualMeshComp` was dynamically allocated via `NewObject<UStaticMeshComponent>` only inside `BeginPlay()`, which never executes in the Editor viewport outside of PIE.
- **Fix Applied:**
  1. Initialized `NPCVisualMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NPCVisualMeshComp"));` in `AEOSVillageNPC` constructor and attached to `CapsuleComponent`.
  2. Implemented `OnConstruction(const FTransform& Transform)` and `ApplyNPCTypeVisuals()` to resolve and bind the matching stylized mesh and anime material instance at edit-time and run-time.
  3. Preserved custom authored IDs, names, and dialogue lines when properties are populated.
- **Prevention Rule:** ALL visual components for characters, NPCs, props, and environment actors MUST be created as default subobjects or bound in `OnConstruction()` so production meshes render natively in the Unreal Editor viewport.

---

## VF-007 — Training Dummy /Engine/BasicShapes/Cylinder Fallback Elimination
- **Date:** 2026-10-20
- **Problem:** `AEOSTrainingDummy` constructor was bound to `/Engine/BasicShapes/Cylinder.Cylinder`, emitting continuous `TECHNICAL_DEBUG_ASSET` warnings in every run and rendering an untextured primitive cylinder fallback in editor viewports when unassigned.
- **Root Cause:** Placeholder `/Engine/BasicShapes/Cylinder` was used as constructor fallback pending authoring of `SM_Prop_TargetBoard` and `SM_Prop_TrainingLog_Post`.
- **Fix Applied:**
  1. Authored and imported production `SM_Prop_TargetBoard` static mesh with `MI_AnimeCloth`.
  2. Assigned `MI_AnimeWood` to `SM_Prop_TrainingLog_Post`.
  3. Replaced `CylinderFinder` in `AEOSTrainingDummy` constructor and `OnConstruction` with direct bindings to `SM_Prop_TrainingLog_Post` and `SM_Prop_TargetBoard`.
  4. Fully eradicated `/Engine/BasicShapes` reference from the combat pipeline.
- **Prevention Rule:** NEVER use `/Engine/BasicShapes` primitives in gameplay or combat actor constructors. Always author or bind canonical production meshes directly.

---

## VF-008 — Golden Street Tilted Geometry, Pitch-Black Shadow & Mannequin Monomaterial
- **Date:** 2026-10-21
- **Problem:** Level actors tilted 45°-90° on their sides with a giant pagoda barrel blocking the street; pitch-black shadow over roadway; hero character rendered as a pale single-material silhouette.
- **Root Causes:**
  1. Python `unreal.Rotator(*rot)` positional unpacking mapped the 2nd element to Pitch instead of Yaw, tilting buildings onto their sides.
  2. A 24,000-unit giant Hokage cliff box rotated -90° sliced down the street, casting an enormous shadow and occluding sun/sky.
  3. Directional sun low angle (pitch=-35°, yaw=55°) caused buildings and foliage behind the player to cast long occluding shadows over the avenue.
  4. Hero OBJ lacked `usemtl` tags, causing UE5 to merge all faces into Section 0 and assign pale skin material over the entire character.
- **Fix Applied:**
  1. Rebuilt level using explicit keyword rotators: `unreal.Rotator(pitch=0.0, yaw=yaw, roll=0.0)`.
  2. Removed occluding cliff box and set North Star backdrop at Y=3600 horizon.
  3. Re-authored `SK_Naruto_Genin.obj` with 5 distinct material groups (`Skin`, `Headband`, `Hair`, `Collar`, `Tracksuit`), smoothed vertex normals, and bound all 5 anime material instances.
  4. Aligned high anime sun (pitch=-70°, yaw=90°, 8.5 lux) down avenue axis; boosted SkyLight ambient fill to 8.5 lux (soft azure) and locked PostProcess exposure.
- **Prevention Rule:** Always use explicit keyword rotators (`pitch=0, yaw=Y, roll=0`) in Unreal Python scripts. Always declare `usemtl` per section in authored OBJs. Use high overhead sun (`pitch <= -65°`) aligned with avenue axes to prevent cross-street occlusion.

---

## VF-009 — Master Cel Material Pin Disconnect, Slicing Backdrop & Zero-Irradiance SkyLight
- **Date:** 2026-10-21
- **Problem:** Scene rendered with grey checkerboard materials on road, buildings, and hero character; a giant vertical board sliced down the avenue; shadowed surfaces fell to pitch-black void.
- **Root Causes:**
  1. `MEL.connect_material_expressions(tex_param, "RGB", ...)` failed because TextureSampleParameter2D default output pin is `""` (Color), not `"RGB"`. The unconnected Multiply node caused master shader compilation failure, falling back to WorldGrid checkerboard.
  2. `Backdrop_Konoha_NorthStar_Vista` plane was rotated `Rotator(pitch=0, yaw=90, roll=-90)`, aligning its edge along the street and slicing through the avenue center.
  3. `SkyLight` had `real_time_capture = False` with no pre-baked cubemap, yielding zero indirect irradiance and pitch-black shadows.
- **Fix Applied:**
  1. Rewrote `M_AnimeCel_Textured_Master` with valid default `T_Cobblestones_D` texture, connected using `""` to BaseColor and an Emissive ambient floor (0.22).
  2. Re-parented all material instances (`MI_Road_Cobblestone`, `MI_Bld_Ichiraku_Wood`, `MI_Bld_Ichiraku_Roof`, `MI_Bld_Ichiraku_Noren`, `MI_Naruto_Tracksuit`, etc.) and updated all mesh slot bindings.
  3. Re-aligned backdrop plane to `Rotator(pitch=90.0, yaw=0.0, roll=0.0)` spanning X = -3250 to +3250 at Y = 3600 facing the camera.
  4. Enabled `SkyLight.real_time_capture = True` and set azure lower hemisphere fill.
- **Prevention Rule:**
  - Always use `""` (empty string) for default Color output pins in `unreal.MaterialEditingLibrary`.
  - Always assign a valid default `Texture` to `MaterialExpressionTextureSampleParameter2D` before compiling.
  - Never disable SkyLight real-time capture without an authored high-resolution sky cubemap.

---

## VF-010 — Procedural Box Mannequin & Monolithic Brown Box Architecture Passed as Production Golden Street
- **Date:** 2026-10-23
- **Problem:** Standalone engine captures (`ScreenShot00021.png` / `ScreenShot00023.png`) revealed primitive block limbs on Naruto, monolithic brown box buildings, excessively thick slab roofs, and tunnel corridor perspective.
- **Root Causes:**
  1. **Procedural Geometry Substitute:** Hero character, buildings, and vegetation were generated via procedural Python box/cylinder concatenation scripts (`make_box`, `make_cylinder`) rather than genuine authored 3D stylized models.
  2. **Runtime C++ Slot Array Conflict:** `AEOSCharacterBase::ResolveProductionVisualDefaults` hardcoded a 5-material array for `CHAR_NARUTO`, overriding material slots at runtime without supporting multi-slot models.
  3. **Tight Building Setbacks:** Structures placed at $X = \pm 480$ with eaves reaching $X = \pm 250$ occluded the camera frustum into an unnatural narrow tunnel.
- **Permanent Lesson:** A technical PASS (mesh exists, materials assigned, actor transforms valid, FPS stable) MUST NEVER be treated as an artistic PASS. Scaffolding and procedural boxes are not production assets.
- **Prevention Rule:**
  1. Reject all procedural primitive concatenations for hero characters and hero architecture.
  2. Enforce Phase Gate progression requiring side-by-side reference verification before declaring visual completion.
  3. Character runtime materials must be mapped dynamically by material slot name rather than hardcoded index arrays.

---

## VF-011 — Proving Ground Map Misdirection & Missing C++ Runtime Skeletal Mesh Assignment
- **Date:** 2026-10-30
- **Screenshot Evidence:** `runtime_smoke_test_failure_baseline.jpg` (`media_1793283768094_460b3b9b.jpg`): Grey untextured proxy character standing in front of floating material spheres, wooden training dummy, and reflective test rails under an unpopulated sky in `L_VisualAssetSmokeTest`.
- **Classification:** `DEF-001` (Character Mesh Defect) & `DEF-004` (Scene Misdirection)
- **Root Causes:**
  1. **Map Misdirection:** Standalone automation launcher dispatched to `L_VisualAssetSmokeTest` (calibration testbed) rather than the primary golden map `L_Konoha_GoldenStreet`.
  2. **Missing C++ Skeletal Mesh Assignment:** `AEOSCharacterBase::SetupStylizedVisualMesh()` only set relative transform `(0, 0, -96)` and `(0, -90, 0)` but lacked `SetSkeletalMeshAsset(...)` and `SetAnimInstanceClass(...)`, leaving dynamically spawned pawns with fallback/proxy geometry.
  3. **Unresolved External Linker Symbols:** `ValidateVisualAssetIntegrity`, `ApplyMeshComponentLayout`, `ResolveProductionVisualDefaults`, and `SpawnJutsuVFXAtSocket` were declared in `EOSCharacterBase.h` but lacked function definitions in `EOSCharacterBase.cpp`, breaking UBT compilation.
  4. **Facial Feature Defect:** Character model lacked facial textures, catchlights, whiskers, and blend shapes.
- **Fix Applied:**
  1. Implemented all declared C++ methods in `EOSCharacterBase.cpp`: `ValidateVisualAssetIntegrity(bool)`, `ResolveProductionVisualDefaults()`, `ApplyMeshComponentLayout()`, `SetupStylizedVisualMesh()`, and `SpawnJutsuVFXAtSocket()`.
  2. Bound canonical soft object paths for `SK_Naruto_Genin`, `ABP_Naruto_Genin`, 7 `MI_Naruto_*` materials, and `NS_Rasengan_Orb_Master` Niagara system in `AEOSCharacterBase::AEOSCharacterBase()`.
  3. Recompiled `EchoesOfShinobiEditor Win64 Development` via UBT with 0 errors.
  4. Authored high-resolution 2048x2048 anime facial texture `T_Naruto_Genin_D.tga` and connected to `MI_Naruto_Skin`.
  5. Synchronized the 7-slot production skeletal mesh across all mesh registry paths (`/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin`, `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged`, and `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin`).
  6. Replaced proxy actor in `L_VisualAssetSmokeTest` and spawned hero Naruto in `L_Konoha_GoldenStreet`.
  7. Rendered native 1080p runtime evidence: `gate2_naruto_face_closeup.png`, `gate2_naruto_three_quarter_fresh.png`, and `gate2_konoha_golden_street_fresh.png`.
- **Permanent Prevention Rule:**
  - Never allow standalone launchers to route default execution to `L_VisualAssetSmokeTest`. Default map must permanently remain `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`.
  - In `AEOSCharacterBase`, constructor and `SetupStylizedVisualMesh()` must synchronously load and assign production skeletal mesh and material slots.
  - Automation tests must enforce that `Shinobi->ValidateVisualAssetIntegrity()` returns verdicts for all slots and fails if any placeholder/basic shape is detected.

---

## VF-012 — Procedural Primitive Concatenation vs. Refined Anime Geometry Standard
- **Date:** 2026-10-31
- **Screenshot Evidence:** Procedural concentric ellipse lofts, cone hair spikes, and flat box-stacked buildings in `L_Konoha_GoldenStreet`.
- **Classification:** `ASSET_DEFECT` / `PRIMITIVE_SUBSTITUTION_DEFECT`
- **Root Causes:**
  1. Early scripts generated character models via concentric ellipse lofts (`make_profile_loft`) and basic cones for hair spikes rather than anatomical polygonal facial planes, 3D oral cavity with individual teeth, and faceted hair clumps.
  2. Architectural models used stacked rectangular prisms (`make_box`) lacking curved terracotta roof tiles (`kawara`), eaves brackets (`tokyō`), sliding lattice shoji screens, or authentic timber joinery.
  3. The Hokage Monument was placed as a flat 2D plane backdrop instead of an authored 3D sculpted canyon mountain cliff with the four Hokage face reliefs.
- **Fix Applied:**
  1. **Refined Hero Character Model (`SK_Naruto_Genin` / `SK_Naruto_Genin_Rigged.glb`):**
     - Authored `author_refined_production_naruto.py` producing 3,899 clean triangles with 0 degenerate indices and 0 zero-area triangles across 7 production material slots.
     - 3D oral cavity with upper and lower curved dental arches (individual teeth) and tongue in Naruto's open grin.
     - Modeled curved metal forehead plate with stamped Konoha crest and 4 corner dome rivets.
     - 28 faceted anime hair clumps with knife-edge crease ridges catching cel-shading light bands.
     - High stand-up shearling fleece collar with rolled cylindrical rim, zipper track, and silver teardrop slider.
     - Tailored orange windbreaker jacket with navy shoulder yokes, elastic waist hem, and back Uzumaki crest.
     - Rolled-up double-layer wrist cuffs and articulated shinobi hands with palms, thumbs, and knuckles.
     - Right thigh white medical bandage wraps and black kunai holster pouch with buckle.
     - Baggy ninja trousers with knee crease folds and rolled ankle cuffs.
     - Authentic navy sandals with 5 individually sculpted anime toes and rubber tread soles.
  2. **Refined Japanese Timber Architecture (`SM_Bld_IchirakuRamen`, Residences, Monument):**
     - Authored `author_refined_konoha_architecture.py` replacing box primitives with post-and-beam cedar frames, mortise-and-tenon joints, recessed dining counter, 4 turned wooden bar stools, 3-panel split noren fabric curtains with wavy folds ("一楽ラーメン"), hanging paper lanterns, and curved terracotta roof tiles.
     - Authored 3D sculpted canyon cliff with the carved stone face reliefs of Hashirama, Tobirama, Hiruzen, and Minato.
  3. **Refined Valley of the End Benchmark (`SM_Lnd_ValleyOfTheEnd_Statues`):**
     - Authored `author_refined_valley_of_the_end.py` depicting Madara Uchiha and Hashirama Senju forming the confrontation hand seal (`対立の印`) across the roaring river gorge waterfall.
  4. **Runtime Verification:** Captured native 1080p verified frames `ScreenShot00057.png` (`golden_street_runtime_verified.png`) and `ScreenShot00058.png` (`naruto_showcase_runtime_verified.png`).
- **Permanent Prevention Rule:**
  - Never generate production characters or buildings by stacking primitive geometric shapes (boxes, cones, cylinders).
  - Every character must adhere to the 5-component standard (Anatomical Head + Modeled Oral Cavity & Teeth + 4-Rivet Headband + Faceted Anime Hair Clumps + Tailored Cloth Folds & Rolled Cuffs + Exposed Toes Sandals).
  - Every environment must feature authored timber framing, curved roof tiles, and 3D landmark vistas.
