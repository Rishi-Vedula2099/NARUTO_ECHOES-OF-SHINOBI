# GLOBAL VISUAL PROFILE SPECIFICATION
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Architecture:** Data-Driven Decoupled Visual Profiles  

---

## 1. System Architecture

To prevent global rendering pollution, all visual systems are decomposed into modular, data-driven profiles. Unrelated systems must never touch another system's parameters.

```
VISUAL PROFILE SUITE
├── Character Visual Profile (CVP)
├── Environment Profile (EP)
├── Presentation Profile (PP)
├── Camera Profile (CP)
├── VFX Profile (VP)
└── UI Profile (UP)
```

---

## 2. Character Visual Profile (CVP) Family

Each character incarnation references a distinct CVP that drives cel-shading thresholds, rim highlights, subsurface tint, and dynamic morph triggers.

| Profile ID | Target Archetype | Cel Shadow Tint | Shadow Threshold | Specular / Rim Response | Special Dynamics |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `CVP_Shinobi_Standard` | Genin/Chunin/Jonin normal forms | Warm Peach/Navy (#1a1f36) | 0.45 / 0.75 | Subtle Fresnel rim highlight (1.5px) | Eye blink, facial smirk/yell |
| `CVP_Mode_Sage` | Naruto/Jiraiya/Kabuto Sage | Warm Terracotta (#3d1c14) | 0.50 / 0.80 | Golden amber rim | Orange eye pigmentation, toad iris |
| `CVP_Jinchuriki_Chakra` | Nine-Tails Cloak / KCM | Emissive Golden Yellow (#ffb700) | 0.20 / 0.90 | Intense flickering chakra rim ($12\text{ cd/m}^2$) | Flowing chakra shroud particles |
| `CVP_Edo_Tensei` | Reanimated Shinobi | Ash Charcoal (#232328) | 0.55 / 0.85 | Cold desaturated rim | Sclera blackening, facial cracked fissures |
| `CVP_Susanoo_Avatar` | Sasuke/Madara/Itachi Susanoo | Deep Translucent Violet/Cyan | Stepped Transparent | Outer ethereal flame distortion | Spectral bone armor geometry |
| `CVP_Otsutsuki_Divine` | Kaguya/Isshiki/Hagoromo | Pearlescent White (#e8eaed) | 0.60 / 0.90 | Horn sheen, Byakugan vein bulging | Horn geometry, Rinnegan pupil shader |
| `CVP_NPC_Civilian` | Konoha villagers/merchants | Soft Neutral Brown (#2a2622) | 0.40 / 0.70 | Muted matte cloth | LOD-culled facial bones |

---

## 3. Environment Profile (EP) Family

Drives Sun Lux, SkyLight color, Atmospheric fog density, post-process color grading, and wind vector.

| Profile ID | Region / Condition | Sun Lux | Sun Temp | SkyLight cd/m² | Sky Color | Fog Distance / Tone |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| `EP_Konoha_Daylight` | Konoha Village (Noon/Aft) | 12.0 Lux | 5400 K | 3.0 cd/m² | Azure Blue (#4a88cc) | Start: 8000, Warm Orange Haze |
| `EP_Konoha_Sunset` | Konoha Village (Golden Hour) | 8.5 Lux | 3800 K | 2.5 cd/m² | Deep Amber (#cc6633) | Start: 4000, Violet Crimson |
| `EP_Konoha_Night` | Konoha Village (Night) | 1.8 Lux | 7500 K | 0.8 cd/m² | Indigo Navy (#0d1526) | Soft moon glow, glowing lanterns ($120\text{ cd/m}^2$) |
| `EP_Forest_Death` | Forest of Death | 4.2 Lux | 5200 K | 1.2 cd/m² | Moss Teal (#1f3d32) | Thick volumetric green canopy haze |
| `EP_Suna_Desert` | Sand Village / Dunes | 16.0 Lux | 5800 K | 4.0 cd/m² | Pale Ochre (#d9b382) | Heat distortion shimmer, blowing dust |
| `EP_Valley_Rain` | Valley of the End (Storm) | 3.5 Lux | 6500 K | 1.5 cd/m² | Slate Grey (#3b4554) | Heavy rain particles, wet specular surfaces |
| `EP_Iron_Snow` | Land of Iron (Snowfield) | 14.0 Lux | 6200 K | 4.5 cd/m² | Ice Silver (#b8d5e5) | High ground albedo fill, drifting blizzard |
| `EP_Dimension_Kamui` | Kamui Pocket Realm | 5.0 Lux | 6000 K | 2.0 cd/m² | Monochrome Ash (#555555) | Floating stone blocks, vignette falloff |

---

## 4. Camera Profile (CP) Family

Strict separation between art review framing and gameplay third-person cameras.

| Profile ID | Target Context | Boom Length | Pitch / Yaw | Socket Offset (X, Y, Z) | FOV |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `CP_ArtReview_Hero` (`CAM_A`) | Reference Comparison | 320 cm | -5.0° / 0.0° | (0.0, 0.0, 15.0) | 48° (Character = 28% screen height) |
| `CP_ArtReview_Face` (`CAM_B`) | Close-Up Face Audit | 95 cm | -2.0° / 0.0° | (0.0, 5.0, 135.0) | 32° (Eye, brow, whisker inspection) |
| `CP_Gameplay_FreeRoam` | Village & World Traversal | 380 cm | -12.0° / Free | (0.0, 35.0, 45.0) | 68° (Smooth collision spring arm) |
| `CP_Gameplay_Combat` | Active Battle / Lock-On | 460 cm | -15.0° / Target | (0.0, 40.0, 50.0) | 75° (Wide framing for multi-enemy reads) |
| `CP_Cinematic_Jutsu` | Rasengan / Secret Technique | Dynamic | Keyframed | Impact zoom & Dutch angle roll | 40° - 80° dynamic easing |

---

## 5. Presentation Contexts (PP)

1. **`PP_FreeRoam`:** Clean exploration HUD, compass minimap, third-person movement, ambient crowd soundscapes.
2. **`PP_Combat`:** Lock-on reticle, chakra gauge pulse, combo hit counter, dynamic combat impact frames, slowdown on final hit.
3. **`PP_Dialogue`:** Letterbox cinestrip bars, character nameplate, voice waveform portrait, camera auto-tracking speaking NPC.
4. **`PP_MotionManga`:** 2.5D comic panel borders, authentic manga screentones, Japanese onomatopoeia sound effect typography.
5. **`PP_Collection_Gacha`:** 360° showroom turntable, golden chakra summon animation, rarity card burst sequence.
