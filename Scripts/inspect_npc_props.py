import unreal

cls = unreal.load_class(None, "/Script/EchoesOfShinobi.EOSVillageNPC")
cdo = unreal.get_default_object(cls)

unreal.log("=== PROPERTIES ON EOSVillageNPC ===")
for p in dir(cdo):
    if "npc" in p.lower() or "dialogue" in p.lower() or "behavior" in p.lower():
        unreal.log(f"  * {p}")
