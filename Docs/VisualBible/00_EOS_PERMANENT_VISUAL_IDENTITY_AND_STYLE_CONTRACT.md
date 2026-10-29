# NARUTO: ECHOES OF SHINOBI
## EOS MASTER VISUAL + CONTENT + RUNTIME PRODUCTION CONTRACT
### *Authoritative Style Law, Permanent Visual Identity, Content Architecture & Master Acceptance Contract*

> **Document Version:** 4.0.0-MASTER-AUTHORITY  
> **Master Visual Reference ID:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`  
> **Authoritative Visual Target:** `Docs/References/Phase13/Konoha_Visual_North_Star.jpg`  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development, DirectX 12 / SM6)  
> **Target Hardware:** NVIDIA GeForce GTX 1650 (1920x1080 @ 60 FPS Locked, 1% low > 45 FPS)  
> **Applicable Scope:** All characters, forms, skins, weapons, gear, jutsu VFX, summons, tailed beasts, NPCs, enemies, bosses, villages, lands, cards, gacha, collection codex, bonds, shinobi intel, motion manga, events, UI, and future expansions.

---

## 1. The Immutable Master Principle

**NARUTO: ECHOES OF SHINOBI is one persistent visual and technical universe.**  
New characters, skins, cards, jutsu, villages, lands, events, dimensions, and future expansions must extend the established **EOS Visual DNA** rather than introduce an unrelated art direction.

- **Regional variation** changes the world identity; it does **not** change the underlying rendering identity.
- **Cosmetic variation** changes character presentation; it does **not** break character continuity or shading rules.
- **Collection presentation** changes framing and rarity treatment; it does **not** change the underlying character or world rendering language.

---

## 2. Definitive EOS Visual Target Specification

```
                    EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01
                                      │
                     ┌────────────────┴────────────────┐
                     ▼                                 ▼
             PRIMARY NORTH STAR               SECONDARY LANGUAGE
          (Authored Konoha Reference)       (NARUTO Storm Series)
                     │                                 │
         - Painterly stylized textures      - Combat animation energy
         - Warm golden afternoon sun        - Jutsu spectacle & trails
         - Hand-painted Japanese facades    - Cinematic dynamic camera
         - Soft cel shadow transitions      - Impact frames & hit reactions
         - Controlled rim lighting          - Character action presentation
         - Natural shinobi proportions      (VFX / Combat dynamics ONLY)
         - Rich, living village crowds
         - Background landmarks & depth
                     │
                     └────────────────┬────────────────┘
                                      ▼
                             NOT THE TARGET (REJECTED)
                                      │
         - Photorealistic AAA realism / gritty military rendering
         - Pure PBR / unshaded flat graybox / default UE mannequin
         - Completely flat 2D cartoon / mobile chibi
         - Neon debug primitives / blown-out uncalibrated bloom
         - Black voids / unlit geometry / unreadable environments
```

### Precise Visual DNA Specs
* **Style:** Stylized 3D anime action RPG.
* **Environment:** Painterly anime-inspired 3D environments with visible material variation, natural timber grain, weathered clay roof tiles, paper shoji, and hand-authored signage.
* **Characters:** Natural anime proportions, stylized 3D geometry construction, strong silhouette readability, expressive anime faces, clean costume separation, and iconic hairstyles.
* **Material:** Controlled stylized/cel response, soft 2–3 value stepping, controlled specular highlights, subtle rim light separation. Zero photorealistic pore/subsurface skin rendering.
* **Lighting:** Warm cinematic daylight (5400K sun), soft atmospheric azure fill, readable shadows, controlled bloom ($\le 0.45$). Zero crushed-black voids, zero overexposed white geometry.
* **World:** Dense authored environments, Japanese-inspired architecture, lush stylized foliage with wind sway, five-tier compositional depth (Foreground, Midground, Background Landmarks, Vista, Sky).
* **Camera:** Third-person cinematic action-RPG framing. Naruto occupies 25%–32% of total screen height, framed to celebrate the surrounding world.
* **Animation:** Natural shinobi locomotion. Anime alert stance, forward-leaning ninja run, smooth acceleration/deceleration, aerial leap, short landing recovery.
* **VFX:** Spectacular anime chakra and jutsu trails. Strict readability hierarchy: **Character Motion $\rightarrow$ Action $\rightarrow$ Jutsu $\rightarrow$ Impact $\rightarrow$ Recovery**.

---

## 3. The 6 Non-Negotiable Visual Acceptance Gates

An asset or level is **never** declared complete because an actor, class, component, or file path exists. It must pass all six gates:

```
ACTOR EXISTS ≠ VISUAL ASSET EXISTS ≠ ASSET IMPORTED ≠ ASSET ASSIGNED ≠ ASSET RENDERED ≠ STYLE CORRECT ≠ GAMEPLAY COMPLETE
```

| Gate | Title | Validation Criteria |
| :---: | :--- | :--- |
| **GATE A** | **Physical Asset Gate** | Production SkeletalMesh/StaticMesh, diffuse/painterly textures, material instances, skeleton, animation assets, and collision physics asset exist physically in `/Game/`. |
| **GATE B** | **Technical Asset Gate** | Correct real-world scale (e.g., Genin Naruto 145 cm), clean UV unwraps, clean skeleton bone hierarchy, valid collision hulls, valid LODs, and zero unassigned material slots. |
| **GATE C** | **Runtime Binding Gate** | Soft references resolve synchronously, asset loads into memory, mesh visibly appears in viewport, AnimBP executes, animations loop, materials render, and collision blocks correctly. |
| **GATE D** | **Visual Fidelity Gate** | Meets `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`. Correct anime proportions, silhouette, color grading, warm lighting, blue sky, soft cel shading, camera framing, and VFX balance. |
| **GATE E** | **Packaged Build Gate** | Standalone / Shipping build runs on NVIDIA GeForce GTX 1650 at 1920x1080 @ locked 60 FPS (1% low > 45 FPS). Zero missing textures, zero default grayboxes, zero debug shapes. |
| **GATE F** | **EOS Visual DNA Consistency Gate** | **"Does this asset unmistakably belong in EOS?"** Evaluates whether the asset matches the unified shader, lighting, color, and proportional language of the game. If it looks photorealistic, flat-chibi, or foreign: **HARD FAIL**. |

$$\mathbf{A + B + C + D + E + F = PRODUCTION\_READY}$$

---

## 4. 12-Tier Lifecycle Status Language

Engineers, tools, and automated reports must report exact lifecycle status:

1. `SPECIFIED`: Documented in data contracts and visual bible.
2. `AUDITED`: Verified on disk; gaps identified.
3. `FACTORY_IMPLEMENTED`: Underlying C++ factory, procedural generator, or data table constructed.
4. `ASSET_AUTHORING`: Source 3D mesh, texture, or animation in production in DCC / source folder.
5. `ASSET_IMPORTED`: Compiled `.uasset` package exists in canonical `/Game/` location.
6. `RUNTIME_BOUND`: Soft reference resolved and attached to actor component.
7. `RUNTIME_RENDERED`: Visibly drawing pixels in viewport without falling back to basic shapes.
8. `VISUAL_VALIDATED`: Passes Gates A, B, C, and D against reference image.
9. `GAMEPLAY_VALIDATED`: Interacts seamlessly with GAS, combat hitboxes, movement, and camera.
10. `PERFORMANCE_VALIDATED`: Profiles within GTX 1650 budget (draw calls, polycount, shader instructions).
11. `PACKAGED_VALIDATED`: Verified in cooked Standalone executable.
12. `PRODUCTION_READY`: All gates pass; officially signed off for release.

---

## 5. Master Architecture Hierarchy

```
                                EOS MASTER AUTHORITY
                                         │
        ┌────────────────────────────────┼────────────────────────────────┐
        ▼                                ▼                                ▼
   GAMEPLAY CORE                   VISUAL DNA ARCHITECTURE            CONTENT DATA CONTRACTS
 C++ / GAS / Subsystems                  │                            Data Tables / Enums / Structs
        │                        ┌───────┼───────┐                        │
        │                        ▼       ▼       ▼                        │
        │                   CHARACTERS WORLDS   UI                        │
        │                        │       │       │                        │
        │                        ▼       ▼       ▼                        │
        │                      FORMS   LANDS   GACHA                      │
        │                        │       │       │                        │
        │                      SKINS   NPCs    CARDS                      │
        │                        │       │       │                        │
        └────────────────────────┼───────┼───────┼────────────────────────┘
                                 ▼       ▼       ▼
                             RUNTIME ACTIVATION PIPELINE
                                         │
                   ┌─────────────────────┼─────────────────────┐
                   ▼                     ▼                     ▼
             COMBAT SYSTEM          LIVING WORLD        NARRATIVE & INTEL
        GAS / Hitboxes / VFX       NPCs / Events / Day    Motion Manga / Story
                   │                     │                     │
                   ▼                     ▼                     ▼
            ADAPTIVE BOSS AI        WORLD STATE          SHINOBI CODEX
         Combat DNA / Fairness    District Triggers     Cards / Lore / Bonds
                   │                     │                     │
                   └─────────────────────┼─────────────────────┘
                                         ▼
                                SAVE & STATE PERSISTENCE
                                         │
                                         ▼
                               COLLECTION & PROGRESSION
                                         │
                                         ▼
                               FUTURE EXPANSIONS (TBV)
```

---

## 6. Layered Adaptive Naturalistic Boss AI Contract

Boss encounters extend `UEOSAdaptiveAISubsystem` through a strict 18-step decision and execution pipeline:

```
PLAYER
  │
  ▼
PLAYER COMBAT MODEL (Tracks attack cadence, dodge timing, jutsu habits, guard tenacity)
  │
  ▼
BOSS DECISION CONTEXT (Distance, chakra, health phase, arena hazard, active clones)
  │
  ▼
FINITE STATE MACHINE (FSM: Idle, Stalking, Engaging, Pressuring, Retreating, JutsuCharging)
  │
  ▼
PHASE MANAGER (Phase 1: Casual/Testing -> Phase 2: Sharingan Active -> Phase 3: High Intensity)
  │
  ▼
BEHAVIOR TREE (High-level tactical branches)
  │
  ▼
ELIGIBILITY FILTER (Cooldowns, chakra availability, positional safety)
  │
  ▼
UTILITY AI (Evaluates highest utility tactical action)
  │
  ▼
BOSS COMBAT DNA (Archetype weighting: AGGRESSOR, TACTICIAN, DUELIST, ZONER, ASSASSIN, BERSERKER, TRICKSTER, CONTROLLER, GIANT, ADAPTIVE_MASTER)
  │
  ▼
ADAPTATION LAYER (Modifies action probability based on Player Combat Model)
  │
  ▼
FAIRNESS GATE (Guarantees humanly reactable telegraphs, recovery openings, no input reading)
  │
  ▼
ACTION SELECTION
  │
  ▼
GAMEPLAY ABILITY SYSTEM (GAS Ability Activation & Commitment)
  │
  ▼
ANIMATION MONTAGE (Telegraph, active strike, recovery)
  │
  ▼
HITBOX & COLLISION (Frame-accurate collision query)
  │
  ▼
DAMAGE & REACTION CALCULATION (AttributeSet modification & hit-stop)
  │
  ▼
RECOVERY & OPENING (Punish window for skilled player)
  │
  ▼
PLAYER RESPONSE EVALUATION
```

### Boss Combat DNA Profiles
* `TACTICIAN` (Kakashi Hatake): Analyzes player attack rhythm; uses substitution when pressured; counter-attacks with Lightning Blade; adjusts strategy across phases.
* `DUELIST` (Sasuke Uchiha): Aggressive forward melee pressure, Sharingan prediction, quick fireball pokes, Chidori dash punishes.
* `CONTROLLER` (Gaara): High defensive zoning, sand shield absorption, ranged area denial, grab punishers.
* `BERSERKER` (Jugo / Curse Mark): High-damage unrelenting hyper-armor strikes, wide sweeps, low defensive self-preservation.
* `ADAPTIVE_MASTER` (Pain / Madara): Multi-range mastery, spatial manipulation, summon assistance, dynamic stance switching.

---

## 7. First-Class Systems Architecture

### 7.1 Skin Architecture
Every skin is an authored visual asset adhering to the EOS Visual DNA:
* **Data Contract:** `SkinID`, `CharacterID`, `EraID`, `FormID`, `CostumeID`, `SkinTier` (Standard, Epic, Legendary, Mythic), `ThemeID`, `ReleasePatchID`, `AcquisitionType` (Gacha, Event, ShinobiPass, Story), `EventID`, `PassTrackID`, `PremiumFlag`, `AnimationVariantID`, `VFXProfileID`, `AudioProfileID`, `PortraitVariantID`, `CollectionEntryID`, `CanonStatus`, `AuthorReviewStatus`.
* **Execution Rule:** Skins share the identical skeleton, hitbox, and GAS abilities of the base character while utilizing authored meshes and materials that pass Gate F.

### 7.2 Shinobi Chronicle Cards
Cards serve as both collectible narrative art and strategic meta progression:
* **Data Contract:** `CardID`, `CardType` (`CHARACTER`, `JUTSU`, `BOSS`, `MEMORY`, `LOCATION`, `SUMMON`, `TAILED_BEAST`, `EVENT`, `TEAM`, `LORE`), `CharacterID`, `FormID`, `JutsuID`, `BossID`, `LocationID`, `MemoryID`, `Rarity` (`C`, `R`, `SR`, `SSR`, `UR`, `LR`), `CollectionSetID`, `EraID`, `ArtProfileID`, `FrameProfileID`, `VFXProfileID`, `LoreEntryID`, `BondEntryID`, `AcquisitionSource`, `ReleaseEventID`.
* **Visual Presentation:** 3D engine-derived illustrations painted with EOS cel-shaded aesthetics.

### 7.3 Shinobi Codex & Collection Museum
An integrated exploration encyclopedia:
* **Codex Sections:** Characters, Forms, Skins, Jutsu, Weapons, Gear, Summons, Tailed Beasts, Bosses, Enemies, NPCs, Villages, Lands, Cards, Bonds, Lore, Motion Manga, Music, Achievements.
* **Museum Walkthrough:** Inspect unlocked 3D character models, weapons, summons, and village dioramas in real-time engine lighting.

### 7.4 Bonds & Shinobi Intel System
Deep narrative and progression loop:
$$\text{Character A} \leftrightarrow \text{Character B} \rightarrow \text{Bond Level (1--8)} \rightarrow \text{Dialogue} \rightarrow \text{Side Mission} \rightarrow \text{Memory Fragment} \rightarrow \text{Card} \rightarrow \text{Motion Manga} \rightarrow \text{Team Synergy Bonus}$$

### 7.5 Regional Village Reputation
* Tracks player standing across major settlements: `KonohaReputation`, `SunaReputation`, `KiriReputation`, `KumoReputation`, `IwaReputation`, `AmeReputation`.
* Unlocks regional shops, unique cosmetics, side story questlines, NPC dialogues, and gacha cards.

### 7.6 Future Expansion & Content Pack Architecture
All content tables natively carry expansion metadata:
* `ContentPackID`, `ExpansionID`, `SeasonID`, `EventID`, `ReleasePatchID`, `EraID`, `CanonStatus`.
* Enables seamless addition of *Boruto / Two Blue Vortex* content, Great Ninja War expansions, and seasonal festivals without database schema migrations.

---

## 8. Factory Status vs. Content Status Decoupling

To prevent declaring world or character completion when only tooling exists, status is tracked in two distinct categories:

| Domain | Factory Status | Content Status | Current State |
| :--- | :--- | :--- | :--- |
| **World: Konoha** | `PROVEN` | `RUNTIME_ACTIVE` | Golden Street vertical slice active |
| **World: Sunagakure** | `PROVEN` | `AUTHORING` | Biome profile specified; desert meshes in production |
| **World: Kirigakure** | `PROVEN` | `PLANNED` | Mist shader and wet stone profiles specified |
| **World: Kumogakure** | `PROVEN` | `PLANNED` | Alpine canyon profile specified |
| **World: Iwagakure** | `PROVEN` | `PLANNED` | Ochre rock and fortress profile specified |
| **World: Amegakure** | `PROVEN` | `PLANNED` | Rain VFX and industrial conduit profile specified |
| **Character: Naruto Genin** | `PRODUCTION_READY` | `RUNTIME_ACTIVE` | Mesh, AnimBP, materials, locomotion active |
| **Character: Sasuke Genin** | `PRODUCTION_READY` | `RUNTIME_ACTIVE` | Mesh, materials active; combat animations in binding |
| **Character: Sakura Genin** | `PRODUCTION_READY` | `AUTHORING` | Mesh authored; AnimBP in assembly |
| **Character: Kakashi Jonin** | `PRODUCTION_READY` | `RUNTIME_ACTIVE` | Boss mesh, Sharingan presentation active |
| **Character: Madara Uchiha** | `PRODUCTION_READY` | `CONCEPT_APPROVED` | Visual prompt locked; mesh in queue |

---

## 9. Authoritative Project Phase 13: Visual Reconstruction & Golden Slice

To eliminate conflicting numbering, **Phase 13** is the authoritative master phase for Visual Reconstruction and Golden Vertical Slice:

* **13.0 Forensic Audit:** Verify all disk packages, skeletons, and materials.
* **13.1 Visual DNA Authority:** Lock in `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`.
* **13.2 Asset Pipeline:** Establish automated import, validation, and LOD tooling.
* **13.3 Golden Naruto Character:** `SK_Naruto_Genin`, full skeleton, 5 material slots.
* **13.4 Locomotion & Animation:** Shinobi idle, ninja run, sprint, jump, dodge, recovery.
* **13.5 Stylized Material System:** `M_AnimeCel_Master` and 13 material instance profiles.
* **13.6 Golden Konoha Street:** 50m composition (Ichiraku, residences, shops, cobblestone road, trees).
* **13.7 Third-Person Camera:** `EOS_ExplorationCameraProfile` framing Naruto and the village vista.
* **13.8 Jutsu Presentation:** Rasengan chakra gather, formation, lunge, impact, camera shake.
* **13.9 Living NPC System:** Teuchi cooking, Sasuke/Sakura eating, gate guard patrol, civilians.
* **13.10 Enemy Archetype:** Rogue Shinobi with readable telegraphs and hit reactions.
* **13.11 Kakashi Golden Boss:** Full boss presentation, Sharingan activation, Lightning Blade.
* **13.12 Layered Adaptive AI:** 18-step AI decision loop with Tactician Combat DNA.
* **13.13 Stage Visualization:** Resolution of Stage 017 to Central Konoha morning preset.
* **13.14 World Factory Pipeline:** Validation of the 50m $\rightarrow$ 100m $\rightarrow$ 250m $\rightarrow$ 500m expansion tool.
* **13.15 Golden Vertical Slice Playable Loop:** The definitive full loop acceptance test.
* **13.16 Performance Profiling:** 60 FPS locked on GTX 1650 @ 1080p.
* **13.17 Visual Automation QA:** In-engine tests verifying zero null meshes or debug shapes.
* **13.18 Packaged Acceptance:** Cooked Standalone Win64 build verification.

---

## 10. The Ultimate Phase 13 Acceptance Loop

Phase 13 passes **only** when a player executes this continuous sequence with zero visual or mechanical failures:

$$\text{BOOT} \rightarrow \text{KONOHA STREET} \rightarrow \text{NARUTO VISIBLE} \rightarrow \text{ANIME MATERIAL} \rightarrow \text{THIRD-PERSON CAMERA}$$
$$\rightarrow \text{IDLE} \rightarrow \text{WALK} \rightarrow \text{NINJA RUN} \rightarrow \text{SPRINT} \rightarrow \text{JUMP} \rightarrow \text{DODGE} \rightarrow \text{LOCK-ON}$$
$$\rightarrow \text{MELEE COMBO} \rightarrow \text{RASENGAN} \rightarrow \text{CHAKRA DEDUCTION} \rightarrow \text{ULTIMATE AWAKENING} \rightarrow \text{CHARACTER SWITCH (SASUKE)}$$
$$\rightarrow \text{NPC INTERACTION (TEUCHI)} \rightarrow \text{MISSION RECEIPT} \rightarrow \text{ROGUE ENEMY ENCOUNTER}$$
$$\rightarrow \text{KAKASHI BOSS} \rightarrow \text{ADAPTIVE AI DECISION} \rightarrow \text{PHASE 1} \rightarrow \text{SHARINGAN PHASE 2}$$
$$\rightarrow \text{BOSS DEFEATED} \rightarrow \text{MOTION MANGA CUTSCENE} \rightarrow \text{MISSION REWARD}$$
$$\rightarrow \text{CHRONICLE CARD \& INTEL UNLOCK} \rightarrow \text{CODEX UPDATE} \rightarrow \text{SAVE SUBSYSTEM} \rightarrow \text{RETURN TO STREET}$$
