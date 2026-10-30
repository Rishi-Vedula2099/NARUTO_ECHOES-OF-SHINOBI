Add-Type -AssemblyName System.Drawing
$src = "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\ScreenShot00032.png"
$dst = "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\naruto_crop_32.png"

$bmp = [System.Drawing.Bitmap]::FromFile($src)
$x0 = [int]($bmp.Width * 0.40)
$y0 = [int]($bmp.Height * 0.40)
$w = [int]($bmp.Width * 0.20)
$h = [int]($bmp.Height * 0.58)

$rect = New-Object System.Drawing.Rectangle($x0, $y0, $w, $h)
$crop = $bmp.Clone($rect, $bmp.PixelFormat)
$crop.Save($dst, [System.Drawing.Imaging.ImageFormat]::Png)
$bmp.Dispose()
$crop.Dispose()
Write-Host "Saved crop to $dst"
