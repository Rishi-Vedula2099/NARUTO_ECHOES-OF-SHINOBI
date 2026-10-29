# GLOBAL VISUAL ASSET PIPELINE & REPEATABLE PRODUCTION FACTORY
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Standard:** Enterprise Game Asset Pipeline & Data-Driven Provenance  

---

## 1. The Eight Asset Maturity States (Strict Lifecycle)

No asset may skip states or claim production readiness without verifiable proof at every stage:

```
[1. BLOCKED_ASSET_AUTHORING] ──> Dependency identified, task queued
         │
[2. SOURCE_ACQUIRED]        ──> 3D model/texture acquired, license logged
         │
[3. DCC_VALIDATED]          ──> Blender/Maya: topology, UVs, skeleton validated
         │
[4. UNREAL_IMPORTED]        ──> Imported as USkeletalMesh / UStaticMesh in Content/
         │
[5. RUNTIME_VALIDATED]      ──> Spawned in level, materials bound, animation playing
         │
[6. ART_APPROVED]           ──> Human review of CAM_A, CAM_B against Visual DNA
         │
[7. PACKAGED_QA]            ──> Tested in Standalone Win64 build @ 1080p 60 FPS
         │
[8. PRODUCTION_READY]       ──> Locked in MASTER_GATE_EXECUTION_TRACKER
```

---

## 2. Character Assembly Architecture

Every character entity in ECHOES OF SHINOBI resolves through this deterministic data-driven chain:

```
CHARACTER ID (e.g., CHAR_NARUTO)
  │
  ├── INCARNATION (e.g., INC_GENIN, INC_SHIPPUDEN, INC_THE_LAST, INC_BORUTO)
  │     │
  │     ├── FORM (e.g., FORM_BASE, FORM_SAGE, FORM_ONE_TAIL, FORM_KCM)
  │     │     │
  │     │     ├── COSTUME (e.g., COSTUME_CLASSIC_TRACKSUIT, COSTUME_CLOAK_SCROLL)
  │     │     │     │
  │     │     │     ├── VISUAL PROFILE: CVP_Shinobi_Standard
  │     │     │     ├── SKELETAL MESH: SK_Naruto_Genin
  │     │     │     ├── SKELETON: SKEL_Humanoid_Shinobi (20-joint canonical)
  │     │     │     ├── MATERIALS: MI_Naruto_Skin, MI_Naruto_Hair, MI_Naruto_Tracksuit...
  │     │     │     ├── ANIM BLUEPRINT: ABP_Naruto_Genin
  │     │     │     ├── ANIMATION SET: Locomotion, Combos, Rasengan, Victory
  │     │     │     ├── COMBAT PROFILE: CP_Naruto_Genin (Damage, frames, hitboxes)
  │     │     │     ├── JUTSU ASSETS: DA_Rasengan, DA_ShadowClone
  │     │     │     └── VFX ASSETS: NS_Chakra_Spiral, NS_Smoke_Poof
  │     │     │
  │     │     └── RUNTIME PAWN: BP_Naruto_Genin_Character
```

---

## 3. World Assembly Architecture

The world is an interconnected geographic continuum, structured hierarchically:

```
WORLD (Shinobi Continents)
  │
  ├── LAND (e.g., Land of Fire, Land of Wind, Land of Lightning)
  │     │
  │     ├── SETTLEMENT (e.g., Konohagakure, Sunagakure, Tanzaku Town)
  │     │     │
  │     │     ├── DISTRICT (e.g., Central Golden Street, Academy, Uchiha Compound)
  │     │     │     │
  │     │     │     ├── MAP / LEVEL (e.g., L_Konoha_GoldenStreet, L_Konoha_Training)
  │     │     │     │     │
  │     │     │     │     ├── ZONE (Street 50m Slice, Crossroads, Park)
  │     │     │     │     ├── LANDMARKS (Ichiraku Ramen, Hokage Mansion, Hokage Rock)
  │     │     │     │     ├── BUILDINGS (Residences A-D, Weapon Shop, Tea House)
  │     │     │     │     ├── TERRAIN & SURFACES (Stone Cobblestones, Earth, Curbs)
  │     │     │     │     ├── PROPS & STREET LIFE (Lanterns, Banners, Crates, Poles)
  │     │     │     │     ├── NPC POPULATION (Civilians, Academy Students, Patrols)
  │     │     │     │     ├── ENCOUNTERS & MISSION HOOKS
  │     │     │     │     └── ENVIRONMENT PROFILE (EP_Konoha_Daylight)
```

---

## 4. Mandatory Asset Provenance Schema

Every asset registered in the repository must be documented in `Docs/VisualBible/ASSET_PROVENANCE_LEDGER.md` with:
- **`AssetID`**: Unique permanent identifier (e.g. `SK_Naruto_Genin`).
- **`AssetPath`**: Content browser path (e.g. `/Game/Characters/Naruto/Meshes/...`).
- **`SourceStatus`**: `AUTHOR_CREATED`, `DCC_AUTHORED`, `LICENSED_EXTERNAL_ASSET`, `EXISTING_PROJECT_ASSET`.
- **`SourceLicense`**: CC-BY, Royalty-Free Commercial, or Custom Authored.
- **`ProductionStatus`**: One of the 8 maturity states.
- **`VisualProfileID`**: Bound CVP/EP identifier.
- **`SkeletonPath`**: Target skeleton asset.
- **`KnownLimitations`**: Explicit visual/technical defects awaiting resolution.
