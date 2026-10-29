# EOS MASTER EXECUTION CONTRACT & PRODUCTION BLUEPRINT
**Project:** NARUTO: ECHOES OF SHINOBI  
**Engine:** Unreal Engine 5.8 | **Repository:** `C:/Projects/NARUTO_ECHOES-OF-SHINOBI/`  
**Primary Map:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`  
**Secondary Map (Proving Ground):** `/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest`  
**Permanent Authority:** Three-Tier Governance Model & 8-State Asset Contract (`GEMINI.md` / `AGENTS.md`)  

---

## 1. Project Identity & Supreme Art Direction
NARUTO: ECHOES OF SHINOBI is a stylized 3D anime action RPG with a painterly anime finish, controlled 2-band cel shading, warm expressive daylight, iconic shinobi silhouettes, authentic Japanese timber architecture, and fluid third-person gameplay.

### Concrete Defect Correction Directive
* **Do not solve missing visual assets with lighting, exposure, bloom, or emissive tweaks.**
* **Do not declare completion on scaffolding, mannequins, or primitive box extrusions.**
* **Never conflate technical compilation with visual completion.**

---

## 2. Character Production Pipeline (5-Component Quality Standard)

$$\text{Formula: } \text{Face Identity} + \text{Facial Anatomy} + \text{Designed Hair} + \text{Costume Construction} + \text{Deformation} + \text{Expressions} = \text{Shinobi}$$

### Component A: Face Identity (Close-Up `CAM_B` Gate)
* Head-to-body ratio ~1:5.5 (Genin), ~1:6.5 (Shippuden).
* Expressive almond-shaped anime eyes with deep black upper lash contour, pupil depth, and specular catchlights.
* Authentic cheek curvature with 3 crisp, non-blurry whisker marks on each cheek.
* Stamped metallic Konoha plate with incised leaf emblem and corner rivets sitting flush across the forehead.

### Component B: Hair Volume & Silhouette
* Multi-tiered volume clumps (front bangs, side temporal locks, radial crown spikes) catching cel shadow bands cleanly without inverted dark artifacts.
* Dynamic headband cloth ribbons trailing behind the head.

### Component C: Costume & Anatomy Construction
* Thick, ribbed shearling/fleece neck collar with modeled vertical accordion folds and central zipper track.
* Saturated cadmium orange tracksuit with navy shoulder yokes, wrist cuffs, and 3D back red Uzumaki spiral.
* Right thigh wrapped in tight white medical bandages beneath a molded black kunai pouch; navy ninja sandals with sculpted toes and thick soles.

### Component D: Facial Animation & Expressions
* Minimum 4 core facial blend shapes: `Neutral_Focus`, `Smirk_Determined`, `Combat_Yell`, `Surprise_Blink`.
* Animation Blueprint integration driving morph curves during combat and dialogue.

### Component E: Runtime Validation
* Verified under close-up camera (`CAM_B` at 95 cm) and gameplay camera (`CAM_A` at 320 cm, character = ~28% screen height).
* Zero mesh tearing, pinching, or floating feet during running, dodging, jumping, and jutsu casting.

---

## 3. World & Architectural Pipeline

```
WORLD (Continents) -> LAND (Fire, Wind, Water...) -> SETTLEMENT (Konoha, Suna...) 
-> DISTRICT (Golden Street) -> MAP (L_Konoha_GoldenStreet) -> ZONE (50m Slice) 
-> LANDMARKS (Ichiraku Ramen, Hokage Rock) -> PROPS (Lanterns, Banners) -> NPCS
```

### Konoha Golden Street Requirements
* **Ichiraku Ramen:** Open dining alcove, recessed wooden counter, 4 rounded stools, split fabric *noren* curtains ("一楽ラーメン"), glowing red paper lanterns ($120\text{ cd/m}^2$), Teuchi NPC.
* **Timber Architecture:** Visible post-and-beam cedar frames, mortise-and-tenon joints, sloped roofs with curved clay tiles (*kawara*), carved eaves brackets, sliding *shoji* screens.
* **Atmosphere & Vista:** Broad central cobblestone avenue ($X \approx \pm 680$) unobstructed framing the towering Hokage Rock cliff monument under an azure painterly sky.

---

## 4. Multi-Nation Scope (65 Official Stages & 5 Great Lands)

* **Land of Fire:** Konoha (Golden Street 50m/100m/250m/500m), Survival Training Ground (Day/Eve), Forest of Death, Chunin Exam Arenas, Tanzaku Town, Valley of the End.
* **Land of Wind:** Sunagakure (Canyon gates, adobe dome structures, wind-blown dunes, Kazekage palace).
* **Land of Water:** Kirigakure (Mist-shrouded canals, wooden docks, elevated stilt houses).
* **Land of Lightning:** Kumogakure (Spire cliff temples, suspension bridges, Falls of Truth).
* **Land of Earth:** Iwagakure (Granite rock-cut mountain fortress, stone gorges).
* **Special Realms:** Amegakure (industrial steel pipes, perpetual rain), Land of Iron (snowfield, samurai bridge), Mount Myōboku, Kamui Void, Kaguya Dimensions.

---

## 5. Performance, Hardware & Evidence Standards

* **Hardware:** NVIDIA GeForce GTX 1650 4GB VRAM.
* **Target:** $1920 \times 1080$ native @ 60 FPS (Frame time $\le 16.67\text{ ms}$, 1% low $\ge 45\text{ FPS}$, VRAM $\le 3.4\text{ GB}$).
* **Reporting Rigor:** Every task must record inspected/modified files, asset maturity states (1-8), build evidence, and in-engine runtime captures across standard camera profiles (`CAM_A` through `CAM_H`).
