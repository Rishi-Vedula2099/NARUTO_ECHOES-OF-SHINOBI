# NARUTO: ECHOES OF SHINOBI
# 11 — MOTION MANGA BIBLE
## *Authoritative Story Presentation Specification*

---

## Core Principle

Story is presented through **Motion Manga** — animated comic-panel sequences rendered in the game engine using the same visual language as gameplay. Motion Manga bridges gameplay → narrative → gameplay seamlessly.

---

## Motion Manga Flow

```
MISSION GAMEPLAY
    → BOSS ENCOUNTER
    → BOSS DEFEAT
    → HUD FADE
    → MUSIC TRANSITION
    → MOTION MANGA SEQUENCE
    → FINAL PANEL (dramatic hold)
    → REWARD SCREEN
    → CARD/INTEL ARCHIVE UPDATE
    → WORLD STATE UPDATE
    → RETURN TO GAMEPLAY
```

---

## Motion Manga Visual Rules

### Shared Identity
Motion Manga scenes MUST share:
- Same character models/identities as gameplay
- Same costume continuity (what they wore in gameplay)
- Same lighting language (warm, stylized)
- Same color language (anime palette)
- Same story-state continuity (no contradictions)

### Panel Composition
- Manga-style panel layouts (dynamic, not static grid)
- Action lines for motion emphasis
- Speed lines for impact
- Dramatic close-ups for emotion
- Wide shots for establishing scenes
- Sound effect text (stylized Japanese/English)

### Animation
- Subtle character movement within panels
- Camera pan/zoom across panels
- Panel-to-panel transitions (slide, dissolve, cut)
- Parallax depth in panels
- Wind/hair/cloth micro-animation

### Audio
- Scene-appropriate music transition
- Character voice lines (if available)
- Sound effects synced to panel reveals
- Ambient atmosphere matching scene

---

## Motion Manga Data Contract

```
MangaSceneID     : Unique identifier
ArcID            : Parent story arc
ChapterID        : Parent chapter
StageID          : Trigger stage
TriggerType      : BOSS_DEFEAT / MISSION_COMPLETE / STORY_MILESTONE
PanelCount       : Number of panels in sequence
Duration         : Total scene duration (seconds)
MusicTrackID     : Background music
VoiceLineIDs     : Array of dialogue references
CharacterIDs     : Characters appearing
CostumeIDs       : What they're wearing (continuity)
LocationID       : Scene location
TimeOfDay        : Scene lighting condition
CardUnlock       : Memory/Motion Manga card to unlock
IntelUnlock      : Intel entry to unlock
WorldStateUpdate : World state changes after scene
```

---

## Story Architecture

### Scale
| Scope | Count | Description |
|---|---|---|
| Arcs | 25 | Major story arcs (Chunin Exam, Sasuke Retrieval, etc.) |
| Chapters | 100 | Individual story chapters within arcs |
| Stages | 500 | Playable stages/missions within chapters |
| Motion Manga | ~200+ | Cinematic scenes across the story |

### Arc Structure
```
ARC
├── Chapter 1
│   ├── Stage 1 (Exploration)
│   ├── Stage 2 (Combat)
│   ├── Stage 3 (Boss)
│   └── Motion Manga (chapter conclusion)
├── Chapter 2
│   ├── ...
│   └── Motion Manga
├── Chapter 3
│   └── ...
├── Chapter 4
│   └── ...
└── Motion Manga (arc conclusion)
```

---

## Motion Manga DO NOTs

- DO NOT use pre-rendered video that doesn't match game style
- DO NOT break costume continuity between gameplay and manga
- DO NOT use character models that don't exist in gameplay
- DO NOT create manga scenes disconnected from visual language
- DO NOT skip world state updates after narrative events
- DO NOT make manga unskippable on replay (skippable after first view)
