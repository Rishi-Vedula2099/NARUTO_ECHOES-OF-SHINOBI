"""
NARUTO: ECHOES OF SHINOBI
Assemble Refined Masterpiece Golden Street & Landmark Environment
==================================================================
Wires the refined 3D character and authentic Japanese timber architecture
into L_Konoha_GoldenStreet and L_Naruto_Showcase, replacing all primitive
boxes, cylinders, and flat backdrop planes.
"""

import unreal

MAP_STREET = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
MAP_SHOWCASE = "/Game/Maps/GoldenTest/L_Naruto_Showcase"

def update_golden_street():
    unreal.log("=== UPDATING L_KONOHA_GOLDENSTREET WITH REFINED ASSETS ===")
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_STREET)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    # 1. Load Refined Assets
    sk_naruto     = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin")
    if not sk_naruto:
        sk_naruto = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")

    sm_ichiraku   = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_IchirakuRamen")
    sm_res_a      = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_A")
    sm_res_b      = unreal.load_asset("/Game/Environment/Konoha/Buildings/Meshes/SM_Bld_Konoha_Residence_B")
    sm_monument   = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Meshes/SM_Lnd_HokageFaces_Monument")
    mat_rock      = unreal.load_asset("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock")

    unreal.log(f"Assets loaded: Naruto={sk_naruto != None}, Ichiraku={sm_ichiraku != None}, ResA={sm_res_a != None}, Monument={sm_monument != None}")

    actors = eas.get_all_level_actors()
    monument_actor = None

    for a in actors:
        if not a:
            continue
        label = a.get_actor_label()
        label_lower = label.lower()

        # Update Ichiraku Ramen Shop
        if "bld_ichirakuramen" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_ichiraku:
                smc.set_static_mesh(sm_ichiraku)
                unreal.log(f"  Updated {label} with refined SM_Bld_IchirakuRamen")

        # Update Residence A
        elif "bld_konoharesidence_a" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_res_a:
                smc.set_static_mesh(sm_res_a)
                unreal.log(f"  Updated {label} with refined SM_Bld_Konoha_Residence_A")

        # Update Residence B
        elif "bld_konoharesidence_b" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_res_b:
                smc.set_static_mesh(sm_res_b)
                unreal.log(f"  Updated {label} with refined SM_Bld_Konoha_Residence_B")

        # Check existing monument actor or backdrop
        elif "hokage" in label_lower and "monument" in label_lower:
            monument_actor = a
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_monument:
                smc.set_static_mesh(sm_monument)
                if mat_rock:
                    smc.set_material(0, mat_rock)
                a.set_actor_location(unreal.Vector(0.0, 4200.0, 350.0), False, False)
                a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)
                a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
                unreal.log(f"  Updated {label} with 3D sculpted SM_Lnd_HokageFaces_Monument")

        # Replace 2D backdrop plane if present with 3D monument
        elif "backdrop" in label_lower and "northstar" in label_lower:
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            if smc and sm_monument:
                smc.set_static_mesh(sm_monument)
                if mat_rock:
                    smc.set_material(0, mat_rock)
                a.set_actor_location(unreal.Vector(0.0, 4200.0, 350.0), False, False)
                a.set_actor_rotation(unreal.Rotator(0.0, -90.0, 0.0), False)
                a.set_actor_scale3d(unreal.Vector(1.0, 1.0, 1.0))
                a.set_actor_label("Lnd_HokageFaces_Monument_3D")
                monument_actor = a
                unreal.log(f"  Replaced 2D backdrop {label} with real 3D SM_Lnd_HokageFaces_Monument")

    # If monument wasn't in level, spawn it
    if not monument_actor and sm_monument:
        spawn_loc = unreal.Vector(0.0, 4200.0, 350.0)
        spawn_rot = unreal.Rotator(0.0, -90.0, 0.0)
        new_act = eas.spawn_actor_from_object(sm_monument, spawn_loc, spawn_rot)
        if new_act:
            new_act.set_actor_label("Lnd_HokageFaces_Monument_3D")
            unreal.log("  Spawned new 3D SM_Lnd_HokageFaces_Monument in Golden Street vista")

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("Saved L_Konoha_GoldenStreet successfully.")

def update_showcase_level():
    unreal.log("=== UPDATING L_NARUTO_SHOWCASE WITH REFINED HERO SKELETAL MESH ===")
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_SHOWCASE)
    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    sk_naruto = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin")
    if not sk_naruto:
        sk_naruto = unreal.load_asset("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin")

    for a in eas.get_all_level_actors():
        if not a:
            continue
        label_lower = a.get_actor_label().lower()
        if "naruto" in label_lower or isinstance(a, unreal.SkeletalMeshActor):
            skc = a.get_component_by_class(unreal.SkeletalMeshComponent)
            if skc and sk_naruto:
                skc.set_skeletal_mesh_asset(sk_naruto)
                unreal.log(f"  Assigned refined SK_Naruto_Genin to {a.get_actor_label()}")

    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.log("Saved L_Naruto_Showcase successfully.")

if __name__ == "__main__":
    update_golden_street()
    update_showcase_level()
    unreal.log("All levels assembled with refined production assets.")
