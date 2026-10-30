import unreal
import os

out_dir = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\Priority1_Naruto_Showcase"
os.makedirs(out_dir, exist_ok=True)

# 1. Load Map
map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
print(f"Loaded map: {map_path}")

# 2. Find Naruto Actor
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
naruto = None
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        naruto = a
        break

if naruto:
    print(f"Found Naruto: {naruto.get_actor_location()}")
    comp = naruto.get_component_by_class(unreal.SkeletalMeshComponent)
    if comp:
        sk = comp.get_skeletal_mesh_asset()
        print(f"Skeletal Mesh: {sk.get_name() if sk else 'None'}")
        for i in range(comp.get_num_materials()):
            mat = comp.get_material(i)
            print(f"  Slot {i}: {mat.get_name() if mat else 'None'}")

# 3. Create SceneCapture2D for Close-Up Face
# Naruto is at (0, -350, 0), facing (0, 180, 0)
# Head is around Z = 135. Face points towards +X or -Y?
# Let's check: in L_VisualAssetSmokeTest, Naruto rotation is (0, 180, 0).
# In Unreal coordinates:
# Yaw = 0 is +X.
# Yaw = 180 is -X.
# Yaw = -90 is -Y.
# Yaw = 90 is +Y.
# Camera is at (0, -520, 130), looking +Y. So Naruto should have Yaw = -90 to face the camera at -Y!
naruto.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
naruto.modify()
print("Set Naruto Yaw to -90 to directly face camera at -Y")

# Spawn SceneCapture2D Actor for Close-Up Face
face_cam = eas.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0.0, -440.0, 134.0))
face_cam.set_actor_label("Capture_Face_CloseUp")
face_cam.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=90.0, roll=0.0), False)

scc = face_cam.get_component_by_class(unreal.SceneCaptureComponent2D)
if scc:
    # Create Render Target 2D (1920x1080)
    rt = unreal.RenderingLibrary.create_render_target2d(world, 1920, 1080, unreal.TextureRenderTargetFormat.RTF_RGBA8)
    scc.set_editor_property("texture_target", rt)
    scc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_HDR)
    scc.capture_scene()

    # Export to disk
    unreal.RenderingLibrary.export_render_target(world, rt, out_dir, "gate2_naruto_face_closeup.png")
    print(f"Exported gate2_naruto_face_closeup.png to {out_dir}")

# Spawn SceneCapture2D Actor for 3/4 Turnaround
turn_cam = eas.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(120.0, -560.0, 95.0))
turn_cam.set_actor_label("Capture_Turnaround_ThreeQuarter")
turn_cam.set_actor_rotation(unreal.Rotator(pitch=-5.0, yaw=105.0, roll=0.0), False)

scc_turn = turn_cam.get_component_by_class(unreal.SceneCaptureComponent2D)
if scc_turn:
    rt_turn = unreal.RenderingLibrary.create_render_target2d(world, 1920, 1080, unreal.TextureRenderTargetFormat.RTF_RGBA8)
    scc_turn.set_editor_property("texture_target", rt_turn)
    scc_turn.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_HDR)
    scc_turn.capture_scene()

    unreal.RenderingLibrary.export_render_target(world, rt_turn, out_dir, "gate2_naruto_three_quarter_fresh.png")
    print(f"Exported gate2_naruto_three_quarter_fresh.png to {out_dir}")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved L_VisualAssetSmokeTest!")

with open(r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\render_target_result.txt", "w") as f:
    f.write("RENDER TARGET EXPORT COMPLETE")
