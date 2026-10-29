# NARUTO: ECHOES OF SHINOBI — MASTER VISUAL COVERAGE MATRIX
### *Visual Production Coverage, Rigging, Animation, Material & Runtime Reference Tracking*

> **Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Registry Source:** [EOSVisualAssetRegistry.csv](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Data/Visual/EOSVisualAssetRegistry.csv)  
> **Audit Reference:** [VISUAL_ASSET_AUDIT.md](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Docs/VisualBible/Reports/VISUAL_ASSET_AUDIT.md)  
> **Rule:** Every row must state genuine production status. `MISSING_PRODUCTION_ASSET` indicates authored 3D geometry/rig/textures must be generated and imported into Unreal Engine before runtime rendering validation can occur.

```
===================================================================================
PHASE 1–13 SYSTEM IMPLEMENTATION:
PASS / EXISTING

PHASE 13 VISUAL ASSET RECTIFICATION:
IN PROGRESS

VISUAL ASSET AUTHORING:
IN PROGRESS

VISUAL RUNTIME ACTIVATION:
BLOCKED UNTIL REQUIRED ASSETS EXIST

FINAL VISUAL VERIFICATION:
PENDING
===================================================================================
```

---

## 1. Playable Shinobi Roster Coverage

| ActorID | CharacterID | FormID | SkinID | Mesh | Rig | Anim | Material | VFX | Portrait | Turnaround | CollectionRender | LOD | Collision | RuntimeReference | Status |
| :--- | :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_CHR_NARUTO_01` | `CHAR_NARUTO` | `NONE` | `SKIN_NARUTO_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_SASUKE_01` | `CHAR_SASUKE` | `NONE` | `SKIN_SASUKE_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Sasuke/Meshes/SK_Sasuke_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_SAKURA_01` | `CHAR_SAKURA` | `NONE` | `SKIN_SAKURA_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Sakura/Meshes/SK_Sakura_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_KAKASHI_01` | `CHAR_KAKASHI` | `NONE` | `SKIN_KAKASHI_JONIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Kakashi/Meshes/SK_Kakashi_Jonin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_ITACHI_01` | `CHAR_ITACHI` | `NONE` | `SKIN_ITACHI_ROBE` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Itachi/Meshes/SK_Itachi_Akatsuki` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_GAARA_01` | `CHAR_GAARA` | `NONE` | `SKIN_GAARA_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Gaara/Meshes/SK_Gaara_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_HINATA_01` | `CHAR_HINATA` | `NONE` | `SKIN_HINATA_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Hinata/Meshes/SK_Hinata_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_ROCKLEE_01` | `CHAR_ROCKLEE` | `NONE` | `SKIN_ROCKLEE_GENIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/RockLee/Meshes/SK_RockLee_Genin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_JIRAIYA_01` | `CHAR_JIRAIYA` | `NONE` | `SKIN_JIRAIYA_SANNIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Jiraiya/Meshes/SK_Jiraiya_Sannin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_CHR_TSUNADE_01` | `CHAR_TSUNADE` | `NONE` | `SKIN_TSUNADE_SANNIN` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Tsunade/Meshes/SK_Tsunade_Sannin` | `MISSING_PRODUCTION_ASSET` |

---

## 2. Character Forms & Awakenings

| ActorID | CharacterID | FormID | SkinID | Mesh | Rig | Anim | Material | VFX | Portrait | Turnaround | CollectionRender | LOD | Collision | RuntimeReference | Status |
| :--- | :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_FORM_NARUTO_01` | `CHAR_NARUTO` | `FORM_VERMILION` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Naruto/Forms/SK_Naruto_Vermilion` | `MISSING_PRODUCTION_ASSET` |
| `ACT_FORM_NARUTO_02` | `CHAR_NARUTO` | `FORM_SAGE_MODE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Naruto/Forms/SK_Naruto_Sage` | `MISSING_PRODUCTION_ASSET` |
| `ACT_FORM_NARUTO_03` | `CHAR_NARUTO` | `FORM_KURAMA_LINK` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Naruto/Forms/SK_Naruto_KCM` | `MISSING_PRODUCTION_ASSET` |
| `ACT_FORM_SASUKE_01` | `CHAR_SASUKE` | `FORM_SHARINGAN` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Sasuke/Forms/MI_Sasuke_Sharingan` | `MISSING_PRODUCTION_ASSET` |
| `ACT_FORM_SASUKE_02` | `CHAR_SASUKE` | `FORM_CURSE_MARK_2`| `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Sasuke/Forms/SK_Sasuke_CurseMark2` | `MISSING_PRODUCTION_ASSET` |
| `ACT_FORM_ROCKLEE_01` | `CHAR_ROCKLEE` | `FORM_EIGHT_GATES` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/RockLee/Forms/SK_RockLee_Gates` | `MISSING_PRODUCTION_ASSET` |

---

## 3. Bosses & Combat Enemies

| ActorID | CharacterID | FormID | SkinID | Mesh | Rig | Anim | Material | VFX | Portrait | Turnaround | CollectionRender | LOD | Collision | RuntimeReference | Status |
| :--- | :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_BOSS_ZABUZA` | `CHAR_ZABUZA` | `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Bosses/Zabuza/Meshes/SK_Zabuza_Executioner` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BOSS_GAARA_SHUKAKU` | `CHAR_GAARA` | `FORM_AWAKENED` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Bosses/Gaara/Meshes/SK_Gaara_HalfShukaku` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BOSS_OROCHIMARU` | `CHAR_OROCHIMARU`| `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Bosses/Orochimaru/Meshes/SK_Orochimaru_Kusanagi` | `MISSING_PRODUCTION_ASSET` |
| `ACT_ENM_ROGUE_MELEE` | `CHAR_ROGUE_NINJA`| `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Melee` | `MISSING_PRODUCTION_ASSET` |
| `ACT_ENM_ROGUE_RANGED`| `CHAR_ROGUE_NINJA`| `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Enemies/RogueNinja/Meshes/SK_RogueNinja_Ranged` | `MISSING_PRODUCTION_ASSET` |
| `ACT_ENM_SOUND_BRUTE` | `CHAR_SOUND_BRUTE`| `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Enemies/SoundFour/Meshes/SK_SoundBrute_Jirobo` | `MISSING_PRODUCTION_ASSET` |
| `ACT_ENM_PUPPET_SOLDIER`| `CHAR_PUPPET` | `NONE` | `DEFAULT` | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/Enemies/Puppets/Meshes/SK_Karasu_Puppet` | `MISSING_PRODUCTION_ASSET` |

---

## 4. Konoha Living NPCs

| ActorID | CharacterID | Archetype | Mesh | Rig | Anim | Material | VFX | Portrait | Turnaround | CollectionRender | LOD | Collision | RuntimeReference | Status |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_NPC_VILLAGER_MALE` | `CHAR_NPC_VILLAGER` | Civilian | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/NPCs/KonohaVillagers/Meshes/SK_Villager_Male01` | `MISSING_PRODUCTION_ASSET` |
| `ACT_NPC_SHOPKEEPER_TEUCHI` | `CHAR_NPC_SHOPKEEPER`| Merchant | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi` | `MISSING_PRODUCTION_ASSET` |
| `ACT_NPC_GATE_GUARD` | `CHAR_NPC_GATEGUARD` | Security | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/NPCs/Guards/Meshes/SK_Chunin_GateGuard` | `MISSING_PRODUCTION_ASSET` |
| `ACT_NPC_ACADEMY_STUDENT` | `CHAR_NPC_STUDENT` | Youth | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Characters/NPCs/Students/Meshes/SK_AcademyStudent_Male` | `MISSING_PRODUCTION_ASSET` |

---

## 5. Konoha Architecture & Environment Buildings

| ActorID | BuildingID | Type | Mesh | Collision | Material | UV Unwrapped | LOD0-3 | Occlusion | RuntimeReference | Status |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_BLD_ICHIRAKU` | `BLD_ICHIRAKU` | Shop/Ramen | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_RESIDENCE_A` | `BLD_RESIDENCE_A` | Residential | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_RESIDENCE_B` | `BLD_RESIDENCE_B` | Residential | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_WEAPON_SHOP` | `BLD_WEAPON_SHOP` | Shop/Armory | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_WeaponShop` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_TEA_HOUSE` | `BLD_TEA_HOUSE` | Commercial | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_TeaHouse` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_MISSION_OFFICE`| `BLD_MISSION_OFFICE`| Civic | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_MissionOffice` | `MISSING_PRODUCTION_ASSET` |
| `ACT_BLD_HOKAGE_MANSION`| `BLD_HOKAGE_MANSION`| Civic/Landmark| ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_HokageMansion` | `MISSING_PRODUCTION_ASSET` |

---

## 6. Street Props, Furnishings & Vegetation

| ActorID | PropID | Type | Mesh | Collision | Material | Physics | RuntimeReference | Status |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :--- | :--- |
| `ACT_PROP_LANTERN_STONE` | `PROP_LANTERN_STONE` | StreetFurnishing | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_StoneLantern_01` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_LANTERN_HANGING`| `PROP_LANTERN_HANGING`| StreetFurnishing | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_PaperLantern_Hanging` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_WOODEN_BENCH` | `PROP_WOODEN_BENCH` | StreetFurnishing | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_Bench_01` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_TRAINING_POST` | `PROP_TRAINING_POST` | TrainingGear | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_TrainingLog_Post` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_SUPPLY_CRATE` | `PROP_SUPPLY_CRATE` | Container | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShinobiSupply_Crate` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_WATER_BARREL` | `PROP_WATER_BARREL` | Container | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_RainBarrel_Wood` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_SHOP_BANNER` | `PROP_SHOP_BANNER` | Signage | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_ShopBanner_Ichiraku` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_FIRE_HYDRANT` | `PROP_FIRE_HYDRANT` | Infrastructure | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_WaterPumpingStation` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_UTILITY_POLE` | `PROP_TELEGRAPH_POLE`| Infrastructure | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_WoodenUtilityPole` | `MISSING_PRODUCTION_ASSET` |
| `ACT_PROP_TRASH_BIN` | `PROP_TRASH_BIN` | Sanitation | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Props/Meshes/SM_Prop_Konoha_TrashBin` | `MISSING_PRODUCTION_ASSET` |
| `ACT_VEG_CEDAR_LARGE` | `VEG_CEDAR_LARGE` | Foliage/Tree | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaCedar_Large` | `MISSING_PRODUCTION_ASSET` |
| `ACT_VEG_CHERRY_BLOSSOM` | `VEG_CHERRY_BLOSSOM`| Foliage/Tree | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_Sakura_Bloom` | `MISSING_PRODUCTION_ASSET` |
| `ACT_VEG_HEDGE_SHRUB` | `VEG_HEDGE_SHRUB` | Foliage/Bush | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_KonohaShrub_01` | `MISSING_PRODUCTION_ASSET` |
| `ACT_VEG_CLOVER_GRASS` | `VEG_GRASS_PATCH` | Foliage/Ground | ❌ | ❌ | ❌ | ❌ | `/Game/Environment/Konoha/Foliage/Meshes/SM_Veg_GrassTuft_Stylized` | `MISSING_PRODUCTION_ASSET` |

---

## 7. Ninjutsu VFX & Combat Particle Systems

| JutsuID | VisualProfileID | Element | CastAnim | ChargeVFX | ProjectileVFX | ImpactVFX | CameraShake | AudioEvent | RuntimeReference | Status |
| :--- | :--- | :--- | :---: | :---: | :---: | :---: | :---: | :---: | :--- | :--- |
| `JUTSU_RASENGAN` | `VP_JUTSU_RASENGAN` | Wind/Chakra | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master` | `MISSING_PRODUCTION_ASSET` |
| `JUTSU_CHIDORI` | `VP_JUTSU_CHIDORI` | Lightning | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_Chidori_Discharge_Master` | `MISSING_PRODUCTION_ASSET` |
| `JUTSU_FIREBALL` | `VP_JUTSU_FIREBALL` | Fire | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_Goukakyuu_Fireball_Master`| `MISSING_PRODUCTION_ASSET` |
| `JUTSU_SHADOW_CLONE` | `VP_JUTSU_CLONE` | Yin/Yang | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst` | `MISSING_PRODUCTION_ASSET` |
| `JUTSU_WATER_DRAGON` | `VP_JUTSU_WATER_DRAGON`| Water | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_Suiryuudan_WaterVortex` | `MISSING_PRODUCTION_ASSET` |
| `JUTSU_SAND_TSUNAMI` | `VP_JUTSU_SAND_SURGE` | Earth/Magnet | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | `/Game/VFX/Niagara/Jutsu/NS_SabakuTaisou_SandCrush` | `MISSING_PRODUCTION_ASSET` |
