$UE = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$Proj = "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\EchoesOfShinobi.uproject"
$Map = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"

$argList = "`"$Proj`" $Map -game -windowed -ResX=1920 -ResY=1080 -stdout -unattended -ExecCmds=`"Shot`""
Write-Host "Launching UE5 to capture ScreenShot..."
$proc = Start-Process -FilePath $UE -ArgumentList $argList -PassThru

Start-Sleep -Seconds 25

if (-not $proc.HasExited) {
    Write-Host "Stopping UE5 process..."
    Stop-Process -Id $proc.Id -Force
}
Write-Host "Capture complete."
