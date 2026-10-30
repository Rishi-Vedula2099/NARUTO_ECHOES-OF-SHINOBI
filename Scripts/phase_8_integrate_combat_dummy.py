"""
NARUTO: ECHOES OF SHINOBI
Phase 8: Integrate Interactive Training Dummy into L_Konoha_GoldenStreet
"""
import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"

def integrate_combat_dummy():
    unreal.log("======================================================================")
    unreal.log(f"  INTEGRATING PHASE 8 COMBAT TRAINING DUMMY: {MAP_PKG}")
    unreal.log("======================================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
    if not world:
        unreal.log_error(f"Failed to load map {MAP_PKG}")
        return

    eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = eas.get_all_level_actors()

    # Clean up previous training dummy if any
    for a in actors:
        if not a:
            continue
        label = a.get_actor_label()
        cls_name = a.get_class().get_name()
        if "TrainingDummy" in label or cls_name == "EOSTrainingDummy":
            eas.destroy_actor(a)

    dummy_class = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSTrainingDummy")
    if not dummy_class:
        unreal.log_error("Could not load /Script/EchoesOfShinobi.EOSTrainingDummy")
        return

    # Position in training area along the street (X=-320, Y=1400, Z=0) near the tea house training grounds
    dummy = unreal.EditorLevelLibrary.spawn_actor_from_class(
        dummy_class,
        unreal.Vector(-320.0, 1400.0, 0.0),
        unreal.Rotator(0.0, -90.0, 0.0)
    )

    if dummy:
        dummy.set_actor_label("Prop_Interactive_TrainingDummy")
        dummy.tags.append("CombatTarget")
        dummy.tags.append("TrainingDummy")
        unreal.log(f"  [OK] Spawned Interactive Training Dummy: {dummy.get_actor_label()} at (-320, 1400, 0)")

    unreal.EditorLoadingAndSavingUtils.save_current_level()
    unreal.log("======================================================================")
    unreal.log("  PHASE 8 COMBAT TRAINING DUMMY INTEGRATED SUCCESSFULLY!")
    unreal.log("======================================================================")

if __name__ == "__main__":
    integrate_combat_dummy()
