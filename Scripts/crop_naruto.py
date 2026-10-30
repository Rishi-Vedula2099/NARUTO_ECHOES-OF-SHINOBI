import os
from PIL import Image

src = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\ScreenShot00032.png"
dst = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\naruto_crop_32.png"

img = Image.open(src)
w, h = img.size
# Naruto is centered horizontally and occupies middle to bottom
x0 = int(w * 0.40)
y0 = int(h * 0.40)
x1 = int(w * 0.60)
y1 = int(h * 0.98)

crop = img.crop((x0, y0, x1, y1))
crop.save(dst)
print(f"Saved cropped Naruto to {dst}")
