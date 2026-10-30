"""
NARUTO: ECHOES OF SHINOBI
Priority 1: Authentic Rigged Naruto Turnaround Captures (Front, 3/4 Hero, Right Side, Back)
"""

import unreal
import os

PROJECT_DIR = unreal.SystemLibrary.get_project_directory()
MAP_PATH = "/Game/Maps/GoldenTest/L_Naruto_Showcase"
world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PATH)

EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# Find HeroNaruto_Showcase
naruto = None
for a in EAS.get_all_level_actors():
    if a.get_actor_label() == "HeroNaruto_Showcase":
        naruto = a
        break

if not naruto:
    print("Could not find HeroNaruto_Showcase!")
else:
    # Ensure proper rotation facing forward (-Y)
    naruto.set_actor_location(unreal.Vector(0.0, 0.0, 0.0), False, False)
    naruto.set_actor_rotation(unreal.Rotator(0.0, 0.0, 0.0), False)
    print("Naruto located at (0,0,0) facing forward.")

# Create or Update Camera
cam = None
for a in EAS.get_all_level_actors():
    if a.get_actor_label() == "ShowcaseCamera":
        cam = a
        break

if not cam:
    cam = EAS.spawn_actor_from_class(unreal.CameraActor, unreal.Vector(0, -220, 100))
    cam.set_actor_label("ShowcaseCamera")

# Setup Camera Component
cc = cam.get_component_by_class(unreal.CameraComponent)
if cc:
    cc.set_editor_property("field_of_view", 45.0)

# Save Camera facing Front
cam.set_actor_location(unreal.Vector(0.0, -220.0, 95.0), False, False)
cam.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

# Add a PlayerStart right at the front camera location
pstart = None
for a in EAS.get_all_level_actors():
    if isinstance(a, unreal.PlayerStart):
        pstart = a
        break
if not pstart:
    pstart = EAS.spawn_actor_from_class(unreal.PlayerStart, unreal.Vector(0.0, -220.0, 95.0))
    pstart.set_actor_label("PlayerStart_ShowcaseFront")
pstart.set_actor_location(unreal.Vector(0.0, -220.0, 95.0), False, False)
pstart.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

unreal.EditorLoadingAndSavingUtils.save_current_level()
print("Saved Front Camera Setup in L_Naruto_Showcase.")
