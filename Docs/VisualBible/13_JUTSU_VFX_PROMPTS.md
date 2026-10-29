# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 13: NINJUTSU VFX PRODUCTION PROMPTS & NIAGARA SYSTEMS

> **Document ID:** EOS-VB-MOD-13  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **GAS Linkage:** [EOSGameplayAbility_JutsuBase.h](file:///c:/Projects/NARUTO_ECHOES-OF-SHINOBI/Source/EchoesOfShinobi/Public/Ability/EOSGameplayAbility_JutsuBase.h)

---

## 1. Jutsu VFX Architecture: GAS to Niagara Pipeline

```
+-----------------------------------------------------------------------------------+
|                           JUTSU VISUAL ACTIVATION PIPELINE                        |
+-----------------------------------------------------------------------------------+
| 1. Player Input Trigger         | Keybind Q / Slot 1 -> CastJutsuSlot()           |
| 2. GAS Ability Commitment       | AttributeSet deducts Chakra cost (e.g., 25.0)   |
| 3. Animation Montage Play       | Hand seals or forward palm thrust montage       |
| 4. Niagara Charge Emitter       | Spawns attached to character hand socket (hand_r)|
| 5. Niagara Projectile / Loop    | High-speed projectile with ribbon trail         |
| 6. Collision Sweep Hit          | Overlap or Sweep Multi with Pawn / Environment  |
| 7. Authoritative Damage Apply   | GAS GameplayEffect deals authoritative damage   |
| 8. Niagara Impact Burst         | Dynamic ground scorch / crater decal + shockwave|
| 9. Camera Shake & Audio Cue     | Radial impulse impulse + stereo sonic snap      |
+-----------------------------------------------------------------------------------+
```

---

## 2. Signature Jutsu 1: Rasengan (Spiraling Sphere)

### 2.1 Master Niagara VFX Prompt

```text
VFX PRODUCTION PROMPT — RASENGAN CHAKRA SPHERE (NS_Rasengan_Orb_Master)

FULL VISUAL PROMPT:
EFFECT NAME: Spiraling Chakra Sphere (Rasengan)
NATURE TRANSFORMATION: Pure Shape Transformation (Wind / Raw Chakra)
ATTACHMENT: Character right hand socket (`hand_r`)

CORE VISUAL COMPOSITION:
1. Inner Dense Core: A brilliant ultra-dense white-blue glowing spherical core (radius: 12cm) radiating intense energy with high emissive power (HDR multiplier: 15.0).
2. Swirling Vortex Ribbons: Over 40 high-speed swirling spiraling translucent cerulean-blue ribbons of compressed chakra orbiting the core at extreme velocity in multiple chaotic intersecting spherical axes. Ribbons feature sharp anime brushstroke tapering with glowing cyan leading tips.
3. Outer Shell Compression: A faint translucent spherical boundary layer with subtle Fresnel rim glow and chromatic aberration shimmer, containing the violently spinning vortex within a perfect sphere.
4. Kinetic Sparks & Flecks: Microscopic brilliant cyan spark flecks shedding from the outer sphere, drifting outward 30cm before fading with gentle turbulence.

AUDIO & CAMERA SYNCHRONIZATION:
High-pitched howling vortex drone that builds in frequency as the sphere reaches full compression; subtle micro-vibration camera shake (amplitude: 1.5, frequency: 45Hz) while held in palm.

IMPACT PRESENTATION:
Upon impacting an enemy pawn: The outer containment shatters in a blinding white-cyan spherical flash (radius: 250cm). A conical spiraling chakra drill shockwave tears through the target, launching them backward into ragdoll/knockback. Ground surfaces receive a spiraling gouged crater decal.

RENDERING:
Stylized anime Niagara particle system, custom translucent unlit materials with additive blending, dynamic point light (Cyan: 0.1, 0.7, 1.0, 2500 lumens, radius: 450cm) dynamically illuminating character face and chest.
```

---

## 3. Signature Jutsu 2: Chidori (One Thousand Birds)

### 3.1 Master Niagara VFX Prompt

```text
VFX PRODUCTION PROMPT — CHIDORI LIGHTNING DISCHARGE (NS_Chidori_Discharge_Master)

FULL VISUAL PROMPT:
EFFECT NAME: One Thousand Birds Lightning Blade (Chidori)
NATURE TRANSFORMATION: Lightning Release (Raiton)
ATTACHMENT: Character right hand palm

CORE VISUAL COMPOSITION:
1. Palm Charge Concentration: Violent electrical focal point concentrated around the palm and fingers, pulsing with brilliant blinding white-blue light (HDR: 25.0).
2. Jagged Electrical Arcs: Dozens of razor-sharp, jagged electric blue and white lightning bolts (Niagara ribbon and beam emitters) snapping and branching violently up the forearm and down toward the ground (branch length: 40cm to 120cm). Arcs feature instant frame-to-frame branching without smooth interpolation, capturing classic anime lightning timing.
3. Ground Discharge Sparks: Heavy showers of glowing blue and white electrical sparks bouncing off the floor with physics collision as the character runs forward in sprint thrust.
4. Optical Distortion: Subtle screen-space refractive heat haze surrounding the fist from extreme plasma ionization.

IMPACT PRESENTATION:
Straight piercing thrust; on hit, explosive radial burst of jagged lightning arcs piercing through the target's back, accompanied by a sharp thunderclap audio transient.
```

---

## 4. Element Jutsu 3: Fireball Jutsu (Goukakyuu no Jutsu)

### 4.1 Master Niagara VFX Prompt

```text
VFX PRODUCTION PROMPT — FIRE STYLE: GREAT FIREBALL (NS_Goukakyuu_Fireball_Master)

FULL VISUAL PROMPT:
EFFECT NAME: Fire Style: Great Fireball Technique
NATURE TRANSFORMATION: Fire Release (Katon)
SPAWN LOCATION: Character mouth socket (`head` / `mouth_socket`) facing forward

CORE VISUAL COMPOSITION:
1. Mouth Seal Exhaust: Funnel of white-hot fiery plasma erupting from pursed lips, expanding rapidly into a colossal sphere.
2. Giant Flame Sphere: A massive 400cm diameter stylized anime fireball propelled forward at high speed. The fireball is rendered with distinct 2-tone stylized cel-shading: a brilliant yellow-white interior flame surrounded by layered billowing deep orange and crimson flame crusts.
3. Smoke Trails & Embers: Heavy black-edged stylized smoke billows swirling along the wake of the fireball, trailing hundreds of glowing orange ember particles that float lazily.
4. Thermal Distortion: Pronounced heat distortion warping the environment behind the rolling fiery sphere.

IMPACT:
Detonates in a giant fiery mushroom dome upon hitting ground or enemy, leaving a blackened charred scorch mark on stone pavement.
```
