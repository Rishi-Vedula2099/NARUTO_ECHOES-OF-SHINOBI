import os
import re
import base64

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
PLAN_MD_PATH = os.path.join(PROJECT_DIR, "Docs", "VisualBible", "EOS_MASTER_PRODUCTION_PLAN.md")
REF_DIR = os.path.join(PROJECT_DIR, "Docs", "References", "MasterPlan")

DESKTOP_DIR = r"C:\Users\Rishi Vedula\Desktop"
DOWNLOADS_DIR = r"C:\Users\Rishi Vedula\Downloads"

with open(PLAN_MD_PATH, "r", encoding="utf-8") as f:
    md_content = f.read()

# Copy Markdown files directly to Desktop & Downloads
for d in [DESKTOP_DIR, DOWNLOADS_DIR]:
    target_md = os.path.join(d, "EOS_MASTER_PRODUCTION_PLAN.md")
    with open(target_md, "w", encoding="utf-8") as f:
        f.write(md_content)
    print(f"Copied Markdown to: {target_md}")

# Helper to get base64 for image
def get_image_base64(img_path_or_name):
    clean_name = os.path.basename(img_path_or_name.strip("() \t\r\n"))
    local_path = os.path.join(REF_DIR, clean_name)
    if not os.path.exists(local_path):
        # try checking artifact folder
        art_path = os.path.join(r"C:\Users\Rishi Vedula\.gemini\antigravity\brain\e7bfe4a8-64ad-419a-8219-fdb94cff68bd", clean_name)
        if os.path.exists(art_path):
            local_path = art_path
        else:
            return ""
    mime = "image/png" if clean_name.lower().endswith(".png") else "image/jpeg"
    with open(local_path, "rb") as img_f:
        encoded = base64.b64encode(img_f.read()).decode('utf-8')
    return f"data:{mime};base64,{encoded}"

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
                # Render carousel slides as flex grid with images
                slides = raw_code.split("<!-- slide -->")
                html_body.append('<div class="carousel-container"><div class="carousel-track">')
                for s in slides:
                    s_clean = s.strip()
                    m = re.search(r'!\[(.*?)\]\((.*?)\)', s_clean)
                    if m:
                        caption = m.group(1)
                        img_src = m.group(2)
                        b64 = get_image_base64(img_src)
                        src_attr = b64 if b64 else img_src
                        html_body.append(f'<div class="carousel-slide"><img src="{src_attr}" alt="{caption}"/><p class="caption">{caption}</p></div>')
                html_body.append('</div></div>')
            else:
                html_body.append(f'<pre><code class="language-{code_lang}">{raw_code}</code></pre>')
            code_lines = []
            code_lang = ""
            i += 1
            continue
        else:
            in_code = True
            code_lang = line.strip("` \t")
            code_lines = []
            i += 1
            continue

    if in_code:
        code_lines.append(line.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;"))
        i += 1
        continue

    # Tables
    if "|" in line and line.strip().startswith("|") and line.strip().endswith("|"):
        if not in_table:
            in_table = True
            table_header = True
            html_body.append('<div class="table-wrapper"><table>')
        
        cells = [c.strip() for c in line.strip().strip("|").split("|")]
        if all(re.match(r'^:?-+:?$', c) for c in cells):
            # separator row
            table_header = False
            i += 1
            continue
        
        if table_header:
            row_html = "<tr>" + "".join([f"<th>{c}</th>" for c in cells]) + "</tr>"
        else:
            # format cell content
            def fmt_c(val):
                val = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', val)
                val = re.sub(r'\*(.*?)\*', r'<em>\1</em>', val)
                val = re.sub(r'`(.*?)`', r'<code>\1</code>', val)
                return val
            row_html = "<tr>" + "".join([f"<td>{fmt_c(c)}</td>" for c in cells]) + "</tr>"
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
    elif line.startswith("* ") or line.startswith("- "):
        content = line[2:]
        content = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', content)
        content = re.sub(r'`(.*?)`', r'<code>\1</code>', content)
        html_body.append(f'<ul><li>{content}</li></ul>')
    elif re.match(r'^\d+\.\s', line):
        m = re.match(r'^\d+\.\s*(.*)', line)
        content = m.group(1)
        content = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', content)
        content = re.sub(r'`(.*?)`', r'<code>\1</code>', content)
        html_body.append(f'<ol><li>{content}</li></ol>')
    elif line.strip().startswith("$$") and line.strip().endswith("$$"):
        eq = line.strip("$\t ")
        html_body.append(f'<div class="math-block">{eq}</div>')
    elif line.strip() == "":
        pass
    else:
        p_line = line
        p_line = re.sub(r'\*\*(.*?)\*\*', r'<strong>\1</strong>', p_line)
        p_line = re.sub(r'\*(.*?)\*', r'<em>\1</em>', p_line)
        p_line = re.sub(r'`(.*?)`', r'<code>\1</code>', p_line)
        # Inline images
        img_m = re.search(r'!\[(.*?)\]\((.*?)\)', p_line)
        if img_m:
            caption = img_m.group(1)
            img_src = img_m.group(2)
            b64 = get_image_base64(img_src)
            src_attr = b64 if b64 else img_src
            p_line = f'<div class="img-center"><img src="{src_attr}" alt="{caption}"/><p class="caption">{caption}</p></div>'
        html_body.append(f'<p>{p_line}</p>')
    
    i += 1

if in_table:
    html_body.append('</table></div>')

content_html = "\n".join(html_body)

html_template = f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>EOS Master Production & Visual Execution Plan</title>
<style>
  :root {{
    --bg-primary: #0d1117;
    --bg-secondary: #161b22;
    --bg-tertiary: #21262d;
    --text-primary: #f0f6fc;
    --text-secondary: #8b949e;
    --accent-orange: #f56b00;
    --accent-blue: #388bfd;
    --accent-gold: #e3b341;
    --border-color: #30363d;
    --table-stripe: #1c2128;
  }}
  * {{ box-sizing: border-box; }}
  body {{
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
    background-color: var(--bg-primary);
    color: var(--text-primary);
    line-height: 1.6;
    margin: 0;
    padding: 30px;
  }}
  .container {{
    max-width: 1200px;
    margin: 0 auto;
    background: var(--bg-secondary);
    border: 1px solid var(--border-color);
    border-radius: 12px;
    padding: 40px;
    box-shadow: 0 10px 30px rgba(0,0,0,0.5);
  }}
  .header-bar {{
    display: flex;
    justify-content: space-between;
    align-items: center;
    border-bottom: 2px solid var(--accent-orange);
    padding-bottom: 20px;
    margin-bottom: 30px;
  }}
  .header-title {{
    margin: 0;
    color: var(--accent-orange);
    font-size: 26px;
    font-weight: 700;
    text-transform: uppercase;
    letter-spacing: 1px;
  }}
  .btn-print {{
    background: var(--accent-orange);
    color: #fff;
    border: none;
    border-radius: 6px;
    padding: 10px 20px;
    font-size: 14px;
    font-weight: 600;
    cursor: pointer;
    transition: background 0.2s;
  }}
  .btn-print:hover {{
    background: #d85c00;
  }}
  h1 {{ font-size: 28px; color: var(--accent-gold); border-bottom: 1px solid var(--border-color); padding-bottom: 10px; margin-top: 30px; }}
  h2 {{ font-size: 22px; color: var(--accent-blue); border-bottom: 1px solid var(--border-color); padding-bottom: 8px; margin-top: 28px; }}
  h3 {{ font-size: 18px; color: var(--text-primary); margin-top: 20px; }}
  h4 {{ font-size: 16px; color: var(--text-secondary); }}
  p, li {{ font-size: 15px; color: #c9d1d9; }}
  hr {{ border: 0; height: 1px; background: var(--border-color); margin: 30px 0; }}
  code {{
    background: var(--bg-tertiary);
    color: #ff7b72;
    padding: 2px 6px;
    border-radius: 4px;
    font-family: Consolas, monospace;
    font-size: 13px;
  }}
  pre {{
    background: #0b0e14;
    border: 1px solid var(--border-color);
    padding: 16px;
    border-radius: 8px;
    overflow-x: auto;
    font-family: Consolas, monospace;
    font-size: 13px;
    color: #58a6ff;
  }}
  pre code {{ background: none; color: inherit; padding: 0; }}
  .math-block {{
    background: #121820;
    border-left: 4px solid var(--accent-gold);
    padding: 12px 20px;
    margin: 18px 0;
    font-style: italic;
    color: var(--accent-gold);
    font-size: 16px;
  }}
  .table-wrapper {{
    overflow-x: auto;
    margin: 20px 0;
  }}
  table {{
    width: 100%;
    border-collapse: collapse;
    font-size: 14px;
  }}
  th, td {{
    padding: 12px 14px;
    border: 1px solid var(--border-color);
    text-align: left;
  }}
  th {{
    background: var(--bg-tertiary);
    color: var(--accent-blue);
    font-weight: 600;
  }}
  tr:nth-child(even) {{ background: var(--table-stripe); }}
  .carousel-container {{
    background: #090c10;
    border: 1px solid var(--border-color);
    border-radius: 8px;
    padding: 16px;
    margin: 24px 0;
  }}
  .carousel-track {{
    display: flex;
    gap: 16px;
    overflow-x: auto;
    padding-bottom: 10px;
  }}
  .carousel-slide {{
    flex: 0 0 320px;
    background: var(--bg-secondary);
    border: 1px solid var(--border-color);
    border-radius: 6px;
    overflow: hidden;
    text-align: center;
  }}
  .carousel-slide img {{
    width: 100%;
    height: 200px;
    object-fit: cover;
    display: block;
    border-bottom: 1px solid var(--border-color);
  }}
  .carousel-slide .caption {{
    padding: 8px;
    font-size: 13px;
    font-weight: 600;
    color: var(--text-primary);
    margin: 0;
  }}
  .img-center {{
    text-align: center;
    margin: 20px 0;
  }}
  .img-center img {{
    max-width: 100%;
    max-height: 400px;
    border-radius: 8px;
    border: 1px solid var(--border-color);
  }}
  .caption {{
    font-size: 13px;
    color: var(--text-secondary);
    margin-top: 6px;
  }}
  @media print {{
    body {{ background: #fff; color: #000; padding: 0; }}
    .container {{ border: none; box-shadow: none; padding: 0; }}
    .btn-print {{ display: none; }}
    th {{ background: #eee; color: #000; }}
    tr:nth-child(even) {{ background: #f9f9f9; }}
    pre {{ background: #f5f5f5; color: #000; border: 1px solid #ccc; }}
    code {{ background: #f0f0f0; color: #000; }}
  }}
</style>
</head>
<body>
<div class="container">
  <div class="header-bar">
    <div class="header-title">NARUTO: ECHOES OF SHINOBI — MASTER PRODUCTION PLAN</div>
    <button class="btn-print" onclick="window.print()">Print / Save as PDF</button>
  </div>
  {content_html}
</div>
</body>
</html>
"""

for target_dir in [DESKTOP_DIR, DOWNLOADS_DIR]:
    out_html = os.path.join(target_dir, "EOS_MASTER_PRODUCTION_PLAN.html")
    with open(out_html, "w", encoding="utf-8") as f:
        f.write(html_template)
    print(f"Successfully generated standalone HTML: {out_html}")
