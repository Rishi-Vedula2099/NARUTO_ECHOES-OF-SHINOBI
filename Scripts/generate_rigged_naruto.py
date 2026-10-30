import os
import numpy as np
from pygltflib import GLTF2, Scene, Node, Mesh, Primitive, Attributes, Skin, Buffer, BufferView, Accessor, Material

obj_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin.obj"
glb_path = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI\Content\SourceMeshes\SK_Naruto_Genin_Rigged.glb"

print(f"Reading {obj_path}...")
verts = []
uvs = []
normals = []

material_order = ["Skin", "Navy", "Hair", "Collar", "Tracksuit", "Emblem", "Metal"]
material_faces = {mat: [] for mat in material_order}
current_mat = "Skin"

with open(obj_path, "r", encoding="utf-8") as f:
    for line in f:
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        parts = line.split()
        tag = parts[0]
        
        if tag == "v":
            verts.append([float(parts[1]), float(parts[2]), float(parts[3])])
        elif tag == "vt":
            uvs.append([float(parts[1]), float(parts[2])])
        elif tag == "vn":
            normals.append([float(parts[1]), float(parts[2]), float(parts[3])])
        elif tag == "usemtl":
            mat_name = parts[1]
            if mat_name in material_faces:
                current_mat = mat_name
            else:
                current_mat = "Tracksuit"
        elif tag == "f":
            f_indices = []
            for token in parts[1:]:
                v_idx = int(token.split("/")[0]) - 1
                f_indices.append(v_idx)
            for k in range(1, len(f_indices) - 1):
                # Flip winding order to account for Z = -Y coordinate reflection
                material_faces[current_mat].extend([f_indices[0], f_indices[k + 1], f_indices[k]])

verts = np.array(verts, dtype=np.float32)
num_verts = len(verts)

# In OBJ: X is Right/Left, Y is Forward/Back, Z is Up (height ~145cm).
# In standard glTF 2.0 (Right-handed, Y is UP):
# gltf_X = OBJ_X * 0.01 (meters)
# gltf_Y = OBJ_Z * 0.01 (meters, HEIGHT!)
# gltf_Z = -OBJ_Y * 0.01 (meters, depth)
gltf_verts = np.zeros((num_verts, 3), dtype=np.float32)
gltf_verts[:, 0] = verts[:, 0] * 0.01
gltf_verts[:, 1] = verts[:, 2] * 0.01
gltf_verts[:, 2] = -verts[:, 1] * 0.01

# Compute accurate, outward-pointing smooth vertex normals from geometry
gltf_normals = np.zeros_like(gltf_verts)
for mat in material_order:
    f_list = material_faces[mat]
    for i in range(0, len(f_list), 3):
        i0, i1, i2 = f_list[i], f_list[i + 1], f_list[i + 2]
        p0, p1, p2 = gltf_verts[i0], gltf_verts[i1], gltf_verts[i2]
        fn = np.cross(p1 - p0, p2 - p0)
        norm = np.linalg.norm(fn)
        if norm > 1e-7:
            fn /= norm
            gltf_normals[i0] += fn
            gltf_normals[i1] += fn
            gltf_normals[i2] += fn

norms = np.linalg.norm(gltf_normals, axis=1, keepdims=True)
norms[norms == 0] = 1.0
gltf_normals /= norms

# Handle uvs
if len(uvs) < num_verts:
    pad = np.zeros((num_verts - len(uvs), 2), dtype=np.float32)
    uvs = np.vstack([np.array(uvs, dtype=np.float32), pad])
else:
    uvs = np.array(uvs[:num_verts], dtype=np.float32)

print(f"Loaded {num_verts} vertices. Faces per material:")
for mat in material_order:
    print(f"  {mat}: {len(material_faces[mat]) // 3} triangles")

# 2. Canonical Humanoid Skeleton in standard glTF space (Y is Up!)
bones = [
    # (name, parent_idx, local_position [X, Y(up), Z(depth)])
    ("root", None, [0.0, 0.0, 0.0]),
    ("pelvis", 0, [0.0, 0.85, 0.0]),
    ("spine_01", 1, [0.0, 0.15, 0.0]),
    ("spine_02", 2, [0.0, 0.15, 0.0]),
    ("neck_01", 3, [0.0, 0.12, 0.0]),
    ("head", 4, [0.0, 0.12, 0.0]),
    # Left Arm
    ("clavicle_l", 3, [0.04, 0.10, 0.0]),
    ("upperarm_l", 6, [0.12, 0.0, 0.0]),
    ("lowerarm_l", 7, [0.18, 0.0, 0.0]),
    ("hand_l", 8, [0.16, 0.0, 0.0]),
    # Right Arm
    ("clavicle_r", 3, [-0.04, 0.10, 0.0]),
    ("upperarm_r", 10, [-0.12, 0.0, 0.0]),
    ("lowerarm_r", 11, [-0.18, 0.0, 0.0]),
    ("hand_r", 12, [-0.16, 0.0, 0.0]),
    # Left Leg
    ("thigh_l", 1, [0.10, -0.05, 0.0]),
    ("calf_l", 14, [0.0, -0.38, 0.0]),
    ("foot_l", 15, [0.0, -0.38, -0.08]),
    # Right Leg
    ("thigh_r", 1, [-0.10, -0.05, 0.0]),
    ("calf_r", 17, [0.0, -0.38, 0.0]),
    ("foot_r", 18, [0.0, -0.38, -0.08]),
]
num_joints = len(bones)

# Compute world positions of bones in glTF space
world_positions = []
for i, (name, parent, loc_pos) in enumerate(bones):
    if parent is None:
        world_positions.append(np.array(loc_pos, dtype=np.float32))
    else:
        world_positions.append(world_positions[parent] + np.array(loc_pos, dtype=np.float32))
world_positions = np.array(world_positions)

# 3. Calculate Skin Weights in glTF space (Y is Up!)
joints_data = np.zeros((num_verts, 4), dtype=np.uint16)
weights_data = np.zeros((num_verts, 4), dtype=np.float32)

for v_idx in range(num_verts):
    vx, vy, vz = gltf_verts[v_idx, 0], gltf_verts[v_idx, 1], gltf_verts[v_idx, 2]
    
    dists = []
    for j_idx in range(num_joints):
        jx, jy, jz = world_positions[j_idx]
        d = np.sqrt((vx - jx)**2 + (vy - jy)**2 + (vz - jz)**2)
        
        # Anatomical prior weighting (vy is height in glTF space)
        if vy >= 1.22 and j_idx in (4, 5): # Head/Hair/Headband
            d *= 0.25
        elif abs(vx) >= 0.16 and vy >= 0.85: # Arms
            if vx > 0 and j_idx in (6, 7, 8, 9):
                d *= 0.3
            elif vx < 0 and j_idx in (10, 11, 12, 13):
                d *= 0.3
        elif 0.85 <= vy < 1.22 and abs(vx) < 0.20 and j_idx in (1, 2, 3): # Torso
            d *= 0.3
        elif vy < 0.85: # Legs
            if vx > 0 and j_idx in (14, 15, 16):
                d *= 0.3
            elif vx < 0 and j_idx in (17, 18, 19):
                d *= 0.3
                
        dists.append(d)
        
    sorted_joints = np.argsort(dists)
    j0, j1 = sorted_joints[0], sorted_joints[1]
    d0, d1 = dists[j0], dists[j1]
    
    w0 = 1.0 / (d0 + 1e-4) ** 2
    w1 = 1.0 / (d1 + 1e-4) ** 2
    total_w = w0 + w1
    w0 /= total_w
    w1 /= total_w
    
    joints_data[v_idx, 0] = j0
    joints_data[v_idx, 1] = j1
    weights_data[v_idx, 0] = w0
    weights_data[v_idx, 1] = w1

# 4. Inverse Bind Matrices (glTF MAT4 in column-major order)
inv_bind_matrices = []
for j_pos in world_positions:
    mat = np.array([
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -j_pos[0], -j_pos[1], -j_pos[2], 1.0
    ], dtype=np.float32)
    inv_bind_matrices.append(mat)
inv_bind_matrices = np.concatenate(inv_bind_matrices)

# 5. Pack binary buffers
pos_bytes = gltf_verts.tobytes()
normal_bytes = gltf_normals.tobytes()
uv_bytes = uvs.tobytes()
joints_bytes = joints_data.tobytes()
weights_bytes = weights_data.tobytes()
inv_bind_bytes = inv_bind_matrices.tobytes()

material_idx_bytes = {}
for mat in material_order:
    arr = np.array(material_faces[mat], dtype=np.uint32)
    material_idx_bytes[mat] = arr.tobytes()

buffer_data = pos_bytes + normal_bytes + uv_bytes + joints_bytes + weights_bytes + inv_bind_bytes
for mat in material_order:
    buffer_data += material_idx_bytes[mat]

gltf = GLTF2()

mesh_node_idx = num_joints
scene = Scene(nodes=[0, mesh_node_idx])
gltf.scenes.append(scene)
gltf.scene = 0

for i, (name, parent, loc_pos) in enumerate(bones):
    node = Node(name=name, translation=loc_pos)
    gltf.nodes.append(node)

for i, (name, parent, loc_pos) in enumerate(bones):
    if parent is not None:
        if gltf.nodes[parent].children is None:
            gltf.nodes[parent].children = []
        gltf.nodes[parent].children.append(i)

mesh_node = Node(name="SK_Naruto_Genin", mesh=0, skin=0)
gltf.nodes.append(mesh_node)

skin = Skin(skeleton=0, joints=list(range(num_joints)), inverseBindMatrices=5)
gltf.skins.append(skin)

buffer_views = []
offset = 0

bv_pos = BufferView(buffer=0, byteOffset=offset, byteLength=len(pos_bytes), target=34962)
buffer_views.append(bv_pos)
offset += len(pos_bytes)

bv_norm = BufferView(buffer=0, byteOffset=offset, byteLength=len(normal_bytes), target=34962)
buffer_views.append(bv_norm)
offset += len(normal_bytes)

bv_uv = BufferView(buffer=0, byteOffset=offset, byteLength=len(uv_bytes), target=34962)
buffer_views.append(bv_uv)
offset += len(uv_bytes)

bv_joints = BufferView(buffer=0, byteOffset=offset, byteLength=len(joints_bytes), target=34962)
buffer_views.append(bv_joints)
offset += len(joints_bytes)

bv_weights = BufferView(buffer=0, byteOffset=offset, byteLength=len(weights_bytes), target=34962)
buffer_views.append(bv_weights)
offset += len(weights_bytes)

bv_inv_bind = BufferView(buffer=0, byteOffset=offset, byteLength=len(inv_bind_bytes))
buffer_views.append(bv_inv_bind)
offset += len(inv_bind_bytes)

mat_bv_start = len(buffer_views)
for mat in material_order:
    bv_mat = BufferView(buffer=0, byteOffset=offset, byteLength=len(material_idx_bytes[mat]), target=34963)
    buffer_views.append(bv_mat)
    offset += len(material_idx_bytes[mat])

gltf.bufferViews = buffer_views

accessors = []
acc_pos = Accessor(bufferView=0, byteOffset=0, componentType=5126, count=num_verts, type="VEC3",
                   min=gltf_verts.min(axis=0).tolist(), max=gltf_verts.max(axis=0).tolist())
accessors.append(acc_pos)

acc_norm = Accessor(bufferView=1, byteOffset=0, componentType=5126, count=num_verts, type="VEC3")
accessors.append(acc_norm)

acc_uv = Accessor(bufferView=2, byteOffset=0, componentType=5126, count=num_verts, type="VEC2")
accessors.append(acc_uv)

acc_joints = Accessor(bufferView=3, byteOffset=0, componentType=5123, count=num_verts, type="VEC4")
accessors.append(acc_joints)

acc_weights = Accessor(bufferView=4, byteOffset=0, componentType=5126, count=num_verts, type="VEC4")
accessors.append(acc_weights)

acc_inv_bind = Accessor(bufferView=5, byteOffset=0, componentType=5126, count=num_joints, type="MAT4")
accessors.append(acc_inv_bind)

mat_acc_start = len(accessors)
for i, mat in enumerate(material_order):
    f_count = len(material_faces[mat])
    acc_mat = Accessor(bufferView=mat_bv_start + i, byteOffset=0, componentType=5125, count=f_count, type="SCALAR")
    accessors.append(acc_mat)

gltf.accessors = accessors

gltf.materials = [Material(name=mat) for mat in material_order]

primitives = []
for i, mat in enumerate(material_order):
    prim = Primitive(
        attributes=Attributes(
            POSITION=0,
            NORMAL=1,
            TEXCOORD_0=2,
            JOINTS_0=3,
            WEIGHTS_0=4
        ),
        indices=mat_acc_start + i,
        material=i
    )
    primitives.append(prim)

mesh_obj = Mesh(name="SK_Naruto_Genin_Mesh", primitives=primitives)
gltf.meshes.append(mesh_obj)

gltf.buffers.append(Buffer(byteLength=len(buffer_data)))
gltf.set_binary_blob(buffer_data)

os.makedirs(os.path.dirname(glb_path), exist_ok=True)
gltf.save(glb_path)
print(f"Successfully generated upright Y-up glTF: {glb_path} ({os.path.getsize(glb_path)} bytes)")
