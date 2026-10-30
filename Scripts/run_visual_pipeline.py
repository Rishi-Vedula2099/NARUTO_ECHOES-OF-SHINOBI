"""
NARUTO: ECHOES OF SHINOBI
Master Visual Pipeline Orchestrator
Executes:
1. setup_anime_materials.py
2. build_golden_visual_test.py
3. polish_konoha_level.py
"""
import unreal
import os

SCRIPTS_DIR = os.path.dirname(os.path.abspath(__file__))

def run_script(filename):
    script_path = os.path.join(SCRIPTS_DIR, filename)
    unreal.log(f"\n>>> EXECUTING VISUAL SCRIPT: {filename} <<<")
    with open(script_path, "r", encoding="utf-8") as f:
        code = compile(f.read(), script_path, "exec")
        exec(code, globals())
    unreal.log(f">>> FINISHED SCRIPT: {filename} <<<\n")

run_script("setup_anime_materials.py")
run_script("build_golden_visual_test.py")
run_script("polish_konoha_level.py")

unreal.log("======================================================================")
unreal.log("  ALL VISUAL PIPELINE SCRIPTS EXECUTED SUCCESSFULLY (100% COMPLETE)")
unreal.log("======================================================================")
