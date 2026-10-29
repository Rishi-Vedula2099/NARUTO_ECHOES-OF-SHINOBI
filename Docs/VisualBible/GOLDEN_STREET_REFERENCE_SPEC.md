# GOLDEN STREET VISUAL REFERENCE SPECIFICATION
## NARUTO: ECHOES OF SHINOBI | Unreal Engine 5.8
**Document:** `Docs/VisualBible/GOLDEN_STREET_REFERENCE_SPEC.md`  
**Primary Reference:** `EOS_GOLDEN_VISUAL_REFERENCE_KONOHA_01` (`Docs/Screenshots/Konoha_Visual_North_Star.jpg`, 1376x768)  
**Standard Coordinate Frame:** Normalized Screen Coordinates $[0.0, 1.0] \times [0.0, 1.0]$ where $(0.0, 0.0)$ is Top-Left and $(1.0, 1.0)$ is Bottom-Right.

---

### A. Character Visual Specification (Naruto Genin)

#### 1. Screen Framing & Scale
* **Character Screen Height:** $0.280 \pm 0.020$ (occupies approximately $28\%$ of total viewport height).
* **Horizontal Center Position:** $X = 0.500 \pm 0.025$ (centered on the avenue perspective vanishing point).
* **Vertical Anchor (Feet/Ground Contact):** $Y = 0.900 \pm 0.020$ (feet grounded in the lower tenth of the screen, completely visible with clear cast shadow).
* **Head Top Coordinate:** $Y \approx 0.620$.

#### 2. Silhouette & Anatomy
* **Proportions:** Stylized anime proportions, head-to-body ratio $\sim 1:5.5$ (characteristic of a 12-year-old Genin).
* **Posture:** Active third-person shinobi stance, legs slightly apart, relaxed shoulders, head tilted slightly toward the horizon vanishing point.
* **Hair Silhouette:** Clustered spiky blonde locks arranged in radial star clumps tapering to crisp stylized anime tips; distinct volume framing the metal headband.
* **Headband:** Navy fabric band ($H \approx 0.022$) tied at the nape with flowing fabric tails; rectangular polished metal Leaf village forehead protector ($W \approx 0.040, H \approx 0.018$) with distinct Konoha spiral swirl engraving and 6 corner rivet impressions.

#### 3. Costume Construction & Color Blocking
* **Jacket Torso:** Saturated cadmium orange (`#F26A1B`), stand-up white padded collar (`#F5F5FA`), dark navy shoulder yoke and sleeve trim (`#1C2638`).
* **Back Crest:** Crisp crimson Uzumaki swirl crest (`#C82020`) centered between shoulder blades.
* **Trousers:** Matching cadmium orange with loose cloth fold breaks above the knee and elasticated leg wraps.
* **Holster:** White tape bandage wrap on right thigh with black leather weapon pouch.
* **Sandals:** Dark navy open-toe shinobi sandals with wrapped heels, defined sole thickness, and exposed toes with individual stylized cel definition.

---

### B. Environment Visual Specification (Konoha Central Avenue)

#### 1. Street Perspective & Vanishing Point
* **Perspective Horizon:** $Y = 0.580 \pm 0.015$ (placed slightly below screen center to emphasize towering Hokage monument and open sky).
* **Vanishing Point:** $X = 0.500, Y = 0.580$.
* **Road Base Width (Foreground):** Extends from $X = 0.180$ to $X = 0.820$ ($W \approx 0.640$ normalized screen width at $Y = 0.950$).
* **Road Vanishing Width (Midground):** Tapers down to $W \approx 0.080$ at $Y = 0.580$.
* **Paving Material:** Hand-laid grey cobblestone slabs with subtle dirt accumulation along edges, bounded on both sides by raised stone curbs ($H \approx 0.015$).

#### 2. Left Architectural Massing (Ichiraku Ramen & Shops)
* **Foreground Shop Envelope:** $X = [0.000, 0.320]$, $Y = [0.380, 0.950]$.
* **Structure:** Traditional Japanese timber post-and-beam construction with weathered cedar pillars.
* **Dining Counter & Noren:** Open dining bar recessed under eaves ($Y = 0.650..0.820$) with 4-panel off-white fabric noren curtains bearing bold black "ラーメン" kanji.
* **Lanterns:** Hanging paper lanterns at eave corners ($X \approx 0.280, Y \approx 0.520$) with warm amber emissive interior fill (`#FFAA33`).
* **Roof Profile:** Asymmetrical gabled tile roof with traditional terracotta wave tiles (`Kawara`) sloping down toward the street at $\sim 28^\circ$.

#### 3. Right Architectural Massing (Konoha Residences & Stores)
* **Foreground Residence Envelope:** $X = [0.680, 1.000]$, $Y = [0.320, 0.950]$.
* **Upper Balcony:** Cantilevered wooden balcony on second floor ($Y = 0.450..0.550$) with vertical timber balustrades.
* **Windows:** Recessed Japanese lattice windows (`Shoji`/timber slats) with soft interior warm ambient reflection.
* **Roof Profile:** Two-tier gabled roofs with authentic overhanging rafter extensions and dark charcoal-slate tile finish.

#### 4. Vista & Background Depth Layers
* **Layer 1 (Foreground Street):** $Y = 0.600..1.000$ (Player, Ichiraku, Residence, paving).
* **Layer 2 (Midground Village):** $Y = 0.500..0.600$ (Transverse utility poles, banner wires, midground tiled roofs, circular red Hokage Mansion pagoda at $X = 0.480..0.520, Y = 0.520..0.580$).
* **Layer 3 (Hokage Rock Cliff Monument):** $X = [0.300, 0.700]$, $Y = [0.220, 0.500]$. Carved sandstone mountain faces of the four Hokage (Hashirama, Tobirama, Hiruzen, Minato) catching warm afternoon sunlight.
* **Layer 4 (Sky & Atmosphere):** $Y = [0.000, 0.350]$. Clear painterly anime azure blue sky with sweeping soft white cumulus clouds lit with golden ambient rimming.

#### 5. Vegetation Distribution
* **Cherry Blossom (Sakura):** Dominates upper left framing ($X = [0.000, 0.250]$, $Y = [0.000, 0.350]$) with overhanging clusters of vibrant pink blossoms (`#FFAEC9`) catching golden key light.
* **Japanese Cedars (Sugi):** Flank middle vista ($X = [0.260, 0.360]$ and $X = [0.640, 0.740]$, $Y = [0.350, 0.580]$) with layered deep forest pine green foliage clusters (`#1C442A`).

---

### C. Rendering & Color-Grading Pipeline

#### 1. Color Palette & Temperature
* **Key Light:** Warm golden afternoon sun at $5400\text{ K}$, incident angle $\sim 45^\circ$ elevation, $35^\circ$ azimuth from camera right.
* **Sky Fill:** Soft azure blue sky fill light (`#6898C8`) creating rich, colorful anime shadows without crushed pitch-black zones.
* **Shadow Tone:** Warm indigo-grey shadow band transition ($L \approx 0.35..0.45$), never falling below $0.15$ luminance.

#### 2. Cel Shading & Material Response
* **Step Transition:** 2-band stepped anime shading using `Dot(N, L)` clamped ramp with smooth $0.05$ boundary softness.
* **Specular & Rim:** Controlled Fresnel rim lighting (soft warm highlight on character silhouette and building eaves) with strictly clamped emissive ($< 1.2$ max).
* **Base Roughness:** Matte anime cloth response ($0.75..0.90$ for fabrics), polished semi-matte for wood ($0.55$), metallic plate ($0.15$ roughness, $0.90$ metallic).

#### 3. Exposure & Post-Process
* **Exposure Mode:** Manual EV with target brightness range $\text{EV} \approx 9.5 - 10.0$ at midday/afternoon.
* **Highlights:** No white blowout on character skin or sky clouds.
* **Bloom:** Tight Gaussian bloom kernel with high threshold ($1.05$) to prevent blinding glare on lights or signs.

---

### D. Presentation & UI Specification

#### 1. Camera Configuration
* **Field of View (FOV):** $70^\circ$ horizontal FOV for natural, cinematic anime action RPG perspective.
* **Target Arm Length:** $480.0\text{ cm}$ (calibrated to preserve Naruto at $28\%$ screen height).
* **Camera Pitch:** $-8.5^\circ$ down toward the street vanishing point.
* **Camera Offset:** Right shoulder bias $+15.0\text{ cm}$ ($X = 0, Y = 15, Z = 65$).

#### 2. HUD Architecture & Layout
* **Top-Left Player Status:** $X = [0.020, 0.220], Y = [0.025, 0.120]$. Circular character portrait medallion ($D \approx 0.065$) with curved green HP gauge, blue Chakra gauge, and stamina indicators.
* **Top-Right Minimap:** $X = [0.820, 0.980], Y = [0.025, 0.180]$. Circular translucent Konoha minimap with compass bearing and region banner ("Konoha — Main Street").
* **Bottom-Right Jutsu Dock:** $X = [0.780, 0.980], Y = [0.800, 0.960]$. 4 diamond jutsu skill icons with cooldown sweeps and input binding labels (`[Q]`, `[E]`, `[R]`, `[F]`).
* **Active Mission Tracker:** $X = [0.020, 0.200], Y = [0.220, 0.320]$. Subdued translucent scroll panel with current active objective.
