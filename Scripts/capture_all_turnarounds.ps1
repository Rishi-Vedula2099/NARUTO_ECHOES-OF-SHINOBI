$UE = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
$Proj = "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\EchoesOfShinobi.uproject"
$Map = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
$ArtifactDir = "C:\Users\Rishi Vedula\.gemini\antigravity-ide\brain\801206fe-4fd9-48a7-9f2a-758587bf1a55"

function Capture-Angle($yaw, $outName) {
    Write-Host "Setting Naruto Yaw to $yaw..."
    & $UE $Proj -run=pythonscript -script="c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\set_naruto_turnaround_yaw.py $yaw" -stdout -unattended -NoLogTimes | Out-Null
    
    Write-Host "Capturing frame for Yaw $yaw..."
    $argList = "`"$Proj`" $Map -game -windowed -ResX=1920 -ResY=1080 -stdout -unattended -ExecCmds=`"Shot`""
    $proc = Start-Process -FilePath $UE -ArgumentList $argList -PassThru
    Start-Sleep -Seconds 22
    if (-not $proc.HasExited) {
        Stop-Process -Id $proc.Id -Force
    }
    
    # Grab newest screenshot
    $latest = Get-ChildItem -Path "c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Saved\Screenshots\WindowsEditor" | Sort-Object LastWriteTime -Descending | Select-Object -First 1
    if ($latest) {
        Copy-Item $latest.FullName "$ArtifactDir\$outName" -Force
        Write-Host "Copied $($latest.Name) to $outName"
    }
}

Capture-Angle 45.0 "gate2_naruto_three_quarter.png"
Capture-Angle 90.0 "gate2_naruto_profile.png"
Capture-Angle 180.0 "gate2_naruto_back_crest.png"

# Reset back to Front View (0)
& $UE $Proj -run=pythonscript -script="c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\set_naruto_turnaround_yaw.py 0.0" -stdout -unattended -NoLogTimes | Out-Null
Write-Host "All turnaround captures complete!"
