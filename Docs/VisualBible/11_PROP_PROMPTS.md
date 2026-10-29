# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 11: STREET PROP PRODUCTION PROMPTS & FURNISHING SPECIFICATIONS

> **Document ID:** EOS-VB-MOD-11  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Registry Mapping:** [EOSVisualAssetRegistry.csv](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Data/Visual/EOSVisualAssetRegistry.csv)

---

## 1. Street Lighting Props

### 1.1 Stone Garden Lantern (SM_Prop_StoneLantern_01)
```text
PROP PRODUCTION PROMPT — TRADITIONAL STONE LANTERN (TORO)

FULL IMAGE-GENERATION PROMPT:
ASSET: Japanese Granite Stone Lantern (Yukimi / Kasuga Toro style)
DIMENSIONS: 80 cm wide × 80 cm deep × 160 cm high
GEOMETRY & MATERIALS:
Carved from light-grey weathered granite stone with subtle green moss accumulation along base and roof crevices. Composed of a hexagonal pedestal base, a cylindrical pillar shaft with gentle flared swelling, a carved firebox chamber (hibukuro) with latticed window openings, a flared hexagonal overhanging stone roof canopy, and a pointed jewel finial (hoju) resting at apex.
LIGHTING & VFX:
Inside the firebox chamber rests a flickering warm golden candle/oil lamp, casting warm light patterns (PointLight, 2700K, 600 lumens, radius 400cm) through the stone lattice onto the surrounding stone pavement.
COLLISION: Solid blocking collision (BlockAll).
RENDERING: Stylized 3D anime cel-shaded static mesh, hand-painted stone texture, crisp contour outlines.
```

### 1.2 Hanging Paper Lantern (SM_Prop_PaperLantern_Hanging)
```text
PROP PRODUCTION PROMPT — HANGING RED FESTIVAL LANTERN (CHOCHIN)

FULL IMAGE-GENERATION PROMPT:
ASSET: Cylindrical Japanese Red Paper Lantern
DIMENSIONS: 45 cm diameter × 90 cm high
GEOMETRY & MATERIALS:
Collapsible bamboo rib structure wrapped in translucent vibrant crimson-red washi paper with black lacquered wooden rims at top and bottom. Suspended from an iron curved arm bracket by a small steel chain. Black brush calligraphy kanji for "Shinobi" (忍) or "Tea" painted boldly on paper face.
LIGHTING:
Interior glowing bulb producing warm red-orange emissive subsurface scattering and omnidirectional ambient illumination.
COLLISION: OverlapAllDynamic with subtle physical pendulum sway on actor hit.
RENDERING: Stylized 3D anime cel-shading with glowing subsurface paper shader.
```

---

## 2. Street Furniture & Training Gear

### 2.1 Weathered Cedar Village Bench (SM_Prop_Konoha_Bench_01)
```text
PROP PRODUCTION PROMPT — WEATHERED CEDAR VILLAGE BENCH

FULL IMAGE-GENERATION PROMPT:
ASSET: Traditional Japanese Public Wooden Bench
DIMENSIONS: 180 cm long × 50 cm wide × 45 cm high
GEOMETRY & MATERIALS:
Three thick, smoothed planks of natural Japanese cedar (sugi) wood forming the seat surface, showing gentle grain lines and rounded edges from years of use. Supported by two sturdy square-cut timber leg blocks joined by mortise-and-tenon cross braces. Warm honey-amber natural wood finish with slight sun bleaching on top face.
GAMEPLAY & COLLISION:
Walkable top surface; interactive seat prompt for player and civilian NPCs.
RENDERING: Stylized 3D anime cel-shaded environment prop, hand-painted anime wood texture.
```

### 2.2 Rope-Wrapped Shinobi Training Log (SM_Prop_TrainingLog_Post)
```text
PROP PRODUCTION PROMPT — TRAINING LOG POST

FULL IMAGE-GENERATION PROMPT:
ASSET: Traditional Shinobi Taijutsu Training Post
DIMENSIONS: 40 cm diameter × 220 cm high
GEOMETRY & MATERIALS:
A solid cylindrical trunk of dark untreated oak sunk deeply into packed dirt ground. The center strike zone (torso height: 90cm to 160cm) is wound securely in thick, coarse natural jute rope wraps, showing fraying, scuffs, and dented impact impressions from thousands of punches and kicks. Top is cut flat with concentric tree ring contours.
COLLISION: Heavy solid blocking hull (BlockAll); registers weapon impacts with wood splinter Niagara sparks.
RENDERING: Stylized 3D anime cel-shading, rugged hand-painted rope fibers.
```

### 2.3 Wooden Utility Pole with Transformers (SM_Prop_WoodenUtilityPole)
```text
PROP PRODUCTION PROMPT — KONOHA WOODEN TELEGRAPH UTILITY POLE

FULL IMAGE-GENERATION PROMPT:
ASSET: Konoha Wooden Utility Pole
DIMENSIONS: 50 cm diameter × 900 cm high
GEOMETRY & MATERIALS:
Tall creosote-treated dark brown timber telephone pole with metal climbing rungs on sides. Top third features horizontal wooden cross-arms supporting white glazed ceramic spool insulators, a cylindrical grey metal step-down transformer box, and attachment hooks for black overhead electrical cables.
GAMEPLAY:
Climbable / wire-walkable; top cross-arms serve as perches for ninja scouts.
RENDERING: Stylized 3D anime environment prop, crisp anime outlines.
```
