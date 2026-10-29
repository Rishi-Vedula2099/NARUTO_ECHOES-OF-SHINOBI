# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 26: LOD BUDGETS & GTX 1650 PERFORMANCE VISUAL RULES

> **Document ID:** EOS-VB-MOD-26  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Target Hardware:** NVIDIA GeForce GTX 1650 (4GB VRAM) / 1080p 60 FPS

---

## 1. Geometric Polygon Budgets by Asset Class

| Asset Class | LOD0 (0–12m) | LOD1 (12–30m) | LOD2 (30–60m) | LOD3 (>60m) | Max Bone Count |
| :--- | :---: | :---: | :---: | :---: | :---: |
| **Playable Hero** | 45,000 tris | 22,000 tris | 10,000 tris | 4,000 tris | 96 bones |
| **Awakened Boss** | 65,000 tris | 35,000 tris | 15,000 tris | 6,000 tris | 140 bones |
| **Living Village NPC**| 18,000 tris | 9,000 tris | 4,000 tris | Impostor / Cull | 48 bones |
| **Combat Enemy Grunt**| 20,000 tris | 10,000 tris | 4,500 tris | 1,800 tris | 54 bones |
| **Modular Building** | 15,000 tris | 7,500 tris | 2,500 tris | 800 tris | Static |
| **Street Prop (Lantern)**| 2,400 tris | 1,200 tris | 400 tris | Cull | Static |
| **Cedar Tree / Sakura**| 12,000 tris | 6,000 tris | 2,000 tris | Billboard | 12 wind bones |

---

## 2. Texture Resolution & VRAM Allocation

- **Hero Character:** 1 × 2048×2048 BaseColor/Tint, 1 × 2048×2048 Normal/Mask.
- **Environment Buildings:** 1 × 2048×2048 Trim Sheet shared across multiple structures.
- **Street Props:** 1 × 1024×1024 shared texture atlas.
- **Total VRAM Target:** Under 3.2 GB total VRAM allocation during full vertical slice execution.
