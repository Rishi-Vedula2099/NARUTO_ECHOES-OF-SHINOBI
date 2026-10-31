"""
NARUTO: ECHOES OF SHINOBI
Refined Valley of the End Landmark Production Asset (Stage 17 Clear Sky Benchmark)
==================================================================================
Builds the iconic opposing colossal stone titan statues:
- Madara Uchiha (spiky wild mane, armor plates, confrontation seal right hand)
- Hashirama Senju (straight layered hair, armor plates, confrontation seal left hand)
- Central river gorge canyon cliffs & cascading waterfall shelf
"""

import os
import math

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
MESH_DIR = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
os.makedirs(MESH_DIR, exist_ok=True)

def make_box(width, length, height, center=(0,0,0)):
    hw, hl, hh = width * 0.5, length * 0.5, height * 0.5
    cx, cy, cz = center
    verts = [
        [cx - hw, cy - hl, cz - hh], [cx + hw, cy - hl, cz - hh],
        [cx + hw, cy + hl, cz - hh], [cx - hw, cy + hl, cz - hh],
        [cx - hw, cy - hl, cz + hh], [cx + hw, cy - hl, cz + hh],
        [cx + hw, cy + hl, cz + hh], [cx - hw, cy + hl, cz + hh],
    ]
    uvs = [[0,0],[1,0],[1,1],[0,1], [0,0],[1,0],[1,1],[0,1]]
    normals = [
        [-0.577, -0.577, -0.577], [ 0.577, -0.577, -0.577],
        [ 0.577,  0.577, -0.577], [-0.577,  0.577, -0.577],
        [-0.577, -0.577,  0.577], [ 0.577, -0.577,  0.577],
        [ 0.577,  0.577,  0.577], [-0.577,  0.577,  0.577],
    ]
    faces = [
        [0, 2, 1], [0, 3, 2], [4, 5, 6], [4, 6, 7],
        [0, 1, 5], [0, 5, 4], [2, 3, 7], [2, 7, 6],
        [0, 4, 7], [0, 7, 3], [1, 2, 6], [1, 6, 5],
    ]
    return verts, uvs, faces, normals

def write_multi_material_obj(filepath, sections):
    with open(filepath, "w", encoding="utf-8") as f:
        f.write("# Valley of the End Landmark Production Mesh\n")
        f.write(f"mtllib {os.path.basename(filepath).replace('.obj', '.mtl')}\n\n")

        v_offset = 1
        for mat_name, mesh_list in sections.items():
            f.write(f"usemtl {mat_name}\n")
            f.write(f"g {mat_name}\n")
            for verts, uvs, faces, normals in mesh_list:
                for v in verts:
                    f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
                for uv in uvs:
                    f.write(f"vt {uv[0]:.4f} {uv[1]:.4f}\n")
                for n in normals:
                    f.write(f"vn {n[0]:.4f} {n[1]:.4f} {n[2]:.4f}\n")
                for face in faces:
                    i0 = face[0] + v_offset
                    i1 = face[1] + v_offset
                    i2 = face[2] + v_offset
                    f.write(f"f {i0}/{i0}/{i0} {i1}/{i1}/{i1} {i2}/{i2}/{i2}\n")
                v_offset += len(verts)
            f.write("\n")

    mtl_path = filepath.replace(".obj", ".mtl")
    with open(mtl_path, "w", encoding="utf-8") as f:
        for mat_name in sections.keys():
            f.write(f"newmtl {mat_name}\n")
            f.write("Ka 1.0 1.0 1.0\n")
            f.write("Kd 1.0 1.0 1.0\n")
            f.write("Ks 0.0 0.0 0.0\n")
            f.write("d 1.0\n\n")

def build_valley_of_the_end_statues():
    print("Authoring Refined SM_Lnd_ValleyOfTheEnd_Statues (Madara vs Hashirama Titans)...")
    sections = {
        "MI_Stone_Titan": [],
        "MI_Canyon_Cliff": [],
        "MI_Waterfall_Spray": []
    }

    # Canyon Gorge Base Walls (Left & Right Canyon Cliffs, Gorge width 1800cm)
    sections["MI_Canyon_Cliff"].append(make_box(1200, 800, 1600, center=(-1500, 0, 800))) # Left Cliff
    sections["MI_Canyon_Cliff"].append(make_box(1200, 800, 1600, center=( 1500, 0, 800))) # Right Cliff

    # Central Waterfall Cliff Face & Spilling Lip
    sections["MI_Waterfall_Spray"].append(make_box(1800, 120, 1200, center=(0, 280, 600))) # Roaring waterfall plane

    # --- MADARA UCHIHA STATUE (Left Side: X = -750, facing right +X) ---
    mx = -750
    # Pedestal & Torso (Armor Plate Cuirass)
    sections["MI_Stone_Titan"].append(make_box(450, 420, 750, center=(mx, 0, 1150)))
    # Segmented Shoulder Armor Plates (Sode)
    sections["MI_Stone_Titan"].append(make_box(180, 360, 240, center=(mx - 240, 0, 1380)))
    sections["MI_Stone_Titan"].append(make_box(180, 360, 240, center=(mx + 200, 0, 1380)))
    # Head & Helmet Brow
    sections["MI_Stone_Titan"].append(make_box(220, 240, 260, center=(mx, 0, 1650)))
    # Madara's Spiky Wild Hair Clumps (flaring left and backward)
    for sp_y, sp_z, sp_l in [(-120, 1720, 280), (0, 1780, 320), (120, 1720, 280), (-160, 1600, 240), (160, 1600, 240)]:
        sections["MI_Stone_Titan"].append(make_box(140, 90, sp_l, center=(mx - 150, sp_y, sp_z)))
    # Raised Hand Forming Confrontation Seal (対立の印) - Two fingers pointed upright
    sections["MI_Stone_Titan"].append(make_box(110, 110, 340, center=(mx + 260, 0, 1450))) # Forearm raised
    sections["MI_Stone_Titan"].append(make_box(65, 65, 260, center=(mx + 260, 0, 1700)))   # Index & Middle fingers upright

    # --- HASHIRAMA SENJU STATUE (Right Side: X = 750, facing left -X) ---
    hx = 750
    # Pedestal & Torso (Senju Clan Segmented Armor)
    sections["MI_Stone_Titan"].append(make_box(450, 420, 750, center=(hx, 0, 1150)))
    # Shoulder Armor Plates
    sections["MI_Stone_Titan"].append(make_box(180, 360, 240, center=(hx - 200, 0, 1380)))
    sections["MI_Stone_Titan"].append(make_box(180, 360, 240, center=(hx + 240, 0, 1380)))
    # Head & Face
    sections["MI_Stone_Titan"].append(make_box(220, 240, 260, center=(hx, 0, 1650)))
    # Hashirama's Long Straight Layered Hair (framing shoulders)
    sections["MI_Stone_Titan"].append(make_box(120, 260, 480, center=(hx + 140, 0, 1500)))
    # Raised Hand Forming Confrontation Seal (対立の印) - Fingers upright opposing Madara
    sections["MI_Stone_Titan"].append(make_box(110, 110, 340, center=(hx - 260, 0, 1450))) # Forearm raised
    sections["MI_Stone_Titan"].append(make_box(65, 65, 260, center=(hx - 260, 0, 1700)))   # Fingers upright

    out_path = os.path.join(MESH_DIR, "SM_Lnd_ValleyOfTheEnd_Statues.obj")
    write_multi_material_obj(out_path, sections)
    print(f"Exported refined {out_path} ({os.path.getsize(out_path):,} bytes).")

if __name__ == "__main__":
    build_valley_of_the_end_statues()
