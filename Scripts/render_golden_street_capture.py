import unreal
import os

out_dir = r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Docs\Screenshots\Priority1_Naruto_Showcase"
os.makedirs(out_dir, exist_ok=True)

# 1. Load Golden Street
map_path = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)
print(f"Loaded Golden Street: {map_path}")

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

# Find or spawn Naruto in the avenue
naruto = None
for a in eas.get_all_level_actors():
    if "Naruto" in a.get_actor_label():
        naruto = a
        break

sk_mesh = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged")
abp = unreal.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")

if not naruto and sk_mesh:
    # Spawn hero Naruto at the start of the golden avenue facing camera
    naruto = eas.spawn_actor_from_class(unreal.SkeletalMeshActor, unreal.Vector(0.0, -250.0, 0.0))
    naruto.set_actor_label("Hero_Naruto_GoldenStreet")
    print("Spawned Hero_Naruto_GoldenStreet in L_Konoha_GoldenStreet")

if naruto and sk_mesh:
    naruto.set_actor_location(unreal.Vector(0.0, -250.0, 0.0), False, False)
    # Face towards camera at -Y
    naruto.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=-90.0, roll=0.0), False)
    comp = naruto.get_component_by_class(unreal.SkeletalMeshComponent)
    if comp:
        comp.set_skeletal_mesh_asset(sk_mesh)
        if abp:
            comp.set_anim_instance_class(abp.generated_class())
        # Assign 7 materials
        slot_names = ["Skin", "Navy", "Hair", "Collar", "Tracksuit", "Emblem", "Metal"]
        for idx, key in enumerate(slot_names):
            mi = unreal.load_asset(f"/Game/Characters/Naruto/Materials/MI_Naruto_{key}")
            if mi:
                comp.set_material(idx, mi)
    naruto.modify()
    print("Configured Hero Naruto in Golden Street with 7 production materials!")

# 2. Spawn SceneCapture2D framing Naruto + Avenue + Hokage Rock in the background
cam = eas.spawn_actor_from_class(unreal.SceneCapture2D, unreal.Vector(0.0, -650.0, 95.0))
cam.set_actor_label("Capture_GoldenStreet_Hero")
# Pitch up slightly to capture Hokage Rock monument vista
cam.set_actor_rotation(unreal.Rotator(pitch=3.0, yaw=90.0, roll=0.0), False)

scc = cam.get_component_by_class(unreal.SceneCaptureComponent2D)
if scc:
    rt = unreal.RenderingLibrary.create_render_target2d(world, 1920, 1080, unreal.TextureRenderTargetFormat.RTF_RGBA8)
    scc.set_editor_property("texture_target", rt)
    scc.set_editor_property("capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_HDR)
    scc.capture_scene()

    unreal.RenderingLibrary.export_render_target(world, rt, out_dir, "gate2_konoha_golden_street_fresh.png")
    print(f"Exported gate2_konoha_golden_street_fresh.png to {out_dir}")

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
print("Saved L_Konoha_GoldenStreet successfully!")
