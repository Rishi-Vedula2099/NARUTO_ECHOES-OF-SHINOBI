import unreal

master_path = "/Game/Materials/Master/M_AnimeCel_Master"
master_mat = unreal.load_asset(master_path)

if master_mat:
    print(f"Master Material Name: {master_mat.get_name()}")
    # Check parameters
    MEL = unreal.MaterialEditingLibrary
    scalars = MEL.get_scalar_parameter_names(master_mat)
    vectors = MEL.get_vector_parameter_names(master_mat)
    textures = MEL.get_texture_parameter_names(master_mat)
    print(f"Scalars: {scalars}")
    print(f"Vectors: {vectors}")
    print(f"Textures: {textures}")

    with open(r"C:\Projects\NARUTO_ECHOES-OF-SHINOBI\Scripts\mat_params.txt", "w") as f:
        f.write(f"Scalars: {scalars}\nVectors: {vectors}\nTextures: {textures}\n")
else:
    print("Master material not found")

