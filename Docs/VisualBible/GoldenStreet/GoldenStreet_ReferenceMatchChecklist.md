# GOLDEN STREET REFERENCE MATCH CHECKLIST
## NARUTO: ECHOES OF SHINOBI | Unreal Engine 5.8
- **Benchmark Target:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01` (`Konoha_Visual_North_Star.jpg`)

---

### Non-Negotiable Art Direction Acceptance Criteria

| Check # | System | Target Reference Requirement | Current Status | Gate Dependency |
| :---: | :--- | :--- | :---: | :---: |
| **01** | **Naruto Hero** | Watertight 3D anime mesh, sculpted spiky hair clumps, metal headband with Konoha crest, upright white collar, orange tracksuit with navy panels and 3D back red swirl, thigh bandage with kunai pouch, open-toe sandals. Humanoid skeleton with skin weights and full locomotion. | **BLOCKED** | Gate 2 |
| **02** | **Ichiraku Ramen** | Authentic post-and-beam timber alcove, recessed wooden dining counter with 4 rounded stools, split wave fabric noren ("一楽ラーメン"), hanging red lanterns with warm amber glow, Teuchi behind counter. | **BLOCKED** | Gate 3 |
| **03** | **Konoha Residences** | Two-story traditional Japanese timber architecture, cantilevered balcony with balustrade railings, framed sliding lattice shoji windows, multi-tiered sloped roofs with curved terracotta tiles and ridge caps. | **BLOCKED** | Gate 3 |
| **04** | **Street Avenue** | Wide thoroughfare (facades at $X = \pm 680$), light grey sandstone cobblestone paving with varied stone tints, raised stone curbs ($Z=+10$), drainage gutters, dirt weathering along building bases. | **BLOCKED** | Gate 4 |
| **05** | **Vista Sightline** | Unobstructed vanishing point leading directly to the towering Hokage Rock cliff monument (Hashirama, Tobirama, Hiruzen, Minato faces) under an azure sky with painterly clouds. | **PASS** | Gate 4 |
| **06** | **Foliage & VFX** | Overhanging branching Sakura cherry blossom trees with drifting windblown pink petal Niagara particle system (`NS_SakuraPetals_Drift`). Leaves set to `NoCollision`. | **BLOCKED** | Gate 6 |
| **07** | **Street Life & NPCs** | 8+ animated NPCs (Teuchi cooking, Genin squads in conversation, flak-jacket Chunin guard, pedestrians) with non-blocking collision. | **BLOCKED** | Gate 6 |
| **08** | **Lighting & Tone** | 5400K warm golden afternoon daylight, soft azure SkyLight fill (clamped at 0.22 shadow floor to prevent crushed blacks), manual exposure EV 9.8, zero highlight clipping on orange cloth. | **HOLD** | Gate 5 |
| **09** | **Anime RPG HUD** | Circular gold Naruto portrait medallion, rounded gloss HP & Chakra pill bars bound to live GAS attributes, circular parchment minimap, comic combo counter, 4-way diamond jutsu cross. | **PASS (Logic) / HOLD (Art)** | Gate 6 |
| **10** | **Measured Performance** | 1920×1080 standalone execution maintaining $\ge 60\text{ FPS}$ (16.67 ms overall frame budget) on target NVIDIA GTX 1650 hardware. | **HOLD** | Gate 7 |
