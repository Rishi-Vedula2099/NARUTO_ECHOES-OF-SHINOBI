"""
NARUTO: ECHOES OF SHINOBI
Apply Clean Anime Material Parameters
====================================
Disables Fresnel rim blowout by setting RimLightIntensity = 0.0 on all character
and environment materials. Enables warm emissive ONLY on paper lanterns.
Sets vibrant canonical anime colors across the board.
"""

import unreal

MEL = unreal.MaterialEditingLibrary
EAL = unreal.EditorAssetLibrary

def set_mi_params(path, base_color, roughness=0.8, specular=0.1, emissive_color=(0,0,0,1), emissive_intensity=0.0, rim_intensity=0.0):
    mi = unreal.load_asset(path)
    if not mi:
        unreal.log_warning(f"Could not load MI: {path}")
        return
    
    MEL.set_material_instance_vector_parameter_value(mi, "BaseColor", unreal.LinearColor(*base_color))
    MEL.set_material_instance_vector_parameter_value(mi, "SkinToneMultiplier", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))
    MEL.set_material_instance_scalar_parameter_value(mi, "Roughness", roughness)
    MEL.set_material_instance_scalar_parameter_value(mi, "Specular", specular)
    MEL.set_material_instance_vector_parameter_value(mi, "EmissiveColor", unreal.LinearColor(*emissive_color))
    MEL.set_material_instance_scalar_parameter_value(mi, "EmissiveIntensity", emissive_intensity)
    MEL.set_material_instance_scalar_parameter_value(mi, "RimLightIntensity", rim_intensity)
    MEL.update_material_instance(mi)
    EAL.save_loaded_asset(mi)
    unreal.log(f"  [OK] {mi.get_name()}: BaseColor={base_color[:3]}, Rim={rim_intensity}, Emissive={emissive_intensity}")

def apply_all():
    unreal.log("======================================================================")
    unreal.log("  CONFIGURING CLEAN ANIME MATERIAL PARAMETERS (NO FRESNEL BLOWOUT)")
    unreal.log("======================================================================")

    # 1. Naruto Character
    set_mi_params("/Game/Characters/Naruto/Materials/MI_Naruto_Skin",      (0.98, 0.82, 0.70, 1.0), roughness=0.85, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Characters/Naruto/Materials/MI_Naruto_Navy",      (0.06, 0.10, 0.22, 1.0), roughness=0.80, specular=0.08, rim_intensity=0.0)
    set_mi_params("/Game/Characters/Naruto/Materials/MI_Naruto_Hair",      (0.98, 0.82, 0.12, 1.0), roughness=0.65, specular=0.20, rim_intensity=0.0)
    set_mi_params("/Game/Characters/Naruto/Materials/MI_Naruto_Collar",    (0.92, 0.92, 0.88, 1.0), roughness=0.85, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit", (0.92, 0.35, 0.04, 1.0), roughness=0.82, specular=0.08, rim_intensity=0.0)

    # 2. Environment - Roads
    set_mi_params("/Game/Environment/Konoha/Roads/Materials/MI_Road_Cobblestone", (0.42, 0.39, 0.35, 1.0), roughness=0.88, specular=0.06, rim_intensity=0.0)

    # 3. Environment - Buildings
    set_mi_params("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Wood", (0.38, 0.22, 0.12, 1.0), roughness=0.82, specular=0.06, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_A",   (0.48, 0.32, 0.18, 1.0), roughness=0.80, specular=0.06, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Residence_B",   (0.44, 0.28, 0.16, 1.0), roughness=0.80, specular=0.06, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_Ichiraku_Noren",(0.88, 0.84, 0.74, 1.0), roughness=0.90, specular=0.04, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Buildings/Materials/MI_Bld_HokageMansion", (0.75, 0.15, 0.10, 1.0), roughness=0.70, specular=0.15, rim_intensity=0.0)

    # 4. Environment - Foliage
    set_mi_params("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_CedarNeedles_Stylized", (0.10, 0.26, 0.12, 1.0), roughness=0.80, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Foliage/Materials/MI_Veg_PinkPetals_Translucent", (0.95, 0.65, 0.75, 1.0), roughness=0.85, specular=0.08, rim_intensity=0.0)

    # 5. Environment - Landmarks & Props
    set_mi_params("/Game/Environment/Konoha/Landmarks/Materials/MI_HokageRock",        (0.55, 0.49, 0.42, 1.0), roughness=0.90, specular=0.04, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Landmarks/Materials/MI_Torii_Red",          (0.80, 0.12, 0.08, 1.0), roughness=0.70, specular=0.12, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Stone",     (0.52, 0.50, 0.46, 1.0), roughness=0.85, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Environment/Konoha/Props/Materials/MI_Prop_Lantern_Glow",      (1.0, 0.85, 0.45, 1.0), roughness=0.50, specular=0.20,
                  emissive_color=(1.0, 0.75, 0.35, 1.0), emissive_intensity=2.0, rim_intensity=0.0)

    # 6. Generic Environment fallbacks
    set_mi_params("/Game/Materials/Environment/MI_AnimeWood",    (0.40, 0.25, 0.14, 1.0), roughness=0.80, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Materials/Environment/MI_AnimeGround",  (0.45, 0.40, 0.34, 1.0), roughness=0.90, specular=0.04, rim_intensity=0.0)
    set_mi_params("/Game/Materials/Environment/MI_AnimeFoliage", (0.12, 0.28, 0.14, 1.0), roughness=0.80, specular=0.05, rim_intensity=0.0)
    set_mi_params("/Game/Materials/Characters/MI_AnimeMetal",   (0.65, 0.68, 0.72, 1.0), roughness=0.40, specular=0.45, rim_intensity=0.0)

    unreal.log("======================================================================")
    unreal.log("  ALL MATERIAL INSTANCES SUCCESSFULLY UPDATED!")
    unreal.log("======================================================================")

if __name__ == "__main__":
    apply_all()
