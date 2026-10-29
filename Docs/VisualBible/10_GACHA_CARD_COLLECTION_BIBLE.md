# NARUTO: ECHOES OF SHINOBI
# 10 — GACHA, CARD & COLLECTION BIBLE
## *Authoritative Collection System Specification*

---

## Core Principle

The collection system is a deep exploration encyclopedia, not just "pull character → upgrade character." Cards, collection entries, and gacha presentations must share the same anime visual language as gameplay.

---

## Shinobi Chronicle Card System

### Card Types
| Type | Description | Example |
|---|---|---|
| CHARACTER | Character portrait card | Naruto Uzumaki — Genin |
| FORM | Transformation variant | Naruto — Sage Mode |
| JUTSU | Technique documentation | Rasengan |
| SUMMON | Summoning creature | Gamabunta |
| WEAPON | Weapon/tool card | Kubikiribōchō |
| ARTIFACT | Special item card | Scroll of Sealing |
| LORE | Story/history fragment | The Fourth Hokage's Sacrifice |
| EVENT | Event completion card | Chunin Exam — Forest of Death |
| MEMORY | Story moment card | Naruto vs. Zabuza |
| MOTION MANGA | Cinematic scene card | Team 7 Formation |
| BOSS | Boss defeat record | Kakashi — Tactician |
| TEAM | Team composition card | Team 7 |
| LOCATION | World discovery card | Ichiraku Ramen |

### Card Rarity
| Rarity | Symbol | Drop Rate | Pity |
|---|---|---|---|
| C (Common) | ★★★ | 40% | — |
| R (Rare) | ★★★★ | 30% | — |
| SR (Super Rare) | ★★★★★ | 20% | — |
| SSR (Super Super Rare) | ★★★★★★ | 8% | 50 pulls |
| UR (Ultra Rare) | ★★★★★★★ | 1.8% | 90 pulls |
| LR (Legendary Rare) | ★★★★★★★★ | 0.2% | Featured banner |

### Card Data Contract
```
CardID           : Unique identifier
CardType         : CHARACTER / FORM / JUTSU / etc.
CharacterID      : Associated character (if applicable)
FormID           : Associated form
JutsuID          : Associated jutsu
BossID           : Associated boss
LocationID       : Associated location
MemoryID         : Associated memory
Rarity           : C / R / SR / SSR / UR / LR
CollectionSetID  : Set this card belongs to
EraID            : Timeline era
ArtProfileID     : Reference to card art
FrameProfileID   : Card border/frame style per rarity
VFXProfileID     : Reveal VFX per rarity
LoreEntryID      : Lore text reference
BondEntryID      : Bond connection (if applicable)
AcquisitionSource: Gacha / Event / Story / Achievement / Shop
ReleaseEventID   : Release event reference
```

---

## Shinobi Codex (Collection Museum)

### Codex Sections
| Section | Content |
|---|---|
| Characters | All unlocked character profiles |
| Forms | Transformation variants |
| Skins | Costume variants |
| Jutsu | Learned/collected technique archive |
| Weapons | Weapon/tool database |
| Gear | Equipment database |
| Summons | Summoning creature profiles |
| Tailed Beasts | Bijū profiles |
| Bosses | Defeated boss records with Combat DNA |
| Enemies | Enemy archetype database |
| NPCs | Village character profiles |
| Villages | Village discovery records |
| Lands | Regional exploration records |
| Cards | Full card collection |
| Bonds | Relationship progression tracker |
| Lore | Story/history fragments |
| Motion Manga | Collected cinematic scenes |
| Music | Unlocked soundtrack |
| Achievements | Completion tracker |

### Museum Feature
- Inspect unlocked 3D character models in real-time engine lighting
- Rotate, zoom, view animations
- View weapons, summons, village dioramas
- Read lore entries and card descriptions

---

## Gacha Presentation

### Summoning Sequence
```
BANNER SCREEN (Character art + rate info)
    → SUMMON BUTTON
    → CHAKRA GATHERING EFFECT
    → SUMMONING CIRCLE ANIMATION
    → SMOKE / SCROLL UNROLLING
    → CARD REVEAL (back → flip → front)
    → RARITY FLASH (star burst + color)
    → CHARACTER REVEAL (3D model pose)
    → NEW CHARACTER PRESENTATION (if first time)
    → DUPLICATE CONVERSION (if already owned)
    → COLLECTION UPDATE
    → RETURN TO BANNER
```

### Gacha Rules
- Visual must match game's anime art language
- Never disconnect gacha visuals from gameplay style
- Show rates transparently
- Pity system visible to player
- Clear distinction between rarity tiers
- Celebration effects scale with rarity (not obnoxious for common)

---

## Shinobi Intel System

Collectible intelligence affecting gameplay:

| Intel Type | Content | Gameplay Impact |
|---|---|---|
| Enemy Weakness | Boss/enemy vulnerability data | Damage bonus |
| Boss Patterns | Attack sequence documentation | Predict telegraphs |
| Jutsu Intel | Technique origin and counters | Unlock counter abilities |
| Character Bonds | Relationship history | Team synergy bonus |
| Location Intel | Hidden routes, secret areas | Access new areas |
| Clan History | Clan techniques and lore | Unlock clan abilities |
| Historical Events | Shinobi world history | Lore completion |

---

## World Discovery Cards

Discover and collect:
- Villages and Districts
- Landmarks (Hokage Monument, etc.)
- Hidden Routes
- Training Grounds
- Secret Caves/Shrines
- Battlefields
- Lore Objects

Each discovery → Card → Codex Entry → World State Update.
