import unreal

out_file = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\test_assign_working_result.txt"
lines = []

mesh_path = "/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin_Rigged"
sk_mesh = unreal.load_asset(mesh_path)

# Use the EXACT verified materials from the proving box spheres that we can see are vibrant:
# Sphere 7: MI_AnimeRoof (Vibrant Orange)
# Sphere 8: MI_AnimeFoliage (Vibrant Green)
# Sphere 3: MI_Naruto_Skin
roof_mat = unreal.load_asset("/Game/Materials/Environment/MI_AnimeRoof")
foliage_mat = unreal.load_asset("/Game/Materials/Environment/MI_AnimeFoliage")
wood_mat = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
skin_mat = unreal.load_asset("/Game/Characters/Naruto/Materials/MI_Naruto_Skin")

map_path = "/Game/Maps/VisualSmoke/L_VisualAssetSmokeTest"
world = unreal.EditorLoadingAndSavingUtils.load_map(map_path)

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
for a in eas.get_all_level_actors():
    if a.get_actor_label() == "SK_Naruto_Genin_ProvingActor":
        comp = a.get_component_by_class(unreal.SkeletalMeshComponent)
        if comp:
            comp.set_material(0, skin_mat)    # Skin
            comp.set_material(1, wood_mat)    # Navy -> Wood (Brown)
            comp.set_material(2, foliage_mat) # Hair -> Foliage (Bright Green)
            comp.set_material(3, skin_mat)    # Collar
            comp.set_material(4, roof_mat)    # Tracksuit -> Roof (Bright Orange)
            comp.set_material(5, roof_mat)    # Emblem
            comp.set_material(6, wood_mat)    # Metal
            lines.append("Assigned verified proving box materials to actor component slots!")
        # Rotate to face front camera (Yaw = 180)
        a.set_actor_rotation(unreal.Rotator(pitch=0.0, yaw=180.0, roll=0.0), False)
        a.modify()
        break

unreal.EditorLoadingAndSavingUtils.save_map(world, map_path)
lines.append("Saved map.")

with open(out_file, "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

print("Completed test_assign_working_materials.py")
