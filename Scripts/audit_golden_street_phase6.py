import unreal

def audit_level():
    pkg = "/Game/Maps/GoldenTest/L_Konoha_GoldenStreet"
    unreal.log("===========================================================")
    unreal.log(f"AUDITING PHASE 6 GOLDEN STREET MAP: {pkg}")
    unreal.log("===========================================================")

    world = unreal.EditorLoadingAndSavingUtils.load_map(pkg)
    if not world:
        unreal.log_error(f"Failed to load map {pkg}")
        return

    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
    unreal.log(f"Total Actors in Level: {len(actors)}")

    lights = []
    buildings = []
    roads = []
    foliage = []
    props = []
    landmarks = []
    characters = []
    others = []

    for a in actors:
        if not a:
            continue
        cls_name = a.get_class().get_name()
        label = a.get_actor_label()
        loc = a.get_actor_location()

        if isinstance(a, unreal.Light) or "Light" in cls_name or "Atmosphere" in cls_name or "Fog" in cls_name or "PostProcess" in cls_name:
            details = ""
            if isinstance(a, unreal.DirectionalLight):
                dlc = a.get_component_by_class(unreal.DirectionalLightComponent)
                if dlc:
                    details = f" [Intensity={dlc.get_editor_property('intensity')}, Color={dlc.get_editor_property('light_color')}]"
            elif isinstance(a, unreal.SkyLight):
                slc = a.get_component_by_class(unreal.SkyLightComponent)
                if slc:
                    details = f" [Intensity={slc.get_editor_property('intensity')}, Color={slc.get_editor_property('light_color')}]"
            lights.append((label, cls_name, (round(loc.x, 1), round(loc.y, 1), round(loc.z, 1)), details))

        elif isinstance(a, unreal.StaticMeshActor):
            smc = a.static_mesh_component
            sm = smc.static_mesh if smc else None
            sm_name = sm.get_name() if sm else "None"
            mat_names = []
            if smc:
                for m_idx in range(smc.get_num_materials()):
                    m = smc.get_material(m_idx)
                    mat_names.append(m.get_name() if m else "None")

            info = {
                "label": label,
                "sm": sm_name,
                "mats": mat_names,
                "loc": (round(loc.x, 1), round(loc.y, 1), round(loc.z, 1))
            }

            if any(k in sm_name for k in ["Bld_", "Residence", "Ichiraku", "House"]):
                buildings.append(info)
            elif any(k in sm_name for k in ["Road", "Cobblestone", "Paved"]):
                roads.append(info)
            elif any(k in sm_name for k in ["Veg_", "Tree", "Sakura", "Cedar", "Shrub", "Grass"]):
                foliage.append(info)
            elif any(k in sm_name for k in ["Prop_", "Lantern", "Bench", "Crate", "Barrel", "Banner", "Pole"]):
                props.append(info)
            elif any(k in sm_name for k in ["Lnd_", "Hokage", "Monument", "Gate"]) or "Backdrop" in label:
                landmarks.append(info)
            else:
                others.append(info)
        else:
            others.append({"label": label, "cls": cls_name, "loc": (round(loc.x, 1), round(loc.y, 1), round(loc.z, 1))})

    unreal.log(f"\n--- LIGHTING & ATMOSPHERE ({len(lights)}) ---")
    for lbl, cls_n, loc, details in lights:
        unreal.log(f"  * [{cls_n}] '{lbl}' at {loc}{details}")

    unreal.log(f"\n--- BUILDINGS ({len(buildings)}) ---")
    for b in buildings:
        unreal.log(f"  * '{b['label']}': Mesh={b['sm']}, Mats={b['mats']}, Loc={b['loc']}")

    unreal.log(f"\n--- ROADS & GROUND ({len(roads)}) ---")
    for r in roads:
        unreal.log(f"  * '{r['label']}': Mesh={r['sm']}, Mats={r['mats']}, Loc={r['loc']}")

    unreal.log(f"\n--- FOLIAGE & VEGETATION ({len(foliage)}) ---")
    for f in foliage:
        unreal.log(f"  * '{f['label']}': Mesh={f['sm']}, Mats={f['mats']}, Loc={f['loc']}")

    unreal.log(f"\n--- PROPS & STREET LIFE ({len(props)}) ---")
    for p in props:
        unreal.log(f"  * '{p['label']}': Mesh={p['sm']}, Mats={p['mats']}, Loc={p['loc']}")

    unreal.log(f"\n--- LANDMARKS & VISTA ({len(landmarks)}) ---")
    for l in landmarks:
        unreal.log(f"  * '{l['label']}': Mesh={l['sm']}, Mats={l['mats']}, Loc={l['loc']}")

    unreal.log(f"\n--- OTHERS / NPCS / ENTITIES ({len(others)}) ---")
    for o in others:
        unreal.log(f"  * {o}")

    unreal.log("\n===========================================================")
    unreal.log("AUDIT SUMMARY:")
    unreal.log(f"  Buildings: {len(buildings)}")
    unreal.log(f"  Roads:     {len(roads)}")
    unreal.log(f"  Foliage:   {len(foliage)}")
    unreal.log(f"  Props:     {len(props)}")
    unreal.log(f"  Landmarks: {len(landmarks)}")
    unreal.log(f"  Lighting:  {len(lights)}")
    unreal.log(f"  Others:    {len(others)}")
    unreal.log("===========================================================")

if __name__ == "__main__":
    audit_level()
