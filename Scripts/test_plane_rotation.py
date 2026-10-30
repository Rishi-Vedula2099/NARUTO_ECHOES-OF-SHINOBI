import unreal

for pitch in [-90, 0, 90]:
    for yaw in [0, 90, 180, -90]:
        for roll in [-90, 0, 90]:
            r = unreal.Rotator(pitch=pitch, yaw=yaw, roll=roll)
            # Plane normal in local space is (0, 0, 1)
            # We want world normal to be (0, -1, 0) [facing -Y towards player at Y=0]
            # And width (local X or Y) to span along world X (-X to +X)
            # And height to span along world Z (0 to +Z)
            t = unreal.Transform(rotation=r.quaternion())
            up = t.transform_vector(unreal.Vector(0, 0, 1))
            loc_x = t.transform_vector(unreal.Vector(1, 0, 0))
            loc_y = t.transform_vector(unreal.Vector(0, 1, 0))
            if abs(up.y + 1.0) < 0.01: # facing -Y
                print(f"MATCH: pitch={pitch}, yaw={yaw}, roll={roll} -> Normal: ({up.x:.1f}, {up.y:.1f}, {up.z:.1f}), LocX: ({loc_x.x:.1f}, {loc_x.y:.1f}, {loc_x.z:.1f}), LocY: ({loc_y.x:.1f}, {loc_y.y:.1f}, {loc_y.z:.1f})")
