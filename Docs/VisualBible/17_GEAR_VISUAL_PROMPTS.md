# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 17: GEAR VISUAL PROMPTS & 6-SLOT COSMETIC SOCKET SPECIFICATIONS

> **Document ID:** EOS-VB-MOD-17  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Gear Architecture Linkage:** [EOSCharacterBase.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Character/EOSCharacterBase.h#L163-L169)

---

## 1. The 6-Slot Shinobi Gear Socket Architecture

In *ECHOES OF SHINOBI*, gear items are not purely abstract stats; every equipped gear piece attaches dynamically to authoritative skeletal sockets:

```
+-----------------------------------------------------------------------------------+
|                           6-SLOT GEAR SOCKET MAPPING                              |
+-----------------------------------------------------------------------------------+
| 1. HEADWEAR (Socket: head)       | Forehead protector, Anbu mask, straw hat       |
| 2. TORSO / VEST (Socket: spine_02)| Chunin flak jacket, Sage haori, Anbu harness   |
| 3. GAUNTLETS (Sockets: hand_l/r) | Iron handguards, bandage wraps, weighted cuffs |
| 4. WAIST / SCROLLS (Socket: pelvis)| Dual sealing scrolls, rope shimenawa sash     |
| 5. LEG POUCH (Socket: thigh_r)   | Weapon holster, medical supply pouch, tape     |
| 6. FOOTWEAR (Sockets: foot_l/r)  | Shinobi sandals, iron-reinforced boots, geta   |
+-----------------------------------------------------------------------------------+
```

---

## 2. Master Gear Visual Prompts

### 2.1 Konoha Flak Jacket (Jonin/Chunin Vest — SM_GEAR_KonohaFlakJacket)
```text
GEAR PRODUCTION PROMPT — KONOHA TACTICAL FLAK JACKET

FULL IMAGE-GENERATION PROMPT:
ASSET: Standard Konoha Military Flak Jacket (Jonin Edition)
SLOT: Torso / Vest (Socket: `spine_02`)
GEOMETRY & MATERIALS:
Heavy olive-green padded tactical ballistic canvas vest. Features a high protective stiffened collar protecting nape and throat, twin front zippered ammunition scroll pockets with silver zipper pulls, shoulder epaulets with brass snap buttons, and the red embroidered spiral crest of the Uzumaki clan centered on upper back.
RENDERING: Stylized 3D anime cel-shaded static mesh with woven canvas normal map.
```

### 2.2 Anbu Black Ops Porcelain Animal Mask (SM_GEAR_AnbuFoxMask)
```text
GEAR PRODUCTION PROMPT — ANBU PORCELAIN FOX MASK

FULL IMAGE-GENERATION PROMPT:
ASSET: Anbu Black Ops Operative Mask (Kitsune / Fox design)
SLOT: Headwear (Socket: `head_accessory` or tilted at `head_side`)
GEOMETRY & MATERIALS:
Smooth white porcelain ceramic mask with glossy glaze finish. Features stylized painted crimson-red whisker marks, pointed fox ear crests, narrow slit eye openings, and dark red brow accents. Held by black elastic cord with red beaded tassel hanging on left side.
RENDERING: Stylized 3D anime cel-shading with reflective ceramic specular sheen.
```
