# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 07: LIVING NPC VISUAL SYSTEM & ARCHETYPE PROMPTS

> **Document ID:** EOS-VB-MOD-07  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **C++ Linkage:** [EOSVillageNPC.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Environment/EOSVillageNPC.h)

---

## 1. Living Village NPC Architecture & Distance Tiers

To populate the Konoha district without tanking GPU frame time on the GTX 1650 target, NPCs use four data-driven distance tiers:

```
+-----------------------------------------------------------------------------------+
|                           NPC DISTANCE OPTIMIZATION TIERS                         |
+-----------------------------------------------------------------------------------+
| TIER 1: NEAR (< 8m)      | Full skeletal rig, facial blendshapes, eye blink,      |
|                          | dynamic fabric physics, audio dialogue trigger.        |
| TIER 2: MEDIUM (8m-25m)  | Simplified locomotion blendspace, static facial mesh,   |
|                          | ambient idle animations (sweeping, chatting, eating).  |
| TIER 3: FAR (25m-60m)    | Reduced bone evaluation (LOD1), no secondary physics,   |
|                          | path-following waypoint patrol loops.                  |
| TIER 4: VERY FAR (> 60m) | Static impostor billboard / culled from render pass.   |
+-----------------------------------------------------------------------------------+
```

---

## 2. Archetype 1: Shopkeeper Teuchi (Ichiraku Ramen Master)

### 2.1 Master Character-Wise Refined Image Generation Prompt

```text
CHARACTER-WISE REFINED IMAGE PROMPT — SHOPKEEPER TEUCHI (RAMEN MASTER)

FULL IMAGE-GENERATION PROMPT:
CHARACTER: Teuchi (Ichiraku Ramen Shopkeeper)
ERA: Konoha Classic Era
ROLE: Master Ramen Chef / Key Konoha Shopkeeper NPC

FACE: Warm kindly middle-aged Japanese anime face, cheerful smiling eyes crinkled in welcoming crescents, gentle laugh lines around mouth, friendly benevolent expression.
HAIR: Short dark grey-brown hair, neatly tucked under a white cotton chef bandanna tied securely at forehead.
BODY PROPORTIONS: 1:7.0 proportion ratio, height 173 cm, sturdy healthy working man's build, slightly rounded apron-wrapped torso.
SKIN: Warm lightly tanned anime skin tone with subtle chef steam flush.

COSTUME: Traditional Japanese ramen chef uniform. Clean white short-sleeved cotton tunic with crossed collar, navy-blue waist apron tied in front with white string knot, loose dark grey work trousers. White chef towel draped over left shoulder.
FOOTWEAR: Traditional wooden geta sandals with dark fabric straps or practical dark straw work sandals.
ACCESSORIES: Large wooden soup ladle held in right hand, steaming ceramic ramen bowl with chopsticks held in left hand.

MATERIALS: Textured white cotton canvas for apron and bandanna, glazed ceramic bowl with rich steaming broth, polished natural wood for ladle.
SILHOUETTE: Classic Japanese ramen shop master silhouette, upright bandanna, white apron over dark clothes, welcoming posture.
COMBAT LANGUAGE: Non-combat civilian NPC; responsive interaction gestures (bowing, wiping counter, offering steaming bowl).

POSE: Standing behind wooden ramen counter, holding steaming bowl forward with a warm inviting smile.
CAMERA: Medium-shot eye-level shop front perspective, warm shallow depth of field.
LIGHTING: Warm golden lantern light from hanging red paper lanterns overhead, soft steam backlight.
ENVIRONMENT: Ichiraku Ramen bar counter with wooden stools, hanging blue and white noren curtains with ramen kanji.
RENDERING: Stylized 3D anime cel-shaded rendering, warm palette, clean inverted-hull outlines.
CONTINUITY: Faithful to Teuchi's appearance in Part 1 and Part 2.

NEGATIVE PROMPT:
ninja weapons, armor, fighting stance, angry expression, realistic skin pores, photorealistic food, modern clothes, western chef hat.
```

---

## 3. Archetype 2: Konoha Civilian Villager (Male & Female)

### 3.1 Master Civilian Villager Prompt

```text
CHARACTER-WISE REFINED IMAGE PROMPT — KONOHA CIVILIAN VILLAGERS

FULL IMAGE-GENERATION PROMPT:
CHARACTERS: Konoha Civilian Villagers (Male and Female pair)
ERA: Classic Konoha Era
ROLE: Ambient Village Population NPCs

MALE VILLAGER:
FACE: Pleasant youthful anime villager, neat brown hair, friendly open expression.
BODY: 1:7.0 anime proportions, height 175 cm, lean everyday citizen build.
COSTUME: Earth-toned cotton yukata (olive-green / taupe) wrapped and secured with a dark brown sash (obi), brown utility pouch at hip, dark fabric sandals. Carrying a woven bamboo produce basket.

FEMALE VILLAGER:
FACE: Gentle friendly anime features, dark hair tied back in a neat practical bun with hairpin, pleasant welcoming smile.
BODY: 1:6.8 anime proportions, height 160 cm, graceful civilian posture.
COSTUME: Light indigo-blue cotton kimono robe with subtle geometric leaf pattern, cream-colored sash, beige aproned skirt, white tabi socks with wooden zori sandals. Carrying a folded paper umbrella.

MATERIALS: Matte woven cotton and linen fabrics, natural bamboo weave for basket, clean cel-shaded anime materials.
SILHOUETTE: Relaxed everyday village civilian silhouettes, distinct from combat shinobi by absence of headbands and weapons.
BEHAVIOR & AMBIENCE: Walking along Konoha street, conversing pleasantly, observing shinobi leaping across rooftops.

POSE: Strolling side-by-side down stone street in animated mid-conversation.
CAMERA: Medium-wide street-level shot, 50mm lens.
LIGHTING: Bright clear Konoha morning sunlight, soft building shadows.
ENVIRONMENT: Konoha main commercial street, timber shopfronts and flower boxes.
RENDERING: Stylized 3D anime cel-shading, 2-step diffuse quantization, clean outlines.

NEGATIVE PROMPT:
ninja headbands, combat flak jackets, kunai, weapons, modern jeans, photorealistic textures, excessive noise, grotesque faces.
```

---

## 4. Archetype 3: Chunin Gate Guard (Izumo/Kotetsu Inspired)

### 4.1 Master Gate Guard Prompt

```text
CHARACTER-WISE REFINED IMAGE PROMPT — KONOHA CHUNIN GATE GUARD

FULL IMAGE-GENERATION PROMPT:
CHARACTER: Konoha Chunin Gate Guard (Izumo/Kotetsu archetype)
ERA: Part 1 & 2 Era
ROLE: Village Perimeter Security / Checkpoint Guard NPC

FACE: Alert stoic young veteran shinobi, brown anime hair with spiky bangs peeking from headband, bandage strip across bridge of nose, watchful vigilant eyes.
BODY: 1:7.2 athletic shinobi proportion ratio, height 178 cm, disciplined standing military posture.

COSTUME: Full standard Konoha Chunin uniform. High-collared navy-blue long-sleeve undershirt with red swirl emblems, dark navy pants, standard olive-green tactical flak jacket with zippered front scroll pockets, high protective collar, shoulder straps.
FOOTWEAR: Standard dark navy shinobi sandals with white shin bandages.
ACCESSORIES: Leaf Village forehead protector with dark navy band, iron guard plate polished. Standard clipboard / village registry ledger held in hand, or resting hand on hip near kunai pouch. Giant iron spiked shell weapon resting against guard booth.

MATERIALS: Padded heavy olive nylon canvas for flak jacket, matte navy cotton, cold polished steel headband plate.
SILHOUETTE: Disciplined military guard silhouette, high collar, sturdy vest, official village gate presence.

POSE: Standing guard beside red Konoha gate pillar, clipboard in one hand, scanning incoming travelers with professional vigilance.
CAMERA: Low-angle imposing guard shot, 35mm lens framing gate arch in background.
LIGHTING: Sharp midday sunlight casting crisp anime shadow under flak jacket and headband rim.
ENVIRONMENT: The Great Konoha Village Entrance Gate, colossal timber pillars, paved stone entryway.
RENDERING: Stylized 3D anime cel-shaded rendering, clean inverted-hull outlines, high anime action fidelity.

NEGATIVE PROMPT:
sleeping on duty, casual clothes, missing vest, missing headband, western soldier camo, photorealistic human skin.
```
