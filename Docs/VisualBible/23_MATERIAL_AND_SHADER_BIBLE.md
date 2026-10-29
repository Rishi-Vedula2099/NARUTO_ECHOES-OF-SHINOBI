# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 23: ANIME CEL-SHADING & MASTER MATERIAL BIBLE

> **Document ID:** EOS-VB-MOD-23  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Shader Pipeline:** Material Graph / HLSL Custom Nodes / Post-Process Shading

---

## 1. Master Anime Cel-Shading Architecture

```
                                  [ CAMERA & SUN ]
                                         |
                                         v
                         +-------------------------------+
                         |   Surface Normal · Light Dir  |
                         |           (N · L)             |
                         +-------------------------------+
                                         |
                                         v
                         +-------------------------------+
                         | 2-Step / 3-Step Quantization  |
                         |   (Threshold A & B Steps)     |
                         +-------------------------------+
                                         |
                                         v
                         +-------------------------------+
                         | Base Base Color × Shadow Color|
                         +-------------------------------+
                                         |
         +-------------------------------+-------------------------------+
         |                               |                               |
         v                               v                               v
+------------------+           +-------------------+           +-------------------+
|  Fresnel Rim Glow|           | Stylized Specular |           | Inverted Hull Line|
|  (Camera Facing) |           | (Anisotropic Cap) |           | (Vertex Push Norm)|
+------------------+           +-------------------+           +-------------------+
         \                               |                               /
          +------------------------------+------------------------------+
                                         |
                                         v
                         +-------------------------------+
                         | FINAL ANIME SHADED PIXEL      |
                         +-------------------------------+
```

---

## 2. Mathematical Specification

### 2.1 Diffuse Quantization (Half-Lambert + Step Function)
```hlsl
// Calculate wrapped N dot L to prevent pitch-black termination lines
float HalfLambert = saturate((dot(WorldNormal, LightDirection) * 0.5) + 0.5);

// 2-Step Cel Quantization
float Step1 = smoothstep(ShadowThreshold - ShadowSmoothness, ShadowThreshold + ShadowSmoothness, HalfLambert);
float Step2 = smoothstep(MidtoneThreshold - ShadowSmoothness, MidtoneThreshold + ShadowSmoothness, HalfLambert);

// Final Diffuse Color Blending
float3 ShadedColor = lerp(ShadowColor.rgb, MidtoneColor.rgb, Step1);
ShadedColor = lerp(ShadedColor, BaseColor.rgb, Step2);
```

### 2.2 Inverted Hull Outline Shading
- **Technique:** Duplicate mesh or secondary vertex shader pass with flipped polygon winding (cull front).
- **Vertex Displacement:**
  `WorldPositionOffset = VertexNormal * OutlineWidth * saturate(CameraDistance / ReferenceDistance);`
- **Vertex Color Masking:** Mesh vertex color Alpha channel modulates outline thickness (e.g., Alpha=0 on eyes, mouth, and nostril vertices to prevent unsightly black blob artifacts).

### 2.3 Fresnel Silhouette Rim Lighting
```hlsl
float NdotV = saturate(dot(WorldNormal, CameraDirection));
float Rim = pow(1.0 - NdotV, RimPower) * saturate(dot(LightDirection, CameraDirection) + 0.3);
float3 FinalRim = Rim * RimColor.rgb * RimIntensity;
```

---

## 3. Dedicated Material Profiles & Parameters

| Profile Name | Target Asset Category | Base Roughness | Specular Step | Shadow Tint | Outline Thickness | Special Features |
| :--- | :--- | :---: | :---: | :--- | :---: | :--- |
| `M_Anime_Skin` | Character Faces & Hands | 0.85 | Off | Warm Peach/Pink | 0.8 mm | Soft shadow softness (no harsh lines on cheeks) |
| `M_Anime_Hair` | Spiky Anime Hair | 0.40 | Anisotropic Band | Hair Hue - 20% | 1.2 mm | Stylized halo highlight ring |
| `M_Anime_Fabric` | Tracksuits, Robes, Shirts | 0.90 | Off | Deep Saturated Tint | 1.0 mm | Matte cloth response, woven micro-bump |
| `M_Anime_Metal` | Headband Plates, Kunai | 0.15 | Sharp Step (90%) | Cool Grey/Blue | 1.5 mm | High-contrast anime shine glint |
| `M_Anime_Wood` | Timber Beams, Benches | 0.70 | Broad Soft | Warm Umber | 1.0 mm | Painterly wood grain directional mapping |
| `M_Anime_RoofTile`| Konoha Orange Roofs | 0.50 | Medium Step | Terracotta Brick | 1.2 mm | Glazed ceramic reflectivity |
| `M_Anime_Plaster` | White Village Walls | 0.95 | Off | Pale Lavender/Grey | 0.8 mm | Clean lime plaster stipple |
| `M_Anime_Stone` | Cobblestone, Lanterns | 0.80 | Low Soft | Slate Charcoal | 1.0 mm | Moss fringe tinting |
| `M_Anime_Foliage` | Cedar Trees, Sakura | 0.60 | Off | Deep Emerald / Rose | 0.0 mm | Two-sided foliage transmission with wind sway |
| `M_Anime_VFX` | Chakra Auras, Rasengan | 0.00 | Full Emissive | Pure White Core | 0.0 mm | Additive unlit blending with HDR bloom |
