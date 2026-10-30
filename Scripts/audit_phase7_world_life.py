"""
Phase 7 Level & World Life Audit Script
Audits L_Konoha_GoldenStreet for all NPC, prop, and world life actors.
"""
import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def audit_phase7():
    unreal.log("======================================================================")
    unreal.log("  PHASE 7 AUDIT: LEVEL ACTORS & WORLD LIFE INSPECTION")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    if not world:
        unreal.log_error(f"Failed to load map: {MAP_PKG}")
        return

    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = eas.get_all_level_actors()
    unreal.log(f"Total Level Actors: {len(actors)}")

    npc_count = 0
    dining_count = 0
    props_count = 0
    foliage_count = 0
    building_count = 0

    npc_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSVillageNPC")

    for a in actors:
        if not a:
            continue
        label = a.get_actor_label()
        cls_name = a.get_class().get_name()
        
        if isinstance(a, unreal.EOSVillageNPC) or cls_name == "EOSVillageNPC":
            npc_count += 1
            npc_id = a.get_editor_property("npcid")
            npc_type = a.get_editor_property("npc_type")
            behavior = a.get_editor_property("current_behavior")
            loc = a.get_actor_location()
            tags = list(a.tags)
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            mesh_name = smc.static_mesh.get_name() if (smc and smc.static_mesh) else "NONE"
            mat_name = smc.get_material(0).get_name() if (smc and smc.get_material(0)) else "NONE"
            unreal.log(f"  [LIVING NPC {npc_count}] {label}: ID='{npc_id}', Type={npc_type}, Behavior={behavior}, Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}), Mesh={mesh_name}, Mat={mat_name}, Tags={tags}")
        elif "Dining" in label:
            dining_count += 1
            loc = a.get_actor_location()
            smc = a.get_component_by_class(unreal.StaticMeshComponent)
            mesh_name = smc.static_mesh.get_name() if (smc and smc.static_mesh) else "NONE"
            unreal.log(f"  [DINING CHARACTER {dining_count}] {label}: Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f}), Mesh={mesh_name}")
        elif "Prop_" in label or "Lantern" in label or "Bench" in label or "Barrel" in label or "Crate" in label:
            props_count += 1
        elif "Foliage_" in label or "Tree" in label or "Sakura" in label or "Cedar" in label or "Shrub" in label:
            foliage_count += 1
        elif "Building_" in label or "House" in label or "Shop" in label or "Hokage" in label:
            building_count += 1

    unreal.log("----------------------------------------------------------------------")
    unreal.log(f"Summary Counts:")
    unreal.log(f"  - Living AEOSVillageNPC Actors: {npc_count}")
    unreal.log(f"  - Dining Shinobi Actors:       {dining_count}")
    unreal.log(f"  - Building Actors:             {building_count}")
    unreal.log(f"  - Foliage Actors:              {foliage_count}")
    unreal.log(f"  - Street Prop Actors:          {props_count}")
    unreal.log(f"  - Total Audited Actors:        {len(actors)}")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase7()
