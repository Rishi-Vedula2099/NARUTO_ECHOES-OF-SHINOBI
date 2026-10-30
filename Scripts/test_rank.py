import unreal

hud = unreal.new_object(unreal.EOSShinobiHUD)
unreal.log("RANK ATTRS: " + str([x for x in dir(hud) if 'rank' in x.lower() or 'style' in x.lower() or 'combo' in x.lower()]))

for attr in [x for x in dir(hud) if 'rank' in x.lower() or 'style' in x.lower() or 'combo' in x.lower()]:
    v = getattr(hud, attr)
    val = v() if callable(v) else v
    unreal.log(f"ATTR {attr} = {val} (type={type(val)})")

hud.register_hit(8, 800.0)
for attr in [x for x in dir(hud) if 'rank' in x.lower() or 'style' in x.lower() or 'combo' in x.lower()]:
    v = getattr(hud, attr)
    val = v() if callable(v) else v
    unreal.log(f"AFTER HIT ATTR {attr} = {val} (type={type(val)})")
