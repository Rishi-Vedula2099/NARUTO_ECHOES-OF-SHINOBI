import os
import re

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
DOCS_DIR = os.path.join(PROJECT_DIR, "Docs", "VisualBible")
BRAIN_DIR = r"C:\Users\Rishi Vedula\.gemini\antigravity\brain\e7bfe4a8-64ad-419a-8219-fdb94cff68bd"

MASTER_PLAN_PATH = os.path.join(DOCS_DIR, "EOS_MASTER_PRODUCTION_PLAN.md")
CHAR_ATLAS_PATH = os.path.join(DOCS_DIR, "MASTER_CHARACTER_BENCHMARK_ATLAS.md")
STAGE_ATLAS_PATH = os.path.join(DOCS_DIR, "MASTER_STAGE_BENCHMARK_ATLAS.md")

with open(MASTER_PLAN_PATH, "r", encoding="utf-8") as f:
    master_text = f.read()

with open(CHAR_ATLAS_PATH, "r", encoding="utf-8") as f:
    char_text = f.read()

with open(STAGE_ATLAS_PATH, "r", encoding="utf-8") as f:
    stage_text = f.read()

# Extract the registry tables from char_text and stage_text
char_section_idx = char_text.find("## 2. Complete 157-Character Benchmark Registry & Specifications")
char_registry_content = char_text[char_section_idx:] if char_section_idx != -1 else char_text

stage_section_idx = stage_text.find("## 2. Complete 65-Stage Canonical Benchmark Registry")
stage_registry_content = stage_text[stage_section_idx:] if stage_section_idx != -1 else stage_text

# Update Section 2 in Master Plan:
# Insert the full Connections character benchmark registry under Section 2
sec2_marker = "## 2. The 5-Component Character Factory Standard"
sec2_replacement = f"""## 2. The 5-Component Character Factory Standard & Storm Connections Roster Benchmark

### Official Storm Connections Character Visual Benchmark
Directly referencing the official Bandai Namco / CyberConnect2 character catalog (`https://naruto-game-sc.bn-ent.net/character/`), every character in the production pipeline must achieve anatomical, geometric, and material parity with this standard:

````carousel
![Official Storm Connections Genin Naruto Benchmark](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/target_naruto_connections_card.jpg)
<!-- slide -->
![Naruto Genin Stance Reference](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/ref_naruto_genin_stance.png)
<!-- slide -->
![Naruto Genin Face Reference](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/ref_naruto_genin_face.png)
````

{char_registry_content}

---

### The 5-Component Character Production Standard
"""

if sec2_marker in master_text:
    master_text = master_text.replace(sec2_marker, sec2_replacement)
    print("Embedded Character Benchmark Registry into Section 2.")
else:
    print("Section 2 marker not found!")

# Update Section 3 in Master Plan:
# Insert the full 65-stage benchmark registry under Section 3
sec3_marker = "## 3. Story-Critical World Atlas (Narrative-Driven Land Priority)"
sec3_replacement = f"""## 3. Story-Critical World Atlas & 65-Stage Canonical Benchmark

### Official Storm Connections Environmental Benchmark: Valley of the End
Directly referencing the official Bandai Namco / CyberConnect2 stage catalog (`https://naruto-game-sc.bn-ent.net/stage/`), the environmental landmark hierarchy must achieve parity with the official benchmark:

````carousel
![Official Storm Connections Benchmark: Valley of the End](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/target_valley_of_the_end_connections.jpg)
<!-- slide -->
![Official Target: Konoha North Star](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/target_konoha_north_star.jpg)
<!-- slide -->
![Official Target: Hokage Monument](C:/Users/Rishi%20Vedula/.gemini/antigravity/brain/e7bfe4a8-64ad-419a-8219-fdb94cff68bd/target_hokage_monument.jpg)
````

{stage_registry_content}

---

### Story-Critical Narrative Land Architecture
"""

if sec3_marker in master_text:
    master_text = master_text.replace(sec3_marker, sec3_replacement)
    print("Embedded Stage Benchmark Registry into Section 3.")
else:
    print("Section 3 marker not found!")

with open(MASTER_PLAN_PATH, "w", encoding="utf-8") as f:
    f.write(master_text)
print(f"Updated {MASTER_PLAN_PATH} ({len(master_text)} characters)")

# Mirror to brain artifact dir
brain_plan_path = os.path.join(BRAIN_DIR, "EOS_MASTER_PRODUCTION_PLAN.md")
with open(brain_plan_path, "w", encoding="utf-8") as f:
    f.write(master_text)
print(f"Mirrored to brain artifact dir: {brain_plan_path}")
