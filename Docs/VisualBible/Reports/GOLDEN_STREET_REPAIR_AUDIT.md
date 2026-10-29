# GOLDEN STREET REPAIR AUDIT
**Project:** NARUTO: ECHOES OF SHINOBI  
**Engine:** Unreal Engine 5.8.3 | D3D12 (PCD3D_SM6) | Forward/Desktop Shading  
**Hardware:** Intel Core i5-11400H | NVIDIA GeForce GTX 1650 | 16 GB RAM  
**Date:** 2026-10-21  
**Map Under Audit:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet.umap`  

---

## 1. Executive Summary
The runtime capture `GoldenStreet_RuntimeCapture_04.png` revealed serious visual breakdowns:
1. **Geometric Rotational Inversion:** All buildings (Ichiraku Ramen, Residences, Hokage Mansion), trees, and lanterns were rotated 45° to 90° on their pitch axes due to an `unreal.Rotator` positional argument mapping mismatch in Python.
2. **Shader Color Washout:** Hero character and environment rendered as flat monochrome white/grey due to a Fresnel-to-emissive connection in `M_AnimeCel_Master` and low dynamic range scene color format (`r.SceneColorFormat=3`).
3. **Procedural Geometry Assembly:** Several assets were procedurally assembled from primitive cylinders and boxes using trimesh without authored UVs or vertex normals.

---

## 2. Technical Asset & System Audit

### Rendering & Pipeline Configuration
- **RHI:** Direct3D 12 (Targeted Shader Format: `PCD3D_SM6`)
- **Global Illumination:** Screen Space GI (`r.DynamicGlobalIlluminationMethod=0`, Lumen disabled for GTX 1650 performance stability)
- **Reflections:** Screen Space Reflections (`r.ReflectionMethod=0`)
- **Shadows:** Cascaded Shadow Maps with 4 cascades, 2048 resolution (`r.Shadow.CSM.MaxCascades=4`, `r.Shadow.MaxResolution=2048`)
- **Exposure:** Histogram Auto-Exposure (`r.DefaultFeature.AutoExposure.Method=0`)
- **Scene Color Format:** Was set to `3` (8-bit LDR clamp). Rectified to `2` (64-bit RGBA FP16 HDR).

### Existing Level Actors in `L_Konoha_GoldenStreet`
- **Lighting Actors:**
  - `DirLight_Konoha_Sun`: DirectionalLight, Intensity 10.0 lux, warm 5400K color `(255, 242, 218)`. Rotator: pitch -35°, yaw 45°.
  - `SkyLight_AnimeAzureFill`: SkyLight, Intensity 4.0 lux, soft azure color `(180, 220, 255)`.
  - `Fog_KonohaSunsetHaze`: ExponentialHeightFog, density 0.001.
  - `PostProcess_GoldenStreet`: Histogram exposure, min brightness 0.05, max brightness 2.0.
- **Environment Actors (Defective Rotations):**
  - `Bld_IchirakuRamen`: Rot: (85, 0, 0) — lying on side.
  - `Bld_HokageMansion_MidVista`: Rot: (45, 0, 0) — giant tilted cylinder structure in center.
  - `Prop_StoneLantern_01` (Left & Right): Rot: (±35, 0, 0) — tilted into camera frame.
  - `Veg_KonohaCedar_Large`: Rot: (25..60, 0, 0) — tilted into ground.

### Material System Audit
- **Master Material:** `/Game/Materials/Master/M_AnimeCel_Master`
  - Shading Model: `MSM_DEFAULT_LIT`
  - Blend Mode: `BLEND_OPAQUE`
  - Defect: `Fresnel` node was connected to `MP_EMISSIVE_COLOR` with `RimLightIntensity` overrides up to 0.45.
- **Material Instances:** 18 MIs configured in `/Game/Characters/Naruto/Materials`, `/Game/Environment/Konoha/.../Materials`.

---

## 3. Repair Prioritization Matrix
1. **P0 - Fix Rotator Convention:** Replace all positional `unreal.Rotator(*rot)` with keyword `unreal.Rotator(pitch=0.0, yaw=angle, roll=0.0)`.
2. **P0 - Clear Street Central Axis:** Remove giant tilted cylinder pagoda blocking central street; place Hokage pagoda at proper mid-vista depth.
3. **P0 - Eliminate Emissive Fresnel Blowout:** Set `RimLightIntensity = 0.0` on all non-emissive materials; restore vibrant anime BaseColors.
4. **P0 - Fix Hero Character Normals & Materials:** Re-import `SK_Naruto_Genin` with smoothed vertex normals and verified material slot bindings.
5. **P1 - Recompose 50m Golden Slice:** Form clean avenue framed by upright Ichiraku Ramen on left, residences on right, and Hokage Monument on central vista.
