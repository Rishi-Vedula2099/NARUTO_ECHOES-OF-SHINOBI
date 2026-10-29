# NARUTO: ECHOES OF SHINOBI
# 04 — MOVEMENT & ANIMATION BIBLE
## *Authoritative Shinobi Movement Specification*

---

## Core Principle

Movement must feel like a **shinobi** — not a generic third-person character. Every state must have anticipation, weight, follow-through, and recovery.

---

## Locomotion States

### Ground Movement
| State | Description | Speed | Transition |
|---|---|---|---|
| Idle | Alert shinobi stance, subtle breathing | 0 cm/s | → Walk/Run/Sprint/Combat |
| Walk | Casual exploration pace | ~200 cm/s | Idle ↔ Walk ↔ Run |
| Run (Ninja Run) | Arms back, forward lean, classic Naruto run | ~500 cm/s | Walk ↔ Run ↔ Sprint |
| Sprint | Maximum speed ninja dash | ~750 cm/s | Run ↔ Sprint |
| Strafe | Side-step during combat lock-on | ~300 cm/s | Combat state |
| Backpedal | Reverse while facing target | ~200 cm/s | Combat state |

### Aerial Movement
| State | Description |
|---|---|
| Jump | Single jump, rising arc |
| Double Jump (later) | Chakra-assisted second jump |
| Fall | Gravity descent, arms adjust |
| Air Recovery | Mid-air stabilization after hit |
| Landing | Knee bend absorption, recovery frames |

### Combat Movement
| State | Description |
|---|---|
| Combat Idle | Ready stance, guard position |
| Combat Dodge | Quick evasion step (iframes) |
| Substitution | Log replacement jutsu |
| Combat Repositioning | Quick dash to optimal range |
| Jutsu Movement | Special jutsu-specific movement |
| Ultimate Movement | Cinematic rush/teleport |

### Advanced Traversal (Later Phases)
| State | Description |
|---|---|
| Wall Run | Chakra-assisted vertical run |
| Rooftop Leap | Building-to-building traversal |
| Ledge Grab | Edge interaction |
| Water Walk | Chakra surface tension |
| Tree Branch | Forest canopy movement |

---

## Movement Feel Requirements

```
ANTICIPATION
    ↓
ACCELERATION
    ↓
SUSTAINED SPEED
    ↓
DECELERATION
    ↓
RECOVERY/IDLE

Each transition MUST have:
- Blend time (not instant snapping)
- Directional responsiveness
- Foot IK grounding
- No floating/sliding
```

---

## Animation Requirements per State

| Animation | Loops | Root Motion | Priority |
|---|---|---|---|
| Idle | Yes | No | Phase 4 |
| Walk Forward | Yes | Yes | Phase 4 |
| Walk Backward | Yes | Yes | Phase 4 |
| Walk Left/Right | Yes | Yes | Phase 4 |
| Ninja Run | Yes | Yes | Phase 4 |
| Sprint | Yes | Yes | Phase 4 |
| Turn 90°/180° | No | Yes | Phase 4 |
| Jump Start | No | Yes | Phase 4 |
| Jump Loop (Apex) | Yes | No | Phase 4 |
| Fall | Yes | No | Phase 4 |
| Land Light | No | No | Phase 4 |
| Land Heavy | No | No | Phase 4 |
| Dodge Forward | No | Yes | Phase 4 |
| Dodge Back | No | Yes | Phase 4 |
| Dodge Left | No | Yes | Phase 4 |
| Dodge Right | No | Yes | Phase 4 |
| Combat Idle | Yes | No | Phase 8 |
| Basic Attack 1 | No | Root Motion | Phase 8 |
| Basic Attack 2 | No | Root Motion | Phase 8 |
| Basic Attack 3 | No | Root Motion | Phase 8 |
| Combo Finisher | No | Root Motion | Phase 8 |
| Hit Reaction Front | No | No | Phase 8 |
| Hit Reaction Back | No | No | Phase 8 |
| Knockdown | No | Root Motion | Phase 8 |
| Get Up | No | Root Motion | Phase 8 |
| Guard/Block | Yes | No | Phase 8 |
| Substitution | No | Root Motion | Phase 8 |
| Rasengan Charge | No | No | Phase 8 |
| Rasengan Release | No | Root Motion | Phase 8 |
| Victory Pose | No | No | Phase 11+ |
| Defeat | No | No | Phase 11+ |

---

## Animation Blueprint Architecture

```
AnimBP_Naruto_Genin
├── Locomotion State Machine
│   ├── Idle
│   ├── Walk (Blend Space 2D: direction × speed)
│   ├── Run (Ninja Run anim)
│   ├── Sprint
│   └── Turn In Place
├── Jump/Fall State Machine
│   ├── Jump Start
│   ├── Jump Loop
│   ├── Fall
│   └── Land
├── Combat State Machine
│   ├── Combat Idle
│   ├── Attack Chain (Montage-driven)
│   ├── Hit Reactions (Montage-driven)
│   ├── Guard
│   └── Dodge
├── Jutsu Layer (Montage Override)
│   ├── Rasengan
│   ├── Shadow Clone
│   └── Ultimate
├── Upper Body Layer (Additive)
│   ├── Hand Signs
│   ├── Chakra Charging
│   └── Item Interaction
└── IK Layer
    ├── Foot IK (ground conformity)
    ├── Hand IK (weapon holding)
    └── Look At IK (head tracking)
```

---

## DO NOTs

- DO NOT use default UE mannequin animations
- DO NOT use generic shooter locomotion
- DO NOT skip anticipation/recovery frames
- DO NOT have characters slide along the ground (foot IK required)
- DO NOT have instant 180° direction changes (must turn naturally)
- DO NOT blend directly from sprint to idle (deceleration required)
