import trimesh
import numpy as np
import os

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
glb_path = os.path.join(PROJECT_DIR, "Content", "SourceMeshes", "test_box.glb")

# Create a box mesh
box = trimesh.creation.box(extents=[20, 20, 40])
box.export(glb_path)
print(f"Exported test_box.glb to {glb_path}")
