Add-Type -AssemblyName System.Drawing
$bmp = [System.Drawing.Bitmap]::FromFile("C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\996825ff-e180-4266-9715-ffe2cad52858\GoldenStreet_RuntimeCapture_04.png")
Write-Host "Image Size: $($bmp.Width) x $($bmp.Height)"

$x1 = [int]($bmp.Width * 0.44)
$y1 = [int]($bmp.Height * 0.48)
$w = [int]($bmp.Width * 0.12)
$h = [int]($bmp.Height * 0.45)

$rect = New-Object System.Drawing.Rectangle($x1, $y1, $w, $h)
$crop = $bmp.Clone($rect, $bmp.PixelFormat)
$cropPath = "C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\996825ff-e180-4266-9715-ffe2cad52858\naruto_crop.png"
$crop.Save($cropPath, [System.Drawing.Imaging.ImageFormat]::Png)
Write-Host "Saved crop to $cropPath"

$bmp.Dispose()
$crop.Dispose()
