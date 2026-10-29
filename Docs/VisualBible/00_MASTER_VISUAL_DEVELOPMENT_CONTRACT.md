# NARUTO: ECHOES OF SHINOBI
# MASTER VISUAL DEVELOPMENT + PRODUCTION CONTRACT
## *Authoritative Visual Law — Version 5.0.0*

> **SUPREME DIRECTIVE:**
> NARUTO: ECHOES OF SHINOBI is not being developed as a technically functional Unreal prototype that will receive its visual identity later. The visual identity is part of the product itself. Every character, form, skin, jutsu, animation, environment, NPC, boss, UI element, VFX effect, card, gacha presentation, Motion Manga scene and future expansion must be authored through one unified stylized 3D anime production language established by the approved gameplay reference and character production reference.

> **DIAGNOSTIC DIRECTIVE:**
> Do not attempt to solve missing visual assets through lighting, exposure, fog, emissive intensity, post-processing or arbitrary material changes. Missing assets are an asset-production problem. Incorrect rendering is a rendering problem. Incorrect animation is an animation problem. Incorrect camera framing is a camera problem. Incorrect UI is a UI problem. Diagnose and fix the correct layer.

---

## ENGINE & ARCHITECTURE

| Property | Value |
| :--- | :--- |
| Engine | Unreal Engine 5.8 |
| Language | C++ |
| Visual Scripting | Blueprint |
| Combat | Gameplay Ability System (GAS) |
| Input | Enhanced Input |
| AI | UEOSAdaptiveAISubsystem + Behavior Trees + Utility AI + Adaptive Combat + Boss FSM + EQS |
| Project Type | Third-Person Stylized Anime Action RPG / Gacha RPG |
| Hardware Target | NVIDIA GTX 1650 @ 1920x1080 @ 60 FPS locked |
| Visual Reference | `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01` |
| Character Reference | `EOS_CHARACTER_PRODUCTION_BIBLE_01` |

---

## 0. ABSOLUTE VISUAL AUTHORITY

The gameplay reference image (`Docs/References/Phase13/Konoha_Visual_North_Star.jpg`) is the **PRIMARY VISUAL TARGET** for the entire game.

The character production sheet images define the **PRIMARY CHARACTER AUTHORING / COSTUME / SKIN / COMBAT-VISUAL REFERENCE FORMAT**.

These references define:
- visual language
- color language
- lighting language
- environment density
- character presentation
- anime proportions
- camera presentation
- UI presentation
- VFX readability
- world atmosphere
- asset-detail expectations
- character production documentation
- skin production documentation
- collection presentation

**DO NOT** interpret the reference as a request for photorealism.
**DO NOT** interpret the reference as a request for a 2D background.
**DO NOT** interpret the reference as a request for a simple toon filter over Unreal primitives.

The target is: **STYLIZED 3D ANIME ACTION RPG.**

---

## 1. VISUAL NORTH STAR

The game must visually communicate: NARUTO, ANIME, ADVENTURE, SHINOBI, WARMTH, COLOR, CHARACTER, MOVEMENT, JUTSU, EXPLORATION, LIFE, CINEMATIC ACTION.

The final visual target must contain:

- REAL 3D CHARACTERS
- REAL 3D BUILDINGS
- REAL 3D ENVIRONMENT
- REAL VEGETATION
- REAL PROPS
- REAL MATERIALS
- REAL ANIMATIONS
- REAL VFX
- REAL LIGHTING
- REAL CAMERA
- REAL COLLISION
- REAL UI

**Never substitute** any of the following for production assets:
- Cube / Plane / Engine BasicShapes
- Default mannequin
- Checkerboard material
- Untextured primitive
- Debug mesh / White fallback mesh / Black fallback material

Temporary placeholders may exist ONLY inside explicitly marked development test maps.

---

## 2. VISUAL STYLE DEFINITION

**TARGET STYLE:**
Stylized 3D anime + hand-painted environment appearance + controlled cel shading + soft painterly gradients + strong readable silhouettes + warm cinematic daylight + colorful materials + anime facial rendering + controlled rim lighting + soft atmospheric depth + high visual readability.

**NOT:**
photorealistic / hyper-realistic / dark realistic / PBR showcase / cinematic black crush / white emissive scene / neon cyberpunk / generic fantasy / generic Unreal demo.

---

## 3. REFERENCE IMAGE INTERPRETATION

The gameplay reference establishes the target relationship between: CHARACTER, WORLD, CAMERA, LIGHT, UI, NPC, ARCHITECTURE, VEGETATION, DISTANCE, ATMOSPHERE.

The reference must NOT be copied as a literal composition. Instead derive a reusable visual language from it. Every future location must feel like it belongs to the same game.

---

## 4. VISUAL CONTINUITY LAW

The same visual production language must continue across: Konoha, Land of Fire, Suna, Land of Wind, Kiri, Land of Water, Kumo, Land of Lightning, Iwa, Land of Earth, Ame, Land of Rain, Land of Grass, Land of Hot Water, Land of Iron, War Fronts, Mount Myoboku, Valley of the End, Kamui Dimension, Divine Tree Realms, Future Boruto/Two Blue Vortex Content, Original Game Expansions.

Each location may have different: architecture, palette, vegetation, terrain, weather, lighting, materials, cultural details, NPC clothing, enemy types.

**BUT ALL MUST SHARE THE SAME:** character rendering language, material quality, camera language, animation quality, VFX readability, UI language, visual hierarchy, production quality.

---

## 5. WORLD VISUAL IDENTITY

Every world receives: WorldID, LandID, VillageID, DistrictID, BiomeID, ArchitectureProfile, TerrainProfile, MaterialProfile, FoliageProfile, WeatherProfile, LightingProfile, NPCProfile, EnemyProfile, BossProfile, TraversalProfile, AudioProfile, VFXProfile, CameraProfile.

---

## 6. GOLDEN CHARACTER

**FIRST PRODUCTION CHARACTER:** NARUTO UZUMAKI — GENIN — BASE FORM.

Do NOT begin mass character production. Naruto is the Golden Character. He must prove the complete pipeline.

Required:
- High-quality skeletal mesh
- Anime face, anime hair
- Correct clothing, headband, pouches, boots
- Skin/hair/cloth/eye materials
- Proper proportions
- Skeleton + physics asset
- Animation Blueprint + locomotion
- Combat animations + jutsu animations
- Facial presentation
- VFX attachment points + weapon/tool sockets
- GAS integration
- Portrait + UI icon + collection render

---

## 7. CHARACTER PRODUCTION SHEET STANDARD

Every production character must eventually receive:

| # | Deliverable |
|---|---|
| 01 | KEY ART |
| 02 | FRONT TURNAROUND |
| 03 | BACK TURNAROUND |
| 04 | LEFT PROFILE |
| 05 | RIGHT PROFILE |
| 06 | THREE-QUARTER |
| 07 | FACE CLOSE-UP |
| 08 | HAIR DETAIL |
| 09 | COSTUME DETAIL |
| 10 | ACCESSORY DETAIL |
| 11 | WEAPON DETAIL |
| 12-20 | IDLE, WALK, RUN, SPRINT, JUMP, FALL, LAND, DODGE, GUARD |
| 21-22 | HIT REACTION, KO |
| 23-30 | BASIC ATTACK, COMBO, JUTSU 1-3, SIGNATURE, ULTIMATE, TRANSFORMATION |
| 31 | VFX PRESENTATION |
| 32 | PORTRAIT |
| 33 | COLLECTION CARD |
| 34-35 | GACHA ART, BANNER ART |
| 36-37 | VICTORY POSE, DEFEAT POSE |

---

## 8. CHARACTER VISUAL PROPORTIONS

Use stylized anime proportions. Do NOT turn characters into photorealistic humans, generic realistic soldiers, generic UE mannequins, or hyper-detailed realistic humans.

Maintain: anime facial proportions, clean silhouette, expressive eyes, stylized hair, readable costume shapes, controlled anatomy, combat readability.

Character proportions must remain consistent across the entire roster.

---

## 9. CHARACTER MOVEMENT

Movement must feel like a SHINOBI.

Required: walk, run, sprint, strafe, backpedal, jump, double-step correction, landing, air recovery, dodge, combat dodge, substitution, wall interaction, rooftop traversal, ledge movement, chakra-assisted movement, combat repositioning, jutsu movement, ultimate movement.

Movement must have: anticipation, acceleration, weight, follow-through, recovery, directional responsiveness.

Do NOT use generic mannequin movement.

---

## 10. COMBAT ANIMATION LANGUAGE

Combat must communicate: speed, agility, chakra, martial arts, hand signs, weapon technique, impact, recovery, counterplay.

Use: anticipation, attack wind-up, active frames, impact, hit reaction, recovery, cancel windows, jutsu charge, release, recovery.

Every attack must have gameplay timing.

---

## 11. JUTSU SYSTEM VISUAL REQUIREMENT

Every jutsu requires: JutsuID, CharacterID, JutsuType, Element, ChakraCost, Cooldown, CastTime, ChargeTime, ReleaseTime, HitWindow, RecoveryWindow, AnimationProfile, VFXProfile, AudioProfile, CameraProfile, GameplayTags, DamageProfile.

Visual sequence: PREPARATION → HAND SIGNS → CHAKRA BUILDUP → JUTSU FORMATION → RELEASE → IMPACT → SECONDARY EFFECT → RECOVERY.

---

## 12. HAND-SIGN / QTE SYSTEM

Jutsu learning path: Character Advancement → Jutsu Eligibility → Jutsu Trial → Hand-Sign Practice → Chakra Execution → Mastery → Jutsu Unlocked.

QTE modes: hand-sign recognition, hand-sign sequence, timed input, chakra control, timing release, reaction input, practical combat test.

Must support: keyboard, mouse, controller, mobile future input.

Never invent unknown canon hand-sign sequences. If canon data is unavailable: `TODO_AUTHORING`.

---

## 13. RASENGAN GOLDEN TEST

Naruto Rasengan is the first complete Jutsu pipeline.

Required: hand-sign/charging presentation, chakra gathering, hand animation, body animation, camera emphasis, Rasengan VFX, dynamic light contribution, impact effect, hit reaction, damage, knockback, recovery, cooldown, chakra cost, sound, camera shake, UI cooldown, animation cancel rules.

Rasengan must visually belong to the reference style.

---

## 14. ANIME MATERIAL SYSTEM

Create one authoritative anime material framework. Do NOT create random material graphs per character.

Required material instances:
- `M_AnimeCharacter` / `M_AnimeSkin` / `M_AnimeHair`
- `M_AnimeCloth` / `M_AnimeMetal` / `M_AnimeWood`
- `M_AnimeStone` / `M_AnimeRoof` / `M_AnimeFoliage`
- `M_AnimeGround` / `M_AnimeJutsu` / `M_AnimeOutline`

Required characteristics: controlled cel bands, soft gradient transition, anime shadow, skin/hair/cloth/metal/wood/stone/foliage shading, controlled Fresnel/rim, controlled specular, controlled emissive, optional outline solution.

Must remain readable under: DAY, SUNSET, NIGHT, RAIN, FOG, COMBAT, JUTSU, BOSS ARENA.

---

## 15. LIGHTING

**PRIMARY KONOHA TARGET:** warm daylight, soft sky, blue/cyan atmospheric background, warm sunlight, soft shadows, controlled AO, subtle atmospheric perspective, bright but NOT overexposed.

**DO NOT** create: black world, white world, cyan-only world, extreme bloom, uncontrolled emissive, crushed shadows, blown highlights.

Lumen must NEVER be used to hide a missing lighting setup. If Lumen configuration is incorrect: FIX THE CONFIGURATION.

---

## 16. EXPOSURE

Exposure must be explicitly controlled. Prevent: automatic exposure pumping, black frames, white frames, extreme contrast, jutsu permanently exposing the scene, indoor/outdoor exposure instability.

---

## 17. ENVIRONMENT PRODUCTION — KONOHA GOLDEN STREET (50m × 50m)

Required: road, sidewalk, drainage, wooden buildings, Ichiraku-style ramen location, residences, shopfront, signboards, lanterns, banners, trees, grass, rocks, benches, crates, NPCs, street props, background buildings, distant Hokage Monument, sky, clouds, atmosphere.

No empty graybox street.

---

## 18. KONOHA DENSITY — LIVING VILLAGE

Include: villagers, shinobi, shopkeepers, children, patrolling shinobi, workers, restaurant customers, training characters, background pedestrians, ambient movement.

NPCs require: idle, walk, talk, look, gesture, work, eat, sit, react, flee, combat response.

---

## 19. CAMERA

Primary: third-person action RPG.

Requirements: character visible, environment visible, soft follow, collision handling, camera lag, camera obstruction handling, combat framing, lock-on, jutsu framing, ultimate cinematic framing, boss framing, exploration framing.

Character occupies ~25-32% screen height. Do not place camera so close world disappears, nor so far character becomes unreadable.

---

## 20. UI

Reference-style UI: character portrait, level, HP, chakra, party members, jutsu slots, input indicators, minimap, compass, mission objective, boss health, combo, status, damage feedback, interaction prompts.

UI must be: anime, clean, readable, consistent, non-photorealistic, non-debug-looking.

---

## 21. COMBAT PRESENTATION

Combat loop: TARGET → APPROACH → BASIC COMBO → JUTSU → REACTION → DODGE → COUNTER → RECOVERY → REPOSITION.

Visual feedback: hit sparks, chakra effects, dust, debris, motion trails, impact flashes, camera response, hit stop, VFX, audio.

---

## 22. BOSS PRESENTATION

Boss encounter must include: entrance, health bar, phase system, telegraph, attack, movement, VFX, hit reaction, stagger, recovery, transition, defeat.

Boss AI: FSM + Behavior Tree + Utility AI + Player Combat Model + Adaptive Combat + Combat DNA + Fairness + Phase System + EQS + GAS. Extends `UEOSAdaptiveAISubsystem`. Do NOT create a second adaptive AI subsystem.

---

## 23. BOSS FAIRNESS

Bosses cannot: read hidden input, predict future input, ignore cooldowns, ignore hit reactions, ignore recovery, counter everything, teleport arbitrarily, remove player agency.

Bosses must expose: telegraphs, reaction windows, recovery, weaknesses, counter opportunities.

---

## 24. CHARACTER FORMS

Every transformation receives its own: FormID, visual profile, mesh/mesh variant, material profile, animation profile, VFX profile, audio profile, camera profile, GAS ability set, stat modifiers, jutsu set, UI representation.

Do not merge visually distinct forms into one ambiguous state.

---

## 25. SKIN SYSTEM

Skins must preserve: character identity, age continuity, silhouette, face, hair identity, combat readability.

Skin categories: Canon Variant, Hokage, ANBU, Chunin, Shinobi Pass, Event, Festival, Summer, Winter, Samurai, Ronin, Modern Shinobi, Dark Chakra, Future Shinobi, Luxury, Anniversary, Regional/Cultural-inspired, Original Game Variant.

Every skin: SkinID, CharacterID, EraID, FormID, CostumeID, ThemeID, Tier, AcquisitionType, ReleasePatch, VFXProfile, AnimationVariant, PortraitVariant, CollectionEntry, CanonStatus.

---

## 26. CULTURAL VARIANTS

Regional-inspired cosmetics may explore: Japanese festival, samurai-inspired, Chinese-inspired fantasy, Korean-inspired fantasy, South Asian-inspired fantasy, Middle Eastern-inspired fantasy, European-inspired fantasy, African-inspired fantasy, Southeast Asian-inspired fantasy, modern global fashion.

Must remain respectful fictional game variants. Do not rewrite canon identity.

---

## 27. CARD COLLECTION

Collection categories: Character Card, Form Card, Jutsu Card, Summon Card, Weapon Card, Artifact Card, Lore Card, Event Card, Memory Card, Motion Manga Card.

Rarity: 3★, 4★, 5★, 6★+.

Cards include: portrait, art, name, rarity, era, faction, element, combat role, jutsu, lore, collection number, release patch, obtain method.

---

## 28. GACHA PRESENTATION

Required: banner art, character reveal, rarity animation, chakra effect, summoning sequence, card reveal, character reveal, new-character presentation, duplicate conversion, collection update.

Must be visually connected to gameplay.

---

## 29. WEAPONS / GEAR

Every gear item receives: concept, front, side, back, material, rarity, VFX if applicable, character compatibility, animation compatibility, collection presentation.

Gear must remain visually readable in third-person gameplay.

---

## 30. WORLD FACTORY

One reusable world-production framework supporting: Konoha, Suna, Kiri, Kumo, Iwa, Ame, other lands, war zones, special dimensions.

Standardizes: asset quality, LOD, collision, materials, lighting, NPC setup, VFX, audio, streaming, performance, QA.

Must NOT make every village look identical.

---

## 31. STORY / MOTION MANGA

Sequence: MISSION → GAMEPLAY → BOSS → DEFEAT → HUD FADE → MUSIC TRANSITION → MOTION MANGA → FINAL PANEL → REWARD → ARCHIVE → WORLD STATE UPDATE.

Motion Manga shares: character identity, costume continuity, lighting language, color language, story-state continuity.

---

## 32. FUTURE GAME EXPANSION

Visual system must support future content WITHOUT rebuilding the core visual architecture: Naruto arcs, Boruto arcs, Two Blue Vortex, new characters, forms, lands, bosses, skins, events, summons, cards, weapons, artifacts, jutsu, story chapters, Motion Manga, gacha banners, live events.

---

## 33. DATA-DRIVEN VISUAL ARCHITECTURE

Use: Data Assets, Data Tables, Soft Object References, Asset Manager, Visual Asset Registry, Visual Profiles.

Do not hardcode individual asset paths throughout C++.

Chain: GAMEPLAY DATA → VISUAL PROFILE → ASSET REGISTRY → SOFT ASSET REFERENCE → MESH → MATERIAL → ANIMATION → VFX → AUDIO → CAMERA → RUNTIME.

---

## 34. VISUAL ASSET REGISTRY

Each asset tracks: AssetID, AssetType, CharacterID, WorldID, FormID, SkinID, JutsuID, Source, CanonStatus, AuthoringStatus, ImportStatus, MaterialStatus, AnimationStatus, RuntimeStatus, QAStatus, PerformanceStatus.

Asset source types: `GENERATED_CONCEPT`, `AUTHOR_CREATED`, `DCC_AUTHORED`, `IMPORTED_EXTERNAL`, `EXISTING_PROJECT_ASSET`, `TEMPORARY_TEST_ASSET`, `TODO_AUTHORING`.

Final lifecycle: `CONCEPT_ONLY` → `CONCEPT_APPROVED` → `3D_AUTHORING` → `RIGGING` → `TEXTURING` → `ANIMATION` → `UNREAL_IMPORT` → `MATERIAL_SETUP` → `RUNTIME_VALIDATION` → `PRODUCTION_READY`.

---

## 35. VISUAL GATES

Every visual asset must pass:

| Gate | Requirement |
|------|-------------|
| GATE 1 | Physical asset exists |
| GATE 2 | Technical validation |
| GATE 3 | Runtime activation |
| GATE 4 | Visual review |
| GATE 5 | Packaged build |

Only then: `PRODUCTION_READY`.

---

## 36. AUTOMATED VISUAL VALIDATION

**FAIL** if: Mesh == nullptr, Material == nullptr, Skeleton missing, AnimInstance missing, Animation missing, Texture dependency missing, Collision missing, Soft reference invalid, Asset package missing, Placeholder asset active, Engine BasicShapes active, Default mannequin active, Checkerboard material active, Unlit fallback active, Debug mesh active, Temporary test asset active in production map.

---

## 37. GOLDEN VISUAL SLICE

The first complete acceptance scene must contain:

1. Naruto Genin
2. Real NPC(s)
3. Real enemy
4. Real buildings
5. Real road
6. Real trees
7. Real props
8. Real anime materials
9. Real lighting
10. Real animations
11. Real camera
12. Real HUD
13. Real minimap
14. Real jutsu
15. Real VFX
16. Real combat
17. Real collision
18. Real NPC life
19. Real Konoha background
20. Real mission objective

Must visually resemble the target gameplay reference in overall visual language.

---

## 38. PHASE EXECUTION RULE

**DO NOT IMPLEMENT ALL SYSTEMS SIMULTANEOUSLY.**

Each phase: SPECIFICATION → AUDIT → ASSET REQUIREMENTS → IMPLEMENTATION → RUNTIME ACTIVATION → VISUAL REVIEW → AUTOMATED TEST → PERFORMANCE TEST → EVIDENCE → GATE.

If a gate fails: **STOP.** Fix the current phase. Do NOT proceed to the next visual production phase.

---

## 39. ANTI-REGRESSION RULE

Before modifying a working system, capture: current behavior, current screenshot, current test result, current asset registry state.

After modification: re-run previous validation.

Never replace a working system with a new implementation without evidence that the new implementation is authoritative.

---

## 40. CRITICAL IDE BEHAVIOR

**DO NOT ASSUME:**
- actor exists = visual exists
- mesh component exists = mesh exists
- light exists = lighting is correct
- material exists = material is correct
- C++ compiles = feature works
- asset reference exists = asset loads
- HUD exists = UI is production-ready
- jutsu ability exists = jutsu visually works
- AI node exists = AI works
- animation asset exists = animation works

---

## 41. REQUIRED REPORT AFTER EVERY PHASE

Return: PHASE, STATUS, IMPLEMENTED, REUSED, CREATED, MODIFIED, BLOCKED, ASSETS REQUIRED, ASSETS AVAILABLE, ASSETS MISSING, TESTS, TEST RESULTS, SCREENSHOT EVIDENCE, PERFORMANCE, KNOWN ISSUES, NEXT STEP.

Use: `PASS`, `CONDITIONAL_PASS`, `FAIL`, `BLOCKED`.

**Never fabricate evidence.**

---

## 42. VISUAL STATUS AUTHORITY

```
SYSTEM_IMPLEMENTATION:     PASS / IN_PROGRESS / BLOCKED
VISUAL_ASSET_PRODUCTION:   NOT_STARTED / IN_PROGRESS / COMPLETE
VISUAL_RUNTIME_ACTIVATION: NOT_STARTED / IN_PROGRESS / COMPLETE
VISUAL_QA:                 NOT_STARTED / IN_PROGRESS / PASS / FAIL
PACKAGED_VISUAL_ACCEPTANCE: NOT_STARTED / PASS / FAIL
```

Never report "VISUAL COMPLETE" until the Golden Visual Slice actually renders correctly in a playable build.

---

## 43. PHASE ORDER

```
PHASE 0   Visual Authority Freeze
PHASE 1   Rendering Baseline
PHASE 2   Anime Material System
PHASE 3   Golden Naruto
PHASE 4   Animation + Shinobi Movement
PHASE 5   Third-Person Camera
PHASE 6   50m Konoha Golden Street
PHASE 7   NPC + Props + World Life
PHASE 8   Combat + Jutsu + VFX
PHASE 9   Hand Signs + QTE + Chakra Control
PHASE 10  Final HUD + Minimap + UI
PHASE 11  Golden Boss Encounter
PHASE 12  100m Konoha
PHASE 13  250m Konoha
PHASE 14  500m Konoha
PHASE 15  Character Factory
PHASE 16  Forms + Transformations
PHASE 17  Skin Factory
PHASE 18  Weapons + Gear + Collection
PHASE 19  Card Collection + Gacha Presentation
PHASE 20  Global World Factory
PHASE 21  Lands + Villages + Special Realms
PHASE 22  Motion Manga + Cinematic Visuals
PHASE 23  25 Arcs / 100 Chapters / 500 Stages
PHASE 24  Global Visual QA
PHASE 25  Packaged Golden Build
```

---

## 44. FINAL DIRECTIVE

**BUILD THE VISUAL FOUNDATION FIRST.**

- DO NOT mask missing assets with lighting.
- DO NOT mask missing characters with placeholders.
- DO NOT mask missing environments with fog.
- DO NOT mask missing materials with emissive.
- DO NOT mask missing animation with code.
- DO NOT mask missing VFX with debug effects.
- DO NOT mask missing UI with debug text.
- DO NOT move to the next phase until the current visual gate passes.

The final game must consistently use this visual language across the entire Naruto: Echoes of Shinobi project.

```
REAL ASSETS + REAL MATERIALS + REAL ANIMATION + REAL LIGHTING + REAL VFX + REAL CAMERA + REAL WORLD + REAL UI = FINAL VISUAL

C++ CODE ALONE IS NEVER VISUAL COMPLETION.
```
