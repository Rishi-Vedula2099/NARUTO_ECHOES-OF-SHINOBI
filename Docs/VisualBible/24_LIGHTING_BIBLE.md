# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 24: LIGHTING & ATMOSPHERE BIBLE

> **Document ID:** EOS-VB-MOD-24  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Target Hardware:** GTX 1650 Development Target (60 FPS)

---

## 1. Konoha Golden Hour Lighting Target

The visual soul of Konohagakure is defined by its warm, hopeful, cinematic late-afternoon sunlight:

```
+-----------------------------------------------------------------------------------+
|                        KONOHA LIGHTING COMPOSITION PARAMETERS                     |
+-----------------------------------------------------------------------------------+
| 1. Sun Directional Light        | Temperature: 5400K (Warm golden daylight)       |
|                                 | Lux: 75,000 lux (bright afternoon clear sky)    |
|                                 | Sun Elevation: 38° above horizon                |
|                                 | Sun Azimuth: 220° (striking rooftops diagonally)|
| 2. Skylight & SkyAtmosphere     | Color: Soft azure/cerulean sky fill             |
|                                 | Preserves shadow details; prevents harsh black  |
| 3. Atmospheric Fog              | ExponentialHeightFog; fog density: 0.002        |
|                                 | Inscattering color: Pale warm peach-gold        |
|                                 | Hokage Rock cliff remains sharp & readable      |
| 4. Character Rim Key            | Backlight directional angle creates bright edge |
|                                 | halo on spiky hair and shoulders                |
+-----------------------------------------------------------------------------------+
```

---

## 2. Scalable Lighting Presets & Hardware Optimization

To ensure stable 60 FPS on GTX 1650 hardware without sacrificing visual excellence, five lighting scalability profiles are established:

| Setting Parameter | `LOW` | `MEDIUM / GTX1650_SAFE` (Default Dev) | `HIGH` | `EPIC` |
| :--- | :--- | :--- | :--- | :--- |
| **Shadow Method** | Standard Shadow Maps | Cascaded Shadow Maps (CSM - 3 Cascades) | CSM (4 Cascades) + Contact | Virtual Shadow Maps (VSM) |
| **Global Illumination** | SSGI (Low) | SSGI + Baked Skylight | Lumen (Software Tracing) | Lumen (Hardware Ray Tracing)|
| **Reflections** | Screen Space (Low) | Screen Space Reflections (Standard) | Lumen Reflections | Lumen Hardware Reflections |
| **Volumetric Fog** | Disabled | Half-Resolution Grid (Grid size: 64) | Full Resolution Grid | Full Resolution + Volumetric|
| **View Distance** | 300 meters | 600 meters | 1200 meters | 2500 meters |
| **Target FPS (1080p)**| 75+ FPS | **60 FPS Solid (1% Low > 48 FPS)** | 50–55 FPS | 35–45 FPS |
| **Target Hardware** | Intel UHD Graphics | **GeForce GTX 1650 / AMD RX 570** | RTX 3060 / RX 6600 | RTX 4070+ |

---

## 3. Directional Light & Sky Rig C++ Setup Guide

In `L_VisualAssetSmokeTest` and `L_Konoha_Phase13_District`, lighting is instantiated with runtime parameters ensuring reproducible visuals in PIE and standalone packaged builds:

```cpp
// Canonical Lighting Configuration for Phase 13
DirectionalLightComponent->SetIntensity(75000.0f);
DirectionalLightComponent->SetLightColor(FLinearColor(1.0f, 0.94f, 0.86f)); // 5400K Warm Sun
DirectionalLightComponent->bUseTemperature = true;
DirectionalLightComponent->SetTemperature(5400.0f);
DirectionalLightComponent->DynamicShadowDistanceMovableLight = 6500.0f; // 65 meters CSM
DirectionalLightComponent->CascadeDistributionExponent = 2.5f;

SkyLightComponent->SetIntensity(1.5f);
SkyLightComponent->SetLightColor(FLinearColor(0.85f, 0.92f, 1.0f)); // Soft Blue Fill
SkyLightComponent->bRealTimeCapture = true;
```
