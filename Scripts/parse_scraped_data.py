import re
import os

char_file = r"C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\801206fe-4fd9-48a7-9f2a-758587bf1a55\.system_generated\steps\1409\content.md"
stage_file = r"C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\801206fe-4fd9-48a7-9f2a-758587bf1a55\.system_generated\steps\1413\content.md"

with open(char_file, "r", encoding="utf-8") as f:
    c_text = f.read()

# Pattern for character items: data-src="detail.php?c=(.*?)" ... alt="(.*?)"
char_matches = re.findall(r'data-src="detail\.php\?c=([^"]+)".*?alt="([^"]+)"', c_text, re.DOTALL)
print(f"Total characters found: {len(char_matches)}")

with open(stage_file, "r", encoding="utf-8") as f:
    s_text = f.read()

stage_matches = re.findall(r'data-src="detail\.php\?s=([^"]+)".*?alt="([^"]+)"', s_text, re.DOTALL)
print(f"Total stages found: {len(stage_matches)}")

out_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\VisualBible\SCRAPED_CONNECTIONS_CATALOG.txt"
with open(out_path, "w", encoding="utf-8") as out:
    out.write("=== CHARACTERS CATALOG ===\n")
    for cid, name in char_matches:
        out.write(f"{cid}: {name}\n")
    out.write("\n=== STAGES CATALOG ===\n")
    for sid, name in stage_matches:
        out.write(f"{sid}: {name}\n")

print(f"Wrote catalog to {out_path}")
