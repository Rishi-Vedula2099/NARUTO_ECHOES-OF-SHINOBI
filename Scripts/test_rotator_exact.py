import unreal

r = unreal.Rotator(pitch=10.0, yaw=20.0, roll=30.0)
unreal.log(f"Explicit keyword Rotator: pitch={r.pitch}, yaw={r.yaw}, roll={r.roll}")
