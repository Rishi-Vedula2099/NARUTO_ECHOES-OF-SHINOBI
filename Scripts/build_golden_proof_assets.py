"""
NARUTO: ECHOES OF SHINOBI
Golden Proof #1 — Procedural 3D Asset Generation (UV-Mapped OBJ)
=================================================================
Uses pure Python + numpy to generate UV-mapped .obj files for
all Golden Proof #1 assets. Each face gets planar-UV coordinates.

Trimesh is used only for mesh creation; OBJ is written manually
to ensure valid `vt` UV coords that Unreal Engine's Interchange
OBJ importer requires.

Output: Content/SourceMeshes/<AssetName>.obj
"""

import os
import math
import numpy as np
import trimesh
from trimesh import creation as tc

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT_DIR  = os.path.join(BASE_DIR, "Content", "SourceMeshes")
os.makedirs(OUT_DIR, exist_ok=True)


def planar_uv(vertices, faces):
    """
    Generate planar UV coordinates for each face vertex by projecting
    each triangle's centroid onto its dominant axis.
    Returns uvs per-face-vertex as list of (u,v) tuples.
    """
    uvs_per_face = []
    vx = np.asarray(vertices[:, 0], dtype=float)
    vy = np.asarray(vertices[:, 1], dtype=float)
    vz = np.asarray(vertices[:, 2], dtype=float)
    rx = max(float(np.ptp(vx)), 1e-6)
    ry = max(float(np.ptp(vy)), 1e-6)
    rz = max(float(np.ptp(vz)), 1e-6)
    for face in faces:
        tri = vertices[face]  # 3x3
        e0 = tri[1] - tri[0]
        e1 = tri[2] - tri[0]
        n = np.cross(e0, e1)
        anx, any_, anz = abs(n[0]), abs(n[1]), abs(n[2])
        for v in tri:
            if anz >= anx and anz >= any_:
                u = (v[0] - vx.min()) / rx
                vv = (v[1] - vy.min()) / ry
            elif anx >= any_:
                u = (v[1] - vy.min()) / ry
                vv = (v[2] - vz.min()) / rz
            else:
                u = (v[0] - vx.min()) / rx
                vv = (v[2] - vz.min()) / rz
            uvs_per_face.append((float(np.clip(u, 0, 1)), float(np.clip(vv, 0, 1))))
    return uvs_per_face


def write_obj_with_uv(mesh, path, mtl_name="M_AnimeCel_Master"):
    """
    Write a trimesh mesh as OBJ with proper UV coords + mtllib reference.
    """
    verts  = mesh.vertices
    faces  = mesh.faces
    norms  = mesh.vertex_normals
    uvs    = planar_uv(verts, faces)

    with open(path, "w", encoding="utf-8") as f:
        obj_name = os.path.splitext(os.path.basename(path))[0]
        mtl_path = path.replace(".obj", ".mtl")

        f.write(f"# NARUTO: ECHOES OF SHINOBI — {obj_name}\n")
        f.write(f"# Procedurally generated UV-mapped mesh\n")
        f.write(f"mtllib {obj_name}.mtl\n")
        f.write(f"o {obj_name}\n\n")

        # Vertices
        for v in verts:
            f.write(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}\n")
        f.write("\n")

        # UV coords (one per face-vertex)
        for u, v in uvs:
            f.write(f"vt {u:.6f} {v:.6f}\n")
        f.write("\n")

        # Vertex normals
        for n in norms:
            f.write(f"vn {n[0]:.6f} {n[1]:.6f} {n[2]:.6f}\n")
        f.write("\n")

        f.write(f"usemtl {mtl_name}\n")

        # Faces — 1-indexed, with per-face-vertex UV index
        uv_idx = 1
        for fi, face in enumerate(faces):
            v0, v1, v2 = face[0]+1, face[1]+1, face[2]+1
            n0, n1, n2 = face[0]+1, face[1]+1, face[2]+1
            u0, u1, u2 = uv_idx, uv_idx+1, uv_idx+2
            f.write(f"f {v0}/{u0}/{n0} {v1}/{u1}/{n1} {v2}/{u2}/{n2}\n")
            uv_idx += 3

    # Write minimal MTL file
    with open(mtl_path, "w", encoding="utf-8") as f:
        f.write(f"newmtl {mtl_name}\n")
        f.write("Ka 1.0 1.0 1.0\n")
        f.write("Kd 1.0 0.6 0.1\n")  # orange base for Naruto theme
        f.write("Ks 0.1 0.1 0.1\n")
        f.write("Ns 32.0\n")
        f.write("d 1.0\n")

    return len(verts), len(faces)


def save(mesh, name):
    path = os.path.join(OUT_DIR, name + ".obj")
    verts, faces = write_obj_with_uv(mesh, path)
    print(f"  [OK] {name}.obj  |  {verts} verts  |  {faces} tris")
    return path


def combine(*meshes):
    return trimesh.util.concatenate(meshes)


def translate(mesh, xyz):
    m = mesh.copy()
    m.apply_translation(xyz)
    return m


# ═══════════════════════════════════════════════════════════════════════════════
# 1. SK_NARUTO_GENIN — 175cm humanoid
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[1/6] SK_Naruto_Genin ...")
def build_capsule(radius, height, sections=12):
    cyl  = tc.cylinder(radius=radius, height=height, sections=sections)
    top  = tc.icosphere(radius=radius, subdivisions=1)
    bot  = tc.icosphere(radius=radius, subdivisions=1)
    top  = translate(top,  [0, 0,  height/2])
    bot  = translate(bot,  [0, 0, -height/2])
    return combine(cyl, top, bot)

head     = translate(tc.icosphere(radius=13.0, subdivisions=2), [0, 0, 165])
hair     = translate(tc.icosphere(radius=15.0, subdivisions=2), [0, 0, 170])
headband = translate(tc.box(extents=[28, 4, 8]),                 [0, -14, 166])
torso    = translate(build_capsule(14, 52, 16),                  [0, 0, 108])
l_arm    = translate(build_capsule(5.5, 30, 10),                 [-22, 0, 115])
r_arm    = translate(build_capsule(5.5, 30, 10),                 [ 22, 0, 115])
l_fore   = translate(build_capsule(4.5, 26, 10),                 [-22, 0, 82])
r_fore   = translate(build_capsule(4.5, 26, 10),                 [ 22, 0, 82])
l_hand   = translate(tc.icosphere(radius=6, subdivisions=1),     [-22, 0, 65])
r_hand   = translate(tc.icosphere(radius=6, subdivisions=1),     [ 22, 0, 65])
hips     = translate(build_capsule(13, 18, 16),                  [0, 0, 68])
l_thigh  = translate(build_capsule(7.5, 36, 12),                 [-9, 0, 44])
r_thigh  = translate(build_capsule(7.5, 36, 12),                 [ 9, 0, 44])
l_shin   = translate(build_capsule(5.5, 32, 12),                 [-9, 0, 10])
r_shin   = translate(build_capsule(5.5, 32, 12),                 [ 9, 0, 10])
l_foot   = translate(tc.box(extents=[14, 22, 7]),                [-9, 2, -3])
r_foot   = translate(tc.box(extents=[14, 22, 7]),                [ 9, 2, -3])

naruto_mesh = combine(
    head, hair, headband, torso,
    l_arm, r_arm, l_fore, r_fore, l_hand, r_hand,
    hips, l_thigh, r_thigh, l_shin, r_shin, l_foot, r_foot
)
save(naruto_mesh, "SK_Naruto_Genin")


# ═══════════════════════════════════════════════════════════════════════════════
# 2. SM_BLD_ICHIRAKURAMEN
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[2/6] SM_Bld_IchirakuRamen ...")
back_w  = translate(tc.box([400, 20, 260]),  [0, -140, 130])
left_w  = translate(tc.box([20, 300, 260]),  [-190, 0, 130])
right_w = translate(tc.box([20, 300, 260]),  [ 190, 0, 130])
counter = translate(tc.box([360, 40, 90]),   [0, 60, 45])
c_front = translate(tc.box([360, 8, 90]),    [0, 80, 45])
floor_s = translate(tc.box([400, 300, 10]),  [0, 0, -5])
roof    = translate(tc.box([430, 330, 30]),  [0, 0, 265])
noren_l = translate(tc.box([160, 4, 80]),    [-90, 90, 170])
noren_r = translate(tc.box([160, 4, 80]),    [ 90, 90, 170])
lan_b   = translate(tc.cylinder(16, 30, 16), [0, 30, 230])
lan_t   = translate(tc.cylinder(8, 8, 8),    [0, 30, 248])
lan_bot = translate(tc.cylinder(8, 8, 8),    [0, 30, 212])
beam_l  = translate(tc.cylinder(6, 260, 8),  [-170, 90, 130])
beam_r  = translate(tc.cylinder(6, 260, 8),  [ 170, 90, 130])

ichiraku = combine(back_w, left_w, right_w, counter, c_front, floor_s,
                   roof, noren_l, noren_r, lan_b, lan_t, lan_bot, beam_l, beam_r)
save(ichiraku, "SM_Bld_IchirakuRamen")


# ═══════════════════════════════════════════════════════════════════════════════
# 3. SM_ROAD_PAVED_01
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[3/6] SM_Road_Paved_01 ...")
road_b  = translate(tc.box([400, 400, 12]), [0, 0, 6])
curb_l  = translate(tc.box([20, 400, 18]), [-190, 0, 9])
curb_r  = translate(tc.box([20, 400, 18]), [ 190, 0, 9])
stones  = []
sw, sh  = 44, 44
for ix in range(-4, 5):
    for iy in range(-4, 5):
        s = translate(tc.box([sw-3, sh-3, 4]), [ix*sw, iy*sh, 14])
        stones.append(s)
road_mesh = combine(road_b, curb_l, curb_r, *stones)
save(road_mesh, "SM_Road_Paved_01")


# ═══════════════════════════════════════════════════════════════════════════════
# 4. SM_PROP_STONELANTERN_01
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[4/6] SM_Prop_StoneLantern_01 ...")
cap    = translate(tc.cylinder(28, 10, 6), [0, 0, 140])
firbox = translate(tc.cylinder(20, 40, 6), [0, 0, 100])
collar = translate(tc.cylinder(10, 12, 8), [0, 0, 77])
shaft  = translate(tc.cylinder(8,  60, 8), [0, 0, 42])
plin_t = translate(tc.box([40, 40, 14]),   [0, 0, 7])
plin_b = translate(tc.box([56, 56, 8]),    [0, 0, -4])
save(combine(cap, firbox, collar, shaft, plin_t, plin_b), "SM_Prop_StoneLantern_01")


# ═══════════════════════════════════════════════════════════════════════════════
# 5. SM_VEG_CEDAR_LARGE
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[5/6] SM_Veg_Cedar_Large ...")
trunk = translate(tc.cylinder(12, 220, 10), [0, 0, 110])
t1    = translate(tc.cone(90, 100, 14),     [0, 0, 120])
t2    = translate(tc.cone(70, 80,  14),     [0, 0, 170])
t3    = translate(tc.cone(50, 60,  14),     [0, 0, 212])
t4    = translate(tc.cone(30, 45,  14),     [0, 0, 245])
save(combine(trunk, t1, t2, t3, t4), "SM_Veg_Cedar_Large")


# ═══════════════════════════════════════════════════════════════════════════════
# 6. BP_TRAININGDUMMY
# ═══════════════════════════════════════════════════════════════════════════════
print("\n[6/6] BP_TrainingDummy ...")
post      = translate(tc.cylinder(7, 200, 10), [0, 0, 100])
body      = translate(tc.cylinder(22, 100, 12),[0, 0, 165])
d_head    = translate(tc.icosphere(18, 2),     [0, 0, 228])
arm_l     = tc.cylinder(6, 80, 8)
arm_l.apply_transform(trimesh.transformations.rotation_matrix(math.radians(90),[1,0,0]))
arm_l     = translate(arm_l, [-30, 0, 185])
arm_r     = tc.cylinder(6, 80, 8)
arm_r.apply_transform(trimesh.transformations.rotation_matrix(math.radians(90),[1,0,0]))
arm_r     = translate(arm_r, [ 30, 0, 185])
base      = translate(tc.box([60, 60, 14]),    [0, 0, 7])
save(combine(post, body, d_head, arm_l, arm_r, base), "BP_TrainingDummy")


# ─── REPORT ───────────────────────────────────────────────────────────────────
import trimesh.transformations
print("\n" + "="*65)
print("  GOLDEN PROOF #1 — UV-MAPPED ASSET GENERATION COMPLETE")
print("="*65)
generated = [f for f in os.listdir(OUT_DIR) if f.endswith(".obj")]
total_bytes = sum(os.path.getsize(os.path.join(OUT_DIR, f)) for f in generated)
print(f"  Files: {len(generated)} OBJ + {len(generated)} MTL")
print(f"  Total: {total_bytes/1024:.1f} KB")
for f in sorted(generated):
    kb = os.path.getsize(os.path.join(OUT_DIR, f))/1024
    print(f"    {f:<42}  {kb:>8.1f} KB")
print()
print("  NEXT: UnrealEditor-Cmd.exe -run=pythonscript ue5_import_and_build.py")
print("="*65)
