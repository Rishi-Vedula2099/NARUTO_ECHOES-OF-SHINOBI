import unreal

world = unreal.EditorLoadingAndSavingUtils.load_map('/Game/Maps/GoldenTest/L_Konoha_GoldenStreet')
eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

actors = eas.get_all_level_actors()
unreal.log(f"=== CHECKING ALL {len(actors)} ACTORS FOR OVERHANGS/ROOFS ===")

for a in actors:
    if not a:
        continue
    label = a.get_actor_label()
    loc = a.get_actor_location()
    rot = a.get_actor_rotation()
    scale = a.get_actor_scale3d()
    cls = a.get_class().get_name()
    
    # Check bounds
    origin, box_extent = a.get_actor_bounds(False)
    
    # Check if bounds cover X=0, Y=0..1000 and Z > 100
    is_overhead = (abs(origin.x) < (box_extent.x + 200) and 
                   origin.y - box_extent.y < 2000 and 
                   origin.y + box_extent.y > -500 and 
                   origin.z + box_extent.z > 200)
    
    if is_overhead and cls != "PlayerStart" and not isinstance(a, (unreal.DirectionalLight, unreal.SkyLight, unreal.SkyAtmosphere, unreal.ExponentialHeightFog, unreal.PostProcessVolume)):
        unreal.log(f"  [OVERHEAD ACTOR] '{label}' ({cls})")
        unreal.log(f"      Loc=({loc.x:.1f}, {loc.y:.1f}, {loc.z:.1f})")
        unreal.log(f"      Rot=(P={rot.pitch:.1f}, Y={rot.yaw:.1f}, R={rot.roll:.1f})")
        unreal.log(f"      Scale=({scale.x:.2f}, {scale.y:.2f}, {scale.z:.2f})")
        unreal.log(f"      Origin=({origin.x:.1f}, {origin.y:.1f}, {origin.z:.1f}), Extent=({box_extent.x:.1f}, {box_extent.y:.1f}, {box_extent.z:.1f})")
