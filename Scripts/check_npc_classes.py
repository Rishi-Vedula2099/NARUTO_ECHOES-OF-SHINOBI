import unreal

npc_paths = [
    "/Game/Characters/NPCs/Guards/Meshes/SK_Chunin_GateGuard",
    "/Game/Characters/NPCs/Shopkeepers/Meshes/SK_Shopkeeper_Teuchi",
    "/Game/Characters/NPCs/Students/Meshes/SK_AcademyStudent_Male",
    "/Game/Characters/NPCs/KonohaVillagers/Meshes/SK_Villager_Male01"
]

for p in npc_paths:
    obj = unreal.load_asset(p)
    if obj:
        unreal.log(f"[NPC ASSET] {p} -> {obj.get_class().get_name()}")
    else:
        unreal.log_warning(f"[NPC ASSET] FAILED TO LOAD: {p}")
