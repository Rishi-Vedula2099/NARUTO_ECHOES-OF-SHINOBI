# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 27: ASSET NAMING CONVENTIONS & UNREAL FOLDER MAPPING

> **Document ID:** EOS-VB-MOD-27  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Registry Mapping:** [EOSVisualAssetRegistry.csv](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Data/Visual/EOSVisualAssetRegistry.csv)

---

## 1. Asset Prefix Standards

| Unreal Asset Type | Canonical Prefix | Example |
| :--- | :--- | :--- |
| **Skeletal Mesh** | `SK_` | `SK_Naruto_Genin.uasset` |
| **Static Mesh** | `SM_` | `SM_Bld_IchirakuRamen.uasset` |
| **Skeleton Asset** | `SKEL_` | `SKEL_Naruto_Genin.uasset` |
| **Animation Blueprint**| `ABP_` | `ABP_Naruto_Genin.uasset` |
| **Animation Sequence** | `AS_` | `AS_Naruto_Run_Loop.uasset` |
| **Animation Montage** | `AM_` | `AM_Naruto_Rasengan_Cast.uasset` |
| **Master Material** | `M_` | `M_CelShading_Master.uasset` |
| **Material Instance** | `MI_` | `MI_Naruto_Genin.uasset` |
| **Niagara System** | `NS_` | `NS_Rasengan_Orb_Master.uasset` |
| **Texture 2D** | `T_` | `T_Naruto_BaseColor.uasset` |
| **Level / Map** | `L_` | `L_VisualAssetSmokeTest.umap` |

---

## 2. Directory Hierarchy Structure

```
/Game/
  ├── Characters/
  │     ├── Naruto/ (Meshes/, Anims/, Materials/, Forms/, Skins/)
  │     ├── Sasuke/
  │     ├── Sakura/
  │     ├── Kakashi/
  │     ├── Bosses/ (Zabuza/, Gaara/, Orochimaru/)
  │     ├── Enemies/ (RogueNinja/, SoundFour/, Puppets/)
  │     └── NPCs/ (KonohaVillagers/, Shopkeepers/, Guards/, Students/)
  ├── Environment/
  │     └── Konoha/ (Buildings/, Foliage/, Props/, Landmarks/, Roads/)
  ├── Materials/ (Characters/, Environment/, PostProcess/, Shaders/, VFX/)
  ├── VFX/ (Niagara/Jutsu/, Materials/, Textures/)
  └── Maps/ (VisualSmoke/, Konoha/)
```
