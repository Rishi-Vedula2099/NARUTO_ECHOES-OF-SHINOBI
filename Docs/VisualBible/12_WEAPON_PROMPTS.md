# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 12: SHINOBI WEAPON PROMPTS & RIGGING SPECIFICATIONS

> **Document ID:** EOS-VB-MOD-12  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Registry Mapping:** [EOSVisualAssetRegistry.csv](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Data/Visual/EOSVisualAssetRegistry.csv)

---

## 1. Standard Shinobi Arsenal

### 1.1 Steel Throwing Kunai (SM_WPN_Kunai_Standard)
```text
WEAPON PRODUCTION PROMPT — STEEL SHINOBI KUNAI

FULL IMAGE-GENERATION PROMPT:
ASSET: Standard Hidden Leaf Steel Shinobi Kunai
DIMENSIONS: 26 cm length × 4.5 cm widest blade width × 1.2 cm thick
GEOMETRY & MATERIALS:
Dual-edged diamond-cross-section forged high-carbon steel blade with tapered spear point and sharp beveled cutting edges. Small rectangular hilt crossguard, cylindrical grip wrapped tightly in white cotton medical tape with crisscross binding, terminating in a solid 3.5cm circular ring pommel. Blade features a subtle gunmetal-grey matte finish with polished metallic bevels reflecting cold ambient light.
ATTACHMENT SOCKET: `weapon_r` / `hand_r_socket` / `thigh_holster_socket`
COLLISION: Precision weapon capsule with trace collision enabled on attack swings.
RENDERING: Stylized 3D anime cel-shaded static mesh with sharp anisotropic specular edge highlight.
```

### 1.2 Four-Pointed Shuriken (SM_WPN_Shuriken_Standard)
```text
WEAPON PRODUCTION PROMPT — FOUR-POINTED SHURIKEN (HIRA-SHURIKEN)

FULL IMAGE-GENERATION PROMPT:
ASSET: Standard Four-Pointed Throwing Shuriken
DIMENSIONS: 12 cm diameter × 0.4 cm thick
GEOMETRY & MATERIALS:
Flat, symmetrical four-pointed curved aerodynamic star crafted from hardened matte-black blackened steel. Central circular hole (diameter 2.5cm) with beveled perimeter for finger spin balance. Each of the four triangular blades features hollow ground cutting edges with razor-sharp tips.
VFX: High-speed spinning motion blur disc (Niagara ribbon emitter) when thrown in combat.
ATTACHMENT: Holstered in waist pouch or spawned dynamically as projectile actor.
RENDERING: Stylized 3D anime weapon model, crisp clean contours.
```

---

## 2. Legendary & Heavy Weapons

### 2.1 The Kubikiribocho (Executioner's Blade — SM_WPN_Executioner_Blade)
```text
WEAPON PRODUCTION PROMPT — KUBIKIRIBOCHO (EXECUTIONER'S BLADE)

FULL IMAGE-GENERATION PROMPT:
ASSET: The Kubikiribocho (Decapitating Broadsword)
OWNER: Zabuza Momochi / Seven Swordsmen of the Mist
DIMENSIONS: 250 cm overall length × 32 cm blade width × 3.5 cm spine thickness
GEOMETRY & MATERIALS:
Colossal two-handed broadsword. Single-edged flat steel blade with a massive semicircular cutout near the base of the blade and a circular hole near the pointed tip. Long, two-handed cylindrical wooden grip wrapped in weathered off-white cloth tape, round iron pommel cap, and minimalist steel crossguard plate. Blade metal shows ancient combat scratches and a cold, ominous silver sheen.
ATTACHMENT SOCKET: `spine_02_back_holster` / `hand_r_twohanded`
COLLISION: Heavy sweep volume; dynamic impact decals (stone gouges) when striking environment.
RENDERING: Stylized 3D anime cel-shading, heavy physical presence, reflective anime specular sheen.
```

### 2.2 Sword of Kusanagi (SM_WPN_Kusanagi_Orochimaru)
```text
WEAPON PRODUCTION PROMPT — SWORD OF KUSANAGI (OROCHIMARU)

FULL IMAGE-GENERATION PROMPT:
ASSET: Sword of Kusanagi (Serpentine Mythic Blade)
OWNER: Orochimaru
DIMENSIONS: 95 cm blade length × 3.2 cm blade width
GEOMETRY & MATERIALS:
Double-edged straight chokuto sword without a traditional crossguard (shirasaya style). Pristine, flawlessly polished mythic steel blade radiating a cold faint violet aura. Simple matte black lacquered wooden scabbard and hilt with subtle silver trim.
SPECIAL TRAITS: Can extend instantaneously up to several meters in combat; pierces through diamond-hard defense.
ATTACHMENT SOCKET: Hand or summoned from mouth orifice.
RENDERING: Stylized 3D anime cel-shading with glowing violet emissive outline on thrust.
```
