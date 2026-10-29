# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 20: UI VISUAL LANGUAGE & HUD PRESENTATION SYSTEM

> **Document ID:** EOS-VB-MOD-20  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **C++ Linkage:** [EOSShinobiHUD.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/UI/EOSShinobiHUD.h)

---

## 1. Traditional Japanese Shinobi Aesthetic Design Language

```
+-----------------------------------------------------------------------------------+
|                           UI VISUAL DESIGN PILLARS                                |
+-----------------------------------------------------------------------------------+
| 1. Washi Rice Paper & Ink Brushwork | Gauges and panels use textured washi paper  |
|                                     | backgrounds with sumi-e ink brush borders.  |
| 2. Dynamic Curved Status Gauges     | High-visibility curved health (green/vermilion)
|                                     | and chakra (cyan/amber) bars anchored top-L.|
| 3. Diamond Jutsu Diamond Cluster    | 4-slot diamond layout (Bottom-Right)        |
|                                     | with glowing chakra cooldown rims.          |
| 4. Circular Shinobi Minimap         | Compass ring with Leaf insignia North needle|
|                                     | and radar blips for enemies and NPCs.       |
| 5. High-Impact Floating Damage      | Stylized comic onomatopoeia font with       |
|                                     | elemental color coding (Fire=Red, Chidori=Cyan).
+-----------------------------------------------------------------------------------+
```

---

## 2. HUD Element Layout & Data Binding

```
+-----------------------------------------------------------------------------------+
| [PORTRAIT]   (HP BAR: ========================> 100/100)                          |
|  [NARUTO]    (CHAKRA: ===================> 85/100)                                |
|  [STANCE]    (STAMINA: =============> 60/60)                                      |
|                                                                                   |
|                                                      [MINIMAP]                    |
|                                                      (North / Konoha St)          |
|                                                                                   |
|                                                                                   |
|                                                                                   |
| [SQUAD SWAP]                                        [JUTSU CLUSTER]               |
| [1: Sasuke]                                            [ Q: Rasengan ]            |
| [2: Sakura]                                        [E: Clone]   [R: Sub]          |
|                                                        [ F: Ultimate ]            |
+-----------------------------------------------------------------------------------+
```

### Authoritative GAS Binding
- The HUD is **strictly presentation-only**. It reads directly from [`UEOSAttributeSet`](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Ability/EOSAttributeSet.h) delegates.
- Never creates duplicate local HP or Chakra variables.
- When `GetHealth()` changes, the curved green gauge updates via smooth linear interpolation (lerp speed: 12.0) with an underlying white damage-trail bar.
