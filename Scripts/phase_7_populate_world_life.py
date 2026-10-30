"""
NARUTO: ECHOES OF SHINOBI
PHASE 7: NPC + PROPS + WORLD LIFE LEVEL INTEGRATION
===================================================
Level: /Game/Maps/GoldenTest/L_Konoha_GoldenStreet
Integrates:
  - AEOSVillageNPC living actors (Teuchi, Chunin Gate Guard, Leaf Villager, Academy Student)
  - 4-Tier distance simulation LOD (Near, Medium, Far, Culled)
  - Smooth head-tracking towards player within 600cm
  - Dialogue and interaction handoff triggers
  - Dining shinobi at Ichiraku counter (Sasuke, Sakura)
  - Interactive shop prompts & mission dispatch handoff points
"""

import unreal

EAL = unreal.EditorAssetLibrary
EAS = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def integrate_world_life():
    unreal.log("======================================================================")
    unreal.log(f"  INTEGRATING PHASE 7 NPC + PROPS + WORLD LIFE: {MAP_PKG}")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    if not world:
        unreal.log_error(f"Failed to load map {MAP_PKG}")
        return

    actors = EAS.get_all_level_actors()

    # Remove previous NPC actors if present
    for a in actors:
        if not a:
            continue
        label = a.get_actor_label()
        cls_name = a.get_class().get_name()
        if label.startswith("NPC_") or cls_name == "EOSVillageNPC":
            EAS.destroy_actor(a)

    npc_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSVillageNPC")
    if not npc_class:
        unreal.log_error("Could not load /Script/EchoesOfShinobi.EOSVillageNPC")
        return

    # 1. Teuchi inside Ichiraku Ramen behind the counter
    teuchi = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class,
        unreal.Vector(-420.0, 320.0, 20.0),
        unreal.Rotator(0.0, 85.0, 0.0)
    )
    if teuchi:
        teuchi.set_actor_label("NPC_Teuchi_IchirakuChef")
        teuchi.set_editor_property("npc_type", unreal.EOSNPCType.SHOPKEEPER)
        teuchi.set_editor_property("current_behavior", unreal.EOSNPCBehavior.INSPECTING_SHOP)
        teuchi.set_editor_property("npcid", "NPC_ICHIRAKU_TEUCHI")
        teuchi.set_editor_property("npc_name", unreal.Text("Teuchi (Ichiraku Ramen)"))
        teuchi.set_editor_property("dialogue_text", unreal.Text("Welcome! Try our special Miso Chashu Ramen!"))
        teuchi.tags.append("InteractiveShopkeeper")
        unreal.log("  [OK] Spawned Living NPC: Teuchi (Shopkeeper)")

    # 2. Chunin Gate Guard at the Konoha Frontier Gate
    guard = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class,
        unreal.Vector(0.0, -850.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0)
    )
    if guard:
        guard.set_actor_label("NPC_Chunin_GateGuard")
        guard.set_editor_property("npc_type", unreal.EOSNPCType.CHUNIN_GUARD)
        guard.set_editor_property("current_behavior", unreal.EOSNPCBehavior.GUARDING_GATE)
        guard.set_editor_property("npcid", "NPC_CHUNIN_GUARD_GATE")
        guard.set_editor_property("npc_name", unreal.Text("Chunin Gate Guard"))
        guard.set_editor_property("dialogue_text", unreal.Text("Shinobi! Head to the training grounds immediately for the tactical investigation!"))
        guard.tags.append("MissionHandoffNPC")
        unreal.log("  [OK] Spawned Living NPC: Chunin Gate Guard (Mission Handoff)")

    # 3. Leaf Villager strolling along the East sidewalk
    villager = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class,
        unreal.Vector(340.0, 400.0, 4.0),
        unreal.Rotator(0.0, 0.0, 0.0)
    )
    if villager:
        villager.set_actor_label("NPC_LeafVillager_EastSidewalk")
        villager.set_editor_property("npc_type", unreal.EOSNPCType.VILLAGER)
        villager.set_editor_property("current_behavior", unreal.EOSNPCBehavior.PATROLLING_ROAD)
        villager.set_editor_property("npcid", "NPC_LEAF_VILLAGER_01")
        villager.set_editor_property("npc_name", unreal.Text("Leaf Villager"))
        villager.set_editor_property("dialogue_text", unreal.Text("Konohagakure is lively today. The cherry blossoms are in full bloom."))
        unreal.log("  [OK] Spawned Living NPC: Leaf Villager (Patrolling Road)")

    # 4. Academy Student near the Tea House
    student = unreal.EditorLevelLibrary.spawn_actor_from_class(
        npc_class,
        unreal.Vector(-340.0, 1200.0, 4.0),
        unreal.Rotator(0.0, 45.0, 0.0)
    )
    if student:
        student.set_actor_label("NPC_AcademyStudent_TeaHouse")
        student.set_editor_property("npc_type", unreal.EOSNPCType.ACADEMY_STUDENT)
        student.set_editor_property("current_behavior", unreal.EOSNPCBehavior.IDLE_TALKING)
        student.set_editor_property("npcid", "NPC_ACADEMY_KONOHAMARU")
        student.set_editor_property("npc_name", unreal.Text("Academy Student"))
        student.set_editor_property("dialogue_text", unreal.Text("One day I'm going to become the greatest Hokage!"))
        unreal.log("  [OK] Spawned Living NPC: Academy Student (Idle Talking)")

    # 5. Dining Shinobi at Ichiraku counter stools (Sasuke & Sakura)
    def spawn_dining(mesh_path, mat_path, loc, rot, label):
        mesh = unreal.load_asset(mesh_path)
        mat  = unreal.load_asset(mat_path)
        actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
            unreal.StaticMeshActor,
            unreal.Vector(*loc),
            unreal.Rotator(*rot)
        )
        if actor:
            smc = actor.static_mesh_component
            if mesh:
                smc.set_static_mesh(mesh)
            if mat:
                for idx in range(max(smc.get_num_materials(), 1)):
                    smc.set_material(idx, mat)
            actor.set_actor_scale3d(unreal.Vector(0.95, 0.95, 0.95))
            actor.set_actor_label(label)
            unreal.log(f"  [OK] Spawned Dining Character: {label}")
        return actor

    spawn_dining(
        "/Game/Characters/Sasuke/Meshes/SK_Sasuke_Genin",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Navy",
        [-360.0, 260.0, 0.0], (0.0, 85.0, 0.0), "NPC_Sasuke_Dining"
    )

    spawn_dining(
        "/Game/Characters/Sakura/Meshes/SK_Sakura_Genin",
        "/Game/Characters/Naruto/Materials/MI_Naruto_Collar",
        [-360.0, 360.0, 0.0], (0.0, 85.0, 0.0), "NPC_Sakura_Dining"
    )

    # Save level
    unreal.EditorLoadingAndSavingUtils.save_current_level()
    unreal.log("======================================================================")
    unreal.log("  PHASE 7: NPC + PROPS + WORLD LIFE SUCCESSFULLY INTEGRATED!")
    unreal.log("======================================================================")

if __name__ == "__main__":
    integrate_world_life()
