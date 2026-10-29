# NARUTO: ECHOES OF SHINOBI
# 06 — ENVIRONMENT & WORLD BIBLE
## *Authoritative World Production Specification*

---

## Core Principle

Environments are **production-quality authored 3D spaces**, not grayboxes with lights. Every building, road, tree, prop, and landmark must be a real asset rendered with real anime materials.

---

## Konoha Golden Street (50m × 50m) — Phase 6

The first and most important environment. Must visually match the gameplay reference.

### Required Assets

#### Buildings (7 minimum)
| Asset | Description | Status |
|---|---|---|
| `SM_Bld_IchirakuRamen` | Ramen shop with noren curtains, counter, stools | EXISTS |
| `SM_Bld_Konoha_Residence_A` | Two-story wooden house, tiled roof | EXISTS |
| `SM_Bld_Konoha_Residence_B` | Variant house, different facade | EXISTS |
| `SM_Bld_Konoha_WeaponShop` | Shopfront with display, signage | EXISTS |
| `SM_Bld_Konoha_TeaHouse` | Tea house with open front | EXISTS |
| `SM_Bld_Konoha_MissionOffice` | Shinobi mission board building | EXISTS |
| `SM_Bld_Konoha_HokageMansion` | Red pagoda mansion (mid-background) | EXISTS |

#### Roads & Ground
| Asset | Description | Status |
|---|---|---|
| `SM_Road_Paved_01` | Cobblestone main avenue | EXISTS |
| Sidewalk sections | Stone curb sidewalks | TODO |
| Drainage channels | Small water drainage | TODO |

#### Vegetation (5 types)
| Asset | Description | Status |
|---|---|---|
| `SM_Veg_Cedar_Large` | Japanese cedar tree | EXISTS |
| `SM_Veg_KonohaCedar_Large` | Konoha-specific large tree | EXISTS |
| `SM_Veg_Sakura_Bloom` | Pink cherry blossom tree | EXISTS |
| `SM_Veg_KonohaShrub_01` | Ground shrub | EXISTS |
| `SM_Veg_GrassTuft_Stylized` | Grass patch | EXISTS |

#### Props (11 types)
| Asset | Description | Status |
|---|---|---|
| `SM_Prop_PaperLantern_Hanging` | Warm amber hanging lantern | EXISTS |
| `SM_Prop_StoneLantern_01` | Stone garden lantern | EXISTS |
| `SM_Prop_Konoha_Bench_01` | Wooden bench | EXISTS |
| `SM_Prop_ShinobiSupply_Crate` | Wooden crate | EXISTS |
| `SM_Prop_RainBarrel_Wood` | Water barrel | EXISTS |
| `SM_Prop_ShopBanner_Ichiraku` | Shop fabric banner | EXISTS |
| `SM_Prop_WoodenUtilityPole` | Utility pole with wires | EXISTS |
| `SM_Prop_TrainingLog_Post` | Training ground post | EXISTS |
| `SM_Prop_WaterPumpingStation` | Water pump | EXISTS |
| `SM_Prop_Konoha_TrashBin` | Village trash bin | EXISTS |
| `BP_TrainingDummy` | Practice target dummy | EXISTS |

#### Landmarks
| Asset | Description | Status |
|---|---|---|
| `SM_Lnd_HokageFaces_Monument` | Distant Hokage rock faces | EXISTS |
| `SM_Lnd_KonohaFrontierGate` | Village entrance gate | EXISTS |

---

## Environment Composition Layout

```
                    ┌─────────────────────────────────────────┐
                    │              HOKAGE MONUMENT             │
                    │           (distant background)           │
                    ├─────────────────────────────────────────┤
                    │     HOKAGE MANSION (mid-background)      │
                    │                                          │
                    │  ┌──────┐  ┌──────┐  ┌──────┐          │
                    │  │HOUSE │  │HOUSE │  │TEA   │  CEDAR   │
                    │  │  A   │  │  B   │  │HOUSE │  TREES   │
                    │  └──────┘  └──────┘  └──────┘          │
    SAKURA TREES    │                                          │
                    │  ════════════════════════════════════    │
                    │  ║    COBBLESTONE MAIN AVENUE      ║    │
                    │  ║                                  ║    │
                    │  ║  NARUTO ──►   NPCs   LANTERNS  ║    │
                    │  ║              BENCHES  CRATES    ║    │
                    │  ════════════════════════════════════    │
                    │                                          │
                    │  ┌──────────┐  ┌──────┐  ┌──────┐      │
                    │  │ICHIRAKU  │  │WEAPON│  │MISSION│      │
                    │  │RAMEN     │  │SHOP  │  │OFFICE │      │
                    │  └──────────┘  └──────┘  └──────┘      │
                    │                                          │
                    │        PROPS  SHRUBS  UTILITY POLES      │
                    └─────────────────────────────────────────┘
                              50m × 50m
```

---

## Architectural Language

### Konoha Architecture Rules
- Traditional Japanese timber construction
- Warm brown wood grain (painted, not photorealistic)
- Clay/ceramic tiled gabled roofs
- Paper shoji screens and sliding doors
- Fabric noren curtains on shopfronts
- Hand-painted wooden signboards
- Balcony balustrades with wooden railings
- Stone/brick foundations
- Wooden window frames

### Architecture DO NOTs
- No modern materials (glass, steel, concrete)
- No sci-fi elements
- No generic Western architecture
- No identical copy-pasted buildings
- No pure white untextured walls
- No pitch-black doorways

---

## Sky & Atmosphere

| Element | Specification |
|---|---|
| SkyAtmosphere | Enabled, clear daytime |
| Sky color | Blue/cyan with warm haze near horizon |
| Clouds | Soft, painterly, volumetric or sprite |
| Sun position | ~45° elevation, warm golden (5400K) |
| Atmospheric fog | Subtle, depth-based, warm at distance |
| Distant mountains | Silhouette visible behind Hokage Monument |
| Time of day | Warm afternoon (golden hour approaching) |

---

## World Factory Data Contract

```cpp
USTRUCT(BlueprintType)
struct FEOSWorldVisualProfile
{
    GENERATED_BODY()

    UPROPERTY() FName WorldID;                 // "WORLD_KONOHA"
    UPROPERTY() FName LandID;                  // "LAND_FIRE"
    UPROPERTY() FName VillageID;               // "VILLAGE_KONOHA"
    UPROPERTY() FName DistrictID;              // "DISTRICT_GOLDEN_STREET"
    UPROPERTY() FName BiomeID;                 // "BIOME_TEMPERATE_FOREST"
    UPROPERTY() FName ArchitectureProfile;     // "ARCH_JAPANESE_TIMBER"
    UPROPERTY() FName TerrainProfile;          // "TERRAIN_COBBLESTONE_WOOD"
    UPROPERTY() FName MaterialProfile;         // "MAT_WARM_PAINTERLY"
    UPROPERTY() FName FoliageProfile;          // "FOLIAGE_CEDAR_SAKURA"
    UPROPERTY() FName WeatherProfile;          // "WEATHER_CLEAR_WARM"
    UPROPERTY() FName LightingProfile;         // "LIGHT_GOLDEN_AFTERNOON"
    UPROPERTY() FName NPCProfile;              // "NPC_KONOHA_VILLAGERS"
    UPROPERTY() FName EnemyProfile;            // "ENEMY_ROGUE_SHINOBI"
    UPROPERTY() FName BossProfile;             // "BOSS_KAKASHI_TRAINING"
    UPROPERTY() FName AudioProfile;            // "AUDIO_KONOHA_VILLAGE"
    UPROPERTY() FName VFXProfile;              // "VFX_PEACEFUL_VILLAGE"
    UPROPERTY() FName CameraProfile;           // "CAM_EXPLORATION"
};
```

---

## Konoha Expansion Path

```
Phase 6:   50m × 50m  (Golden Street — 1 district)
Phase 12: 100m × 100m (+ Training Ground, Gate area)
Phase 13: 250m × 250m (+ Academy, Hokage Tower approach)
Phase 14: 500m × 500m (+ Forest of Death edge, rooftop area)
```

**DO NOT expand beyond 50m until Phase 6 visual gate passes.**
