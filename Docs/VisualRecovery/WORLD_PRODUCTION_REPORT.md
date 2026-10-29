# WORLD PRODUCTION & MULTI-NATION STAGE PIPELINE REPORT
**Project:** NARUTO: ECHOES OF SHINOBI (UE 5.8)  
**Permanent Governance Anchor:** GEMINI.md World Scope & Golden Street 50m Acceptance Standard  
**Benchmark Target Map:** `/Game/Maps/GoldenTest/L_Konoha_GoldenStreet`  

---

## 1. Reusable World Assembly Architecture

World spaces in ECHOES OF SHINOBI are not disconnected combat platforms. They are structured into traversable, streaming-aware hierarchies:

```
WORLD PIPELINE
└── WORLD (Shinobi Continent)
    └── LAND (e.g., Land of Fire / Hi no Kuni)
        └── SETTLEMENT (e.g., Konohagakure no Sato)
            └── DISTRICT (e.g., Konoha Central Golden District)
                └── MAP (e.g., L_Konoha_GoldenStreet)
                    └── ZONE (e.g., Commercial Shopping Avenue)
                        └── LANDMARK (e.g., Ichiraku Ramen Shop)
                            ├── BUILDING (Timber post-and-beam architecture)
                            ├── TERRAIN (Paved cobblestone avenue with stone curbs)
                            ├── PROPS (Benches, crates, barrels, lanterns, utility poles)
                            ├── NPCS (Teuchi, Chunin guards, civilians, academy students)
                            ├── ENCOUNTERS (Training sparring, dialogue, quests)
                            └── WORLD STATE (Daylight 5400K, weather, destruction flags)
```

---

## 2. Five Great Shinobi Nations & Secondary Realms Scope

### 1. Land of Fire (*Hi no Kuni*) — The Benchmark Realm
- **Konoha Golden Street (Primary Map):** 50m vertical slice connecting the Frontier Gate to the central avenue; expanding to 100m crossroads and 500m village district (`L_Konoha_Phase13_District`).
- **Hero Landmark: Ichiraku Ramen Shop:** Post-and-beam cedar timber construction, mortise-and-tenon joints, recessed counter with 4 rounded stools, three-section split fabric *noren* ("一楽ラーメン"), and hanging red paper lanterns emitting warm amber glow ($120\text{ cd/m}^2$).
- **Hokage Monument Vista:** Towing granite cliff monument carving the faces of Hashirama, Tobirama, Hiruzen, and Minato on the vanishing axis, framed behind the red circular Hokage Mansion pagoda.
- **Connected Survival Training Ground:** Three wooden training posts, boundary log fences, riverbank water transition, and a direct forest trail connecting into Konoha village gates.
- **Forest of Death & Naka Shrine:** Dense cedar canopy, towering roots, submerged stone shrines, and secret Uchiha underground tablet chambers.

### 2. Land of Wind (*Kaze no Kuni*)
- **Sunagakure:** Monolithic canyon fissure entry gates, dense cylindrical adobe dome houses, stepped terraces, and fabric sun-shades.
- **Traversal:** Vast wind-blown sand dunes, desert sandstone arches, oasis caravanserais, and subterranean water aqueducts.
- **Atmosphere:** High-lux desert sun ($16.0\text{ Lux}$), heat shimmer distortion, drifting dust particles, and pale ochre skies.

### 3. Land of Water (*Mizu no Kuni*)
- **Kirigakure:** Coastal port settlements, elevated stilt houses, thatched moss-covered gabled roofs, iron mooring chains, and wooden docks.
- **Traversal:** Canals spanned by arched wooden bridges, mist-shrouded alleyways, and vertical cliff-face bamboo ladders.
- **Atmosphere:** Cool sea breeze, dense volumetric moisture fog, wet cobblestones with specular reflections, and damp teal lighting.

### 4. Land of Lightning (*Kaminari no Kuni*)
- **Kumogakure:** Jagged mountain peak temples rising above sea of clouds, cantilevered wooden platforms, and multi-tiered pagodas.
- **Traversal:** High-altitude rope suspension bridges over bottomless chasms, thundering Falls of Truth, and lightning-scorched plateaus.
- **Atmosphere:** Crisp high-altitude lighting, drifting cloud layers below cliff edges, and thundering atmospheric ambiance.

### 5. Land of Earth (*Tsuchi no Kuni*)
- **Iwagakure:** Fortress settlements carved directly into vertical granite cliffs, monolithic stone halls, and clay-mortared slate roofs.
- **Traversal:** Narrow mountain passes, stone bridges spanning rocky gorges, defensive stone gatehouses, and terraced quarry paths.
- **Atmosphere:** High contrast canyon shadows, arid dust haze, and rich mineral-tinted stone textures.

### 6. Secondary Realms & Hero Battlefields
- **Amegakure (Village Hidden in Rain):** Industrial metallic skyscrapers, endless rainwater cascades, rusted exterior piping, and iron suspension wires under dark slate skies.
- **Land of Iron:** Samurai council lodge, frozen timber bridges, blizzard snowfields, and frosted pine forests.
- **Valley of the End:** Gigantic cliffside statues of Hashirama Senju and Madara Uchiha with hands of confrontation, framing the roaring waterfall chasm.
- **Special Realms:** Kamui dimensional pocket realm (floating monochrome blocks), Mount Myōboku (toad temples, giant lotus leaves, oil waterfalls), and Divine Tree battlefields.

---

## 3. Stage Production Standard (65-Stage Canonical Template)

Each stage implementation record tracks:
- **Stage ID:** Canonical identifier (e.g., `STAGE_KONOHA_GOLDEN_STREET`)
- **Category:** Open World / Combat Arena / Story Mission / Training Ground / Boss Arena
- **Era & Variants:** Part 1, Shippuden, War Era; Day, Sunset, Night, Rain, Destroyed
- **Boundaries & Traversal:** NavMesh bounds, collision fences, vertical wall-run volumes
- **Landmarks & Props:** Authored architectural models, interactive objects, training dummies
- **Atmosphere & Lighting:** Assigned Environment Profile (`EP_*`), fog parameters, skybox
- **NPC Population:** Ambient civilians, shopkeepers, sentries, and spawner policies
- **Performance Budget:** $\le 16.67\text{ ms}$ total frame time on GTX 1650 at native 1080p

---

## 4. Current Map Audit & Remediation: `L_Konoha_GoldenStreet`

| Actor Category | Count | Status | Required Remediation |
| :--- | :--- | :--- | :--- |
| **Sky & Lighting** | 4 | Complete | Calibrated Sun (10 Lux), SkyLight (3.0 cd/m²), Sunset Haze Fog |
| **Avenue Surface** | 13 | Complete | Modular paved road tiles (`SM_Road_Center`) & sidewalks |
| **Hero Landmark (Ichiraku)**| 5 | Complete | Timber shopfront, 3 hanging paper lanterns, noren sign |
| **Commercial Facades** | 6 | Complete | Weapon Shop, Mission Office, Residence A/B, Tea House |
| **Vista Anchors** | 2 | Complete | Hokage Mansion mid-vista & Hokage Rock backdrop |
| **Street Furniture / Props** | 12 | Complete | Benches, supply crates, rain barrels, utility poles |
| **Vegetation** | 7 | Complete | Large Konoha Cedars & Flowering Sakura Cherry Blossoms |
| **NPC Population** | 5 | Inactive in test | Activate Teuchi and civilian idle routines |

