# REPOSITORY AUDIT & RUNTIME ENVIRONMENT BASELINE
**Project:** NARUTO: ECHOES OF SHINOBI  
**Engine:** Unreal Engine 5.8 (DirectX 12 / SM6)  
**Target Hardware:** NVIDIA GeForce GTX 1650 4GB VRAM  
**Target Performance:** 1080p @ 60 FPS (VSync ON)  
**Audit Timestamp:** October 2026  
**Primary Map:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`  
**Secondary Map (Proving Ground):** `/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest`  

---

## 1. Executive Summary & Root Cause of Runtime Screenshot Defect

### A. Defect Identification from Runtime Evidence
The uploaded in-engine screenshot (`media_1793283768094_460b3b9b.jpg`) demonstrates the visual baseline failure state:
1. **Untextured / Grey-Blue Player Character:** The player pawn displays a low-detail proxy silhouette without facial features, anime eye contours, whiskers, or proper cel-shaded textures.
2. **Exposed Testbed Primitives:** A row of 11 material calibration swatch spheres (`Swatch_Skin`, `Swatch_Hair`, `Swatch_Tracksuit`, etc.), an isolated combat dummy (`BP_TrainingDummy_01`), and two stone lantern posts are visible behind the character.
3. **Artifact-Laden Ground Surface:** Stepped reflective/glass channels with intense specular blowout.
4. **Devoid Environment & Flat Sky:** Pure atmosphere gradient devoid of Konoha timber architecture, shopfronts, cherry blossoms, or the Hokage Monument vista.

### B. Root Cause Breakdown
1. **Level Misdirection:** The running map was `/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest` (an isolated 50m material calibration testbed) rather than `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`.
2. **Missing C++ Default Pawn Mesh Assignment:** `AEOSGameModeBase` specifies `DefaultPawnClass = AEOSCharacterBase::StaticClass();`. Within `AEOSCharacterBase::SetupStylizedVisualMesh()`, `GetMesh()->SetSkeletalMeshAsset(...)` was omitted in C++, resulting in an unassigned skeletal mesh component on the dynamically spawned pawn.
3. **Facial Texture & Geometry Absence:** The imported skeletal mesh `SK_Naruto_Genin` lacked UV-mapped eye/pupil textures, vector cheek whiskers, and facial morph targets, rendering as a blank conical face.
4. **Procedural Geometry Artifacts:** The procedural OBJ generation in earlier scripts produced simplified volume shells without multi-tiered bangs, temporal hair clumping, or ribbed collar zipper details.

---

## 2. Engine & Build Environment Status

| Subsystem | Configuration | Verified Status |
| :--- | :--- | :--- |
| **Engine Version** | Unreal Engine 5.8 (`UE_5.8`) | Installed at `C:\Program Files\Epic Games\UE_5.8` |
| **UProject** | `EchoesOfShinobi.uproject` | EngineAssociation: "5.8", Modules: EchoesOfShinobi |
| **Target Platform** | Win64 | DirectX 12 / SM6, TSR Anti-Aliasing |
| **Active Plugins** | GameplayAbilities, EnhancedInput, CommonUI, StateTree, Niagara, PythonScriptPlugin, EditorScriptingUtilities | All Verified Enabled |
| **C++ Build System** | UnrealBuildTool (UBT) / Win64 Development | Clean Compilation Verified |
| **Launcher / Scripts** | `LaunchGame.ps1`, `RunGame.bat`, `EchoesOfShinobiLauncher.exe` | Verified Operational |

---

## 3. Map & Content Level Inventory

### A. Level Directory (`Content/Maps/`)
- `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap`: The primary 50m production visual vertical slice.
  - Contains: `SkyAtmosphere_KonohaAnime`, `DirLight_Konoha_Sun`, `SkyLight_AnimeAzureFill`, `PostProcess_GoldenStreet`, 13 Avenue road sections (`Road_Avenue_00` to `Road_Avenue_12`), `Bld_IchirakuRamen` with hanging paper lanterns and noren curtains, `Bld_KonohaWeaponShop`, `Bld_KonohaMissionOffice`, `Bld_KonohaResidence_A/B`, `Bld_KonohaTeaHouse`, `Bld_HokageMansion_MidVista`, `Backdrop_Konoha_NorthStar_Vista`, and 5 living Konoha NPCs.
- `/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest.umap`: Isolated 50m asset calibration testbed.
  - Contains: 11 material swatch spheres, stone lanterns, combat training post, lighting probes.
  - **Policy:** Restricted strictly to technical QA. Must never be used as a public visual benchmark.
- `/Game/Maps/GoldenTest/L_Naruto_Showcase.umap`: Isolated turntable showcase map.
- `/Game/Maps/Konoha/L_Konoha_Phase13_District.umap`: 500m expanded Konoha district layout.

---

## 4. Character Asset Inventory & Maturity State

| Asset Name | Asset Type | In-Engine Path | Current Maturity State | Target State |
| :--- | :--- | :--- | :--- | :--- |
| `SK_Naruto_Genin` | USkeletalMesh | `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin` | `UNREAL_IMPORTED` (State 4) | `ART_APPROVED` (State 6) |
| `SKEL_Naruto_Genin` | USkeleton | `/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton` | `RUNTIME_VALIDATED` (State 5) | `PRODUCTION_READY` (State 8) |
| `ABP_Naruto_Genin` | UAnimBlueprint | `/Game/Characters/Naruto/Anims/ABP_Naruto_Genin` | `RUNTIME_VALIDATED` (State 5) | `PRODUCTION_READY` (State 8) |
| `M_AnimeCel_Master` | UMaterial | `/Game/Materials/Master/M_AnimeCel_Master` | `RUNTIME_VALIDATED` (State 5) | `PRODUCTION_READY` (State 8) |
| `MI_Naruto_Skin` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Skin` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Navy` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Navy` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Hair` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Hair` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Collar` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Collar` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Tracksuit` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Emblem` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Emblem` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `MI_Naruto_Metal` | UMaterialInstance | `/Game/Characters/Naruto/Materials/MI_Naruto_Metal` | `RUNTIME_VALIDATED` (State 5) | `ART_APPROVED` (State 6) |
| `T_Naruto_Genin_Face_D` | UTexture2D | `/Game/Characters/Naruto/Textures/T_Naruto_Genin_Face_D` | `TODO_AUTHORING` (State 1) | `ART_APPROVED` (State 6) |

---

## 5. Gameplay Framework & Ability System Status

- **GAS Integration:** `AEOSCharacterBase` hosts `UEOSAbilitySystemComponent` and `UEOSAttributeSet`.
- **Input Mappings:** Full Enhanced Input mapping defined in `Config/DefaultInput.ini`:
  - `MoveForward`, `MoveRight`, `Turn`, `LookUp`
  - `Jump` (Space), `Sprint` (LeftShift), `Dodge` (LeftAlt)
  - `AttackLight` (LMB), `AttackHeavy` (RMB), `Substitution` (C)
  - `Jutsu1` / Rasengan (Q), `Jutsu2` (E), `Jutsu3` (R), `Ultimate` (G)
  - Party Switch: `[1] Naruto`, `[2] Sasuke`, `[3] Sakura`
- **HUD System:** `AEOSShinobiHUD` driving Slate/CommonUI with HP, Chakra, minimap, combo counter, and team squad cards.
- **Combat Subsystems:** `EOSAdaptiveAISubsystem` and `EOSComboManager` operational.

---

## 6. Action Items for Immediate Remediation

1. **Assign Production Skeletal Mesh in C++:** In `AEOSCharacterBase::SetupStylizedVisualMesh()`, load and bind `SK_Naruto_Genin_Rigged` (or its Blueprint subclass) to eliminate untextured proxy spawning.
2. **Author High-Resolution Anime Facial Texture:** Create `T_Naruto_Genin_Face_D` with cerulean blue pupils, specular catchlights, crisp cheek whiskers, and mouth contours.
3. **Upgrade Naruto 3D Head & Hair Mesh:** Sculpt tiered bangs, side temporal locks, radial crown spikes, and ribbed fleece collar funnel.
4. **Direct Standalone Launcher Exclusively to `L_Konoha_GoldenStreet`:** Ensure all default gameplay tests boot into the authentic village avenue.

