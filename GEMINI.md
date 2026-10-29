# NARUTO: ECHOES OF SHINOBI — PERMANENT VISUAL PRODUCTION AUTHORITY & RECONSTRUCTION CONTRACT
## Engine: Unreal Engine 5.8 | Supreme Visual Directive

Create a stylized 3D anime action RPG with a painterly anime finish, controlled cel shading, warm and expressive colors, recognizable shinobi silhouettes, detailed but optimized Japanese-inspired environments, cinematic composition and fluid third-person gameplay.

The visual identity must feel like a living shinobi adventure, not a generic Japanese village simulator, a photorealistic open world, a primitive low-poly prototype or a static anime illustration.

---

### CORE PILLARS
1. **Shinobi-first characters**: Distinct face, hair silhouette, tailored costume, expressive locomotion, and fluid combat.
2. **Authored, visually distinctive architecture and environmental storytelling**: Timber frames, layered eaves, roof tiles, shop interiors, ramen counters, and regional construction.
3. **Rich regional atmosphere and environmental depth**: Living world with wind, drifting petals, ambient life, NPC routines, and vertical exploration.
4. **Fluid shinobi movement and readable action combat**: Clear silhouettes, readable anticipation, distinct chakra/handsigns/impact VFX.
5. **Consistent art direction, HUD language, materials, lighting and VFX**: Preserved across Konoha, all 5 Great Shinobi Villages, secondary settlements, and dimensional realms.

---

### THE 14 NON-NEGOTIABLE PRODUCTION RULES

1. **Inspect Before Modifying**: Inspect the actual repository, existing implementation, assets, engine version, rendering configuration, and source-control state before modifying anything.
2. **Verifiable Evidence Required**: Never claim an asset exists, is production-ready, or has passed visual validation without verifiable runtime evidence.
3. **No Primitive Substitutions**: Never substitute primitive boxes, cylinders, default mannequins, flat color materials, or repeated placeholder NPCs for required production art.
4. **Honest Blocker Reporting**: If a required production asset is unavailable, record `BLOCKED_ASSET_AUTHORING`, identify its dependencies, and specify the actual authoring/acquisition task. Never disguise the blocker.
5. **Art-Direction Faithfulness**: The Konoha North Star is the primary art-direction benchmark. Reproduce its visual hierarchy, character presentation, architecture, atmosphere, environmental richness, and composition—not merely its colors.
6. **Camera Separation**: Treat the fixed art-review camera (`CAM_A`) and gameplay follow camera as separate camera profiles. Do not distort gameplay framing to manufacture a matching screenshot.
7. **Traceable Asset Provenance**: All character, environment, material, animation, and VFX assets must have traceable sources, stable IDs, version information, and defined maturity states.
8. **No Artificial Defect Masking**: Never change global lighting, exposure, post-processing, camera settings, or unrelated assets to hide a local asset defect.
9. **Preserve Operational Systems**: Preserve existing C++, GAS, combat, progression, save, input, AI, and world systems. Reuse operational architecture rather than creating duplicates.
10. **Distinct Regional Identities**: Every region must have its own visual identity, environment profile, architecture, landmarks, vegetation, NPCs, traversal, and regional validation.
11. **Runtime Verification**: Test actual runtime rendering. Compilation, successful imports, actor spawning, and screenshots of editor previews do not prove visual completion.
12. **Recoverable Regression Lock**: Use source-control checkpoints and regression testing. Approved assets may only change through explicit, reviewable modifications with rollback support.
13. **Exhaustive Acceptance**: Do not mark an asset or gate complete without evidence for every required acceptance criterion.
14. **Truth in Validation**: Never fabricate reference matches, benchmark results, hardware performance, test completion, or human approval.

---

### NON-NEGOTIABLE VISUAL QUALITY RULES
1. **A visible character must not be mistaken for a mannequin, robot, debug actor or generic placeholder.**
2. **A production environment must not contain unexplained floating spheres, test primitives, disconnected platforms or exposed blockout geometry.**
3. **A sky gradient is not a finished environment.**
4. **A functioning HUD is not proof that the gameplay experience is finished.**
5. **Correct material colors are not proof of correct character modeling.**
6. **Correct actor rotation is not proof of correct architecture.**
7. **A populated registry is not proof of implemented assets.**
8. **A successful build is not proof of visual acceptance.**
9. **A screenshot is not proof of playable functionality.**
10. **Every accepted visual change must be verified in the real runtime.**
11. **Never hide missing assets by silently substituting unrelated meshes.**
12. **Preserve existing valid gameplay systems and fix the smallest responsible layer.**
13. **Every missing production asset must have a precise authoring specification and status.**
14. **Keep debug content in dedicated development/QA environments.**
15. **Maintain a coherent global visual identity across the entire game.**
16. **Build one gold-standard vertical slice before mass-producing characters, stages and worlds.**
17. **No task is complete until its acceptance criteria are verified.**
18. **Report actual implementation, not intended implementation.**

---

### THREE-TIER LOCKING POLICY (SUSTAINABLE DEVELOPMENT)

To prevent locking in bugs or freezing bad numbers while permanently protecting art direction:

| Category | Locking Policy | Modification Rule |
| :--- | :--- | :--- |
| **1. Visual Identity** | **Permanently Locked** | Supreme art-direction pillars (cel shading bands, timber architecture, anime proportions). Cannot be altered without project-level design review. |
| **2. Approved Asset Revisions** | **Version-Controlled & Protected** | Once an asset passes its gates, its mesh and textures are locked. Unrelated scripts cannot alter it. Changes require a named change request and regression run. |
| **3. Technical Tuning Values** | **Changeable Through Testing** | Light Lux, SkyLight cd/m², exposure EV, shadow cascades, and bias remain empirically tunable to eliminate rendering artifacts and optimize performance. |

---

### ADDENDUM — AEOS VISUAL CONSISTENCY & REGRESSION LOCK

#### 1. Purpose
Prevent repeated visual regressions, unnecessary art-direction changes, placeholder replacement cycles, and arbitrary rendering modifications throughout NARUTO: ECHOES OF SHINOBI.

#### 2. Approved Visual Baseline
Every approved production asset and world configuration must have:
- Stable AssetID / WorldID.
- Version-controlled source and configuration.
- Approved reference images.
- Approved material and lighting configuration.
- Validated asset dependencies.
- Deterministic test camera profiles (CAM_A through CAM_H).
- Runtime visual evidence.
- Human art-direction approval.
- Packaged QA status.

#### 3. Immutable Approval Rule
Once an asset passes its applicable visual and technical gates:
1. Record its approved revision.
2. Record its source files and configuration.
3. Preserve its approved camera captures.
4. Protect it from unrelated automatic modifications.
5. Require explicit change authorization for modifications to approved visual parameters.  
*Do not silently overwrite approved production assets.*

#### 4. Regression Detection
Every change affecting an approved asset must trigger the relevant tests:
- Asset existence and class validation.
- Material assignment validation.
- Skeleton and animation validation where applicable.
- Reference camera capture.
- Visual regression comparison.
- Gameplay validation.
- Performance checks where relevant.  
*A failed regression blocks promotion.*

#### 5. Change Isolation
When correcting Naruto, do not automatically modify:
- Global exposure.
- Other character materials.
- Village architecture.
- Camera settings for unrelated scenes.
- Regional lighting profiles.
- HUD layout.
- Combat tuning.  
*When correcting global rendering settings, rerun all affected regression tests.*

#### 6. Visual Defect Escalation
Every visual defect must be classified into one of ten specific categories before fixing:
1. `ASSET_DEFECT` (Missing/incorrect 3D mesh geometry)
2. `MATERIAL_DEFECT` (Incorrect shader, albedo, normal, or cel-band response)
3. `LIGHTING_DEFECT` (Improper Sun Lux, SkyLight intensity, or angle)
4. `CAMERA_DEFECT` (FOV, boom length, height, or socket offset error)
5. `COMPOSITION_DEFECT` (Building setback, road width, or vista sightline error)
6. `ANIMATION_DEFECT` (Rigging, skinning, or state machine glitch)
7. `VFX_DEFECT` (Chakra, petal particles, or impact frame occlusion)
8. `UI_DEFECT` (HUD styling, alignment, or GAS data-binding fault)
9. `PERFORMANCE_DEFECT` (Frame time exceeding 16.67 ms, VRAM leak, stutter)
10. `MISSING_SOURCE_ASSET` (Art asset not yet authored or acquired)  
*Correct the actual cause instead of applying arbitrary compensation.*

#### 7. Eight Asset Maturity States
- `BLOCKED_ASSET_AUTHORING`
- `SOURCE_ACQUIRED`
- `DCC_VALIDATED`
- `UNREAL_IMPORTED`
- `RUNTIME_VALIDATED`
- `ART_APPROVED`
- `PACKAGED_QA`
- `PRODUCTION_READY`  
*No automated process may grant ART_APPROVED or PRODUCTION_READY without the required evidence and human approval.*

#### 8. Scope Protection
Do not expand world production, mass-generate character variants, or populate all stages until the Golden Street vertical slice passes its mandatory gates.

#### 9. Recovery
Before modifying approved production assets:
- Verify the working branch and clean/dirty state.
- Establish a rollback point.
- Identify exact affected files.
- Produce a proposed change list.
- Preserve unrelated user changes.

#### 10. Final Law
- Code compilation is **not** visual completion.
- Actor spawning is **not** visual completion.
- Material assignment is **not** visual completion.
- Only verified, art-approved, runtime-tested, packaged assets qualify for production readiness.

---

### MANDATORY TASK REPORTING FORMAT
For each task executed, report:
- Actual files inspected or modified
- Asset source and maturity state
- Existing functionality preserved
- Build/test results
- Runtime screenshot evidence
- Known defects and blockers
- Performance measurements and test hardware
- Gate status: PASS, FAIL, BLOCKED, or NOT TESTED

---

### PERMANENT VISUAL ACCEPTANCE CONTRACT

#### 1. The Three Distinct Achievements (Never Conflate Them)
1. **Technical Success**: Unreal builds, C++ compiles, actors spawn, 60 FPS achieved on empty/low-poly geometry. *(Prerequisite only; NOT visual completion).*
2. **Visual Success**: Production 3D anime models, authentic timber architecture, stylized cel materials, organic foliage, and living NPCs match the North Star reference in runtime rendering.
3. **Production Success**: The entire slice remains consistent, performant, packaged, and regression-locked across all test camera profiles.

#### 2. Concrete Character Acceptance Criteria (`SK_Naruto_Genin`)
- **Proportions**: Anatomical anime proportions (~1:5.5 head-to-body ratio for Genin).
- **Hair**: Sculpted multi-layered blonde hair clumps with clear silhouette separation.
- **Headband**: Modeled metallic plate with stamped Konoha leaf crest and cloth knot behind the head.
- **Garments**: High upright white collar; tailored orange jacket with navy shoulder yokes and 3D back red Uzumaki spiral; right-thigh white bandage wrap with black kunai holster pouch.
- **Sandals**: Authentic ninja sandals with exposed toes and dark navy cloth straps.
- **Skeleton & Deformation**: Humanoid skeletal rig with vertex skin weights. Zero mesh tearing or pinching during idle, walk, run, sprint, jump, fall, land, and dodge.
- **Materials**: 2-3 band cel response, soft shadow floor ($\ge 0.15$ luminance to prevent crushed blacks), controlled specular rim highlights.

#### 3. Concrete Architectural Acceptance Criteria
- **Timber Construction**: Visible post-and-beam cedar frames, mortise-and-tenon joints, corner posts, and exposed beams.
- **Window & Wall Details**: Sliding lattice shoji screens, framed glass windows, and textured wall infill.
- **Roof Architecture**: Multi-tiered sloped roofs lined with curved terracotta clay tiles, eaves brackets, and ridge end caps.
- **Balconies**: Second-story cantilevered wooden balconies with balustrade railings.
- **Ichiraku Ramen Shopfront**: Open-front dining alcove, recessed wooden counter, 4 rounded stools, authentic split fabric noren curtains ("一楽ラーメン") with wave folds, red paper lanterns with warm amber emissive material, and Teuchi at the counter.

#### 4. Concrete Composition & Camera Criteria
- **Avenue Layout**: Broad readable avenue with building facade baseline at $X \approx \pm 680$.
- **Camera Staging**: Naruto centered in lower-middle third, covering $\approx 28\%$ vertical screen height in Camera A.
- **Vista Sightline**: Completely unobstructed central vanishing point framing the towering Hokage Rock cliff monument (Hashirama, Tobirama, Hiruzen, Minato faces) under an azure sky.
- **Camera Separation**: Fixed art-review camera (`CAM_A`) used exclusively for reference comparison; dynamic gameplay camera tuned for 3rd-person movement and collision.

#### 5. Concrete Performance & Packaging Criteria
- **Resolution**: 1920×1080 native.
- **Frame Budget**: Total frame time $\le 16.67\text{ ms}$ ($\ge 60\text{ FPS}$) on NVIDIA GeForce GTX 1650 4GB.
- **Profiling Metrics**: Track Game Thread, Render Thread, GPU Time, VRAM usage, and 1% lows under active combat and populated street conditions.

