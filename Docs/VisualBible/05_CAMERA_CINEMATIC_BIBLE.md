# NARUTO: ECHOES OF SHINOBI
# 05 — CAMERA & CINEMATIC BIBLE
## *Authoritative Camera System Specification*

---

## Core Principle

The camera serves the **relationship between character and world.** Naruto must be visible, the world must be visible, and the player must always understand spatial context.

---

## Primary Camera: Third-Person Action RPG

### Exploration Mode
```
┌──────────────────────────────────────┐
│          SKY / MOUNTAINS             │
│                                      │
│    BUILDINGS  │  DISTANT HOKAGE      │
│               │  MONUMENT            │
│    TREES      │                      │
│               │                      │
│           NARUTO                     │
│             ↓                        │
│    ROAD / STREET                     │
│                                      │
│    NPCs      PROPS                   │
└──────────────────────────────────────┘

Character: 25-32% of screen height
Camera: Slightly elevated, behind and above
Distance: ~400-500 cm from character
Pitch: -15° to -20°
FOV: 75-80°
```

### Camera Parameters
| Parameter | Value | Notes |
|---|---|---|
| SpringArm Length | 400-500 cm | Adjustable per context |
| Camera Height Offset | +80 cm above character | Slightly elevated |
| FOV | 75-80° | Wider for exploration |
| Camera Lag | Enabled, Speed 8-12 | Soft follow |
| Camera Rotation Lag | Enabled, Speed 6-8 | Smooth turn |
| Collision Probe Size | 12 cm | Prevent clipping |
| Look-At Offset | +20 cm above character center | Focus on upper body |

---

## Camera Modes

### 1. Exploration Camera
- Free orbit around character
- Soft follow with lag
- Collision avoidance (pull in)
- Zoom controlled by player
- Indoor/outdoor transition smoothing

### 2. Combat Camera
- Tighter framing (300-350 cm)
- Slightly lower angle
- Enemy awareness (frame both combatants)
- Faster response (lower lag)
- FOV tightens slightly (70-75°)

### 3. Lock-On Camera
- Centers between player and target
- Maintains both in frame
- Auto-adjusts distance based on gap
- Smooth transition in/out

### 4. Jutsu Camera
- Brief cinematic cut during signature jutsu
- Camera emphasis on technique
- Returns to gameplay camera after
- No loss of player control during basic jutsu
- Full cinematic only for Ultimate jutsu

### 5. Boss Camera
- Wider framing to show boss scale
- Phase transition cuts
- Telegraph emphasis framing
- Arena awareness

### 6. Cinematic Camera (Motion Manga transitions)
- Full cinematic control
- Letterbox bars
- Depth of field emphasis
- Returns to gameplay smoothly

---

## Camera Rules

### DO
- Keep character and environment BOTH visible
- Use soft camera lag for natural feel
- Handle collision by pulling camera inward
- Maintain consistent framing across scenes
- Smooth all transitions (no hard cuts during gameplay)

### DO NOT
- Place camera so close the world disappears
- Place camera so far the character becomes unreadable
- Allow camera to clip through geometry
- Lock camera rotation during exploration
- Use motion blur (disabled in project)
- Create jarring cuts during normal gameplay

---

## Camera Data

```cpp
USTRUCT(BlueprintType)
struct FEOSCameraProfile
{
    GENERATED_BODY()

    UPROPERTY() FName ProfileID;           // "CAM_EXPLORATION"
    UPROPERTY() float SpringArmLength;     // 450.0
    UPROPERTY() float HeightOffset;        // 80.0
    UPROPERTY() float FOV;                 // 78.0
    UPROPERTY() float CameraLagSpeed;      // 10.0
    UPROPERTY() float RotationLagSpeed;    // 7.0
    UPROPERTY() float CollisionProbeSize;  // 12.0
    UPROPERTY() float LookAtOffset;        // 20.0
    UPROPERTY() bool bEnableCameraLag;     // true
    UPROPERTY() bool bEnableRotationLag;   // true
};
```

---

## Reference Framing

The gameplay reference image shows:
```
Naruto: ~28% screen height
Position: Center-lower third
Horizon: Upper third of screen
Architecture: Fills middle band
Sky + Mountains: Upper quarter
Street + Props: Lower quarter
```

This framing must be the default exploration camera target.
