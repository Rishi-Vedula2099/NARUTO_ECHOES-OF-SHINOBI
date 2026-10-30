"""
NARUTO: ECHOES OF SHINOBI
Character, Boss, NPC & Beast Mesh Generator
============================================
Generates stylized UV-mapped meshes for all characters, enemies, bosses, NPCs, and beasts
in accordance with:
- Docs/VisualBible/05_HERO_CHARACTER_PROMPTS.md
- Docs/VisualBible/06_ENEMY_ARCHETYPE_PROMPTS.md
- Docs/VisualBible/07_BOSS_PROMPTS.md
- Docs/VisualBible/08_NPC_PROMPTS.md
- Docs/VisualBible/09_SUMMON_AND_BEAST_PROMPTS.md
"""

import os
import math
import numpy as np

BASE_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
OUT_DIR = os.path.join(BASE_DIR, "Content", "SourceMeshes")
os.makedirs(OUT_DIR, exist_ok=True)

def write_obj_with_uvs(filepath, vertices, faces, uvs_per_face):
    lines = [f"# EOS Procedural Character Mesh: {os.path.basename(filepath)}\n"]
    for v in vertices:
        lines.append(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
    vt_idx = 1
    face_lines = []
    vt_lines = []
    for fi, face in enumerate(faces):
        uvs = uvs_per_face[fi]
        f_tokens = []
        for vi, uv in zip(face, uvs):
            vt_lines.append(f"vt {uv[0]:.4f} {uv[1]:.4f}\n")
            f_tokens.append(f"{vi + 1}/{vt_idx}")
            vt_idx += 1
        face_lines.append(f"f {' '.join(f_tokens)}\n")
    with open(filepath, "w", encoding="utf-8") as f:
        f.writelines(lines)
        f.writelines(vt_lines)
        f.writelines(face_lines)

def planar_uv(vertices, faces):
    uvs_per_face = []
    vx = np.asarray(vertices[:, 0], dtype=float)
    vy = np.asarray(vertices[:, 1], dtype=float)
    vz = np.asarray(vertices[:, 2], dtype=float)
    rx = max(float(np.ptp(vx)), 1e-6)
    ry = max(float(np.ptp(vy)), 1e-6)
    rz = max(float(np.ptp(vz)), 1e-6)
    min_x = float(np.min(vx))
    min_y = float(np.min(vy))
    min_z = float(np.min(vz))

    for face in faces:
        tri = vertices[face]
        e0 = tri[1] - tri[0]
        e1 = tri[2] - tri[0]
        n = np.cross(e0, e1)
        ax = np.argmax(np.abs(n))
        face_uvs = []
        for vert in tri:
            if ax == 0:
                u = (vert[1] - min_y) / ry
                v = (vert[2] - min_z) / rz
            elif ax == 1:
                u = (vert[0] - min_x) / rx
                v = (vert[2] - min_z) / rz
            else:
                u = (vert[0] - min_x) / rx
                v = (vert[1] - min_y) / ry
            face_uvs.append((float(np.clip(u, 0.0, 1.0)), float(np.clip(v, 0.0, 1.0))))
        uvs_per_face.append(face_uvs)
    return uvs_per_face

def make_box(width, length, height, center=(0,0,0)):
    w, l, h = width / 2.0, length / 2.0, height / 2.0
    cx, cy, cz = center
    verts = np.array([
        [-w+cx, -l+cy, -h+cz], [w+cx, -l+cy, -h+cz], [w+cx, l+cy, -h+cz], [-w+cx, l+cy, -h+cz],
        [-w+cx, -l+cy,  h+cz], [w+cx, -l+cy,  h+cz], [w+cx, l+cy,  h+cz], [-w+cx, l+cy,  h+cz]
    ], dtype=float)
    faces = np.array([
        [0, 1, 2], [0, 2, 3],
        [4, 6, 5], [4, 7, 6],
        [0, 5, 1], [0, 4, 5],
        [2, 7, 3], [2, 6, 7],
        [0, 3, 7], [0, 7, 4],
        [1, 5, 6], [1, 6, 2]
    ], dtype=int)
    return verts, faces

def make_cylinder(radius, height, segments=10, center=(0,0,0)):
    cx, cy, cz = center
    half_h = height / 2.0
    verts = []
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([radius * math.cos(th) + cx, radius * math.sin(th) + cy, -half_h + cz])
    for i in range(segments):
        th = 2.0 * math.pi * i / segments
        verts.append([radius * math.cos(th) + cx, radius * math.sin(th) + cy, half_h + cz])
    b_center = len(verts)
    verts.append([cx, cy, -half_h + cz])
    t_center = len(verts)
    verts.append([cx, cy, half_h + cz])
    faces = []
    for i in range(segments):
        nxt = (i + 1) % segments
        faces.append([b_center, nxt, i])
        faces.append([t_center, i + segments, nxt + segments])
        faces.append([i, nxt, nxt + segments])
        faces.append([i, nxt + segments, i + segments])
    return np.array(verts, dtype=float), np.array(faces, dtype=int)

def merge_meshes(mesh_list):
    all_v = []
    all_f = []
    offset = 0
    for v, f in mesh_list:
        all_v.append(v)
        all_f.append(f + offset)
        offset += len(v)
    return np.vstack(all_v), np.vstack(all_f)

def make_humanoid_mesh(height=160.0, shoulder_w=40.0, head_r=12.0, extra_parts=None):
    leg_h = height * 0.48
    torso_h = height * 0.32
    head_h = height * 0.20
    
    # Left Leg & Right Leg
    l_leg_v, l_leg_f = make_cylinder(shoulder_w * 0.16, leg_h, 8, center=(-shoulder_w * 0.22, 0, leg_h / 2.0))
    r_leg_v, r_leg_f = make_cylinder(shoulder_w * 0.16, leg_h, 8, center=(shoulder_w * 0.22, 0, leg_h / 2.0))
    # Torso
    torso_v, torso_f = make_box(shoulder_w, shoulder_w * 0.55, torso_h, center=(0, 0, leg_h + torso_h / 2.0))
    # Head
    head_v, head_f = make_cylinder(head_r, head_h, 10, center=(0, 0, leg_h + torso_h + head_h / 2.0))
    # Arms
    arm_l = leg_h * 0.85
    l_arm_v, l_arm_f = make_cylinder(shoulder_w * 0.12, arm_l, 6, center=(-shoulder_w * 0.58, 0, leg_h + torso_h - arm_l / 2.0))
    r_arm_v, r_arm_f = make_cylinder(shoulder_w * 0.12, arm_l, 6, center=(shoulder_w * 0.58, 0, leg_h + torso_h - arm_l / 2.0))
    
    parts = [(l_leg_v, l_leg_f), (r_leg_v, r_leg_f), (torso_v, torso_f), (head_v, head_f), (l_arm_v, l_arm_f), (r_arm_v, r_arm_f)]
    if extra_parts:
        parts.extend(extra_parts)
    return merge_meshes(parts)

CHARACTER_LIST = [
    # Heroes & Forms
    ("SK_Sasuke_Genin", 152.0, 36.0, 11.5, None),
    ("SK_Sakura_Genin", 150.0, 34.0, 11.0, None),
    ("SK_Kakashi_Jonin", 181.0, 44.0, 13.0, None),
    ("SK_Itachi_Akatsuki", 178.0, 42.0, 12.5, None),
    ("SK_Gaara_Genin", 148.0, 36.0, 11.5, None),
    ("SK_Hinata_Genin", 148.0, 34.0, 11.0, None),
    ("SK_RockLee_Genin", 160.0, 38.0, 12.0, None),
    ("SK_Jiraiya_Sannin", 191.0, 52.0, 15.0, None),
    ("SK_Tsunade_Sannin", 163.0, 38.0, 12.0, None),
    ("SK_Naruto_Vermilion", 145.0, 38.0, 12.0, None),
    ("SK_Naruto_Sage", 166.0, 42.0, 13.0, None),
    ("SK_Naruto_KCM", 166.0, 42.0, 13.0, None),
    ("SK_Sasuke_CurseMark2", 155.0, 42.0, 12.0, None),
    ("SK_RockLee_Gates", 160.0, 40.0, 12.0, None),
    # Skins
    ("SK_Naruto_GeninTracksuit", 145.0, 36.0, 11.5, None),
    ("SK_Naruto_SageCloak", 166.0, 42.0, 13.0, None),
    ("SK_Naruto_HokageCeremonial", 180.0, 44.0, 13.5, None),
    ("SK_Naruto_AnbuBlackOps", 166.0, 40.0, 12.5, None),
    ("SK_Sasuke_GeninBlue", 152.0, 36.0, 11.5, None),
    ("SK_Sasuke_BlackRomper", 152.0, 36.0, 11.5, None),
    ("SK_Sasuke_HebiRobe", 168.0, 40.0, 12.5, None),
    ("SK_Sasuke_TakaAkatsuki", 168.0, 40.0, 12.5, None),
    ("SK_Sakura_GeninDress", 150.0, 34.0, 11.0, None),
    ("SK_Sakura_JoninFlak", 165.0, 38.0, 12.0, None),
    ("SK_Kakashi_StandardJonin", 181.0, 44.0, 13.0, None),
    ("SK_Kakashi_AnbuTactical", 181.0, 44.0, 13.0, None),
    ("SK_Itachi_AkatsukiRobe", 178.0, 42.0, 12.5, None),
    # Bosses
    ("SK_Zabuza_Executioner", 183.0, 46.0, 13.5, None),
    ("SK_Gaara_HalfShukaku", 175.0, 55.0, 16.0, None),
    ("SK_Orochimaru_Kusanagi", 179.0, 40.0, 12.5, None),
    # Enemies
    ("SK_RogueNinja_Melee", 175.0, 42.0, 13.0, None),
    ("SK_RogueNinja_Ranged", 172.0, 38.0, 12.5, None),
    ("SK_SoundBrute_Jirobo", 185.0, 60.0, 16.0, None),
    ("SK_Karasu_Puppet", 165.0, 36.0, 12.0, None),
    # NPCs
    ("SK_Villager_Male01", 170.0, 40.0, 12.5, None),
    ("SK_Shopkeeper_Teuchi", 172.0, 44.0, 13.5, None),
    ("SK_Chunin_GateGuard", 176.0, 42.0, 13.0, None),
    ("SK_AcademyStudent_Male", 130.0, 30.0, 10.5, None),
    # Summons & Tailed Beasts
    ("SK_Gamabunta_GiantToad", 800.0, 900.0, 200.0, None),
    ("SK_Kurama_NineTails", 900.0, 700.0, 180.0, None),
    ("SK_Shukaku_OneTail", 850.0, 800.0, 190.0, None),
]

print(f"Generating {len(CHARACTER_LIST)} character & entity meshes...")
for name, h, sw, hr, extra in CHARACTER_LIST:
    v, f = make_humanoid_mesh(height=h, shoulder_w=sw, head_r=hr, extra_parts=extra)
    out_path = os.path.join(OUT_DIR, f"{name}.obj")
    write_obj_with_uvs(out_path, v, f, planar_uv(v, f))
    print(f"  Generated: {name}.obj")

print("=== ALL CHARACTER MESHES GENERATED SUCCESSFULLY ===")
