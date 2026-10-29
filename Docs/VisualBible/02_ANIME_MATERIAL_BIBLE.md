# NARUTO: ECHOES OF SHINOBI
# 02 — ANIME MATERIAL BIBLE
## *Authoritative Material System Specification*

---

## Core Principle

One unified anime material framework for the entire project. Do NOT create random material graphs per character or per world.

---

## Master Material

**Asset:** `M_AnimeCel_Master` (`/Game/Materials/Master/M_AnimeCel_Master`)

### Required Material Characteristics
- Controlled 2-3 band cel shading
- Soft gradient transition between bands (not hard-step)
- Anime shadow color (warm shadows, never pure black)
- Skin sub-surface approximation (warm undertone)
- Hair anisotropic highlight band
- Cloth soft diffuse with fold shadow
- Metal controlled specular + rim
- Wood painterly grain texture
- Stone weathered painterly surface
- Controlled Fresnel/rim lighting
- Controlled specular highlights
- Controlled emissive (for jutsu/chakra only)
- Optional outline solution (inverted hull or post-process)

---

## Required Material Instances

| Material Instance | Purpose | Location |
|---|---|---|
| `MI_AnimeSkin` | Character skin shading | `/Game/Materials/Characters/` |
| `MI_AnimeHair` | Character hair shading (anisotropic) | `/Game/Materials/Characters/` |
| `MI_AnimeCloth` | Clothing, fabric, bandages | `/Game/Materials/Characters/` |
| `MI_AnimeMetal` | Headbands, kunai, armor plates | `/Game/Materials/Characters/` |
| `MI_AnimeWood` | Building timber, shop walls, poles | `/Game/Materials/Environment/` |
| `MI_AnimeStone` | Cobblestone, walls, monument | `/Game/Materials/Environment/` |
| `MI_AnimeRoof` | Clay/ceramic roof tiles | `/Game/Materials/Environment/` |
| `MI_AnimeFoliage` | Trees, shrubs, grass | `/Game/Materials/Environment/` |
| `MI_AnimeGround` | Dirt paths, roads, sidewalks | `/Game/Materials/Environment/` |
| `MI_AnimeJutsu` | Chakra effects, jutsu VFX | `/Game/Materials/VFX/` |
| `MI_AnimeOutline` | Character/object outline | `/Game/Materials/PostProcess/` |
| `MI_AnimeWater` | Rivers, ponds, rain | `/Game/Materials/Environment/` |

---

## Material Parameters (Exposed)

### Common Parameters (All Instances)
```
BaseColor               : Texture2D
ShadowColor             : LinearColor (default: warm purple-brown)
ShadowBias              : Float (0.0 - 1.0, controls cel transition)
ShadowSoftness          : Float (0.0 - 1.0, gradient vs hard-step)
RimLightIntensity       : Float (0.0 - 2.0)
RimLightColor           : LinearColor (default: warm white)
RimLightPower           : Float (1.0 - 8.0)
EmissiveIntensity       : Float (0.0 - 5.0, default 0)
EmissiveColor           : LinearColor
SpecularIntensity       : Float (0.0 - 1.0)
Roughness               : Float (0.0 - 1.0)
NormalMap               : Texture2D (optional)
```

### Skin-Specific
```
SubsurfaceWarmth        : Float (0.0 - 1.0)
SkinToneMultiplier      : LinearColor
```

### Hair-Specific
```
AnisotropicDirection    : Float
AnisotropicIntensity    : Float
HairHighlightColor      : LinearColor
```

### Foliage-Specific
```
WindStrength            : Float
SubsurfaceScattering    : Float
LeafTranslucency       : Float
```

---

## Lighting Response Requirements

Materials must remain readable under ALL conditions:

| Condition | Response |
|---|---|
| DAY (warm golden sun) | Full color, clear shadows, bright |
| SUNSET (warm orange) | Warmer tones, longer shadows |
| NIGHT (cool blue moonlight) | Desaturated but readable, no pitch black |
| RAIN (diffuse overcast) | Wet specular, muted colors, still readable |
| FOG (atmospheric) | Depth fade, not washed out |
| COMBAT (dynamic lights) | Hit flashes don't blow out materials |
| JUTSU (chakra glow) | Emissive doesn't overpower base colors |
| BOSS ARENA (dramatic) | Enhanced contrast, still readable |

---

## Material DO NOTs

- DO NOT use default UE4/5 lit material as production
- DO NOT use checkerboard texture as final material
- DO NOT use pure white (1,1,1) as base color
- DO NOT use pure black (0,0,0) as shadow color
- DO NOT use emissive > 5.0 on character materials
- DO NOT use metalness = 1.0 on organic surfaces
- DO NOT use roughness = 0.0 on any surface
- DO NOT create per-character material graphs (use instances)
- DO NOT compensate for missing textures with emissive
