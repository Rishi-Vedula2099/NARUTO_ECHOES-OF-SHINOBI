# GLOBAL VISUAL ACCEPTANCE STANDARD
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Verification Protocol:** Dual-Camera Inspection & Quantitative Acceptance  

---

## 1. The 5-Component Character Quality Gate (CyberConnect2 Benchmark)

A character model cannot pass Gate 2 based on full-body distance shots alone. It must pass the **Close-Up Face Approval Gate** before full-body locomotion is accepted.

### Component A: Face Identity (Close-Up `CAM_B` Gate)
* [ ] **Skull Anatomy:** Authentic head-to-body proportions (~1:5.5 for Genin, ~1:6.5 for Shippuden). Defined jawline, chin curvature, and subtle cheekbone planes.
* [ ] **Eye Rendering:** Sharp almond anime eyes; deep black upper lash line with corner flick; vibrant iris color with pupil depth and specular catchlight reflection; zero texture stretching or blur.
* [ ] **Whisker Marks:** 3 distinct, slightly curved whisker lines per cheek conforming to the cheekbone curve; vector-clean resolution at $50\text{ cm}$ camera distance.
* [ ] **Nose & Mouth:** Sharp, delicate anime nose bridge with subtle underside shadow; distinct mouth fissure with defined lower lip contour.
* [ ] **Headband Integration:** Stamped metallic plate with embossed Konoha leaf emblem and 4 corner rivets. Cloth wrap sits flush against forehead without mesh penetration.

### Component B: Hair Volume & Silhouette
* [ ] **Layered Clumps:** Hair modeled in tiered, dimensional volume clumps (front bangs, temporal locks, radial crown spikes).
* [ ] **Hair Rooting:** Front bangs originate naturally underneath and cascade over the top cloth edge of the forehead protector.
* [ ] **Cel Shadow Banding:** Faceted bevel planes on hair spikes catch the directional cel terminator line without inverted dark artifacts.
* [ ] **Headband Tails:** Two navy cloth ribbon tails flowing down the back of the neck with dynamic physics or bone joints.

### Component C: Costume & Anatomy Construction
* [ ] **Fleece Collar:** Thick, ribbed shearling/fleece roll around the neck with modeled vertical compression folds, split by the central zipper track.
* [ ] **Jacket & Seams:** Deep orange fabric with navy shoulder yoke panels, wrist cuffs, waist hem band, and 3D back red Uzumaki spiral emblem.
* [ ] **Shinobi Holster:** Right thigh wrapped in clean white medical bandages beneath a molded black kunai pouch.
* [ ] **Footwear:** Navy shinobi sandals with open toes revealing sculpted toes and dark soles. Zero floating feet above ground plane.

### Component D: Facial Animation & Expressions
* [ ] **Blend Shapes / Morphs:** Minimum 4 core facial morphs:
  1. `Neutral` (Default calm)
  2. `Smirk_Determined` (Signature Naruto grin)
  3. `Combat_Yell` (Open-mouth shouting for jutsu)
  4. `Surprise_Blink` (Wide eyes and blinks)
* [ ] **Animation Blueprint Integration:** Curve-driven morphs fire on combat triggers and dialogue.

### Component E: Runtime Validation
* [ ] Verified under `CAM_B` (Close-up face audit at 95 cm).
* [ ] Verified under `CAM_A` (Third-person gameplay framing at 320 cm, character occupying ~28% vertical screen height).
* [ ] Zero vertex tearing or skinning collapsing during running, dodging, jumping, and landing.

---

## 2. Architectural & Environment Acceptance Standard

* [ ] **Timber Construction:** Exposed cedar post-and-beam frames, mortise-and-tenon joints, corner structural columns ($25 \times 25\text{ cm}$).
* [ ] **Roof Details:** Sloped gabled roofs tiled with terracotta clay *kawara*, carved eaves brackets, and *onigawara* ridge end caps.
* [ ] **Shopfronts (Ichiraku Standard):** Open dining alcove with recessed wooden counter, 4 rounded stools, split fabric *noren* curtains ("一楽ラーメン") with natural folds, and glowing red paper lanterns ($120\text{ cd/m}^2$).
* [ ] **Ground & Surfaces:** Broad central cobblestone avenue with stone curbs; zero flat default grey grid panels.
* [ ] **Foliage:** Stylized Japanese cedars (*cryptomeria*) and flowering pink cherry blossoms (*sakura*); zero billboard cross-quads in foreground.
* [ ] **Vista Sightline:** Unobstructed axial vanishing point framing the towering Hokage Rock cliff monument under an azure painterly sky. Zero pitch-black voids.

---

## 3. Performance & Hardware Budget Gate

* [ ] **Target Hardware:** NVIDIA GeForce GTX 1650 4GB VRAM.
* [ ] **Resolution:** $1920 \times 1080$ native.
* [ ] **Frame Budget:** Total frame time $\le 16.67\text{ ms}$ ($\ge 60\text{ FPS}$).
* [ ] **1% Low Frame Rate:** $\ge 45\text{ FPS}$.
* [ ] **VRAM Allocation:** Total VRAM usage $\le 3.4\text{ GB}$ under active combat and populated street conditions.
