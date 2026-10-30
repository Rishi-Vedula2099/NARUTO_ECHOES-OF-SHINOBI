"""
Phase 8 Combat & VFX Audit Script
Audits L_Konoha_GoldenStreet and character combat data.
"""
import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def audit_phase8():
    unreal.log("======================================================================")
    unreal.log("  PHASE 8 AUDIT: COMBAT + JUTSU + VFX VERIFICATION")
    unreal.log("======================================================================")

    # 1. Audit Training Dummy in L_Konoha_GoldenStreet
    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    if not world:
        unreal.log_error(f"Failed to load map: {MAP_PKG}")
        return

    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = eas.get_all_level_actors()

    dummy_found = False
    for a in actors:
        if not a:
            continue
        label = a.get_actor_label()
        cls_name = a.get_class().get_name()
        if "TrainingDummy" in label or cls_name == "EOSTrainingDummy":
            dummy_found = True
            loc = a.get_actor_location()
            rot = a.get_actor_rotation()
            hp = a.get_editor_property("current_health")
            max_hp = a.get_editor_property("max_health")
            tags = list(a.tags)
            
            # Check components
            post_comp = a.get_component_by_class(unreal.StaticMeshComponent)
            comps = a.get_components_by_class(unreal.StaticMeshComponent)
            unreal.log(f"  [DUMMY FOUND] {label} ({cls_name}) at ({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f})")
            unreal.log(f"    HP: {hp:.1f}/{max_hp:.1f}, Tags: {tags}")
            for c in comps:
                sm = c.static_mesh
                sm_name = sm.get_name() if sm else "NONE"
                mat = c.get_material(0)
                mat_name = mat.get_name() if mat else "NONE"
                unreal.log(f"    Component '{c.get_name()}': Mesh={sm_name}, Material={mat_name}")

    if not dummy_found:
        unreal.log_error("  [FAIL] No EOSTrainingDummy found in level!")

    # 2. Audit Niagara Jutsu VFX Systems
    vfx_rasengan = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master")
    vfx_smoke    = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_KageBunshin_SmokeBurst")
    vfx_chidori  = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Chidori_Discharge_Master")
    vfx_fireball = unreal.load_asset("/Game/VFX/Niagara/Jutsu/NS_Goukakyuu_Fireball_Master")

    unreal.log("----------------------------------------------------------------------")
    unreal.log("Niagara VFX Verification:")
    unreal.log(f"  - NS_Rasengan_Orb_Master:        {'OK' if vfx_rasengan else 'MISSING'}")
    unreal.log(f"  - NS_KageBunshin_SmokeBurst:     {'OK' if vfx_smoke else 'MISSING'}")
    unreal.log(f"  - NS_Chidori_Discharge_Master:   {'OK' if vfx_chidori else 'MISSING'}")
    unreal.log(f"  - NS_Goukakyuu_Fireball_Master: {'OK' if vfx_fireball else 'MISSING'}")

    # 3. Check Materials assigned to combat props
    mat_wood = unreal.load_asset("/Game/Materials/Environment/MI_AnimeWood")
    mat_cloth = unreal.load_asset("/Game/Materials/Characters/MI_AnimeCloth")
    unreal.log(f"  - MI_AnimeWood:  {'OK' if mat_wood else 'MISSING'}")
    unreal.log(f"  - MI_AnimeCloth: {'OK' if mat_cloth else 'MISSING'}")

    unreal.log("======================================================================")
    unreal.log("  PHASE 8 AUDIT COMPLETE!")
    unreal.log("======================================================================")

if __name__ == "__main__":
    audit_phase8()
