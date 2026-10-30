import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
char_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSCharacterBase")
cdo = unreal.get_default_object(char_class)

out = []
out.append(f"CDO: {cdo.get_name()}")

vp = cdo.get_editor_property("visual_profile")
out.append(f"CDO VisualProfile: CharID={vp.character_id}, FormID={vp.form_id}, Mesh={vp.skeletal_mesh_asset}, ABP={vp.anim_blueprint_class}")
for i, m in enumerate(vp.material_instances):
    out.append(f"  CDO MaterialInstance[{i}]: {m}")

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
test_char = eas.spawn_actor_from_class(char_class, unreal.Vector(0, 0, 100))
if test_char:
    out.append(f"Spawned test_char: {test_char.get_name()}")
    comps = test_char.get_components_by_class(unreal.MeshComponent)
    for c in comps:
        out.append(f"  MeshComp: {c.get_name()} ({c.get_class().get_name()}), Visible={c.is_visible()}")
        if isinstance(c, unreal.StaticMeshComponent):
            sm = c.static_mesh
            out.append(f"    StaticMesh: {sm.get_path_name() if sm else 'None'}")
            for mi in range(c.get_num_materials()):
                mat = c.get_material(mi)
                out.append(f"      Material[{mi}]: {mat.get_path_name() if mat else 'None'}")
        elif isinstance(c, unreal.SkeletalMeshComponent):
            sk = c.skeletal_mesh_asset
            out.append(f"    SkeletalMesh: {sk.get_path_name() if sk else 'None'}")
            for mi in range(c.get_num_materials()):
                mat = c.get_material(mi)
                out.append(f"      Material[{mi}]: {mat.get_path_name() if mat else 'None'}")
    eas.destroy_actor(test_char)

with open(r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\test_char_out.txt", "w", encoding="utf-8") as f:
    f.write("\n".join(out))
unreal.log("Wrote test_char_out.txt")
