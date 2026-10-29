# CHARACTER PRODUCTION PIPELINE & ROSTER ARCHITECTURE REPORT
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Permanent Governance Anchor:** 5-Component Character Factory Standard & 8-State Asset Contract  
**Lead Benchmark Character:** `SK_Naruto_Genin` (Classic Part 1 Era)  

---

## 1. The 5-Component Character Factory Standard

Every character in ECHOES OF SHINOBI is produced according to the authoritative character reality formula:

$$\text{Character Reality: } \text{Face Identity} + \text{Facial Anatomy} + \text{Designed Hair} + \text{Costume Construction} + \text{Deformation} + \text{Expressions}$$

```
CHARACTER FACTORY PIPELINE
│
├── COMPONENT A: FACE IDENTITY (Close-Up CAM_B Gate)
│   ├── Head proportions (~1:5.5 Genin, ~1:6.5 Shippuden)
│   ├── Expressive almond anime eyes with cerulean blue pupil depth and specular catchlights
│   ├── 3 crisp vector-painted whisker lines following zygomatic cheek curves
│   └── Stamped curved metallic Konoha headband plate with leaf crest and 4 corner rivets
│
├── COMPONENT B: HAIR & SILHOUETTE
│   ├── Tiered volume clumps (Front Bangs, Side Temporal Locks, Crown Radial Spikes)
│   ├── Rooted naturally under and around the cloth forehead protector
│   └── Billowing navy headband ribbon tails tied behind head
│
├── COMPONENT C: COSTUME & ANATOMY CONSTRUCTION
│   ├── Thick ribbed white fleece collar with central brass zipper slider
│   ├── Tailored tracksuit jacket with navy shoulder yokes and 3D back Uzumaki spiral crest
│   ├── Right-thigh white medical bandage wrap with molded black kunai holster pouch
│   └── Navy ninja sandals with exposed toes, sculpted toenails, and dark tread soles
│
├── COMPONENT D: FACIAL ANIMATION & MORPH TARGETS
│   ├── Blend shapes: `Neutral_Focus`, `Smirk_Determined`, `Combat_Yell`, `Surprise_Blink`
│   └── Curve-driven triggers wired into Animation Blueprint (`ABP_Naruto_Genin`)
│
└── COMPONENT E: RUNTIME VALIDATION
    ├── CAM_B Close-Up Gate (95 cm inspection of face, eyes, and whiskers)
    └── CAM_A Gameplay Gate (320 cm third-person framing, character = 28% screen height)
```

---

## 2. Naruto Genin Technical Asset Specification

| Parameter | Specification | Asset Path / Value |
| :--- | :--- | :--- |
| **Asset ID** | `CHAR_NARUTO_GENIN_01` | Content Package Identifier |
| **Skeletal Mesh** | `SK_Naruto_Genin_Rigged` | `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged` |
| **Skeleton Hierarchy** | `SKEL_Humanoid_Shinobi` | 20-joint canonical humanoid hierarchy |
| **Height / Proportions** | 145 cm total height | 1:5.5 Genin anatomical head-to-body ratio |
| **Master Material** | `M_AnimeCel_Master` | `/Game/Materials/Master/M_AnimeCel_Master` (Cel Shading + Stencil 2 Hull) |
| **Face Texture** | `T_Naruto_Genin_Face_D` | `/Game/Characters/Naruto/Textures/T_Naruto_Genin_Face_D` (2048x2048) |
| **Animation Blueprint**| `ABP_Naruto_Genin` | `/Game/Characters/Naruto/Anims/ABP_Naruto_Genin` |
| **Combat Profile** | `CP_Naruto_Genin` | Data Asset: Combos, hitboxes, frame data |
| **Signature Jutsu** | `DA_Rasengan_Standard` | Niagara particle flow + forward impulse |

---

## 3. Seven Canonical Material Slots

```
Slot 0: MI_Naruto_Skin      -> BaseColor: (0.98, 0.82, 0.70), ShadowFloor: 0.18, Face Texture Hook
Slot 1: MI_Naruto_Navy      -> BaseColor: (0.06, 0.10, 0.22), Collar & Shoulder Panels
Slot 2: MI_Naruto_Hair      -> BaseColor: (1.00, 0.84, 0.10), Sunflower Anime Blonde
Slot 3: MI_Naruto_Collar    -> BaseColor: (0.95, 0.95, 0.95), Thick Ribbed Fleece Funnel
Slot 4: MI_Naruto_Tracksuit  -> BaseColor: (1.00, 0.45, 0.04), Saturated Shinobi Cadmium Orange
Slot 5: MI_Naruto_Emblem    -> BaseColor: (0.85, 0.08, 0.08), Crimson Uzumaki Spiral
Slot 6: MI_Naruto_Metal     -> BaseColor: (0.85, 0.88, 0.92), Specular: 0.75, Metallic: 0.85
```

---

## 4. Franchise Scope Coverage Audit (Explicit Roster Matrix)

The catalog covers the full franchise scope across Main Heroes, Supporting Shinobi, Kage, ANBU, Academy, Civilians, Rogue Ninja, Akatsuki, Edo Shinobi, Jinchuriki, Tailed Beasts, and Ōtsutsuki. 

Explicit audits for the 21 mandatory characters:

| Shinobi Identity | Incarnation | Canonical Village | Combat Role | Asset Maturity State | Mesh / Rig Requirement |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Anko Mitarashi** | Special Jonin | Konoha | Striker / Fire / Snake | `TODO_AUTHORING` | Trenchcoat, fishnet body stocking, cursed seal neck |
| **Mifune** | Samurai General | Land of Iron | Blademaster (Iaidō) | `TODO_AUTHORING` | Iron armor plating, kabuto helmet, meitō Kurosawa katana |
| **Karin** | Healer / Sensor | Grass / Sound | Support / Mind's Eye | `TODO_AUTHORING` | Magenta ponytail, spectacles, Uzumaki chakra chains |
| **Shisui Uchiha** | Teleporter (Sunshin)| Konoha | Speed / Kotoamatsukami | `TODO_AUTHORING` | High-collar Jonin flak, short katana back holster, Mangekyō |
| **Fugaku Uchiha** | Clan Head | Konoha | Tactical / Fire / Sharingan| `TODO_AUTHORING` | Police force emblem haori, severe jawline, stern brows |
| **Ibiki Morino** | Interrogation Unit | Konoha | Grappler / Torture Genjutsu| `TODO_AUTHORING` | Black overcoat, facial torture scars, forehead band |
| **Izumo Kamizuki** | Chunin Sentry | Konoha | Synergy / Water Starch Syrup| `TODO_AUTHORING` | Forehead protector covering right eye, mitsumata weapon |
| **Kotetsu Hagane** | Chunin Sentry | Konoha | Synergy / Genjutsu / Conch | `TODO_AUTHORING` | Bandaged nose bridge, spiky hair, giant conch mace |
| **Hayate Gekko** | Special Jonin | Konoha | Speed / Dance of the Crescent| `TODO_AUTHORING` | Dark eye circles, persistent cough, Konoha katana |
| **Genma Shiranui** | Hokage Guard Platoon| Konoha | Ranged / Senbon Spitting | `TODO_AUTHORING` | Backward bandana protector, senbon needle in lips |
| **Aoba Yamashiro** | Special Jonin | Konoha | Crowd Control / Crow Flock | `TODO_AUTHORING` | Dark sunglasses, spiky black hair, raven summon feather |
| **Raidō Namiashi** | Hokage Guard Platoon| Konoha | Assassin / Black Blade | `TODO_AUTHORING` | Facial burn scars, dark hair, poisoned ninja sword |
| **Ebisu** | Elite Special Jonin | Konoha | Tactician / Fire Technique | `TODO_AUTHORING` | Black sunglasses, blue bandana protector, teacher jacket |
| **Mizuki** | Rogue Chunin | Konoha | Traitor / Giant Shuriken | `TODO_AUTHORING` | White spiky hair, orange flak jacket, twin back shuriken |
| **Iruka Umino** | Academy Instructor | Konoha | Support / Barrier Formation | `TODO_AUTHORING` | Horizontal nose scar, high ponytail, Konoha Chunin flak |
| **Tayuya** | Sound Four | Sound | Ranged / Demon Flute / Curse | `TODO_AUTHORING` | Long salmon hair, black demon flute, Stage 2 horns |
| **Guren** | Crystal Release | Sound Outskirts| Area Denial / Crystal Jade | `TODO_AUTHORING` | Elegant green kimono, red camellia hairpin, crystal blades |
| **Mū** | Second Tsuchikage | Iwagakure | Dust Release / Invisibility | `TODO_AUTHORING` | Full-body medical mummy wraps, dual twin back swords |
| **Ashina Uzumaki** | Uzumaki Clan Leader| Whirlpool | Sealing Master / Kenjutsu | `TODO_AUTHORING` | Long white beard, armor haori, Whirlpool seal scroll |
| **Nagato** | Reanimated / Prime | Rain | Rinnegan Six Paths Avatar | `TODO_AUTHORING` | Crimson Uzumaki hair, reanimation sclera, Rinnegan irises |
| **Yahiko** | Original Akatsuki | Rain | Leader / Water Release | `TODO_AUTHORING` | Short spiky orange hair, Akatsuki clouds, Rain forehead band |

---

## 5. Form & Transformation Resolution Architecture

Transformations in EOS never mutate base skeleton hierarchies or damage gameplay state:
```
CHARACTER: Naruto Uzumaki
  ├── FORM_BASE (Classic Tracksuit, CVP_Shinobi_Standard)
  ├── FORM_VERMILION_EYES (Kyuubi Leak, CVP_Shinobi_Standard + Red Slit Eyes)
  ├── FORM_ONE_TAIL_CLOAK (Chakra Shroud, CVP_Jinchuriki_Chakra + Translucent Shell)
  ├── FORM_FOUR_TAILS (Dark Blood Demon Shroud, CVP_Jinchuriki_Chakra + Skeletal Horns)
  ├── FORM_SAGE_MODE (Toad Iris + Orange Pigmentation + Sage Cloak Mesh Attachment)
  ├── FORM_KURAMA_CHAKRA_MODE (Golden Luminescence, CVP_Jinchuriki_Chakra + Flowing Flame Ribbons)
  └── FORM_SIX_PATHS_SAGE (Gudōdama Orbs + Cross-slit Pupils + Dual Chakra Rods)
```
Each form dynamically switches the assigned `FEOSCharacterVisualProfile` and material instances without instantiating duplicate pawns or corrupting GAS state.

