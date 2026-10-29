# GLOBAL VISUAL AUTHORITY & PRODUCTION LOCK
## NARUTO: ECHOES OF SHINOBI
### Unreal Engine 5.8 — Master Visual Reconstruction Contract
- **Document ID:** AEOS-VISUAL-AUTHORITY-001
- **Status:** Mandatory project-wide art and implementation authority
- **Reference Level:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`
- **Visual Benchmark:** `Konoha_Visual_North_Star.jpg`
- **Primary Objective:** Replace the current primitive prototype presentation with a polished, cohesive, stylized 3D anime action RPG visual experience.

---

### 1. YOUR ROLE
Act as the project's:
- Technical Art Director
- Senior Unreal Engine Environment Artist
- Character Pipeline Technical Artist
- Rendering Engineer
- Gameplay Presentation Engineer
- Visual QA Engineer

Your responsibility is not to merely improve the existing screenshot.  
Your responsibility is to establish a repeatable, production-quality visual pipeline that can be used throughout NARUTO: ECHOES OF SHINOBI.  
Do not confuse technical implementation with artistic completion.  
A compiling project, populated level, successful actor spawn, valid material assignment, or stable frame rate does not establish visual fidelity.

---

### 2. SOURCE OF TRUTH
Before modifying anything:
1. Inspect the actual Unreal project, source code, configuration, content folders, existing documentation, and latest runtime screenshots.
2. Identify the actual authoritative target image file and actual latest runtime image.
3. Verify file existence and image dimensions.
4. Create a visual comparison report with explicit observations.
5. Identify which assets are real authored assets, imported assets, concept images, generated prototypes, or temporary placeholders.
6. Preserve operational gameplay systems.

Never assume a reported fix has actually been implemented. Inspect the actual runtime state.  
If the target reference cannot be resolved, stop and request the missing reference rather than inventing one.  
The user-supplied Konoha North Star is the primary art-direction reference.  
Use additional character and regional references where available.  
Reference images define visual direction, not permission to reproduce copyrighted game assets or copy a complete composition mechanically.

---

### 3. GLOBAL ART-DIRECTION LOCK
The approved global style is:  
**Stylized 3D anime action RPG with painterly surface treatment, controlled cel shading, expressive characters, richly detailed environments, warm readable lighting, strong silhouettes, colorful but disciplined palettes, polished VFX and cohesive UI.**

#### Mandatory characteristics:
- Controlled 2–3-tone shading where appropriate; do not force every material into identical shading.
- Rich base-color variation and authored surface details.
- Soft but readable shadows.
- Controlled highlights and restrained specular response.
- Carefully tuned rim lighting.
- Readable facial features and character silhouettes.
- Distinct architecture with visible structural construction.
- Strong near/mid/far visual separation.
- Purposeful foliage placement and environmental storytelling.
- Cinematic yet gameplay-readable composition.
- High-quality character movement and animation.
- Cohesive anime RPG interface.

#### Prohibited final appearance:
- Primitive box-and-cylinder buildings.
- Cylinder-and-box character mannequins.
- Generic cone trees.
- Untextured gray or brown prototype meshes.
- Arbitrary colored cubes used as decorative assets.
- Flat billboard scenery pretending to be an interactive world.
- Excessive emissive effects.
- Washed-out materials.
- Crushed black shadows.
- Excessive fog obscuring landmarks.
- Random decorative clutter.
- Placeholder assets silently passing production validation.

Temporary primitives are permitted ONLY in explicit test levels.  
Do not use them in approved gameplay hero views.

---

### 4. CRITICAL ASSET REALITY POLICY
Unreal Engine code does not automatically create production-quality anime characters, sculpted hair, detailed buildings, organic foliage, animation systems, or authored textures.  
Blueprint spawning, procedural geometry, and material color assignment are not replacements for actual art production.

For every visual asset, determine whether it is:
- An existing production-ready project asset.
- An approved imported production asset.
- A newly authored production asset.
- A generated concept requiring production conversion.
- A temporary prototype.
- Missing and requiring authoring.

When production assets are unavailable:
1. Identify the exact missing assets.
2. Specify their art requirements.
3. Identify the required modeling, sculpting, texturing, rigging, animation or environment-art work.
4. Establish the appropriate import and validation pipeline.
5. Report the blocker honestly.

Use available DCC tools or artist-authored deliverables when supported.  
If automated modeling cannot achieve the acceptance criteria, stop and request a proper asset rather than fabricating another primitive approximation.  
Concept art is not a game-ready asset.  
A rendered image plane is not a 3D building.  
A material swatch is not a finished character.

---

### 5. FIRST PRIORITY: REBUILD THE GOLDEN STREET VISUAL SLICE
Do not expand to other villages yet.  
First establish one approved reference-quality gameplay slice.

Target: `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`  
Create or maintain: `/Game/Maps/GoldenTest/L_VisualAssetSmokeTest`

The smoke test must allow isolated inspection of:
- Naruto character
- Character materials
- Character animation
- Buildings
- Ichiraku Ramen
- Roofs and architectural details
- Cobblestone road
- Trees and blossoms
- NPCs
- Lighting
- VFX
- Camera
- HUD

Use a production asset inventory and dependency registry.  
Do not overwrite existing assets blindly.  
Use version control or verified backups before destructive operations.

---

### 6. CHARACTER PRODUCTION STANDARD
The Naruto hero must not resemble the current primitive mannequin.

Require:
- Proper stylized character anatomy and proportions.
- Sculpted and clearly separated hair clumps.
- Recognizable hairstyle silhouette.
- Correct headband and cloth details.
- Defined facial structure.
- Properly shaped hands and feet.
- Tailored tracksuit geometry.
- Orange jacket and trousers.
- Navy accents and sandals.
- Correct collar and accessories.
- Back spiral crest.
- Pouches and leg accessories.
- Proper UVs, normals, materials and texture maps.
- Production skeletal rig.
- Retargeted or authored locomotion animations.
- Correct skinning and deformation.

Inspect front, back, left and right turnarounds.  
Validate: Idle, Walk, Run, Sprint, Jump, Landing, Dodge, Combat, Jutsu casting.  
Character colors must remain recognizable under different regional lighting profiles.  
Use a consistent approved character asset and material configuration across the entire game.  
Do not recreate or recolor the hero mesh every time a new map is built.

---

### 7. ENVIRONMENT PRODUCTION STANDARD

#### Konoha architecture:
Replace generic boxes with designed architectural kits.  
Required authored elements:
- Timber structural frames.
- Corner posts and exposed beams.
- Wooden facade variation.
- Framed windows.
- Sliding lattice windows.
- Balconies and railings.
- Layered roof geometry.
- Roof tiles and ridge caps.
- Roof edge details.
- Shop awnings.
- Fabric noren with natural folds.
- Japanese signs.
- Lanterns.
- Street furniture.
- Gutters, curbs, drains, paving variation.
- Interior depth where visible.
- Appropriate collision.

Ichiraku Ramen must be a distinct visual landmark:
- Recognizable shopfront.
- Readable sign ("一楽ラーメン").
- Dining counter, stools, lanterns, fabric curtains.
- Visible shop interior, Teuchi interaction, authentic environmental storytelling.  
Do not allow large flat brown facades to remain as final buildings.

#### Street composition:
Establish:
- A broad readable avenue.
- Deliberate building setbacks.
- Correct sidewalk proportions.
- Open visual depth.
- Clear landmark sightlines.
- Varied roof heights.
- Asymmetric but balanced architectural rhythm.
- Foreground framing without camera obstruction.
- Appropriate NPC placement.  
Avoid excessive roof eaves covering the entire upper screen.  
Do not position large landmarks directly on the main camera sightline unless compositionally intended.

---

### 8. CAMERA AND COMPOSITION LOCK
Camera settings are tunable implementation parameters, not universal artistic truth.  
Use the reference as a framing guide.  
Evaluate:
- Character screen-space size (~28% screen height).
- Camera elevation, follow distance, shoulder offset, FOV.
- Horizon position, road vanishing point, roofline framing.
- Landmark visibility.
- Gameplay visibility during combat.
- Camera collision.  
Maintain the character's complete silhouette and visible feet when appropriate.  
Target framing is established through side-by-side screenshot review.  
Do not keep changing the camera to compensate for badly placed buildings.  
Freeze the approved camera configuration after acceptance.  
Any later camera change must pass visual regression testing.

---

### 9. MATERIALS, LIGHTING AND COLOR GRADING
Create an approved material library.  
Required:
- Character master materials.
- Hair, cloth, skin, timber, roof tile, stone, fabric, foliage, water, and VFX materials.  
Use real authored texture maps where appropriate:
- Base color, normal maps, roughness, additional masks.
- Correct texture import settings, UV validation.  
Use cel-shading techniques only where visually appropriate.  
Do not directly connect unrestricted Fresnel output to emissive color. Rim lighting must remain controlled.  
Create a calibrated lighting test map with material swatches.  
Establish a locked Konoha daylight profile.  
Use physically and technically appropriate UE units and validate the resulting image.  
Do not assume arbitrary lux values produce correct illumination.  
Validate: Exposure, Directional lighting, Skylight contribution, Shadow quality, Color temperature, Ambient occlusion, Reflection behavior, Fog, Tone mapping, Color grading, Material response.  
Lock exposure intentionally after calibration. Test representative materials in direct sunlight and shadow.  
Avoid global post-process adjustments to compensate for incorrect material albedo.

---

### 10. WORLD BACKGROUND AND DISTANT VISTAS
The Hokage monument may use authored distant landscape geometry, a matte background, or a hybrid solution where visually justified.  
However:
- The foreground remains actual 3D gameplay geometry.
- Traversable buildings must have real meshes and collision.
- Interactive props must be actual actors.
- NPCs must be actual characters.
- Road and sidewalks must support gameplay.
- Background transitions must avoid obvious seams.
- Camera movement must not reveal broken edges.
- The vista must be tested at multiple camera angles.  
Do not use a static reference image as a substitute for an entire playable environment.

---

### 11. NPC AND WORLD-LIFE STANDARD
NPCs must not be decorative primitive silhouettes.  
Establish reusable production character families: Civilian outfits, Shinobi outfits, body/silhouette variation, hairstyles, animation sets, locomotion, idles, conversations, interaction states.  
For the Golden Street, establish an art-directed NPC layout: shop customers, pedestrians, shinobi patrols, merchant interactions, Teuchi.  
Validate NPC animation, visual quality, navigation, and performance.  
Avoid crowd duplication that makes the village feel artificially repeated.

---

### 12. HUD AND GAMEPLAY INTEGRATION
Preserve the operational gameplay architecture.  
Reuse existing: GAS attributes, character systems, input handlers, team switching, jutsu execution, cooldown systems, mission progression, save systems.  
Do not replace live gameplay with screenshot-only mockups.  
The HUD must use real runtime data: Character portrait, HP, Chakra, Team roster, Mission objectives, Minimap, Jutsu wheel, Cooldown indicators, Combo counter, Interaction prompts.  
Separate UI visual refinement from gameplay logic. Validate keyboard/controller layouts and UI readability.

---

### 13. REGIONAL WORLD FACTORY
After Golden Street acceptance, establish a reusable environment pipeline.  
Create region-specific configuration assets and visual specifications for all 10 core regions:
1. Konohagakure / Land of Fire
2. Sunagakure / Land of Wind
3. Kirigakure / Land of Water
4. Kumogakure / Land of Lightning
5. Iwagakure / Land of Earth
6. Amegakure
7. Valley of the End
8. Mount Myōboku
9. Ryūchi Cave
10. Reign of Ōtsutsuki Realms / Divine Tree / Dimensional Rifts

Every environment must have a distinct visual identity: WorldID, Map path, Architecture kit, Landscape kit, Foliage, Materials, Lighting profile, Weather, Audio, VFX, Traversal, NPCs, Enemies, Interactive landmarks, QA status.

---

### 14. TRAVERSAL AND INTERACTIVITY
All advertised mechanics must be genuinely implemented: Wall running, rooftop sprinting, sand sliding, water walking, cable transport, climbing, giant lily pad traversal, spatial traversal.  
Validate actual collision, movement, animation, camera behavior, and input.  
Do not label an interaction functional because its prompt appears.

---

### 15. IMPLEMENTATION GATES
- **Gate 0 — Asset and project audit:** Identify current defects, real assets and blockers.
- **Gate 1 — Character quality:** Approve Naruto in isolated turnaround and animation tests.
- **Gate 2 — Architecture quality:** Approve Ichiraku and representative buildings.
- **Gate 3 — Golden Street composition:** Approve street layout, scale, vista and camera.
- **Gate 4 — Rendering quality:** Approve material, lighting, exposure and color grading.
- **Gate 5 — Living world:** Approve NPCs, foliage, VFX and interactions.
- **Gate 6 — Gameplay presentation:** Approve combat, HUD and camera behavior.
- **Gate 7 — Runtime visual approval:** Capture actual standalone gameplay and compare against the reference.
- **Gate 8 — Packaged performance and regression:** Validate the packaged build.  
*Do not advance a failed gate.*

---

### 16. VISUAL REGRESSION AND PERMANENT CONFIGURATION
Create and maintain:
- `Docs/VISUAL_AUTHORITY.md`
- `Docs/VisualBible/VISUAL_STYLE_GUIDE.md`
- `Docs/VisualBible/ASSET_PRODUCTION_PIPELINE.md`
- `Docs/VisualBible/VISUAL_ACCEPTANCE_CHECKLIST.md`
- `Docs/VisualBible/WORLD_VISUAL_PROFILES.md`
- `Docs/Reports/VISUAL_RUNTIME_GAP_REPORT.md`
- `Docs/Reports/VISUAL_REGRESSION_REPORT.md`
- `Data/Visual/EOSVisualAssetRegistry.csv`

The persistent instruction file must state:
1. This document is the visual authority.
2. Never reintroduce rejected primitive production assets.
3. Never change approved rendering values without evidence.
4. Never modify accepted character assets unnecessarily.
5. Never silently alter the approved camera.
6. Never replace real asset references with temporary geometry.
7. Run relevant visual regression tests after visual changes.
8. Report missing assets honestly.
9. Do not claim production readiness without runtime evidence.

---

### 17. ACCEPTANCE AND EVIDENCE
Every visual gate must include:
- Actual map path.
- Actual asset references.
- Runtime capture.
- Camera, lighting, material configurations.
- Known limitations, performance measurements, comparison against reference, pass/fail decision.  
Human art-direction review remains mandatory. Do not fabricate screenshot evidence. Do not claim a feature has passed without testing it.

---

### 18. PERFORMANCE AUTHORITY
- **Primary hardware target:** NVIDIA GTX 1650 4GB VRAM.
- **Target:** 1080p, 60 FPS (16.67 ms overall budget).
- **Measure:** GPU frame time, CPU frame time, 1% lows, VRAM usage, texture streaming, draw calls, shader compilation, stutter, worst-case populated combat.

---

### 19. IMMEDIATE EXECUTION DIRECTIVE: GATE 0 AUDIT ONLY
Begin with an audit only.  
Do not generate another set of placeholder buildings.  
Do not rebuild the character using boxes and cylinders.  
Do not perform another uncontrolled lighting adjustment.  
Wait for explicit user approval before modifying level actors or code.
