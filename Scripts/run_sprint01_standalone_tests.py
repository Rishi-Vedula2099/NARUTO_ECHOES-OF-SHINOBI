import os
import sys
import time
import hashlib
import subprocess
import shutil

PROJECT_ROOT = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI"
BUILD_EXE = os.path.join(PROJECT_ROOT, "Binaries", "Win64", "EchoesOfShinobi.exe")
BUILD_PDB = os.path.join(PROJECT_ROOT, "Binaries", "Win64", "EchoesOfShinobi.pdb")
PKG_DIR = os.path.join(PROJECT_ROOT, "Saved", "Packaged", "Windows", "EchoesOfShinobi", "Binaries", "Win64")
PKG_EXE = os.path.join(PKG_DIR, "EchoesOfShinobi.exe")
PKG_PDB = os.path.join(PKG_DIR, "EchoesOfShinobi.pdb")

SAVED_DIR = os.path.join(PROJECT_ROOT, "Saved")
LOGS_DIR = os.path.join(SAVED_DIR, "Logs")
REPORTS_DIR = os.path.join(SAVED_DIR, "TestReports")
SAVEGAMES_DIR = os.path.join(SAVED_DIR, "SaveGames")
SCREENSHOTS_DIR = os.path.join(SAVED_DIR, "Screenshots")

PKG_SAVED_DIR = os.path.join(PROJECT_ROOT, "Saved", "Packaged", "Windows", "EchoesOfShinobi", "Saved")
PKG_REPORTS_DIR = os.path.join(PKG_SAVED_DIR, "TestReports")
PKG_SAVEGAMES_DIR = os.path.join(PKG_SAVED_DIR, "SaveGames")
PKG_SCREENSHOTS_DIR = os.path.join(PKG_SAVED_DIR, "Screenshots", "Windows")

def get_sha256(filepath):
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(8192 * 1024):
            h.update(chunk)
    return h.hexdigest().upper()

def copy_built_binaries():
    print("[1/5] Checking fresh built binaries...")
    if not os.path.exists(BUILD_EXE):
        print(f"ERROR: Build output executable not found at {BUILD_EXE}")
        return False
    
    os.makedirs(PKG_DIR, exist_ok=True)
    print(f"Deploying {BUILD_EXE} -> {PKG_EXE}...")
    shutil.copy2(BUILD_EXE, PKG_EXE)
    if os.path.exists(BUILD_PDB):
        shutil.copy2(BUILD_PDB, PKG_PDB)
    
    sha256 = get_sha256(PKG_EXE)
    size = os.path.getsize(PKG_EXE)
    print(f"Binary Verified:\n  Path: {PKG_EXE}\n  Size: {size:,} bytes\n  SHA-256: {sha256}")
    return sha256

def run_process_with_streaming_logs(cmd, log_desc, timeout_sec=60):
    print(f"\n================================================================================")
    print(f"LAUNCHING: {log_desc}")
    print(f"Command: {' '.join(cmd)}")
    print(f"================================================================================")
    
    start_time = time.time()
    proc = subprocess.Popen(
        cmd,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1
    )
    
    lines_read = 0
    while True:
        line = proc.stdout.readline()
        if not line and proc.poll() is not None:
            break
        if line:
            clean_line = line.strip()
            if any(k in clean_line for k in ["[SPRINT01", "[MISSION", "[SAVE", "[INTERACTION", "[DIALOGUE", "[NPC", "AUTOCAPTURE", "Error:", "Warning:"]):
                print(f"[{time.time()-start_time:.1f}s] {clean_line}")
            lines_read += 1
            
        if time.time() - start_time > timeout_sec:
            print(f"TIMEOUT: Process exceeded {timeout_sec}s! Terminating...")
            proc.terminate()
            time.sleep(2)
            if proc.poll() is None:
                proc.kill()
            return -999, lines_read
            
    exit_code = proc.wait()
    duration = time.time() - start_time
    print(f"Process terminated with exit code: {exit_code} (Duration: {duration:.2f}s, Lines: {lines_read})")
    return exit_code, lines_read

def find_file(name, dirs):
    for d in dirs:
        p = os.path.join(d, name)
        if os.path.exists(p):
            return p
    return None

def main():
    os.makedirs(LOGS_DIR, exist_ok=True)
    os.makedirs(REPORTS_DIR, exist_ok=True)
    os.makedirs(SAVEGAMES_DIR, exist_ok=True)
    os.makedirs(PKG_REPORTS_DIR, exist_ok=True)
    os.makedirs(PKG_SAVEGAMES_DIR, exist_ok=True)
    
    os.makedirs(SCREENSHOTS_DIR, exist_ok=True)
    os.makedirs(PKG_SCREENSHOTS_DIR, exist_ok=True)

    sha256 = copy_built_binaries()
    if not sha256:
        sys.exit(1)
        
    # Clean previous test artifacts
    for d in [REPORTS_DIR, PKG_REPORTS_DIR]:
        for f in ["SPRINT01_STANDALONE_MISSION_EXECUTION_REPORT.txt", "SPRINT01_SAVE_RESTORE_VERIFICATION.txt"]:
            fp = os.path.join(d, f)
            if os.path.exists(fp):
                os.remove(fp)
    for d in [SAVEGAMES_DIR, PKG_SAVEGAMES_DIR]:
        for f in ["EOS_SaveSlot_Slot_1.json", "EOS_SaveSlot_Slot_2.json"]:
            fp = os.path.join(d, f)
            if os.path.exists(fp):
                os.remove(fp)

    # Clean previous packaged screenshots before test run
    if os.path.exists(PKG_SCREENSHOTS_DIR):
        for f in os.listdir(PKG_SCREENSHOTS_DIR):
            if f.endswith(".png"):
                try:
                    os.remove(os.path.join(PKG_SCREENSHOTS_DIR, f))
                except Exception as e:
                    pass

    # PASS 1: Mission Execution
    cmd_pass1 = [
        PKG_EXE,
        "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet",
        "-windowed",
        "-ResX=1920",
        "-ResY=1080",
        "-d3d12",
        "-Sprint01MissionTest",
        "-stdout",
        f"-abslog={os.path.join(LOGS_DIR, 'Sprint01_Mission_Execution.log')}"
    ]
    
    exit1, lines1 = run_process_with_streaming_logs(cmd_pass1, "PASS 1: Standalone Mission Sequence", timeout_sec=60)
    if exit1 != 0:
        print(f"FAIL: Pass 1 exited with non-zero code {exit1}")
        sys.exit(1)
        
    pass1_report = find_file("SPRINT01_STANDALONE_MISSION_EXECUTION_REPORT.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
    if not pass1_report:
        print("FAIL: Pass 1 report missing in all test directories!")
        sys.exit(1)
        
    with open(pass1_report, "r", encoding="utf-8") as f:
        content1 = f.read()
    print("\n--- PASS 1 REPORT CONTENT ---")
    print(content1)
    
    if "VERDICT: SPRINT 01 PLAYABLE MISSION EXECUTION: 100% PASS" not in content1:
        print("FAIL: Pass 1 report does not contain passing verdict!")
        sys.exit(1)

    # Collect and map the 6 stage screenshots
    stage_names = [
        "Sprint01_Stage1_Exploration.png",
        "Sprint01_Stage2_DialoguePrompt.png",
        "Sprint01_Stage3_OutskirtsTransition.png",
        "Sprint01_Stage4_RasenganCombat.png",
        "Sprint01_Stage5_ZabuzaBossCombat.png",
        "Sprint01_Stage6_MissionRewards.png"
    ]

    captured_shots = []
    if os.path.exists(PKG_SCREENSHOTS_DIR):
        raw_shots = sorted([os.path.join(PKG_SCREENSHOTS_DIR, f) for f in os.listdir(PKG_SCREENSHOTS_DIR) if f.endswith(".png")],
                           key=lambda p: os.path.getmtime(p))
        captured_shots = raw_shots

    print(f"\n--- COLLECTED STAGE SCREENSHOTS ({len(captured_shots)} captured) ---")
    for idx, name in enumerate(stage_names):
        dest_path = os.path.join(SCREENSHOTS_DIR, name)
        if idx < len(captured_shots):
            src_path = captured_shots[idx]
            shutil.copy2(src_path, dest_path)
            size = os.path.getsize(dest_path)
            print(f"  Stage {idx+1}: {name} -> {dest_path} ({size:,} bytes) [PASS]")
        else:
            print(f"  Stage {idx+1}: {name} -> NOT FOUND (only {len(captured_shots)} shots taken)")

    # Verify Multi-Slot Save Files on Disk
    slot1_file = find_file("EOS_SaveSlot_Slot_1.json", [SAVEGAMES_DIR, PKG_SAVEGAMES_DIR])
    slot2_file = find_file("EOS_SaveSlot_Slot_2.json", [SAVEGAMES_DIR, PKG_SAVEGAMES_DIR])

    if not slot1_file:
        print("FAIL: Slot 1 save file missing in all save directories!")
        sys.exit(1)
    if not slot2_file:
        print("FAIL: Slot 2 save file missing in all save directories!")
        sys.exit(1)
        
    with open(slot1_file, "r", encoding="utf-8") as f:
        save1_json = f.read()
    with open(slot2_file, "r", encoding="utf-8") as f:
        save2_json = f.read()

    print("\n--- GENERATED SAVE FILE SLOT_1 (Completed Mission) ---")
    print(save1_json)
    print("\n--- GENERATED SAVE FILE SLOT_2 (Exploratory State) ---")
    print(save2_json)
    
    # PASS 2: Save Restore Verification (Separate Clean Process Relaunch)
    cmd_pass2 = [
        PKG_EXE,
        "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet",
        "-windowed",
        "-ResX=1920",
        "-ResY=1080",
        "-d3d12",
        "-VerifyMissionSave",
        "-stdout",
        f"-abslog={os.path.join(LOGS_DIR, 'Sprint01_Save_Verification.log')}"
    ]
    
    exit2, lines2 = run_process_with_streaming_logs(cmd_pass2, "PASS 2: Save Restore Verification (Clean Process Relaunch)", timeout_sec=30)
    if exit2 != 0:
        print(f"FAIL: Pass 2 exited with non-zero code {exit2}")
        sys.exit(1)
        
    pass2_report = find_file("SPRINT01_SAVE_RESTORE_VERIFICATION.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
    if not pass2_report:
        print("FAIL: Pass 2 report missing in all test directories!")
        sys.exit(1)
        
    with open(pass2_report, "r", encoding="utf-8") as f:
        content2 = f.read()
    print("\n--- PASS 2 REPORT CONTENT ---")
    print(content2)
    
    if "VERDICT: STANDALONE SAVE RELOAD PERSISTENCE: 100% PASS" not in content2:
        print("FAIL: Pass 2 report does not contain passing verdict!")
        sys.exit(1)
        
    print("\n================================================================================")
    print("ALL SPRINT 01 PLAYABLE STANDALONE MISSION VERIFICATION PASSES COMPLETED WITH 100% SUCCESS!")
    print(f"SHA-256: {sha256}")
    print("================================================================================")

if __name__ == "__main__":
    main()
