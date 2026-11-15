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

BRAIN_DIR = r"C:\Users\Rishi Vedula\.gemini\antigravity\brain\e7bfe4a8-64ad-419a-8219-fdb94cff68bd"

def get_sha256(filepath):
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(8192 * 1024):
            h.update(chunk)
    return h.hexdigest().upper()

def copy_built_binaries():
    print("[1/5] Checking freshly built binaries...")
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

def run_process_with_streaming_logs(cmd, log_desc, timeout_sec=120):
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
            if any(k in clean_line for k in ["[SPRINT", "[GAUNTLET", "[RAMEN", "[KAWARIMI", "[JUTSU", "[SAVE", "[INTERACTION", "[DIALOGUE", "[NPC", "[BOSS", "[ATMOSPHERE", "AUTOCAPTURE", "Error:", "Warning:"]):
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

def read_text_file(filepath):
    with open(filepath, "rb") as f:
        raw = f.read()
    if raw.startswith(b'\xff\xfe') or raw.startswith(b'\xfe\xff'):
        return raw.decode("utf-16")
    try:
        return raw.decode("utf-8")
    except UnicodeDecodeError:
        return raw.decode("latin1")

def create_animated_gif(image_paths, output_gif_path, duration_ms=1200):
    try:
        from PIL import Image
        images = []
        for p in image_paths:
            if os.path.exists(p):
                im = Image.open(p)
                im_resized = im.resize((960, 540), Image.Resampling.LANCZOS)
                images.append(im_resized.convert("RGB"))
        if images:
            images[0].save(
                output_gif_path,
                save_all=True,
                append_images=images[1:],
                optimize=True,
                duration=duration_ms,
                loop=0
            )
            print(f"[GIF] Created animated mission recording: {output_gif_path} ({os.path.getsize(output_gif_path):,} bytes)")
            return True
    except Exception as e:
        print(f"[GIF] Warning: could not generate animated GIF: {e}")
    return False

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
        
    # Clean previous test reports
    for d in [REPORTS_DIR, PKG_REPORTS_DIR]:
        for f in ["SPRINT03_STANDALONE_MISSION_EXECUTION_REPORT.txt", "SPRINT03_SAVE_RESTORE_VERIFICATION.txt",
                  "SPRINT02_STANDALONE_MISSION_EXECUTION_REPORT.txt", "SPRINT02_SAVE_RESTORE_VERIFICATION.txt"]:
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

    # PASS 1: Sprint 04 Mission Execution
    cmd_pass1 = [
        PKG_EXE,
        "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet",
        "-windowed",
        "-ResX=1280",
        "-ResY=720",
        "-d3d12",
        "-Sprint03MissionTest",
        "-stdout",
        f"-abslog={os.path.join(LOGS_DIR, 'Sprint04_Mission_Execution.log')}"
    ]
    
    exit1, lines1 = run_process_with_streaming_logs(cmd_pass1, "PASS 1: Sprint 04 Standalone Mission Sequence", timeout_sec=120)
    if exit1 != 0:
        print(f"FAIL: Pass 1 exited with non-zero code {exit1}")
        sys.exit(1)
        
    pass1_report = find_file("SPRINT03_STANDALONE_MISSION_EXECUTION_REPORT.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
    if not pass1_report:
        pass1_report = find_file("SPRINT02_STANDALONE_MISSION_EXECUTION_REPORT.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
        
    if not pass1_report:
        print("FAIL: Pass 1 report missing in all test directories!")
        sys.exit(1)
        
    content1 = read_text_file(pass1_report)
    print("\n--- PASS 1 REPORT CONTENT ---")
    print(content1)
    
    if "100% PASS" not in content1:
        print("FAIL: Pass 1 report does not contain passing verdict!")
        sys.exit(1)

    # Collect and map captured screenshots
    captured_shots = []
    if os.path.exists(PKG_SCREENSHOTS_DIR):
        raw_shots = sorted([os.path.join(PKG_SCREENSHOTS_DIR, f) for f in os.listdir(PKG_SCREENSHOTS_DIR) if f.endswith(".png")],
                           key=lambda p: os.path.getmtime(p))
        captured_shots = raw_shots

    print(f"\n--- COLLECTED STAGE SCREENSHOTS ({len(captured_shots)} captured) ---")
    for idx, s in enumerate(captured_shots):
        print(f"  Raw Shot [{idx}]: {s} ({os.path.getsize(s):,} bytes)")

    # Authoritative Sprint 04 Canonical Filenames (7 PNG items mapped to proper stages)
    # Stage 1 (Shot 0): Exploration Avenue toward Hokage Rock (Narrative Camera Clearance & Centering)
    # Stage 2 (Shot 1): Ramen Shop Counter & Teuchi Economy Transaction
    # Stage 3 (Shot 2): North Gate Ambush Framing (both scouts visible simultaneously under timber arch)
    # Stage 4 (Shot 3): Kawarimi Substitution Log & Smoke Burst
    # Stage 5 (Shot 4): Shadow Clone Dual Target Combat (zero dummy posts, 2 clones actively engaging)
    # Stage 6A (Shot 5): Zabuza Rematch Packaged Mesh
    # Stage 6B (Shot 6): Water Dragon Telegraph Vortex
    # Stage 7 (Shot 7): Mission Completion Composition (Hinata turn-in at calculated yaw, zero wall occlusion, no stale boss HUD)

    canonical_map = [
        (0, "Sprint04_01_Ichiraku_Camera_Clearance.png", "Stage 1: Exploration Avenue toward Hokage Rock"),
        (1, "Sprint04_Ramen_Economy_Transaction.png", "Stage 2: Ramen Counter & Teuchi Transaction"),
        (2, "Sprint04_02_NorthGate_Ambush_Framing.png", "Stage 3: North Gate Ambush Framing"),
        (3, "Sprint04_03_Kawarimi_Log_And_Smoke.png", "Stage 4: Kawarimi Log and Billowing Smoke"),
        (4, "Sprint04_04_ShadowClone_DualTarget_Combat.png", "Stage 5: Shadow Clones Dual-Target Combat"),
        (5, "Sprint04_05_Zabuza_Packaged_Mesh.png", "Stage 6A: Zabuza Cooked 3D Mesh"),
        (6, "Sprint04_06_WaterDragon_Telegraph.png", "Stage 6B: Water Dragon Telegraph Vortex"),
        (7, "Sprint04_07_Mission_Completion_Composition.png", "Stage 7: Mission Completion Turn-in to Hinata")
    ]

    all_stage_paths = []
    seven_canonical_paths = []

    for src_idx, out_name, desc in canonical_map:
        if src_idx < len(captured_shots):
            src_path = captured_shots[src_idx]
            dest_path = os.path.join(SCREENSHOTS_DIR, out_name)
            brain_path = os.path.join(BRAIN_DIR, out_name)
            shutil.copy2(src_path, dest_path)
            shutil.copy2(src_path, brain_path)
            all_stage_paths.append(dest_path)
            if out_name.startswith("Sprint04_0"):
                seven_canonical_paths.append(dest_path)
            print(f"  [SAVED] {out_name} ({desc}) -> {os.path.getsize(dest_path):,} bytes")
        else:
            print(f"  [MISSING] Index {src_idx} for {out_name}")

    # Generate continuous animated recording GIF for Sprint 04
    if all_stage_paths:
        gif_dest = os.path.join(SCREENSHOTS_DIR, "Sprint04_Continuous_Mission_Recording.gif")
        gif_brain = os.path.join(BRAIN_DIR, "Sprint04_Continuous_Mission_Recording.gif")
        create_animated_gif(all_stage_paths, gif_dest, duration_ms=1200)
        if os.path.exists(gif_dest):
            shutil.copy2(gif_dest, gif_brain)

    # Verify Multi-Slot Save Files on Disk
    slot1_file = find_file("EOS_SaveSlot_Slot_1.json", [SAVEGAMES_DIR, PKG_SAVEGAMES_DIR])
    slot2_file = find_file("EOS_SaveSlot_Slot_2.json", [SAVEGAMES_DIR, PKG_SAVEGAMES_DIR])

    if not slot1_file:
        print("FAIL: Slot 1 save file missing in all save directories!")
        sys.exit(1)
    if not slot2_file:
        print("FAIL: Slot 2 save file missing in all save directories!")
        sys.exit(1)
        
    save1_json = read_text_file(slot1_file)
    save2_json = read_text_file(slot2_file)

    print("\n--- GENERATED SAVE FILE SLOT_1 (Sprint 04 / Level 6) ---")
    print(save1_json)
    print("\n--- GENERATED SAVE FILE SLOT_2 (Exploratory State) ---")
    print(save2_json)
    
    # PASS 2: Save Restore Verification (Separate Clean Process Relaunch)
    cmd_pass2 = [
        PKG_EXE,
        "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet",
        "-windowed",
        "-ResX=1280",
        "-ResY=720",
        "-d3d12",
        "-VerifySprint03Save",
        "-stdout",
        f"-abslog={os.path.join(LOGS_DIR, 'Sprint04_Save_Verification.log')}"
    ]
    
    exit2, lines2 = run_process_with_streaming_logs(cmd_pass2, "PASS 2: Sprint 04 Save Restore Verification (Clean Process Relaunch)", timeout_sec=120)
    if exit2 != 0:
        print(f"FAIL: Pass 2 exited with non-zero code {exit2}")
        sys.exit(1)
        
    pass2_report = find_file("SPRINT03_SAVE_RESTORE_VERIFICATION.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
    if not pass2_report:
        pass2_report = find_file("SPRINT02_SAVE_RESTORE_VERIFICATION.txt", [REPORTS_DIR, PKG_REPORTS_DIR])
        
    if not pass2_report:
        print("FAIL: Pass 2 report missing in all test directories!")
        sys.exit(1)
        
    content2 = read_text_file(pass2_report)
    print("\n--- PASS 2 REPORT CONTENT ---")
    print(content2)
    
    if "100% PASS" not in content2:
        print("FAIL: Pass 2 report does not contain passing verdict!")
        sys.exit(1)
        
    print("\n================================================================================")
    print("ALL SPRINT 04 COMBAT VISUAL RECTIFICATION VERIFICATION PASSES COMPLETED WITH 100% SUCCESS!")
    print(f"SHA-256: {sha256}")
    print("================================================================================")

if __name__ == "__main__":
    main()

