"""
NARUTO: ECHOES OF SHINOBI
Phase 1: Asset-Aware Camera Framing, Street Layout & Vista Clearing
===================================================================
Executes:
1. Clearing central street axis (moves Bld_HokageMansion_MidVista into midground foliage).
2. Correcting building orientations so storefronts/balconies face the street center (Yaw 90 for Left, -90 for Right).
3. Widening building setbacks to X = ±680 to frame a wide open village avenue.
4. Repositioning Hokage Rock vista backdrop plane at Y = 4200, Z = 1100 with NoCollision.
5. Calibrating PlayerStart to Pitch -8.0, Yaw 90.0 for exact North Star composition.
"""

import unreal

MAP_PKG = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
unreal.log(f"=== Loading Map: {MAP_PKG} ===")
world = unreal.EditorLoadingAndSavingUtils.load_map(MAP_PKG)
if not world:
    unreal.log_error(f"Failed to load map {MAP_PKG}")
    quit()

eas = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
actors = eas.get_all_level_actors()

actor_map = {}
for a in actors:
    if a:
        actor_map[a.get_actor_label()] = a

unreal.log(f"Found {len(actor_map)} actors in level.")

# 1. Clear street centerline: Move Hokage Mansion MidVista into midground background
mid_vista = actor_map.get("Bld_HokageMansion_MidVista")
if mid_vista:
    # Move to Y=3500, Z=120, scale down slightly so it sits nestled in midground foliage
    mid_vista.set_actor_location(unreal.Vector(0.0, 3500.0, 120.0), False, False)
    mid_vista.set_actor_scale3d(unreal.Vector(0.85, 0.85, 0.85))
    unreal.log("-> Repositioned Bld_HokageMansion_MidVista to (0, 3500, 120)")

# 2. Position Hokage Rock Vista Backdrop
backdrop = actor_map.get("Backdrop_Konoha_NorthStar_Vista")
if backdrop:
    backdrop.set_actor_location(unreal.Vector(0.0, 4200.0, 1100.0), False, False)
    backdrop.set_actor_rotation(unreal.Rotator(90.0, 0.0, 0.0), False)
    backdrop.set_actor_scale3d(unreal.Vector(75.0, 38.0, 1.0))
    smc = backdrop.get_component_by_class(unreal.StaticMeshComponent)
    if smc:
        smc.set_collision_profile_name("NoCollision")
        smc.set_collision_enabled(unreal.CollisionEnabled.NO_COLLISION)
    unreal.log("-> Repositioned Backdrop_Konoha_NorthStar_Vista to (0, 4200, 1100), NoCollision")

# 3. Orient and widen West Buildings (Left Sidewalk) - Front faces street (+X direction -> Yaw 90.0)
west_updates = {
    "Bld_IchirakuRamen": (unreal.Vector(-680.0, 320.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Prop_ShopBanner_Ichiraku": (unreal.Vector(-490.0, 220.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Lantern_Ichiraku_180": (unreal.Vector(-475.0, 180.0, 185.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Lantern_Ichiraku_300": (unreal.Vector(-475.0, 300.0, 185.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Lantern_Ichiraku_420": (unreal.Vector(-475.0, 420.0, 185.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Bld_KonohaWeaponShop": (unreal.Vector(-700.0, 1100.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Bld_KonohaMissionOffice": (unreal.Vector(-700.0, 1850.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
}

for label, (loc, rot) in west_updates.items():
    act = actor_map.get(label)
    if act:
        act.set_actor_location(loc, False, False)
        act.set_actor_rotation(rot, False)
        unreal.log(f"-> Updated {label}: Loc={loc}, Rot={rot}")

# 4. Orient and widen East Buildings (Right Sidewalk) - Front faces street (-X direction -> Yaw -90.0)
east_updates = {
    "Bld_KonohaResidence_A": (unreal.Vector(680.0, 260.0, 0.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Lantern_Residence_180": (unreal.Vector(475.0, 180.0, 185.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Lantern_Residence_300": (unreal.Vector(475.0, 300.0, 185.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Bld_KonohaResidence_B": (unreal.Vector(700.0, 980.0, 0.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Bld_KonohaTeaHouse": (unreal.Vector(700.0, 1800.0, 0.0), unreal.Rotator(0.0, -90.0, 0.0)),
}

for label, (loc, rot) in east_updates.items():
    act = actor_map.get(label)
    if act:
        act.set_actor_location(loc, False, False)
        act.set_actor_rotation(rot, False)
        unreal.log(f"-> Updated {label}: Loc={loc}, Rot={rot}")

# 5. Adjust Sidewalk positions to match wider street profile (X = ±460)
for i in range(13):
    sw_l_label = f"Sidewalk_Left_{i:02d}"
    sw_r_label = f"Sidewalk_Right_{i:02d}"
    sw_l = actor_map.get(sw_l_label)
    if sw_l:
        loc = sw_l.get_actor_location()
        sw_l.set_actor_location(unreal.Vector(-460.0, loc.y, 4.0), False, False)
    sw_r = actor_map.get(sw_r_label)
    if sw_r:
        loc = sw_r.get_actor_location()
        sw_r.set_actor_location(unreal.Vector(460.0, loc.y, 4.0), False, False)

# 6. Adjust Street Props (Utility poles, stone lanterns, benches, barrels)
prop_updates = {
    "Prop_UtilityPole_Left_01": (unreal.Vector(-470.0, 650.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Prop_UtilityPole_Right_01": (unreal.Vector(470.0, 50.0, 0.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Prop_StoneLantern_Left_01": (unreal.Vector(-360.0, 100.0, 0.0), unreal.Rotator(0.0, 0.0, 0.0)),
    "Prop_StoneLantern_Right_01": (unreal.Vector(360.0, 100.0, 0.0), unreal.Rotator(0.0, 0.0, 0.0)),
    "Prop_Bench_Left_01": (unreal.Vector(-450.0, 800.0, 0.0), unreal.Rotator(0.0, 90.0, 0.0)),
    "Prop_Bench_Right_01": (unreal.Vector(450.0, 480.0, 0.0), unreal.Rotator(0.0, -90.0, 0.0)),
    "Prop_RainBarrel_01": (unreal.Vector(480.0, 560.0, 0.0), unreal.Rotator(0.0, 0.0, 0.0)),
    "Prop_SupplyCrate_01": (unreal.Vector(480.0, 620.0, 0.0), unreal.Rotator(0.0, 15.0, 0.0)),
    "Veg_Sakura_FrontLeft": (unreal.Vector(-620.0, 620.0, 0.0), unreal.Rotator(0.0, 75.0, 0.0)),
    "Veg_Sakura_FrontRight": (unreal.Vector(620.0, 650.0, 0.0), unreal.Rotator(0.0, 30.0, 0.0)),
}

for label, (loc, rot) in prop_updates.items():
    act = actor_map.get(label)
    if act:
        act.set_actor_location(loc, False, False)
        act.set_actor_rotation(rot, False)

# 7. PlayerStart calibration (Grounded, Pitch -8.0, Yaw 90 facing +Y)
ps = actor_map.get("PlayerStart_GoldenView")
if ps:
    ps.set_actor_location(unreal.Vector(0.0, 0.0, 95.0), False, False)
    ps.set_actor_rotation(unreal.Rotator(-8.0, 90.0, 0.0), False)
    unreal.log("-> Calibrated PlayerStart_GoldenView: Loc=(0, 0, 95), Rot=(-8.0, 90, 0)")

# Save level
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
unreal.log("=== Phase 1 Layout and Camera Calibration Saved Successfully ===")
