import unreal

hud = unreal.new_object(unreal.EOSShinobiHUD)
unreal.log_warning(f"type: {type(hud.get_current_style_rank)}, repr: {repr(hud.get_current_style_rank)}")
res = hud.get_current_style_rank()
unreal.log_warning(f"res: {repr(res)}, type(res): {type(res)}")
