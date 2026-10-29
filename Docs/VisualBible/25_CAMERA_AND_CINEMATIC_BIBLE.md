# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 25: CAMERA STATE MACHINE & CINEMATIC ACTION BIBLE

> **Document ID:** EOS-VB-MOD-25  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **C++ Linkage:** [EOSCharacterBase.cpp](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Private/Character/EOSCharacterBase.cpp#L130-L150)

---

## 1. Dynamic Camera State Machine Profiles

The third-person camera is completely data-driven via `FEOSCameraProfile`, supporting 8 distinct operational modes:

| Camera Mode | Target Arm Length | FOV | Camera Lag Speed | Pitch Offset | Visual Focus |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **Exploration** | 420 cm | 80° | 10.0 | -12° | Wide environmental visibility, scenic vistas |
| **Combat** | 360 cm | 75° | 16.0 | -8° | Tight character framing, immediate combo read |
| **Lock-On** | 380 cm | 70° | 22.0 | -6° | Dual-target framing (Hero + Locked Enemy) |
| **Boss Arena** | 600 cm | 85° | 8.0 | -18° | High elevated overview of giant boss & arena |
| **Jutsu Dynamic**| 300 cm | 65° | 25.0 | -4° | Dynamic zoom into hand seals and jutsu release|
| **Ultimate** | Scripted Cine | 50° | 0.0 (Snap) | Dynamic | Cinematic anime cutscene camera framing |
| **Dialogue** | 180 cm | 55° | 12.0 | 0° | Over-the-shoulder conversational framing |
| **Motion Manga**| 500 cm | 60° | 5.0 | 0° | 2.5D orthographic parallax panel framing |

---

## 2. Impact Shake & Field of View Kicks

- **Light Attack Hit:** Micro-shake (Pitch: 0.8°, Yaw: 0.4°, Duration: 0.12s).
- **Heavy Finisher / Rasengan Impact:** Radial impulse shake (Pitch: 3.5°, Yaw: 2.0°, Duration: 0.35s) with a temporary 4-degree FOV expansion kick recovering over 0.25 seconds.
