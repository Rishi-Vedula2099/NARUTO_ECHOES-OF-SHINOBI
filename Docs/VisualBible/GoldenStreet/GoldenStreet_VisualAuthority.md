# GOLDEN STREET VISUAL AUTHORITY & SPECIFICATION
## NARUTO: ECHOES OF SHINOBI | Unreal Engine 5.8
- **Document ID:** AEOS-GS-VA-001
- **Visual Authority Reference:** `Konoha_Visual_North_Star.jpg`
- **Secondary Study References:**
  - *NARUTO: Ultimate Ninja STORM* (Village traversal, camera follow, street & rooftop navigation)
  - *Naruto Shippuden: Ultimate Ninja STORM 2* (Village life, NPCs, lived-in storefronts, signage)
  - *Naruto Shippuden: Ultimate Ninja STORM 4* (Character silhouette, spiky hair clumps, cel shading, tailored folds, combat anticipation)
  - *Naruto: Rise of a Ninja* (Environmental density, shop interaction, world vitality)

---

### 1. The Eight-State Asset Contract
Every visual asset in NARUTO: ECHOES OF SHINOBI must be tracked through these states. No proxy or primitive assembly may advance past `BLOCKED_ASSET_AUTHORING`.

| State | Definition & Gate Requirement |
| :--- | :--- |
| **`BLOCKED_ASSET_AUTHORING`** | Required genuine production asset does not exist. Dependent visual gates are strictly blocked. |
| **`SOURCE_ACQUIRED`** | 3D source model/textures acquired; licensing, provenance, and artistic fidelity documented. |
| **`DCC_VALIDATED`** | Topology, watertight geometry, anime proportions, UV unwrapping, bone hierarchy, and skin weights validated. |
| **`UNREAL_IMPORTED`** | Correct asset class verified in UE 5.8 (`USkeletalMesh` for characters, `UStaticMesh` for buildings). |
| **`RUNTIME_VALIDATED`** | Correct asset renders in active gameplay, responds to lighting, and executes locomotion/collision without defects. |
| **`ART_APPROVED`** | Human art-direction sign-off passed against North Star reference (silhouette, joinery, anime aesthetic). |
| **`PACKAGED_QA`** | Tested and verified in standalone shipping/development packaged build at 1080p 60 FPS on GTX 1650. |
| **`PRODUCTION_READY`** | Fully integrated into project registry; locked against regression. |

---

### 2. The Corrected 8-Gate Implementation Pipeline

```
MASTER VISUAL RECONSTRUCTION PIPELINE
│
├── GATE 0: Freeze and Audit (Completed & Verified)
│   ├── Git rollback baseline established
│   └── Defect inventory: 0 skeletal meshes, StaticMesh proxies identified
│
├── GATE 1: Technical Rendering Baseline & Asset Pipeline (Empirical Proving Box)
│   ├── Benchmark active RHI (DX12 SM6 vs SM5) and feature support on target GTX 1650
│   ├── Benchmark shadow methods (VSM vs 4-cascade CSM) and GI alternatives (Screen Space vs None)
│   ├── Calibrate exposure and lighting units (Lux, cd/m²) empirically against material swatches
│   ├── Asset naming, folder structure, import presets (/Game/Characters/, /Game/Environment/)
│   └── 8 Deterministic Test Cameras (Cameras A through H) established for automated capture
│
├── GATE 2: Naruto Production Character Package (Real Model Required)
│   ├── Watertight 3D anime mesh (`SK_Naruto_Genin`), 1:5.5 proportions, sculpted hair clumps
│   ├── Humanoid skeleton, skin weights, tailored tracksuit, Uzumaki crest, sandals
│   └── Turnaround inspection (Front, Back, Left, Right, 3/4) & running locomotion test
│
├── GATE 3: Ichiraku Ramen + One Residence (Modular Architectural Kit)
│   ├── Ichiraku Ramen: Recessed dining alcove, 4 stools, split wave noren ("一楽ラーメン"), red lanterns, Teuchi
│   ├── Two-story residence: Cedar post-and-beam, framed shoji windows, balcony, curved terracotta tiles
│   └── Eye-level close-up inspection before street duplication
│
├── GATE 4: Golden Street Composition & Horizon Vista
│   ├── Avenue setback baseline (X = ±680), unobstructed vanishing point to Hokage Rock
│   ├── Camera framing calibrated interactively (ArmLength ~460, Height ~75, Pitch ~-8°, FOV ~68°)
│   └── Midground pagoda shifted to foliage, Hokage Rock anchored at horizon
│
├── GATE 5: Material & Lighting Art Direction
│   ├── 2-3 band cel-shading response, controlled specular/rim highlights
│   ├── Authored timber wood grain, terracotta tile normal bevels, fabric weave
│   └── Swatch acceptance: Zero crushed black contours, zero blown-out highlights
│
├── GATE 6: Living World & Gameplay Presentation
│   ├── Animated Teuchi behind counter, 7 unique sidewalk NPCs (villagers, Chunin guard)
│   ├── Branching Sakura cherry trees with drifting Niagara blossom petals
│   ├── Overhead utility poles with cables (NoCollision)
│   └── Live GAS-bound HUD: Naruto gold portrait medallion, dynamic HP/Chakra bars, minimap
│
├── GATE 7: Golden Street Standalone Approval
│   ├── Standalone 1080p gameplay capture across all 8 test cameras
│   ├── Human art-direction sign-off against Konoha_Visual_North_Star.jpg
│   └── Combat, Rasengan VFX, traversal, and save/load regression verification
│
└── GATE 8: Multi-Regional Rollout
    ├── Data-driven scalability via FEOSWorldEnvironmentProfile (Suna, Kiri, Kumo, Ame, Iwa, Myoboku, etc.)
    └── Unique regional kits (no recolored Konoha duplicates)
```

---

### 3. Deterministic Test Camera Positions (A through H)
For all subsequent automated regression captures, these 8 deterministic camera views will be used:
1. **Camera A — Street Hero View**: Recreates `Konoha_Visual_North_Star.jpg` framing (Naruto lower-middle third, vanishing point open).
2. **Camera B — Ichiraku Storefront Close-Up**: Focuses on counter, stools, noren, red lanterns, and Teuchi.
3. **Camera C — Right Residence Architectural Close-Up**: Inspects timber posts, beams, balcony balustrade, and terracotta roof tiles.
4. **Camera D — Naruto Full Turnaround**: Front, Left, Right, Back, and 3/4 hero poses.
5. **Camera E — Naruto Locomotion & Deformation**: Idle, Walk, Run, Sprint, Jump, Land, and Dodge.
6. **Camera F — Street-Level NPC Activity**: Evaluates sidewalk pedestrian density, animations, and non-blocking paths.
7. **Camera G — Combat & Jutsu Presentation**: Rasengan casting, hand signs, chakra aura readability, and impact frames.
8. **Camera H — Live HUD Inspection**: Validates portrait medallion, curved gloss gauges, and minimap alignment.

*Note on Comparison Tolerances:* Image difference tolerances must account for Temporal AA, drifting blossom particles, and stochastic rendering. Deterministic captures are used for regression diffs; interactive live captures are used for gameplay feel approval.

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
- Deterministic test camera profiles.
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

#### 7. Approval States
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

