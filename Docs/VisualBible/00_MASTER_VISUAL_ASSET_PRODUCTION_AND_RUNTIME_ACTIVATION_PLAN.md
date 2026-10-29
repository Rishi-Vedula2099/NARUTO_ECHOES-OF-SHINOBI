# NARUTO: ECHOES OF SHINOBI
## MASTER VISUAL RESCUE, CONTENT & RUNTIME ACTIVATION PLAN
### *Authoritative Phase 13 Master Execution Specification | Unreal Engine 5.8*

> **Document Version:** 4.0.0-CONSOLIDATED-MASTER  
> **Master Reference ID:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`  
> **Target Reference Image:** [Konoha_Visual_North_Star.jpg](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Docs/References/Phase13/Konoha_Visual_North_Star.jpg)  
> **Permanent Style Contract:** [00_EOS_PERMANENT_VISUAL_IDENTITY_AND_STYLE_CONTRACT.md](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Docs/VisualBible/00_EOS_PERMANENT_VISUAL_IDENTITY_AND_STYLE_CONTRACT.md)  
> **Benchmark Test Level:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`  
> **Hardware Target:** NVIDIA GeForce GTX 1650 (1920x1080 @ 60 FPS Locked, 1% low > 45 FPS)

---

## 1. Core Principles & Unified Architecture

1. **One Persistent Visual Universe:** All content across characters, skins, villages, lands, jutsu, cards, and future expansions extends the established **EOS Visual DNA**.
2. **Actor Existence ≠ Visual Completion:** An Outliner actor is never complete until the runtime visibly renders the intended production mesh, painterly materials, animations, lighting, camera, and gameplay behavior.
3. **Factory Status vs. Content Status Decoupling:** Tooling/pipelines and actual authored content are tracked independently (`FACTORY_STATUS` vs. `CONTENT_STATUS`).
4. **Hard-Gated Execution:** World scaling beyond the 50m Golden Street benchmark is strictly frozen until the Golden Street loop passes all acceptance criteria against `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`.

---

## 2. The 6 Non-Negotiable Visual Acceptance Gates

$$\mathbf{A + B + C + D + E + F = PRODUCTION\_READY}$$

* **GATE A — Physical Asset Gate:** Production SkeletalMesh/StaticMesh, diffuse/painterly textures, material instances, skeleton, animation assets, and collision physics asset exist physically in `/Game/`.
* **GATE B — Technical Asset Gate:** Correct real-world scale (Genin Naruto 145 cm), clean UV unwraps, clean skeleton bone hierarchy, valid collision hulls, valid LODs, zero unassigned material slots.
* **GATE C — Runtime Binding Gate:** Soft references resolve synchronously, asset loads into memory, mesh visibly appears in viewport, AnimBP executes, animations loop, materials render, collision blocks correctly.
* **GATE D — Visual Fidelity Gate:** Meets `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01`. Correct anime proportions, silhouette, color grading, warm lighting, blue sky, soft cel shading, camera framing, VFX balance.
* **GATE E — Packaged Build Gate:** Standalone / Shipping build runs on NVIDIA GeForce GTX 1650 at 1920x1080 @ locked 60 FPS (1% low > 45 FPS). Zero missing textures, zero default grayboxes, zero debug shapes.
* **GATE F — EOS Visual DNA Consistency Gate:** **"Does this asset unmistakably belong in EOS?"** Evaluates whether the asset matches the unified shader, lighting, color, and proportional language of the game. If it looks photorealistic, flat-chibi, or foreign: **HARD FAIL**.

---

## 3. Authoritative Project Phase 13 Sub-Phase Roadmap

Project Phase 13 is officially designated as **VISUAL RECONSTRUCTION + GOLDEN VERTICAL SLICE**:

```
13.0  VISUAL FORENSIC AUDIT (Audit physical disk assets vs memory references)
  │
  ▼ [Gate 0 Passed]
13.1  EOS VISUAL DNA AUTHORITY (Lock EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01)
  │
  ▼ [Gate 1 Passed]
13.2  ASSET PIPELINE & RENDERER RECOVERY (Fix Black Void, Sky, Sun & Exposure)
  │
  ▼ [Gate 2 Passed]
13.3  GOLDEN NARUTO CHARACTER (SK_Naruto_Genin, Skeleton, 5 Material Slots)
  │
  ▼ [Gate 3 Passed]
13.4  NATURAL SHINOBI MOVEMENT (EOS_NinjaMovementProfile, Ninja Run, Jump, Dodge)
  │
  ▼ [Gate 4 Passed]
13.5  STYLIZED ANIME MATERIAL SYSTEM (M_AnimeCel_Master, 13 Material Instances)
  │
  ▼ [Gate 5 Passed]
13.6  GOLDEN KONOHA STREET (L_Konoha_GoldenStreet 50m Benchmark Composition)
  │
  ▼ [HARD VISUAL GATE: Image Comparison Passed]
  │
13.7  THIRD-PERSON CAMERA (EOS_ExplorationCameraProfile, 25-32% Screen Height)
  │
13.8  JUTSU PRESENTATION & VFX (Rasengan Chakra Gather, Formation, Impact, Recovery)
  │
13.9  LIVING NPC SYSTEM (Teuchi, Sasuke/Sakura Diners, Gate Guard Patrol, Villagers)
  │
13.10 ENEMY ARCHETYPE (Rogue Shinobi with Telegraphs, Hit Reactions & Loot Drop)
  │
13.11 KAKASHI GOLDEN BOSS (Full Mesh, Sharingan Presentation, Lightning Blade)
  │
13.12 LAYERED ADAPTIVE NATURALISTIC BOSS AI (18-Step Decision Loop + Tactician DNA)
  │
13.13 STAGE VISUALIZATION SYSTEM (Story Stage -> World -> District -> Environment)
  │
13.14 REGIONAL WORLD FACTORY PIPELINE (50m -> 100m -> 250m -> 500m Scaling)
  │
13.15 GOLDEN VERTICAL SLICE PLAYABLE LOOP (Boot to Konoha Complete Playable Loop)
  │
13.16 PERFORMANCE PROFILING (GTX 1650 1080p 60 FPS Locked Verification)
  │
13.17 VISUAL AUTOMATION QA (In-Engine Automated Gating Tests)
  │
13.18 PACKAGED BUILD ACCEPTANCE (Shipping Standalone Build Sign-Off)
```

---

## 4. Layered Adaptive Naturalistic Boss AI Contract

Boss AI extends `UEOSAdaptiveAISubsystem` without duplicate subsystems.

### The 18-Step Execution Chain
$$\text{Player} \rightarrow \text{Player Combat Model} \rightarrow \text{Boss Decision Context} \rightarrow \text{FSM} \rightarrow \text{Phase Manager}$$
$$\rightarrow \text{Behavior Tree} \rightarrow \text{Eligibility Filter} \rightarrow \text{Utility AI} \rightarrow \text{Boss Combat DNA}$$
$$\rightarrow \text{Adaptation Layer} \rightarrow \text{Fairness Gate} \rightarrow \text{Action Selection} \rightarrow \text{GAS Activation}$$
$$\rightarrow \text{Animation Montage} \rightarrow \text{Hitbox Query} \rightarrow \text{Damage \& Reaction} \rightarrow \text{Recovery Opening} \rightarrow \text{Player Response}$$

### Boss Combat DNA Profiles
* `TACTICIAN` (Kakashi): Reads player attack frequency, counter-attacks, substitutes under high pressure, changes style in Sharingan phase.
* `DUELIST` (Sasuke): Forward aggression, Sharingan prediction, quick Chidori punishes.
* `CONTROLLER` (Gaara): High defensive zoning, sand shield absorption, ranged area denial.
* `BERSERKER` (Jugo / Curse Mark): Relentless hyper-armor strikes, wide sweeps, low self-preservation.
* `ADAPTIVE_MASTER` (Pain / Madara): Multi-range mastery, spatial manipulation, summon management.

---

## 5. First-Class Meta Architecture

1. **Skins:** Authored meshes using EOS Visual DNA (`SkinID`, `CharacterID`, `EraID`, `FormID`, `CostumeID`, `SkinTier`, `ThemeID`, `ReleasePatchID`, `AcquisitionType`, `EventID`, `PassTrackID`, `PremiumFlag`).
2. **Shinobi Chronicle Cards:** Collectible art and combat bonuses (`CardID`, `CardType`, `CharacterID`, `FormID`, `JutsuID`, `BossID`, `LocationID`, `MemoryID`, `Rarity`, `CollectionSetID`, `EraID`, `ArtProfileID`).
3. **Shinobi Codex:** Master encyclopedia tracking Characters, Forms, Skins, Jutsu, Weapons, Gear, Summons, Tailed Beasts, Bosses, Enemies, NPCs, Villages, Lands, Cards, Bonds, Lore, Motion Manga, Music, Achievements.
4. **Bonds & Shinobi Intel:** Deep relationship progression feeding dialogue, memories, cards, and team synergy effects.
5. **Regional Reputation:** Village standings (Konoha, Suna, Kiri, Kumo, Iwa, Ame) unlocking regional shops, cosmetics, and side stories.
6. **Future Expansion Metadata:** Native support for `ContentPackID`, `ExpansionID`, `SeasonID`, `EventID`, `ReleasePatchID`, `EraID`, `CanonStatus` (enabling *Boruto / Two Blue Vortex* and Great Ninja War expansions).

---

## 6. The Golden Vertical Slice Playable Loop (Sub-Phase 13.15)

The authoritative acceptance test for Phase 13 is the uninterrupted execution of this exact sequence:

$$\text{BOOT} \rightarrow \text{KONOHA STREET} \rightarrow \text{NARUTO VISIBLE} \rightarrow \text{ANIME MATERIAL} \rightarrow \text{THIRD-PERSON CAMERA}$$
$$\rightarrow \text{IDLE} \rightarrow \text{WALK} \rightarrow \text{NINJA RUN} \rightarrow \text{SPRINT} \rightarrow \text{JUMP} \rightarrow \text{DODGE} \rightarrow \text{LOCK-ON}$$
$$\rightarrow \text{MELEE COMBO} \rightarrow \text{RASENGAN} \rightarrow \text{CHAKRA DEDUCTION} \rightarrow \text{ULTIMATE AWAKENING} \rightarrow \text{CHARACTER SWITCH (SASUKE)}$$
$$\rightarrow \text{NPC INTERACTION (TEUCHI)} \rightarrow \text{MISSION RECEIPT} \rightarrow \text{ROGUE ENEMY ENCOUNTER}$$
$$\rightarrow \text{KAKASHI BOSS} \rightarrow \text{ADAPTIVE AI DECISION} \rightarrow \text{PHASE 1} \rightarrow \text{SHARINGAN PHASE 2}$$
$$\rightarrow \text{BOSS DEFEATED} \rightarrow \text{MOTION MANGA CUTSCENE} \rightarrow \text{MISSION REWARD}$$
$$\rightarrow \text{CHRONICLE CARD \& INTEL UNLOCK} \rightarrow \text{CODEX UPDATE} \rightarrow \text{SAVE SUBSYSTEM} \rightarrow \text{RETURN TO STREET}$$
