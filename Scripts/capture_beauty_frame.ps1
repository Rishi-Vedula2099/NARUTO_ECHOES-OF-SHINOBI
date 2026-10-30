$UE = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$Proj = "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\EchoesOfShinobi.uproject"
$Map = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

$argList = "`"$Proj`" $Map -game -windowed -ResX=1920 -ResY=1080 -stdout -unattended -ExecCmds=`"Shot`""
Write-Host "Launching UE5 to capture beauty frame..."
$proc = Start-Process -FilePath $UE -ArgumentList $argList -PassThru

# Wait 30 seconds for map load, mesh streaming, and screenshot flush
Start-Sleep -Seconds 30

if (-not $proc.HasExited) {
    Write-Host "Flushing and stopping process..."
    Stop-Process -Id $proc.Id -Force
}
Write-Host "Screenshot capture script complete."
