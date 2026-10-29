# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 02: UNIVERSAL CHARACTER VISUAL SYSTEM & ACTOR CONTRACT

> **Document ID:** EOS-VB-MOD-02  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **C++ Linkage:** [EOSPresentationDataTypes.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Data/EOSPresentationDataTypes.h) | [EOSCharacterBase.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Character/EOSCharacterBase.h)

---

## 1. Universal Visual Actor Contract

Every character actor in *ECHOES OF SHINOBI* (playable shinobi, awakened forms, bosses, enemy archetypes, living NPCs) implements the universal visual contract:

```
+-----------------------------------------------------------------------------------+
|                        UNIVERSAL VISUAL ACTOR CONTRACT                            |
+-----------------------------------------------------------------------------------+
| 1. ActorID & CharacterID          | Identifies gameplay pawn and role             |
| 2. Faction, Village & EraID       | Defines era continuity and visual motifs      |
| 3. FormID, CostumeID & SkinID     | Determines geometry variations and cosmetics  |
| 4. Silhouette & Proportions       | 1:6.5 to 1:7.5 anime heroic proportion rule   |
| 5. Skeletal Mesh Soft Reference   | TSoftObjectPtr<USkeletalMesh>                 |
| 6. Humanoid Skeleton Reference    | TSoftObjectPtr<USkeleton>                     |
| 7. Animation Blueprint Reference  | TSoftClassPtr<UAnimInstance>                  |
| 8. Material Profile Instances     | Skin, Hair, Fabric, Metal, Eye Instances      |
| 9. VFX Niagara Profiles           | Chakra aura, signature jutsu particle links   |
| 10. Collision Profile             | Standard Pawn Capsule (R=42.0, H=96.0)        |
| 11. Portrait & Turnaround Asset   | Orthographic references & 2D HUD portrait     |
| 12. Validation Status             | AVAILABLE, MISSING_PRODUCTION_ASSET, READY    |
+-----------------------------------------------------------------------------------+
```

---

## 2. Anatomical Proportions & Silhouette Language

### 2.1 Proportion Guidelines by Shinobi Era
- **Part 1 Genin Era (Ages 12–13):**
  - Head-to-Body Ratio: 1:6.2 to 1:6.5.
  - Height Range: 145 cm – 153 cm.
  - Torso/Limb Balance: Youthful athletic build, slightly oversized hands and ninja sandals for stylized readability.
  - Face Shape: Softer jawline, larger expressive anime eyes with clean pupil definition.
- **Part 2 Shippuden Era (Ages 15–17):**
  - Head-to-Body Ratio: 1:7.0 to 1:7.3.
  - Height Range: 165 cm – 175 cm.
  - Torso/Limb Balance: Defined muscular tone, broader shoulders, longer legs for martial arts reach.
  - Face Shape: Sharper chin, narrowed combat-focused eyes, subtle cheek contouring.
- **Adult / Legendary Sannin Era:**
  - Head-to-Body Ratio: 1:7.5 to 1:8.0.
  - Height Range: 178 cm – 195 cm (e.g., Jiraiya, Zabuza).
  - Silhouette: Massive visual footprint, heavy coats, broad chest, imposing stance.

### 2.2 Silhouette Separation & Readability
- Every character's outline must be instantly recognizable in pure black silhouette against a bright sky or burning explosion.
- **Naruto:** Spiky crown hair silhouette, headband knot ribbons fluttering at nape, boxy collar, knee-length pant cuffs.
- **Sasuke:** High wing-collared dark shirt, dual-pronged side bangs, rear spiked hair volume, waist arm-guard wraps.
- **Sakura:** Sleek sleeveless qipao dress silhouette, circular forehead protector worn as a headband, knee-length biker shorts.
- **Kakashi:** Upright silver spike hairstyle defying gravity, tilted forehead protector covering left eye, high face mask, Jonin flak jacket shoulder epaulets.

---

## 3. Modular Mesh Assembly Architecture in C++

```
                                  [ SKELETAL ROOT ]
                                          |
                                          v
                               [ SK_Body_Torso_Legs ]
                                  (Master Skeleton)
                                          |
        +------------------+--------------+---------------+------------------+
        |                  |              |               |                  |
        v                  v              v               v                  v
  [ Head / Face ]      [ Hair Mesh ]  [ Headband ]  [ Weapon Socket ]  [ Form Shroud ]
  (Socket: head)       (Socket: head) (Socket: head) (Socket: hand_r)  (Socket: spine_02)
```

### Authoritative Binding Law:
C++ does not sculpt or generate polygons. C++ performs:
1. `SetSkeletalMeshAsset()` on the master skeletal mesh component.
2. Dynamic attachment of modular accessories to pre-rigged skeletal sockets (`head`, `hand_r`, `hand_l`, `spine_02`, `foot_r`).
3. Instantiation and assignment of dynamic material instances (`CreateAndSetMaterialInstanceDynamic`).
4. Parameter binding (`ChakraIntensity`, `EyePigmentation`, `OutlineColor`).
5. Logging of missing production assets if soft pointers fail to resolve.

---

## 4. Age & Continuity Guardrail Policy

> [!IMPORTANT]
> **Strict Underage Character Protection Rule:**
> - Underage characters (e.g., Genin era Naruto, Sasuke, Sakura, Hinata, Rock Lee) must **NEVER** be sexualized in concept art, 3D meshes, textures, or cosmetic skins.
> - All cosmetic skins designed for younger eras must follow athletic, traditional shinobi, festive holiday (yukata/happi), winter survival, or ceremonial themes.
> - Adult glamour or fashion skins are strictly restricted to canonical adult character eras (e.g., The Last, Adult/Hokage eras) and must maintain tasteful aesthetic standards.
