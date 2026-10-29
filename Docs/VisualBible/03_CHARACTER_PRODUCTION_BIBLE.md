# NARUTO: ECHOES OF SHINOBI
# 03 — CHARACTER PRODUCTION BIBLE
## *Authoritative Character Pipeline Specification*

---

## Core Principle

Every character is a complete **production package** — not just a mesh or a Blueprint actor. A character is NOT complete until the entire package is delivered and validated.

---

## Character Production Package

```
CHARACTER KEY ART
        ↓
FRONT / BACK / LEFT / RIGHT TURNAROUND
        ↓
FACE / HEAD / HAIR / EYES / HEADBAND
CLOTHING / ARMOR / POUCHES / WEAPONS / ACCESSORIES
        ↓
IDLE / WALK / RUN / SPRINT / JUMP / DODGE / COMBAT STANCE
        ↓
BASIC ATTACKS / COMBO / JUTSU 1 / JUTSU 2 / JUTSU 3
SIGNATURE / ULTIMATE / TRANSFORMATION
        ↓
VFX ATTACHMENT + PRESENTATION
        ↓
PORTRAIT
        ↓
COLLECTION CARD
        ↓
GACHA / BANNER ART
        ↓
SKIN VARIANTS
```

---

## Golden Character: NARUTO UZUMAKI (GENIN — BASE FORM)

Naruto is the **first and only** character that must be fully completed before any other character production begins. He proves the entire pipeline.

### Required Deliverables

| # | Category | Deliverable | Status |
|---|---|---|---|
| 01 | Reference | Key Art | TODO_AUTHORING |
| 02 | Turnaround | Front view | TODO_AUTHORING |
| 03 | Turnaround | Back view | TODO_AUTHORING |
| 04 | Turnaround | Left profile | TODO_AUTHORING |
| 05 | Turnaround | Right profile | TODO_AUTHORING |
| 06 | Turnaround | Three-quarter view | TODO_AUTHORING |
| 07 | Detail | Face close-up | TODO_AUTHORING |
| 08 | Detail | Hair detail | TODO_AUTHORING |
| 09 | Detail | Costume detail (tracksuit, collar, swirl emblem) | TODO_AUTHORING |
| 10 | Detail | Accessory detail (headband, leg holster) | TODO_AUTHORING |
| 11 | Detail | Weapon detail (kunai, shuriken) | TODO_AUTHORING |
| 12 | Mesh | `SK_Naruto_Genin` skeletal mesh | EXISTS (validate) |
| 13 | Skeleton | Full humanoid skeleton + physics asset | VALIDATE |
| 14 | Materials | `MI_Naruto_Skin` | EXISTS |
| 15 | Materials | `MI_Naruto_Hair` | EXISTS |
| 16 | Materials | `MI_Naruto_Tracksuit` | EXISTS |
| 17 | Materials | `MI_Naruto_Navy` | EXISTS |
| 18 | Materials | `MI_Naruto_Collar` | EXISTS |
| 19 | Materials | `MI_Naruto_Genin` (master) | EXISTS |
| 20 | Animation | AnimBP | VALIDATE |
| 21 | Animation | Idle | TODO |
| 22 | Animation | Walk | TODO |
| 23 | Animation | Run (ninja run) | TODO |
| 24 | Animation | Sprint | TODO |
| 25 | Animation | Jump | TODO |
| 26 | Animation | Fall | TODO |
| 27 | Animation | Land | TODO |
| 28 | Animation | Dodge | TODO |
| 29 | Animation | Guard | TODO |
| 30 | Animation | Hit Reaction | TODO |
| 31 | Animation | KO | TODO |
| 32 | Combat | Basic Attack 1-3 | TODO |
| 33 | Combat | Combo chain | TODO |
| 34 | Combat | Rasengan (Jutsu 1) | TODO |
| 35 | Combat | Shadow Clone (Jutsu 2) | TODO |
| 36 | Combat | Jutsu 3 | TODO |
| 37 | Combat | Signature move | TODO |
| 38 | Combat | Ultimate | TODO |
| 39 | Combat | Transformation (Nine-Tails Chakra) | TODO |
| 40 | VFX | Rasengan VFX | TODO |
| 41 | VFX | Shadow Clone VFX | TODO |
| 42 | VFX | Chakra charging VFX | TODO |
| 43 | VFX | Hit impact VFX | TODO |
| 44 | UI | Portrait | TODO |
| 45 | UI | Collection card | TODO |
| 46 | UI | Gacha/banner art | TODO |
| 47 | Gameplay | GAS ability set integration | VALIDATE |
| 48 | Gameplay | VFX socket points | VALIDATE |
| 49 | Gameplay | Weapon/tool sockets | VALIDATE |
| 50 | Pose | Victory pose | TODO |
| 51 | Pose | Defeat pose | TODO |

---

## Character Proportions Reference

```
NARUTO GENIN PROPORTIONS:
- Total height: ~145 cm (real-world scale)
- Head-to-body ratio: ~1:5.5
- Facial features: Large expressive eyes, three whisker marks per cheek
- Hair: Spiky blonde, medium length
- Build: Lean, athletic, age-appropriate

ANIME STYLE RULES:
- Eyes: ~30% of face width
- Head: Slightly larger than realistic proportion
- Hands: Slightly enlarged for readability
- Feet: Stylized sandals, visible toes
- Torso: Simplified anatomy
- Limbs: Clean silhouette, no hyper-musculature
```

---

## Character Data Contract

```cpp
// Every character MUST have these fields populated
UPROPERTY() FName CharacterID;       // "CHAR_NARUTO"
UPROPERTY() FName EraID;             // "ERA_GENIN"
UPROPERTY() FName FormID;            // "FORM_BASE"
UPROPERTY() FName CostumeID;         // "COSTUME_GENIN_TRACKSUIT"
UPROPERTY() FName VillageID;         // "VILLAGE_KONOHA"
UPROPERTY() FName ClanID;            // "CLAN_UZUMAKI"
UPROPERTY() FName ElementID;         // "ELEMENT_WIND"
UPROPERTY() FName CombatRoleID;      // "ROLE_MELEE_BRAWLER"
UPROPERTY() TSoftObjectPtr<USkeletalMesh> CharacterMesh;
UPROPERTY() TSoftObjectPtr<UAnimBlueprint> AnimationBlueprint;
UPROPERTY() TArray<TSoftObjectPtr<UMaterialInstance>> MaterialSlots;
UPROPERTY() TSoftObjectPtr<UTexture2D> Portrait;
UPROPERTY() TSoftObjectPtr<UTexture2D> CollectionCard;
```

---

## Character Factory Pipeline (Phase 15+)

Once Naruto is validated, the factory applies to all subsequent characters:

```
1. CHARACTER SPECIFICATION
   ↓
2. REFERENCE SHEET (37 deliverables)
   ↓
3. MESH AUTHORING (DCC → FBX)
   ↓
4. SKELETON + RIGGING
   ↓
5. MATERIAL INSTANCES (from M_AnimeCel_Master)
   ↓
6. UNREAL IMPORT + VALIDATION
   ↓
7. ANIMATION SET
   ↓
8. GAS ABILITY SET
   ↓
9. VFX ATTACHMENT
   ↓
10. UI ASSETS (Portrait, Card, Gacha)
    ↓
11. RUNTIME BINDING
    ↓
12. VISUAL GATE REVIEW
    ↓
13. PRODUCTION_READY
```

---

## Roster Priority (Post-Golden Naruto)

| Priority | Character | Era | Role |
|---|---|---|---|
| 1 | Naruto Uzumaki | Genin | GOLDEN CHARACTER |
| 2 | Sasuke Uchiha | Genin | Party + Boss |
| 3 | Sakura Haruno | Genin | Party |
| 4 | Kakashi Hatake | Jonin | Boss + NPC |
| 5 | Gaara | Chunin Exam | Boss |
| 6 | Rock Lee | Genin | Party |
| 7 | Hinata Hyuga | Genin | Party |
| 8 | Itachi Uchiha | S-Rank | Boss |
| 9 | Jiraiya | Sannin | Mentor NPC |
| 10 | Tsunade | Sannin | Mentor NPC |

**DO NOT begin characters 2-10 until Naruto passes ALL visual gates.**
