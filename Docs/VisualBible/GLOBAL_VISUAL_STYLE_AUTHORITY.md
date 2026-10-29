# GLOBAL VISUAL STYLE AUTHORITY
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Permanent Governance Anchor:** Supreme Art Direction Authority  
**Benchmark Target:** CyberConnect2 *Naruto Shippuden: Ultimate Ninja Storm* Series & Studio Pierrot Cel Animation

---

## 1. Supreme Visual Mandate

NARUTO: ECHOES OF SHINOBI is a stylized 3D anime action RPG with a painterly anime finish, controlled 2-band cel shading, warm expressive colors, iconic shinobi silhouettes, authentic Japanese timber architecture, and dynamic third-person action staging.

### Absolute Negative Constraints (Non-Negotiable)
1. **No Photorealism Substitutions:** Unreal Engine's default photorealistic materials, rough gravel Megascans, high-frequency PBR noise, and Quixel scans are **STRICT FAILURES**.
2. **No Lighting As A Band-Aid:** Missing mesh geometry, broken UVs, or untextured assets must never be compensated for by blowing out directional lights, cranking bloom, or drowning the scene in fog.
3. **No Untextured Primitives As Production:** Primitives, default mannequins, and flat procedural extrusion boxes are strictly development scaffolding and must never be labeled production assets.

---

## 2. Character Visual Anatomy Formula

$$\text{Character Reality: } \text{Face Identity} + \text{Facial Anatomy} + \text{Designed Hair} + \text{Costume Construction} + \text{Deformation} + \text{Expressions}$$

### A. Facial Architecture (CyberConnect2 Benchmark)
- **Eye Topology & Depth:** Large expressive anime almond eyes with deep black upper lash lines, sharp iris contour, dark pupil core, and vibrant primary iris color with specular highlight catchlights.
- **Cheek & Whisker Morphology:** 3 distinct, slightly curved whisker marks on each cheek following the zygomatic arch. Whisker marks must be authored decals or UV-baked crisp vector line art, never smeared vertex colors.
- **Nose & Mouth Contour:** Minimalist sharp nose bridge with small triangular shadow underside; distinct mouth fissure capable of snapping to determined smirk, open combat yell, or clenched teeth.
- **Forehead & Headband Anchor:** Curved stamped metallic plate featuring the incised Konoha leaf crest and corner rivet studs. Cloth wrap sits flush against the browline, with hair clumps growing organically from underneath and tumbling over the metal top edge.

### B. Hair Clump Volume & Silhouettes
- Hair must never be rendered as isolated polygonal cones.
- Authored in volume tiers:
  1. Front bangs framing the forehead protector.
  2. Side temporal clumps wrapping the ears.
  3. Crown and occipital spikes providing multi-layered radial volume with faceted planes that catch cel-shading light bands cleanly.

### C. Costume Tailoring & Shinobi Construction
- **White Fleece Collar:** Thick, ribbed roll with modeled vertical compression folds wrapping the neck, split by the central brass/silver zipper track.
- **Tracksuit Form:** Saturated cadmium orange fabric with navy shoulder yoke panels, wrist cuffs, waist hem band, and the crimson Uzumaki spiral emblem on the back.
- **Shinobi Gear:** Right thigh wrapped in tight white medical bandages beneath a molded black kunai holster pouch; open-toed navy ninja sandals revealing sculpted toes and dark soles.

---

## 3. Environment & Architectural Authority

### A. Authentic Konoha Timber Construction
- **Structural Integrity:** Post-and-beam exposed cedar framing, visible mortise-and-tenon joints, corner structural columns ($25 \times 25\text{ cm}$ minimum), cantilevered second-story balconies with balustrades.
- **Roof Architecture:** Traditional Japanese sloped roofs lined with curved terracotta clay tiles (*kawara*), carved eaves brackets, and ridge end caps (*onigawara*).
- **Windows & Facades:** Translucent sliding *shoji* paper lattices, dark timber window mullions, weathered cedar plank wall infill with warm wood grain.

### B. Hero Landmark Standard (Ichiraku Ramen)
- Open dining alcove with recessed polished wooden counter and 4 rounded wooden stools.
- Three-section split fabric *noren* curtains dyed cream/off-white with bold black kanji ("一楽ラーメン") and natural wave folds.
- Hanging spherical paper lanterns (*chōchin*) emitting warm amber light ($2200\text{K}$, $120\text{ cd/m}^2$).
- Authentic kitchen accessories: noodle boiling baskets, bamboo steamer racks, porcelain ramen bowls, wooden chopstick holders.

### C. Atmospheric Depth & Vista Anchoring
- **Vanishing Point Vista:** Central avenue must unobstructed frame the towering Hokage Rock cliff monument (Hashirama, Tobirama, Hiruzen, Minato) behind the red circular Hokage Mansion pagoda.
- **Painterly Skybox:** Azure anime sky with warm afternoon horizon gradient and soft hand-painted cumulus clouds. **Zero pitch-black skybox voids.**

---

## 4. Shading & Lighting Triad

| Parameter | Calibrated Value | Art-Direction Purpose |
| :--- | :--- | :--- |
| **Key Sun Light** | $12.0\text{ Lux}$ ($5400\text{K}$) | Warm daylight, sharp anime cel terminator line. |
| **SkyLight Fill** | $3.0\text{ cd/m}^2$ (Azure Blue) | Fills shadows with vibrant sky reflections, preventing crushed black silhouettes. |
| **Shadow Floor** | $\ge 0.15\text{ Luminance}$ | Shadows remain colorful, readable, and comic-accurate. |
| **Exposure (EV)** | Clamped EV 1.0 (Histogram) | Constant exposure; zero camera blowout or pitch-black stepping. |
| **Cel Response** | 2-3 Band Quantized Step | Hard-edged anime thresholding driven by DotProduct(N, L). |
