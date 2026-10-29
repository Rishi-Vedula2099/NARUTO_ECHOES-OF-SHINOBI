# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 22: ANIMATION REFERENCE BIBLE & LOCOMOTION STATE MACHINE

> **Document ID:** EOS-VB-MOD-22  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Animation Blueprint:** `ABP_Shinobi_Locomotion`

---

## 1. Locomotion State Machine Architecture

```
                                  [ ENTRY ]
                                      |
                                      v
                             +-----------------+
              +------------> |      IDLE       | <------------+
              |              +-----------------+              |
              |                 |           ^                 |
              |                 v           |                 |
              |              +-----------------+              |
              |              |   WALK / RUN    |              |
              |              | (BlendSpace 2D) |              |
              |              +-----------------+              |
              |                 |           ^                 |
              |                 v           |                 |
              |              +-----------------+              |
              |              |     SPRINT      |              |
              |              |  (Ninja Run)    |              |
              |              +-----------------+              |
              |                       |                       |
              v                       v                       v
      +---------------+       +---------------+       +---------------+
      |  JUMP START   | ----> | FALLING LOOP  | ----> | LAND RECOVERY |
      +---------------+       +---------------+       +---------------+
```

---

## 2. Locomotion Speed Profiles & Key Animation Timings

| Movement State | Speed Range | Forward Spine Lean | Arm Positioning | Footstep Audio Frequency |
| :--- | :---: | :---: | :--- | :---: |
| **Idle Stance** | 0 cm/s | 0° (Upright) | Relaxed fists near waist, slight rhythmic breath | — |
| **Walk** | 1–250 cm/s | 6° forward | Natural arm swing, purposeful stride | 1.8 steps/sec |
| **Run** | 251–600 cm/s | 18° forward | Martial arts pumping arms, high knee lift | 3.2 steps/sec |
| **Sprint (Ninja Run)**| 601–950 cm/s| 32° forward | Both arms swept straight back behind torso | 4.8 steps/sec |
| **Jump Apex** | Vertical impulse | Curved upward | Knees tucked up, ready for recovery roll | In-air wind |
| **Landing Recovery** | On ground touch | Deep knee bend | Hands touch ground briefly; instant cancel to run | Ground thump |

---

## 3. Combat Combo Timing & Hit-Stop Windows

```
+-----------------------------------------------------------------------------------+
|                        LIGHT ATTACK COMBO CHAIN (LMB × 3)                         |
+-----------------------------------------------------------------------------------+
| Hit 1: Left Jab           | Startup: 4 frames (66ms)   | Active: 2 frames         |
|                           | Recovery: 8 frames         | Cancel Window: Frame 6-12|
| Hit 2: Right Roundhouse   | Startup: 6 frames (100ms)  | Active: 3 frames         |
|                           | Recovery: 10 frames        | Cancel Window: Frame 8-15|
| Hit 3: Rising Uppercut    | Startup: 8 frames (133ms)  | Active: 4 frames         |
|                           | Hit-Stop: 3 frames (50ms)  | Launches target vertical |
+-----------------------------------------------------------------------------------+
```

### Hit-Stop Implementation:
Upon confirmed melee impact with an enemy pawn, both the attacking shinobi and target pawn experience a global dilation freeze (time dilation: 0.05 for 0.06 seconds) to deliver visceral anime martial arts impact weight.
