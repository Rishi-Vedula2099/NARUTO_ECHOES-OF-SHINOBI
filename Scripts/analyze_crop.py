from PIL import Image
import numpy as np

img = Image.open(r"C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\996825ff-e180-4266-9715-ffe2cad52858\GoldenStreet_RuntimeCapture_04.png")
w, h = img.size
print("Image size:", w, h)

# Center of image is 640, 360 (if 1280x720) or 960, 540 (if 1920x1080)
# Character is in center
crop_box = (int(w*0.4), int(h*0.35), int(w*0.6), int(h*0.95))
cropped = img.crop(crop_box)
cropped.save(r"C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\996825ff-e180-4266-9715-ffe2cad52858\naruto_crop.png")

arr = np.array(cropped)
print("Cropped shape:", arr.shape)
print("Min RGB:", arr[:,:,:3].min(axis=(0,1)))
print("Max RGB:", arr[:,:,:3].max(axis=(0,1)))
print("Mean RGB:", arr[:,:,:3].mean(axis=(0,1)))

# Sample some distinct pixels on the character
center_x = arr.shape[1] // 2
for y in range(0, arr.shape[0], arr.shape[0]//10):
    print(f"Y={y}: RGB = {arr[y, center_x, :3]}")
