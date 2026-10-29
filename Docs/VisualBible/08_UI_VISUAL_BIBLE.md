# NARUTO: ECHOES OF SHINOBI
# 08 — UI VISUAL BIBLE
## *Authoritative UI/HUD Specification*

---

## Core Principle

UI must be **anime, clean, readable, consistent.** It must visually belong to the same stylized world as the gameplay. Developer/debug UI must NEVER be mistaken for production UI.

---

## HUD Layout

```
┌──────────────────────────────────────────────────────────┐
│ ┌──────────────┐                          ┌──────────┐  │
│ │ [PORTRAIT]   │ LV.12                    │ MINIMAP  │  │
│ │  Naruto      │ ████████████ HP          │    ●N    │  │
│ │              │ ██████████   CHAKRA       │   ╱ ╲   │  │
│ └──────────────┘                          └──────────┘  │
│                                                          │
│                                                          │
│                    GAMEPLAY WORLD                         │
│                                                          │
│                                                          │
│                                           ┌────────────┐ │
│                                           │ MISSION:   │ │
│                                           │ Go to the  │ │
│                                           │ Hokage     │ │
│                                           │ Tower      │ │
│                                           └────────────┘ │
│                                                          │
│ ┌────┐ ┌────┐ ┌────┐                                    │
│ │NAR │ │SAS │ │SAK │   Party                             │
│ └────┘ └────┘ └────┘                                     │
│                                                          │
│         [Q]    [E]    [R]    [G]                          │
│        JUTSU1 JUTSU2 JUTSU3 ULTIMATE                     │
└──────────────────────────────────────────────────────────┘
```

---

## HUD Elements

### Top-Left: Character Status
| Element | Description |
|---|---|
| Portrait | Anime-style character face (current character) |
| Level | Numeric level display |
| HP Bar | Health bar with gradient fill (green → yellow → red) |
| Chakra Bar | Chakra bar with blue gradient fill |
| Status Icons | Buff/debuff indicators (small, readable) |

### Top-Right: Minimap
| Element | Description |
|---|---|
| Map Circle | Transparent overlay showing area |
| Player Marker | Arrow indicating direction |
| NPC Markers | Dots for nearby NPCs |
| Objective Marker | Highlighted destination |
| Compass | N/S/E/W cardinal directions |

### Right Side: Mission
| Element | Description |
|---|---|
| Mission Title | Current active mission name |
| Objective Text | Current step description |
| Progress | Completion indicator if applicable |

### Bottom-Left: Party
| Element | Description |
|---|---|
| Character Icons | Small portraits of party members |
| Active Indicator | Highlight on current character |
| HP Mini-bars | Small HP indicators per member |
| Switch Prompt | Input hint for character switching |

### Bottom-Center: Abilities
| Element | Description |
|---|---|
| Jutsu Slots | 3 jutsu + 1 ultimate |
| Key Bindings | Input prompts (Q, E, R, G or controller) |
| Cooldown Overlay | Sweep/darken when on cooldown |
| Chakra Cost | Small number showing cost |

### Combat-Specific (Contextual)
| Element | Description |
|---|---|
| Boss Health Bar | Wide bar at top of screen with name |
| Combo Counter | "HIT x12" display with escalation |
| Damage Numbers | Stylized floating numbers |
| Lock-On Reticle | Soft circle around locked target |
| Interaction Prompt | "Press F to interact" style prompts |

---

## UI Visual Style

### Colors
| Purpose | Color | Hex |
|---|---|---|
| HP Full | Vibrant Green | #44CC44 |
| HP Mid | Warm Yellow | #CCCC22 |
| HP Low | Alert Red | #CC3333 |
| Chakra | Anime Blue | #4488FF |
| XP | Warm Gold | #FFAA33 |
| Background Panel | Dark Translucent | #1A1A2E88 |
| Border | Soft Gold | #C8A96444 |
| Text Primary | White | #FFFFFFEE |
| Text Secondary | Light Gray | #CCCCCCAA |

### Typography
- Headers: Bold, slightly stylized anime font
- Body: Clean sans-serif, readable at distance
- Numbers: Monospace or tabular for alignment
- Damage: Bold, slightly exaggerated

### Visual Treatment
- Translucent dark panels (glassmorphism-lite)
- Soft gold/warm borders
- No hard rectangles (rounded corners, subtle gradients)
- Consistent opacity across all elements
- Smooth fade-in/fade-out transitions
- No jarring pop-in

---

## UI Animation

| Trigger | Animation |
|---|---|
| HP Change | Bar smoothly slides to new value |
| Chakra Use | Bar drains with trailing glow |
| Damage Number | Float up, scale up, fade out |
| Combo Counter | Punch scale on increment, fade after timeout |
| Jutsu Cooldown | Clockwise sweep overlay |
| Level Up | Full-screen flash + banner |
| Mission Update | Slide in from right, hold, slide out |
| Boss Phase Change | Health bar pulses, name text changes |

---

## UI DO NOTs

- DO NOT use debug text as production UI
- DO NOT use default UMG styling
- DO NOT use white rectangles on black backgrounds
- DO NOT show developer-facing data (object names, FPS counter in production)
- DO NOT use inconsistent fonts across UI elements
- DO NOT overlap UI elements with gameplay-critical space
- DO NOT make UI so large it obscures the game world
- DO NOT use photorealistic UI elements in a stylized anime game
