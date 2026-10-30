import unreal

out_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\list_all_smoke_actors.txt"
lines = []
world = unreal.EditorLoadingAndSavingUtils.load_map("/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest")
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

for a in EAS.get_all_level_actors():
    cname = a.get_class().get_name()
    label = a.get_actor_label()
    loc = a.get_actor_location()
    scale = a.get_actor_scale3d()
    
    # check static mesh
    smc = a.get_component_by_class(unreal.StaticMeshComponent)
    mesh_name = smc.get_editor_property("static_mesh").get_name() if (smc and smc.get_editor_property("static_mesh")) else "None"
    mats = []
    if smc:
        for i in range(smc.get_num_materials()):
            m = smc.get_material(i)
            mats.append(m.get_name() if m else "None")
    
    lines.append(f"{label} ({cname}) | Loc={loc} | Scale={scale} | Mesh={mesh_name} | Mats={mats}")

with open(out_path, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Wrote all actors to", out_path)
