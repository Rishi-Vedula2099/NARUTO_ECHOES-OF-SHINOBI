import unreal

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\set_upright_naruto.txt"
lines = []

sk_mesh = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin")
skeleton = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Skeleton")
abp = unreal.EditorAssetLibrary.load_asset("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin")

for a in unreal.EditorLevelLibrary.get_all_level_actors():
    label = a.get_actor_label()
    if label == "SK_Naruto_Genin_ProvingActor":
        comp = a.skeletal_mesh_component
        comp.set_simulate_physics(False)
        comp.set_enable_gravity(False)
        comp.set_skeletal_mesh_asset(sk_mesh)
        if abp:
            comp.set_anim_instance_class(abp.generated_class())
        # Stand upright: Pitch=0, Yaw=-90 (faces -Y towards camera), Roll=0
        a.set_actor_location(unreal.Vector(0.0, -350.0, 0.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)
        a.modify()
        lines.append(f"Positioned Naruto actor at (0, -350, 0), Rot=(0, -90, 0)")
    elif isinstance(a, unreal.PlayerStart):
        # Position PlayerStart facing Naruto
        a.set_actor_location(unreal.Vector(0.0, -650.0, 90.0), False, False)
        a.set_actor_rotation(unreal.Rotator(0.0, 90.0, 0.0), False)
        a.modify()
        lines.append("Positioned PlayerStart at (0, -650, 90), Rot=(0, 90, 0)")

saved = unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append(f"Saved {map_path}: {saved}")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))
