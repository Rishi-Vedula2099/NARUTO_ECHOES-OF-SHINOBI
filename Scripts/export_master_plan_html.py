import os
import re
import base64

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
PLAN_MD_PATH = os.path.join(PROJECT_DIR, "Docs", "VisualBible", "EOS_MASTER_PRODUCTION_PLAN.md")
REF_DIR = os.path.join(PROJECT_DIR, "Docs", "References", "MasterPlan")
BRAIN_DIR = r"C:\Users\Rishi Vedula\.gemini\antigravity\brain\e7bfe4a8-64ad-419a-8219-fdb94cff68bd"

DESKTOP_DIR = r"C:\Users\Rishi Vedula\Desktop"
DOWNLOADS_DIR = r"C:\Users\Rishi Vedula\Downloads"

with open(PLAN_MD_PATH, "r", encoding="utf-8") as f:
    md_content = f.read()

def safe_write(filepath, content):
    try:
        with open(filepath, "w", encoding="utf-8") as f:
            f.write(content)
        print(f"Successfully wrote: {filepath}")
        return True
    except Exception as e:
        print(f"File locked ({e}), saving to alternative filename...")
        stem, ext = os.path.splitext(filepath)
        alt_path = f"{stem}_UPDATED{ext}"
        try:
            with open(alt_path, "w", encoding="utf-8") as f:
                f.write(content)
            print(f"Successfully wrote alternative: {alt_path}")
            return True
        except Exception as e2:
            print(f"Failed to write alternative ({e2})")
            return False

# Write Markdown to Desktop & Downloads
for d in [DESKTOP_DIR, DOWNLOADS_DIR]:
    target_md = os.path.join(d, "EOS_MASTER_PRODUCTION_PLAN.md")
    safe_write(target_md, md_content)

# Helper to get base64 for image
def get_image_base64(img_path_or_name):
    clean_name = os.path.basename(img_path_or_name.strip("() \t\r\n"))
    local_path = os.path.join(REF_DIR, clean_name)
    if not os.path.exists(local_path):
        art_path = os.path.join(BRAIN_DIR, clean_name)
        if os.path.exists(art_path):
            local_path = art_path
        else:
            return ""
    mime = "image/png" if clean_name.lower().endswith(".png") else "image/jpeg"
    try:
        with open(local_path, "rb") as img_f:
            encoded = base64.b64encode(img_f.read()).decode('utf-8')
        return f"data:{mime};base64,{encoded}"
    except Exception as e:
        return ""

# Convert Markdown to HTML
lines = md_content.split("\n")
html_body = []
in_table = False
table_header = True
in_code = False
code_lang = ""
code_lines = []

i = 0
while i < len(lines):
    line = lines[i]
    
    # Fenced code blocks & carousels
    if line.startswith("````") or line.startswith("```"):
        if in_code:
            in_code = False
            raw_code = "\n".join(code_lines)
            if code_lang == "carousel":
                slides = raw_code.split("<!-- slide -->")
                html_body.append('<div class="carousel-container"><div class="carousel-track">')
                for s in slides:
                    s_clean = s.strip()
                    m = re.search(r'!\[(.*?)\]\((.*?)\)', s_clean)
                    if m:
                        alt = m.group(1)
                        src = m.group(2)
                        b64 = get_image_base64(src)
                        img_tag = f'<img src="{b64}" alt="{alt}" class="carousel-img" />' if b64 else f'<div class="missing-img">[{alt}]</div>'
                        html_body.append(f'<div class="carousel-slide">{img_tag}<div class="carousel-caption">{alt}</div></div>')
                html_body.append('</div></div>')
            else:
                html_body.append(f'<pre><code class="language-{code_lang}">{raw_code}</code></pre>')
            code_lines = []
            code_lang = ""
        else:
            in_code = True
            code_lang = line.strip("` \t\r\n")
            code_lines = []
        i += 1
        continue
        
    if in_code:
        code_lines.append(line)
        i += 1
        continue

    # Tables
    if "|" in line:
        cols = [c.strip() for c in line.split("|")[1:-1]]
        if all(re.match(r'^:?-+:?$', c) for c in cols if c):
            i += 1
            table_header = False
            continue
            
        if not in_table:
            in_table = True
            table_header = True
            html_body.append('<div class="table-wrapper"><table>')
            
        tag = "th" if table_header else "td"
        row_html = "<tr>" + "".join(f"<{tag}>{c}</{tag}>" for c in cols) + "</tr>"
        html_body.append(row_html)
        i += 1
        continue
    else:
        if in_table:
            in_table = False
            html_body.append('</table></div>')

    # Headings
    if line.startswith("# "):
        html_body.append(f'<h1>{line[2:]}</h1>')
    elif line.startswith("## "):
        html_body.append(f'<h2>{line[3:]}</h2>')
    elif line.startswith("### "):
        html_body.append(f'<h3>{line[4:]}</h3>')
    elif line.startswith("#### "):
        html_body.append(f'<h4>{line[5:]}</h4>')
    elif line.startswith("---"):
        html_body.append('<hr/>')
    elif line.strip() == "":
        pass
    else:
        # Check standalone image
        m = re.match(r'!\[(.*?)\]\((.*?)\)', line.strip())
        if m:
            alt = m.group(1)
            src = m.group(2)
            b64 = get_image_base64(src)
            if b64:
                html_body.append(f'<div class="standalone-img"><img src="{b64}" alt="{alt}" /><div class="caption">{alt}</div></div>')
            else:
                html_body.append(f'<div class="missing-img">[{alt}]</div>')
        else:
            # Inline bold & code
            p_text = line
            p_text = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', p_text)
            p_text = re.sub(r'`(.*?)`', r'<code>\1</code>', p_text)
            html_body.append(f'<p>{p_text}</p>')
            
    i += 1

if in_table:
    html_body.append('</table></div>')

content_html = "\n".join(html_body)

full_html = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>EOS Master Production Plan — Naruto: Echoes of Shinobi</title>
<style>
    :root {{
        --bg-color: #0d1117;
        --card-bg: #161b22;
        --text-color: #c9d1d9;
        --text-bright: #f0f6fc;
        --accent: #ff8c00;
        --accent-blue: #58a6ff;
        --border: #30363d;
        --header-bg: #1f242c;
    }}
    body {{
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
        background-color: var(--bg-color);
        color: var(--text-color);
        line-height: 1.6;
        margin: 0;
        padding: 40px;
    }}
    .document-container {{
        max-width: 1200px;
        margin: 0 auto;
        background: var(--card-bg);
        border: 1px solid var(--border);
        border-radius: 12px;
        padding: 50px 60px;
        box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }}
    h1 {{
        color: var(--accent);
        border-bottom: 2px solid var(--accent);
        padding-bottom: 15px;
        font-size: 2.4em;
    }}
    h2 {{
        color: var(--text-bright);
        border-bottom: 1px solid var(--border);
        padding-bottom: 10px;
        margin-top: 40px;
        font-size: 1.8em;
    }}
    h3 {{
        color: var(--accent-blue);
        margin-top: 30px;
        font-size: 1.4em;
    }}
    hr {{
        border: none;
        border-top: 1px solid var(--border);
        margin: 40px 0;
    }}
    code {{
        background: rgba(110,118,129,0.2);
        color: #ff7b72;
        padding: 0.2em 0.4em;
        border-radius: 4px;
        font-family: ui-monospace, SFMono-Regular, "SF Mono", Menlo, Consolas, monospace;
    }}
    pre code {{
        display: block;
        padding: 15px;
        background: #090d13;
        border: 1px solid var(--border);
        border-radius: 6px;
        overflow-x: auto;
        color: var(--text-color);
    }}
    .table-wrapper {{
        overflow-x: auto;
        margin: 25px 0;
    }}
    table {{
        width: 100%;
        border-collapse: collapse;
        font-size: 0.95em;
    }}
    th, td {{
        padding: 12px 16px;
        border: 1px solid var(--border);
        text-align: left;
    }}
    th {{
        background-color: var(--header-bg);
        color: var(--text-bright);
        font-weight: 600;
    }}
    tr:nth-child(even) {{
        background-color: rgba(255,255,255,0.02);
    }}
    .carousel-container {{
        background: #090d13;
        border: 1px solid var(--border);
        border-radius: 8px;
        padding: 20px;
        margin: 30px 0;
    }}
    .carousel-track {{
        display: flex;
        gap: 20px;
        overflow-x: auto;
        padding-bottom: 10px;
    }}
    .carousel-slide {{
        flex: 0 0 340px;
        background: var(--card-bg);
        border: 1px solid var(--border);
        border-radius: 6px;
        overflow: hidden;
        text-align: center;
    }}
    .carousel-img {{
        width: 100%;
        height: 220px;
        object-fit: cover;
        border-bottom: 1px solid var(--border);
    }}
    .carousel-caption {{
        padding: 10px;
        font-size: 0.88em;
        color: var(--accent);
        font-weight: 500;
    }}
    .standalone-img {{
        text-align: center;
        margin: 30px 0;
    }}
    .standalone-img img {{
        max-width: 100%;
        border-radius: 8px;
        border: 1px solid var(--border);
    }}
    .caption {{
        margin-top: 8px;
        font-size: 0.9em;
        color: #8b949e;
    }}
    .print-btn {{
        position: fixed;
        bottom: 30px;
        right: 30px;
        background: var(--accent);
        color: #000;
        font-weight: bold;
        border: none;
        padding: 14px 24px;
        border-radius: 30px;
        cursor: pointer;
        box-shadow: 0 4px 15px rgba(255, 140, 0, 0.4);
        font-size: 15px;
    }}
    @media print {{
        .print-btn {{ display: none; }}
        body {{ background: #fff; color: #000; padding: 0; }}
        .document-container {{ border: none; box-shadow: none; padding: 0; }}
    }}
</style>
</head>
<body>
<button class="print-btn" onclick="window.print()">Print to PDF</button>
<div class="document-container">
{content_html}
</div>
</body>
</html>
"""

for d in [DESKTOP_DIR, DOWNLOADS_DIR]:
    target_html = os.path.join(d, "EOS_MASTER_PRODUCTION_PLAN.html")
    safe_write(target_html, full_html)

print("EXPORT TO DESKTOP AND DOWNLOADS COMPLETED SUCCESSFULLY!")
