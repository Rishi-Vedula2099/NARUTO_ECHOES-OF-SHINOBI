# NARUTO: ECHOES OF SHINOBI
# 09 — SKIN BIBLE
## *Authoritative Skin Production Specification*

---

## Core Principle

Skins are **authored visual assets** — not recolors. Every skin shares the character's skeleton, hitbox, and GAS abilities, but has uniquely authored meshes and materials that pass the EOS Visual DNA gate.

---

## Skin Data Contract

```
SkinID           : Unique identifier
CharacterID      : Parent character
EraID            : Timeline era
FormID           : Character form (base, sage, etc.)
CostumeID        : Specific costume design
ThemeID          : Cosmetic theme
Tier             : Standard / Epic / Legendary / Mythic
AcquisitionType  : Gacha / Event / ShinobiPass / Story / Shop / Achievement
ReleasePatch     : Version when introduced
VFXProfile       : Optional VFX variant
AnimationVariant : Optional unique animations
PortraitVariant  : Unique portrait art
CollectionEntry  : Collection registry entry
CanonStatus      : Canon / Canon-Inspired / Original
```

---

## Skin Categories

| Category | Examples | Notes |
|---|---|---|
| Canon Variant | Genin, Shippuden, The Last, Boruto-era | Direct canon costumes |
| Hokage | Hokage robes, hat | Rank-based variants |
| ANBU | ANBU armor, mask, tattoo | Faction variants |
| Chunin | Chunin vest variants | Rank progression |
| Shinobi Pass | Season pass exclusive | Limited-time |
| Event | Holiday/seasonal event | Event-exclusive |
| Festival | Matsuri, fireworks festival | Cultural celebration |
| Summer | Beach/casual | Seasonal |
| Winter | Cold weather gear | Seasonal |
| Samurai | Samurai-inspired armor | Fantasy alternate |
| Ronin | Wanderer aesthetic | Fantasy alternate |
| Modern Shinobi | Contemporary clothing fusion | Fantasy alternate |
| Dark Chakra | Corrupted/curse mark | Dark variant |
| Future Shinobi | Futuristic ninja | Fantasy alternate |
| Luxury | Premium ornate design | Ultra-rare |
| Anniversary | Game anniversary special | Celebration |
| Regional/Cultural | Respectful cultural inspiration | Fantasy alternate |

---

## Skin Production Pipeline

```
1. SKIN CONCEPT ART
   ↓
2. FRONT/BACK/SIDE REFERENCE
   ↓
3. MESH AUTHORING (variant of base character)
   ↓
4. MATERIAL INSTANCES (from M_AnimeCel_Master)
   ↓
5. UNREAL IMPORT
   ↓
6. SKELETON BINDING (same skeleton as base)
   ↓
7. ANIMATION VERIFICATION (all anims work)
   ↓
8. VFX VARIANT (if applicable)
   ↓
9. PORTRAIT ART
   ↓
10. COLLECTION CARD ART
    ↓
11. GACHA BANNER ART
    ↓
12. VISUAL GATE REVIEW
    ↓
13. PRODUCTION_READY
```

---

## Skin Preservation Rules

Every skin MUST preserve:
- Character identity (recognizable as that character)
- Age continuity (Genin skin on Genin-era character)
- Silhouette readability (identifiable at gameplay camera distance)
- Face structure (anime facial proportions maintained)
- Hair identity (character's hair style, color can vary within reason)
- Combat readability (attacks, jutsu must remain clear)

---

## Skin DO NOTs

- DO NOT make skins that are unrecognizable as the character
- DO NOT rewrite canon character identity with skins
- DO NOT share meshes between different characters
- DO NOT create skins without portrait/card variants
- DO NOT release skins without passing Gate F (EOS Visual DNA)
