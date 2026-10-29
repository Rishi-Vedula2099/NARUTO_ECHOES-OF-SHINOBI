# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 08: KONOHA ARCHITECTURAL LANGUAGE & MATERIAL DESIGN

> **Document ID:** EOS-VB-MOD-08  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **C++ Linkage:** [EOSKonohaVillageBuilder.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Environment/EOSKonohaVillageBuilder.h)

---

## 1. Architectural Visual Pillars

Konohagakure (The Hidden Leaf Village) possesses a distinct, beloved architectural signature that blends traditional Japanese Edo-period craftsmanship with early 20th-century utility and ninja utility:

```
+-----------------------------------------------------------------------------------+
|                        KONOHA ARCHITECTURAL SIGNATURES                            |
+-----------------------------------------------------------------------------------+
| 1. Warm Terracotta Curved Tile Roofs | Distinct orange/salmon glazed tiles with   |
|                                      | gentle flared eaves and wooden ridge caps. |
| 2. Exposed Dark Cedar Timber Framing | Heavy dark-stained wood posts and beams    |
|                                      | framing weathered white/cream plaster.     |
| 3. Multi-Tiered Verticality          | Multi-story residences with external wood  |
|                                      | staircases, rooftop piping and balconies.  |
| 4. Industrial Shinobi Infrastructure | Overhead electrical wires, wooden utility  |
|                                      | poles, galvanized sheet metal stove pipes. |
| 5. Vibrant Commercial Signage        | Indigo and crimson noren curtains, wooden  |
|                                      | carved shop kanji plaques, paper lanterns. |
+-----------------------------------------------------------------------------------+
```

---

## 2. Modular Building Kit Specification

All village architecture is assembled from a modular kit on a standardized 400cm × 400cm grid:

### 2.1 Kit Components
- `MOD_WALL_PLASTER_400x400`: Cream plaster with dark vertical timber framing.
- `MOD_WALL_WINDOW_400x400`: Sliding multi-pane wood window with translucent shoji paper or glass.
- `MOD_WALL_DOOR_400x400`: Recessed sliding wooden entryway door with brass hardware.
- `MOD_ROOF_EAVE_CORNER_400`: Flared curved roof corner with orange ceramic tiles and gutter edge.
- `MOD_ROOF_RIDGE_STRAIGHT_400`: Straight roof crest with end-cap tiles bearing spiral motifs.
- `MOD_BALCONY_WOOD_400x150`: Cantilevered wooden balcony with railing for parkour jumping.
- `MOD_SHOP_CANOPY_400x200`: Slanted fabric awning with blue/white stripes or solid crimson canvas.

---

## 3. Master Architectural Environment Image Prompt

```text
ENVIRONMENT VISUAL PROMPT — KONOHAGAKURE COMMERCIAL STREET VISTA

FULL IMAGE-GENERATION PROMPT:
LOCATION: Hidden Leaf Village (Konohagakure) Main Commercial District
ERA: Classic Konoha Era
VIEW: Ground-level street view looking down bustling avenue toward Hokage Monument Cliff

ARCHITECTURE:
Dense rows of traditional Japanese wooden and plaster buildings lining a wide cobblestone street. Buildings are 2 to 3 stories tall with exposed dark weathered cedar timber beams, cream-white lime plaster infill walls, and iconic flared orange ceramic roof tiles (kawara) glowing warmly in the sun. Second-floor cantilevered wooden balconies with potted bonsai and drying laundry. Shop fronts feature indigo-blue and crimson noren fabric curtains printed with ramen and weapon kanji, sliding wooden latticed doors, and decorative wooden awnings.

INFRASTRUCTURE & DETAILS:
Wooden utility poles line the curb with clusters of black electrical wires stretching across the sky between rooftops. Stone drainage gutters flank the road. Traditional stone lanterns stand at alley entrances. Cylindrical tin stovepipes vent soft chimney smoke from bakeries.

ATMOSPHERE & LIGHTING:
Bathed in warm late-afternoon golden sunlight (5400K), casting long soft-angled toon shadows across the stone paving. Translucent pink cherry blossom (sakura) petals drifting lazily through the air. In the background, rising majestically behind the rooftops, the colossal granite cliff face of the Hokage Monument is visible, with the carved stone faces of the First, Second, Third, and Fourth Hokage overseeing the peaceful village.

STREET ACTIVITY:
Living anime town; civilian villagers in cotton yukatas walking the stone pavement, a shopkeeper sweeping his wooden threshold, a young ninja perched casually on an overhead telegraph wire.

RENDERING:
High-end stylized 3D anime action-RPG cel-shaded rendering, warm curated color palette (terracotta orange, warm cedar brown, indigo blue, cream plaster, vibrant green foliage), subtle inverted-hull ink outline contours, crisp clean lighting without photorealistic noise.

NEGATIVE PROMPT:
futuristic sci-fi, modern skyscrapers, asphalt highway, automobiles, western brick architecture, photorealistic gritty textures, dull gloomy colors, flat graybox, empty lifeless street, checkerboard patterns.
```
