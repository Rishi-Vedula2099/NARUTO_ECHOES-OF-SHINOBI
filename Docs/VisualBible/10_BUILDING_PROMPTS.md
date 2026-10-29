# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 10: BUILDING PRODUCTION PROMPTS & ASSET SPECIFICATIONS

> **Document ID:** EOS-VB-MOD-10  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Registry Mapping:** [EOSVisualAssetRegistry.csv](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Data/Visual/EOSVisualAssetRegistry.csv)

---

## 1. Landmark Building: Ichiraku Ramen Establishment

### 1.1 Master Building Image Generation Prompt

```text
BUILDING PRODUCTION PROMPT — ICHIRAKU RAMEN SHOP (SM_BLD_ICHIRAKU_RAMEN)

FULL IMAGE-GENERATION PROMPT:
ASSET: Ichiraku Ramen Shop Establishment
BUILDING TYPE: Open-Front Commercial Food Stand & Noodle Bar
ERA: Canonical Konoha Era
DIMENSIONS: 600 cm wide × 500 cm deep × 450 cm high

ARCHITECTURE & FACADE:
A charming, intimate traditional Japanese ramen stall nestled between two residential village structures. Open-air wooden front counter crafted from thick sanded cedar timber with five round swivel wooden stools lined up outside. Above the counter hangs a prominent white and navy-blue split noren curtain printed with bold dark blue kanji characters for "Ramen" (ラーメン) and the shop logo, fluttering gently in the breeze. 

ROOF & MATERIALS:
Slanted overhang awning roof covered in warm terracotta-orange ceramic tiles (kawara) with curved ridges, ending in decorative round end-tiles. White lime plaster exterior walls supported by exposed dark charcoal-stained timber corner pillars. On the right side hangs a large glowing cylindrical red paper lantern (chochin) with black calligraphy reading "Ramen". 

INTERIOR APPARATUS:
Behind the wooden bar counter is a compact, authentic kitchen featuring a steaming stainless steel broth cauldron, stacked ceramic ramen bowls with red spiral rims, hanging wooden ladles, bamboo noodle strainer baskets, and stacked wooden condiment boxes with chopsticks and spice jars.

LIGHTING & ATMOSPHERE:
Inviting warm golden interior light spilling onto the cobblestone street from interior bare bulbs and red exterior lantern. Volumetric steam rising softly from the noodle boiler into the cool air.

COLLISION & GAMEPLAY:
Countertop and walls have solid collision (BlockAll); stool seats support player sit/interaction trigger; roof has flat collision for ninja parkour landing.

RENDERING:
Stylized 3D anime cel-shaded environment asset, clean inverted-hull outlines, rich hand-painted wood grain texture, inviting warm anime warmth.

NEGATIVE PROMPT:
modern plastic tables, neon signs, concrete highway, photorealistic grease, dirty grime, western diner, low poly jagged edges, untextured graybox.
```

---

## 2. Residential Building: Konoha Residence Type A

### 2.1 Master Building Image Generation Prompt

```text
BUILDING PRODUCTION PROMPT — KONOHA RESIDENTIAL APARTMENT (SM_BLD_KONOHA_RESIDENCE_A)

FULL IMAGE-GENERATION PROMPT:
ASSET: Konoha Two-Story Residential Apartment
BUILDING TYPE: Standard Village Housing
DIMENSIONS: 800 cm wide × 1000 cm deep × 850 cm high

ARCHITECTURE & FACADE:
A cozy, authentic two-story Japanese village dwelling. Ground floor features a recessed sliding wooden entrance door with brass pull ring, small raised stone entryway step, and twin sliding latticed wooden windows with frosted glass panes. Second floor features a cantilevered dark-stained cedar balcony with a sturdy horizontal wooden railing, where green potted bonsai trees and hanging wind chimes (furin) rest. Exterior wooden staircase on the left side leads directly up to the second-story balcony.

ROOF & MATERIALS:
Gabled Japanese hip roof covered in orange-red glazed clay tiles with cylindrical ceramic ridge beams and flared eaves. Walls are finished in warm ivory-white textured plaster framed by heavy dark-stained exposed timber beams. A cylindrical galvanized sheet-metal chimney pipe extends from the side wall up past the roofline.

COLLISION & GAMEPLAY:
Ground and second-floor decks have precise walkable collision; staircase is fully navigable with custom step collision; roof geometry supports rooftop sprint and jump lines.

RENDERING:
Stylized 3D anime action-RPG environment model, 2-step cel-shaded material response, crisp line art contouring, rich painterly wood and plaster textures.

NEGATIVE PROMPT:
western brick house, glass skyscraper, crumbling ruin, modern aluminum siding, messy textures, flat primitive shapes.
```

---

## 3. Civic Landmark: The Hokage Mansion

### 3.1 Master Building Image Generation Prompt

```text
BUILDING PRODUCTION PROMPT — THE HOKAGE MANSION (SM_BLD_KONOHA_HOKAGE_MANSION)

FULL IMAGE-GENERATION PROMPT:
ASSET: The Hokage Administrative Mansion
BUILDING TYPE: Village Civic Headquarters & Landmark
DIMENSIONS: 3000 cm wide × 3000 cm deep × 2500 cm high

ARCHITECTURE & FACADE:
The iconic circular civic headquarters of Konohagakure. A colossal cylindrical multi-tiered structure painted vibrant cinnabar-red with white trim bands encircling each story. The front entrance features a monumental arched entryway approached by a broad flight of curved grey granite ceremonial steps. The center of the facade proudly displays a massive painted crest: a giant white circle featuring the bold black kanji for "Fire" (火).

ROOF & BALCONY:
The top tier features a vast circular observation roof deck surrounded by a red protective parapet railing. The central rooftop pavilion has a conical green-copper or terracotta-tiled cupola with a tall flagpole flying the official Leaf Village banner. From this high deck, the Hokage surveys the entire village.

LIGHTING & ATMOSPHERE:
Majestic ceremonial presence, bathed in warm golden daylight, overlooking the village square and framed directly against the colossal stone faces of the Hokage Monument Cliff.

RENDERING:
Premium stylized 3D anime cel-shading, vibrant cinnabar-red and clean white palette, crisp architectural contours, heroic scale.

NEGATIVE PROMPT:
dull colors, modern glass office, gothic cathedral, crumbling derelict walls, low detail box.
```
