# NARUTO: ECHOES OF SHINOBI

V5.2.1 --- MASTER PROJECT README / AGENT HANDOFF

Project status: Portfolio-grade development master plan  
Primary engine: Unreal Engine 5.8  
Primary platform: Windows PC first; mobile architecture-ready  
Game scale: 25 arcs × 4 chapters × 5 stages = 500 core story stages  
Documentation status: V5.2 consolidated + V5.2.1 visual/jutsu expansion  
README role: Master entry point for humans, agentic IDEs, content authors, technical implementers, QA, and asset-generation workflows.

---

## 1. WHAT THIS PROJECT IS

**NARUTO: ECHOES OF SHINOBI** is a free-roam, chapter-driven, combat-first Naruto/Boruto action RPG built around:
- Real-time anime-style combat
- Dense free-roam exploration
- Character collection
- Deterministic character progression
- Jutsu learning and mastery
- Elemental/chakra systems
- Forms and transformations
- Summons-only gacha
- Gear and artifact progression
- Adaptive boss/miniboss/minion behavior
- Weekly bounties
- Limited-time events
- Rank/Sage training
- Tailed Beast Remnant Power and Jinchūriki progression
- Motion Manga story presentation
- Story Archive and persistent world-state updates
- A data-driven authoring pipeline intended for an agentic IDE.

The intended presentation combines stylized 3D anime characters, 2.5D readability, cinematic combat, layered traversal, dense interconnected world zones, manga-inspired narrative framing, and original game staging.

The project is designed as a large expandable game architecture, but development must begin with a polished vertical slice rather than attempting to build all 500 stages simultaneously.

---

## 2. READ THIS FIRST --- MASTER RULES

These rules override convenience-based implementation decisions.

### 2.1 Engine rule
- Use Unreal Engine 5.8 as the primary implementation.
- Required primary stack:
  - C++
  - Blueprints
  - Gameplay Ability System (GAS)
  - Enhanced Input
  - Common UI
  - Niagara
  - Animation Blueprints
  - Control Rig
  - StateTree and/or Behavior Trees where appropriate
  - Data Assets / Data Tables
  - Unreal Automation / Functional Testing
  - Unreal Insights and profiling tools
- **Godot is prohibited.** Godot 4 is prohibited. Do not create a parallel Godot implementation.

### 2.2 Data-driven rule
- Do not hard-code content that belongs in authoring data.
- Use stable IDs and data assets/tables for: characters, eras, forms, costumes, weapons, jutsu, skills, stages, chapters, arcs, bosses, AI behaviors, rewards, gear, events, bounties, dialogue, Motion Manga, visual prompts, lore, world states, progression formulas.

### 2.3 Missing-value rule
- Never silently invent an authoritative value.
- If the approved source does not define a numerical threshold, exact unlock level, exact hand-sign sequence, canon move list, historical costume detail, damage value, reward amount, or AI parameter, mark the field: `TODO_AUTHORING`
- If the project deliberately introduces an original mechanic: `ORIGINAL_GAME_VARIANT`

### 2.4 Canon boundary
- Separate:
  - `CANON_STORY`
  - `CANON_INSPIRED_GAMEPLAY`
  - `ORIGINAL_GAME_ARC`
  - `WHAT_IF` / `ALTERNATE_TIMELINE`
- Canon-derived information must not be silently presented as newly invented canon.

### 2.5 Gacha boundary
- **GACHA = SUMMONS ONLY.**
- Do not put the following behind random gacha: required jutsu, required gear, mandatory story progression, required boss mechanics, required elemental systems, core transformations required to understand story scenes.
- Jutsu and gear are earned through gameplay/progression.

### 2.6 Combat-AI rule
- LLMs must not control combat frame-by-frame.
- Adaptive combat behavior is implemented through:
  `Combat Telemetry → Behavior Analyzer → Boss Memory → Adaptation Director → StateTree/BehaviorTree → Ability Selection`
- AI may use recorded player behavior to alter authored behavior weights, counters, phase choices, arena pressure, and ability selection.

### 2.7 Asset continuity rule
- Every generated production asset must carry stable continuity metadata.
- Minimum visual identity chain:
  `CharacterID → EraID → FormID → CostumeID → WeaponSetID → SkillSetID → LocationID → StoryStateID → PromptVersion`
- Missing metadata is a production issue, not a cosmetic issue.

### 2.8 Vertical-slice rule
- Do not mass-produce the 500-stage campaign before the core loop works.
- The first serious slice must prove:
  `Launcher → Konoha → Free Roam → Mission → Combat → Elite/Mini-Boss → Boss → Rewards → Progression → Jutsu → Bounty → Adaptive Rematch → Motion Manga → Story Archive → Konoha`

---

## 3. CURRENT MASTER DOCUMENT SET

The project is intentionally divided into domain documents instead of relying on one giant specification.

### Core V5.2 documents (Located in [`Concept/Understanding/`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/))
- [`01_PRODUCT_SCOPE_AND_REQUIREMENTS.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/01_PRODUCT_SCOPE_AND_REQUIREMENTS.md)
- [`02_PHASE_WISE_EXECUTION_PLAN.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/02_PHASE_WISE_EXECUTION_PLAN.md)
- [`04_GAME_MODES_AND_PLAYER_EXPERIENCES.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/04_GAME_MODES_AND_PLAYER_EXPERIENCES.md)
- [`12_TECHNICAL_AGENTIC_IDE_ARCHITECTURE.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/12_TECHNICAL_AGENTIC_IDE_ARCHITECTURE.md)
- [`13_DATA_SCHEMA_AND_EXCEL_AUTHORING.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/13_DATA_SCHEMA_AND_EXCEL_AUTHORING.md)
- [`19_EXCEL_TECHNICAL_AUTHORING_GUIDE_V5_2.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/19_EXCEL_TECHNICAL_AUTHORING_GUIDE_V5_2.md)
- [`GAME-DESIGN-BOARD.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/GAME-DESIGN-BOARD.md)
- [`VISUAL-DESIGN.md`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/VISUAL-DESIGN.md)
- [`NARUTO-images.docx`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/NARUTO-images.docx)
- [`Naruto_Echoes_of_Shinobi_MASTER_Concept_Board_Compiled.png`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Concept/Understanding/Naruto_Echoes_of_Shinobi_MASTER_Concept_Board_Compiled.png)

### V5.1 expanded domains retained by V5.2
- `10_KONOHA_WORLD_EXPLORATION_AND_LANDS_V5_1`
- `11_RANK_TRAINING_AND_SAGE_PROGRESSION_V5_1`
- `12_TAILED_BEAST_REMNANT_AND_JINCHURIKI_SYSTEM_V5_1`
- `13_GRINDABLE_GAME_MODES_AND_PLAYER_RETENTION_V5_1`
- `14_COMBAT_BALANCE_EXP_DAMAGE_SCORE_FORMULAS_V5_1`
- `15_REFINED_CHARACTER_IMAGE_GENERATION_PROMPT_BIBLE_V5_1`
- `16_OPENING_LOADING_CURSOR_PROFILE_COLLECTION_UI_V5_1`
- `17_GAME_CONTENT_PRODUCTION_PIPELINE_V5_1`
- `18_FINAL_PROJECT_SCORECARD_AND_RELEASE_GATES_V5_1`

### V5.2 execution/reference documents
- `IDE_00_MASTER_FILE_WISE_EXECUTION_MAP_V5_2`
- `19_EXCEL_TECHNICAL_AUTHORING_GUIDE_V5_2`
- `APPENDIX A — EXCEL DATA CONTRACT`

### V5.2.1 expansion documents
- `NARUTO_ECHOES_OF_SHINOBI_V5_2_JUTSU_LEARNING_TRIALS_ADVANCEMENT_DATABASE.docx`
- `NARUTO_ECHOES_OF_SHINOBI_V5_2_REFINED_IMAGE_GENERATION_PROMPTS_SPECIAL_MODES_MISSING_SHINOBI.docx`
- `NARUTO_ECHOES_OF_SHINOBI_V5_2_REFINED_IMAGE_GENERATION_PROMPT_BIBLE_COMPLETE.md`
- `NARUTO_ECHOES_OF_SHINOBI_V5_2_REFINED_IMAGE_GENERATION_PROMPT_BIBLE_COMPLETE.docx`
- `NARUTO_ECHOES_OF_SHINOBI_V5_2_DETAILED_TECHNICAL_AUTHORING_DATABASE.xlsx`

These are additive to the consolidated master specification. They should not be treated as unrelated side documents.

---

## 4. LATEST V5.2.1 DESIGN ADDITIONS

### 4.1 Jutsu Learning Trials
Jutsu acquisition is now a first-class progression system.

**The intended loop:**  
`Character Advancement → Jutsu Eligibility → Trial Briefing → Hand-Sign Recognition → Hand-Sign Execution → Chakra Control → Timing / Release → Practical Trial → Mastery Result → Jutsu Unlocked → Equip / Store / Replace → Jutsu Mastery / Refinement`

A jutsu is eligible, not automatically granted, when the character reaches its approved advancement milestone.

**Trial layers:**
- Hand-Sign Recognition
- Hand-Sign Execution
- Chakra Control
- Timing / Release
- Practical Trial
- Mastery Result

**Advanced trials can increase difficulty through:**
- longer sequences
- visually similar signs
- narrower timing windows
- chakra instability
- interruption pressure
- moving targets
- environmental pressure.

**Learning methods:**
- `HAND_SIGN`
- `CHAKRA_CONTROL`
- `ELEMENT_CONTROL`
- `WEAPON_TRAINING`
- `SUMMONING_CONTRACT`
- `SAGE_TRAINING`
- `BLOODLINE_FORM`
- `MENTOR`
- `COMBAT_DISCOVERY`
- `SCROLL_RESEARCH`
- `BOSS_MEMORY` (`BOSS_MEMORY` is an original-game mechanic unless specifically verified otherwise).

**Failure rule:**
Failure never permanently destroys the learning opportunity.  
`Failed Trial → Practice XP → Retry → Improved Familiarity → Eventual Mastery`  
Practice progress persists.

**Mastery authored states:**
- 0 --- Unlearned
- 1 --- Learned
- 2 --- Familiar
- 3 --- Trained
- 4 --- Expert
- 5 --- Mastered

Mastery should improve meaningful behavior (chakra efficiency, cancel windows, combo compatibility, status duration, recovery, targeting, utility) rather than simple damage inflation.

---

## 5. CHARACTER PROGRESSION

Every character is data-driven and may contain: Level, Star Tier, Ascension, Skill Nodes, Jutsu Mastery, Passive Traits, Signature Ability, Ultimate, Transformation, Gear, Bond Level, Elemental Affinity, Kekkei Genkai, Combat Role, Era, Form, Compatibility rules.

**Era is mandatory.**  
Examples:
- Academy Naruto, Genin Naruto, Sage Naruto, War Naruto, Hokage Naruto, Baryon Naruto
- Hebi Sasuke, Taka Sasuke, War Sasuke, Adult Sasuke
- Academy Boruto, Karma Boruto, TBV Boruto

Do not collapse incompatible eras into one record.

---

## 6. CHARACTER MOVE AUTHORING

Every specific character should have an authored move-learning path rather than receiving a generic universal list.

The move database connects:  
`CharacterID + EraID + AdvancementNodeID + JutsuID + LearningMethod + Compatibility + MentorID + TrialStageID`

**Minimum move fields include:**  
CharacterID, EraID, JutsuID, AdvancementNodeID, UnlockCondition, LearningMethod, Compatibility, MentorID, HandSignSequenceID (where applicable), RecognitionDifficulty, ExecutionWindow, ChakraModelID, ChakraCost, CastTime, Cooldown, Range, HitProfile, Status, ComboTags, InterruptState, CancelWindow, TrialStageID, PassThreshold, InitialMasteryFormulaID, PracticeXPFormulaID, RewardTableID, AnimationID, VFXID, AudioID, VisualPromptID, LoreID, AI_Tags, CounterTags, ContinuityStatus.

**Major character learning paths:**  
Individual paths for Naruto, Sasuke, Sakura, Kakashi, Gaara, Shikamaru, Hinata, Rock Lee, Neji, Tenten, Might Guy, Sai, Yamato, Jiraiya, Tsunade, Orochimaru, Itachi, Obito, and wider roster.

---

## 7. VISUAL PROMPT SYSTEM --- V5.2.1

The visual system is treated as production infrastructure.

### Character asset package
Every major character/form should target:
- Full-body key art
- Front/side/back turnaround
- Portrait / collection render
- Basic attack visual
- Jutsu 1, 2, 3
- Signature technique
- Ultimate
- Transformation/mode activation
- Weapon sheet
- Chakra/VFX sheet
- Boss intro/defeat assets (where applicable)
- Motion Manga frame
- Environment-specific render
- Continuity metadata (`PromptVersion`)

### Universal visual identity formula
`CHARACTER + ERA/FORM + ROLE + FACE/HAIR + COSTUME + VILLAGE/FACTION + WEAPONS + BASIC ATTACK LANGUAGE + JUTSU + SIGNATURE + ULTIMATE + TRANSFORMATION + ELEMENT/VFX + POSE + CAMERA + LIGHTING + BACKGROUND + MATERIALS + CONTINUITY TAGS + NEGATIVE PROMPT`

### Visual negative prompt baseline
Avoid: photorealism, live-action appearance, generic fantasy armor, random modern clothing, incorrect village insignia, incorrect weapon, wrong era costume, incorrect hairstyle, duplicate limbs, extra fingers, distorted hands, malformed faces, inconsistent proportions, unreadable silhouettes, excessive bloom, muddy VFX, plastic materials, realistic glass, random magical effects, unrelated character fusion, non-canonical transformations, watermarks, text artifacts, logo artifacts, inconsistent character identity.

---

## 8. SPECIAL MODES / MISSING SHINOBI VISUAL LIBRARY

The special prompt library explicitly supports: base characters, era variants, costume variants, combat forms, transformations, Edo Tensei, Jinchūriki, Sage Mode, Six Paths, Ten-Tails forms, boss forms, event forms, summon forms, Motion Manga forms.

### Special continuity examples
- **Obito:** Young → Masked → White Mask → War → Ten-Tails Jinchūriki → Redeemed
- **Madara:** Living → Edo → Revived → Rinnegan → Ten-Tails Jinchūriki
- **Nagato:** Young → Akatsuki → Pain → Six Paths of Pain → Edo

---

## 9. LATEST VISUAL REFERENCE ASSET PACK

The visual reference workflow contains generated character visual references spanning the current roster.

**Recommended status labels:**
- `APPROVED_REFERENCE`
- `REFINE_REQUIRED`
- `REGENERATE_REQUIRED`
- `AUTHOR_REVIEW`
- `CANON_REFERENCE`
- `ORIGINAL_GAME_VARIANT`

Before final production integration, perform QA across identity, era, costume, weapon, VFX, silhouette, continuity, prompt metadata, and gameplay-readability.

---

## 10. STORY / CAMPAIGN STRUCTURE

Target: **25 major arcs | 100 chapters | 500 core stages.**

### Core stage types:
- Story
- Elite
- Challenge
- Mini-Boss
- Boss

Each arc authors story stages, elite encounters, mini-bosses, bosses, character unlocks, jutsu eligibility, gear rewards, lore, Motion Manga, world-state changes, side missions, bounties, and event connections.

---

## 11. MOTION MANGA

Motion Manga is a signature presentation system.

**Core transition:**  
`Boss Defeated → HUD Fade → Music Transition → Motion Manga → Final Panel → Arc Complete → Rewards → Story Archive → World State Update`

Generated Motion Manga should use original staging/composition rather than recreating source manga pages directly.

---

## 12. WORLD / EXPLORATION

### Primary hub
- **Konohagakure / Hidden Leaf**: Hokage Tower, Academy, hospital, clan districts, training grounds, memorial area, rooftops, forest edge, underground routes.

### Major villages & lands
- Konohagakure (Land of Fire), Sunagakure (Land of Wind), Kirigakure (Land of Water), Kumogakure (Land of Lightning), Iwagakure (Land of Earth).
- Mount Myōboku, Amegakure, Fourth Great Ninja War fronts, Kamui Dimension, Valley of the End, Divine Tree realms, original challenge realms.

**World design rule:** Prefer dense, meaningful zones over empty open-world scale.

---

## 13. RANK / TRAINING / SAGE SYSTEM

Training progression is a separate progression fantasy (Genin, Chūnin, Jōnin, Sage, character-specific mentor training).

Sage compatibility is character-specific:
- Naruto → Toad/Sage pathway
- Sasuke → Snake-related training pathway (where authored)
- Other characters → compatibility determined by approved data.

Compatibility states: `Natural affinity`, `Trainable`, `Restricted`, `Incompatible`.

---

## 14. TAILED BEAST REMNANT POWER

Tailed Beast systems include Tailed Beast Remnant Power, compatibility, resonance, Jinchūriki progression, beast-related jutsu, special skills, dialogue, modes/forms, stat increases, late-game integration, and boss/event encounters.

---

## 15. SUMMONS / GACHA

- **GACHA = SUMMONS ONLY.**
- Includes summon characters, variants, era variants, and support summons.
- Economy is data-driven, server-authoritative, and all reward grants are idempotent.

---

## 16. EVENTS / WEEKLY BOUNTIES / LIVE OPS

- **Weekly Bounty Cadence:** Default Thursday 00:00 UTC (configurable).
- **Limited Events:** Support for modular event structures (e.g., Juubi Madara, Obito, Indra, Ashura).

---

## 17. ADAPTIVE BOSS / MINIBOSS / MINION AI

Adaptive AI is a gameplay telemetry & state system, not an LLM combat controller.

**Adaptation pipeline:**  
`Telemetry → Behavior Analyzer → Boss Memory → Adaptation Director → StateTree / BehaviorTree → Ability Selection`

Adaptation remains bounded and authored. A boss can alter attack weighting, counters, spacing, pressure, phase behavior, and ability selection.

---

## 18. COMBAT / ELEMENTS / CHAKRA

- **Core Resources:** HP, Chakra, Ultimate meter, Transformation meter, Stamina/Movement, Status effects.
- **Core Actions:** Light attack, Heavy attack, Jutsu 1/2/3, Ultimate, Dodge, Guard, Perfect Dodge, Counter, Jump, Air Attack, Swap, Support, Substitution, Transformation.
- **Elemental Cycle:** `FIRE → WIND → LIGHTNING → EARTH → WATER → FIRE`

---

## 19. GAME MODES

Supports Story, Free Roam, Training (Genin/Chūnin/Jōnin/Sage), Challenge, Elite, Mini-Boss, Boss, Weekly Bounty, Limited Event, Event Boss, Endgame Floors, New Game+, Summon, Story Archive, Motion Manga, Jutsu Learning Trials, Mentor Trials, Weapon Trials, Element Trials, Form Trials, Summoning Trials.

---

## 20. INPUT ARCHITECTURE

Input is action-based via Unreal Enhanced Input.

- **PC Mappings:**
  - WASD — Movement | Mouse — Camera | LMB — Light | RMB — Heavy
  - Q/E/R — Jutsu 1/2/3 | G — Ultimate | X — Transformation
  - F — Swap | V — Support | C — Substitution | Shift — Sprint
  - Space — Jump | Ctrl — Guard | Tab — Lock-on | T — Target
  - 1/2/3 — Character slots | Z — Ninja tool

Controller and Mobile map to the same abstract action layer.

---

## 21. DATA / EXCEL AUTHORING

Numeric and content authority resides in data tables (Excel/Data Assets).  
Every reference must resolve to a valid ID. Missing numbers must be tagged `TODO_AUTHORING`.

---

## 22. PLAYER-FACING FORMULAS

Explicit, data-traced formulas required for: XP gain, level progression, advancement, jutsu eligibility/mastery, chakra costs, cooldowns, damage, hit score, EXP, reward scaling, stage power requirements, gear/artifact progression, event/bounty rewards, and endgame floor scaling.

---

## 23. GAME CONTENT PRODUCTION PIPELINE

`Design Source → Authoring Sheet → Stable ID → Gameplay Data → Animation/VFX → Visual Prompt → Generated Reference → QA → Unreal Asset → Stage Integration → Playtest → Telemetry → Balance → Release Gate`

---

## 24. UI / FRONT-END

Domains: Launcher, Title Screen, Loading, Cursor/Hover feedback, Profile, Settings, Character Collection & Preview, Summon, Progression, Skill Tree, Jutsu Archive & Trial, Gear, Artifacts, Bonds, Shinobi Intel, Map, Mission Board, Bounty, Events, Story Archive, Motion Manga, Rewards, Notifications, Accessibility.

---

## 25. SAVE / ECONOMY / INTEGRITY

Deterministic state, save/load, idempotent rewards, validation, rollback-safe transactions, anti-duplication protections, state ownership, telemetry, and migration/versioning.

---

## 26. TECHNICAL AGENTIC IDE ARCHITECTURE

- **Layer A — Foundation:** Unreal project, source control, input abstraction, game instance, save system, data loading, logging, build pipeline.
- **Layer B — Player:** Movement, camera, targeting, combat controller, GAS integration, animation.
- **Layer C — Content:** Characters, jutsu, skills, enemies, stages, bosses, progression.
- **Layer D — World:** Konoha, free roam, missions, NPCs, world states.
- **Layer E — Narrative:** Dialogue, Motion Manga, Story Archive.
- **Layer F — Live Ops:** Bounty, event, summon, reward tables.
- **Layer G — QA / Release:** Automation, profiling, packaging, regression.

---

## 27. KEY UNREAL COMPONENTS

Service/component boundaries: `CharacterProgressionComponent`, `JutsuEligibilityService`, `JutsuTrialManager`, `HandSignTrialComponent`, `ChakraControlTrialComponent`, `PracticalJutsuTrialStage`, `JutsuMasteryComponent`, `JutsuLoadoutComponent`, `JutsuArchive`, `BossMemoryComponent`, `BossAdaptationDirector`, `CombatTelemetryService`, `RewardGrantService`, `BountyService`, `EventService`, `SummonService`, `WorldStateService`, `MotionMangaService`, `StoryArchiveService`, `SaveGameService`.

---

## 28. ASSET ID / CONTINUITY STANDARD

Minimum identity example:
```ini
CharacterID = CHR_OBITO
EraID = ERA_FOURTH_WAR
FormID = FORM_JUUBI_JINCHURIKI
ModeID = MODE_BOSS
CostumeID = COSTUME_JUUBI
WeaponSetID = WEAPON_TRUTH_SEEKING_STAFF
BossID = BOSS_OBITO_JUUBI
LocationID = LOC_WAR_FRONT
StoryStateID = WAR_XX
PromptVersion = V5.2.1
```

---

## 29. QUALITY / QA

Validation checks: unique IDs, no broken references, legal enum values, non-negative costs, unique stage IDs, monotonic power curves, valid start/end dates, valid character/form/skill mappings, existing reward tables & jutsu trials, visual prompt & asset continuity metadata, idempotent reward grants, working input remapping/controllers, no soft-locks.

---

## 30. PORTFOLIO QUALITY BAR

The portfolio build demonstrates: custom combat architecture, reusable ability system (GAS), adaptive boss AI, data-driven content, stylized rendering, advanced animation, cinematic camera, Motion Manga, live-ops simulation, gacha architecture, server-authoritative economy, PC packaging, profiling, automated tests, clean Git history, technical documentation, diagrams, and gameplay capture.

---

## 31. RELEASE / DEMO FLOW (VERTICAL SLICE)

`Launcher → Title → Konoha → Free Roam → Mission Board → Land of Waves → Combat → Elite / Mini-Boss → Zabuza / Haku Boss → Rewards → Character Progression → Jutsu Learning Trial → Weekly Bounty → Adaptive Boss Rematch → Motion Manga → Story Archive → Return to Konoha`

---

## 32. CURRENT CANON / SOURCE-FRESHNESS RULE

- Includes Naruto / Boruto continuity through **Boruto: Two Blue Vortex Chapter 37** (Snapshot: 19 September 2026).
- Later chapters must be added through verified source updates. Never invent unreleased events, abilities, or forms.

---

## 33. SOURCE / AUTHORITY HIERARCHY

1. Approved current master specification
2. Approved current Excel/data contract
3. Latest approved domain document
4. User-provided source documents
5. User-provided manga database/artwork
6. Verified official Naruto/Boruto source material
7. Proposed implementation
8. Agent inference

---

## 34. IMPORTANT SOURCE FILES

Core sources: `NARUTO.docx`, `my naruto-manga.docx`, `Pasted markdown.md`, concept-board image, manga panel databases, V5/V5.1/V5.2 doc packages, technical spreadsheets, character visual reference pack, special-mode/missing-shinobi prompt library, Jutsu Learning Trials database.

---

## 35. WHAT THE AGENT MUST NOT DO

- Do not switch the engine to Godot.
- Do not hard-code all content into C++ classes.
- Do not make the LLM control real-time combat.
- Do not put jutsu or required gear behind random gacha.
- Do not invent canon as fact or invent hand-sign sequences/numeric thresholds without authoring.
- Do not collapse all character eras into one record.
- Do not ignore visual continuity.
- Do not generate hundreds of unvalidated assets before the vertical slice.
- Do not create an empty giant open world.
- Do not make every character mechanically identical or solve problems solely by inflating damage numbers.
- Do not block story progression behind perfect trial execution.
- Do not make adaptive bosses unreadable or impossible.
- Do not use copyrighted source pages as direct game assets without appropriate rights clearance.

---

## 36. WHAT THE AGENT SHOULD DO

- Read this `README.md` first.
- Read domain specifications before modifying systems.
- Inspect data contracts before choosing numeric values.
- Preserve stable IDs and mark missing values as `TODO_AUTHORING` or `ORIGINAL_GAME_VARIANT`.
- Build reusable services/components, test every system, profile early, and keep input abstract.
- Keep progression deterministic and make jutsu feel learned.
- Maintain era/form visual continuity.
- Build the vertical slice first and document every major implementation decision.

---

## 37. FIRST IMPLEMENTATION ORDER

1. **Phase 1 — Foundation:** Unreal 5.8 project, Git/source control, project modules, data layer, logging, save system, input abstraction.
2. **Phase 2 — Character:** One playable character, movement, camera, targeting, animation, GAS, basic attacks, one jutsu, dodge/substitution.
3. **Phase 3 — Combat:** Enemy, elite, mini-boss, boss, boss phases, telemetry, adaptive behavior.
4. **Phase 4 — Progression:** XP, level, advancement, Jutsu Learning Trial, mastery, gear, rewards.
5. **Phase 5 — World:** Konoha, free roam, mission board, Land of Waves, stage transitions.
6. **Phase 6 — Narrative:** Dialogue, Motion Manga, Story Archive, world-state update.
7. **Phase 7 — Live Systems:** Bounty, event, summon, limited reward flow.
8. **Phase 8 — QA:** Automation, save/load tests, combat tests, economy tests, profiling, packaging, portfolio capture.

---

## 38. FINAL PRODUCT PRINCIPLE

Do not build a huge game badly. Build the first playable experience so well that the player immediately understands: **This is Naruto as an Action RPG, not a menu simulator.**

The 500-stage vision is the destination. The vertical slice is the proof.

---

## 39. README VERSIONING

Version naming: `V5.2.x`  
Patch reasons:
- `.1` — major authoring/visual/jutsu expansion
- `.2` — data-contract expansion
- `.3` — technical implementation change
- `.4` — verified canon/source update
- `.5` — release/QA milestone

Never silently replace an older specification. Record the reason for any version change.

---

## 40. HANDOFF COMMAND

An agent receiving this repository should begin with:

```
READ README.md
READ MASTER SPEC
READ RELEVANT DOMAIN SPEC
READ EXCEL DATA CONTRACT
READ IDE EXECUTION MAP
INSPECT REQUIRED ASSETS
VERIFY IDS
IDENTIFY TODO_AUTHORING FIELDS
IDENTIFY ORIGINAL_GAME_VARIANT FIELDS
IMPLEMENT ONLY THE REQUESTED SCOPE
RUN VALIDATION
REPORT FILES CHANGED
REPORT TESTS
REPORT REMAINING TODOs
```

---

### FINAL STATUS

**NARUTO: ECHOES OF SHINOBI V5.2.1** is the current consolidated planning state represented by this `README.md`.
