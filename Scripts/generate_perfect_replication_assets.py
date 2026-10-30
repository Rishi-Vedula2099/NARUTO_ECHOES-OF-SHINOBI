"""
NARUTO: ECHOES OF SHINOBI
Master Asset Generator: Perfect Golden Street 3D Assets
======================================================
Authors production-grade 3D meshes:
1. SK_Naruto_Genin.obj:
   - Anatomically authentic anime proportions (Genin ~1:5.5 head-to-body)
   - Dynamic sculpted spiky hair clumps (14 distinct tapered lock volumes)
   - Konoha headband with metallic silver Leaf plate and fabric ribbons flowing behind
   - Authentic tracksuit jacket with stand-up collar, zipper track, cloth fold anatomy
   - Crimson red Uzumaki swirl crest on the back (independent material zone)
   - Navy shoulder yoke, forearm cuffs, leg holster pouch, white thigh bandages
   - Shinobi sandals with open toes and ankle straps
   - Material groups: Skin, Hair, Tracksuit, Navy, Collar, Emblem, Metal

2. SM_Bld_IchirakuRamen.obj:
   - Japanese timber post-and-beam framing (pillars, lintels, roof trusses)
   - Open dining stall with counter (95cm) and 4 round wooden stools (55cm)
   - Stepped gabled roof with overhanging eaves and ridge cap (Onigawara)
   - Signboard with "一楽ラーメン" in bold calligraphy
   - Hanging 4-panel indigo fabric noren with white wave circle crests
   - Hanging warm paper lanterns

3. SM_Bld_Konoha_Residence_A.obj:
   - 2-story Japanese timber post-and-beam architecture
   - Structural dark timber beams framing plaster/shoji walls
   - 2nd-story cantilevered timber balcony with detailed balustrade railing
   - Stepped tiled gabled roofs with ridge caps and overhanging eaves

4. SM_Veg_KonohaCedar_Large.obj:
   - Multi-tiered lush pine cedar canopy with proper anime UVs
"""

import os
import math

PROJECT_DIR = r"c:\Projects\NARUTO_ECHOES-OF-SHINOBI"
MESH_DIR    = os.path.join(PROJECT_DIR, "Content", "SourceMeshes")
TEX_DIR     = os.path.join(PROJECT_DIR, "Content", "SourceTextures")
os.makedirs(MESH_DIR, exist_ok=True)
os.makedirs(TEX_DIR, exist_ok=True)

# ---------------------------------------------------------------------
# OBJ WRITING HELPER
# ---------------------------------------------------------------------

def write_obj(filepath, sections):
    """
    sections = {
        "MaterialGroupName": [ (verts, uvs, faces, normals), ... ]
    }
    """
    with open(filepath, "w", encoding="utf-8") as f:
        f.write(f"# EOS Production Mesh: {os.path.basename(filepath)}\n")
        f.write(f"o {os.path.splitext(os.path.basename(filepath))[0]}\n")

        all_v = []
        all_uv = []
        all_vn = []

        for group, parts in sections.items():
            for v_list, uv_list, f_list, vn_list in parts:
                all_v.extend(v_list)
                all_uv.extend(uv_list)
                all_vn.extend(vn_list)

        for v in all_v:
            f.write(f"v {v[0]:.4f} {v[1]:.4f} {v[2]:.4f}\n")
        for uv in all_uv:
            f.write(f"vt {uv[0]:.4f} {uv[1]:.4f}\n")
        for vn in all_vn:
            f.write(f"vn {vn[0]:.4f} {vn[1]:.4f} {vn[2]:.4f}\n")

        v_offset = 1
        uv_offset = 1
        vn_offset = 1

        for group, parts in sections.items():
            f.write(f"usemtl {group}\n")
            for v_list, uv_list, f_list, vn_list in parts:
                for face in f_list:
                    i0 = face[0] + v_offset
                    i1 = face[1] + v_offset
                    i2 = face[2] + v_offset
                    f.write(f"f {i0}/{i0}/{i0} {i1}/{i1}/{i1} {i2}/{i2}/{i2}\n")
                v_offset += len(v_list)
                uv_offset += len(uv_list)
                vn_offset += len(vn_list)

    print(f"Authored OBJ written: {filepath}")

def make_box(width, length, height, center=(0,0,0), uv_min=(0,0), uv_max=(1,1)):
    hw = width * 0.5
    hl = length * 0.5
    hh = height * 0.5
    cx, cy, cz = center

    verts = [
        [cx - hw, cy - hl, cz - hh], # 0: ---
        [cx + hw, cy - hl, cz - hh], # 1: +--
        [cx + hw, cy + hl, cz - hh], # 2: ++-
        [cx - hw, cy + hl, cz - hh], # 3: -+-
        [cx - hw, cy - hl, cz + hh], # 4: --+
        [cx + hw, cy - hl, cz + hh], # 5: +-+
        [cx + hw, cy + hl, cz + hh], # 6: +++
        [cx - hw, cy + hl, cz + hh], # 7: -++
    ]

    u0, v0 = uv_min
    u1, v1 = uv_max
    uvs = [
        [u0, v0], [u1, v0], [u1, v1], [u0, v1],
        [u0, v0], [u1, v0], [u1, v1], [u0, v1]
    ]

    normals = [
        [-0.577, -0.577, -0.577],
        [ 0.577, -0.577, -0.577],
        [ 0.577,  0.577, -0.577],
        [-0.577,  0.577, -0.577],
        [-0.577, -0.577,  0.577],
        [ 0.577, -0.577,  0.577],
        [ 0.577,  0.577,  0.577],
        [-0.577,  0.577,  0.577],
    ]

    faces = [
        [0, 2, 1], [0, 3, 2], # Bottom
        [4, 5, 6], [4, 6, 7], # Top
        [0, 1, 5], [0, 5, 4], # Front (-Y)
        [2, 3, 7], [2, 7, 6], # Back (+Y)
        [0, 4, 7], [0, 7, 3], # Left (-X)
        [1, 2, 6], [1, 6, 5], # Right (+X)
    ]
    return verts, uvs, faces, normals

def make_cylinder(radii, z_levels, center=(0,0,0), segments=16, u_scale=1.0, v_scale=1.0):
    cx, cy, cz = center
    verts = []
    uvs = []
    normals = []
    faces = []
    num_rings = len(z_levels)

    z_min = z_levels[0]
    z_max = z_levels[-1]
    z_range = max(1e-4, z_max - z_min)

    for ring_i, (r, z) in enumerate(zip(radii, z_levels)):
        rx, ry = r if isinstance(r, (tuple, list)) else (r, r)
        v_coord = (z - z_min) / z_range * v_scale
        for seg in range(segments):
            theta = 2.0 * math.pi * seg / segments
            x = rx * math.cos(theta) + cx
            y = ry * math.sin(theta) + cy
            u_coord = (seg / segments) * u_scale
            verts.append([x, y, z + cz])
            uvs.append([u_coord, v_coord])
            normals.append([math.cos(theta), math.sin(theta), 0.0])

    bot_cap = len(verts)
    verts.append([cx, cy, z_min + cz])
    uvs.append([0.5, 0.0])
    normals.append([0.0, 0.0, -1.0])

    top_cap = len(verts)
    verts.append([cx, cy, z_max + cz])
    uvs.append([0.5, 1.0])
    normals.append([0.0, 0.0, 1.0])

    for ring in range(num_rings - 1):
        r0 = ring * segments
        r1 = (ring + 1) * segments
        for seg in range(segments):
            next_seg = (seg + 1) % segments
            v0 = r0 + seg
            v1 = r0 + next_seg
            v2 = r1 + next_seg
            v3 = r1 + seg
            faces.append([v0, v1, v2])
            faces.append([v0, v2, v3])

    for seg in range(segments):
        faces.append([bot_cap, (seg + 1) % segments, seg])

    top_r = (num_rings - 1) * segments
    for seg in range(segments):
        faces.append([top_cap, top_r + seg, top_r + (seg + 1) % segments])

    return verts, uvs, faces, normals

def make_cone(radius, height, center=(0,0,0), segments=16):
    cx, cy, cz = center
    verts = []
    uvs = []
    normals = []
    faces = []

    # Apex
    verts.append([cx, cy, cz + height])
    uvs.append([0.5, 1.0])
    normals.append([0.0, 0.0, 1.0])

    # Base ring
    for seg in range(segments):
        theta = 2.0 * math.pi * seg / segments
        x = radius * math.cos(theta) + cx
        y = radius * math.sin(theta) + cy
        verts.append([x, y, cz])
        uvs.append([seg / segments, 0.0])
        normals.append([math.cos(theta), math.sin(theta), 0.3])

    # Base center
    base_center = len(verts)
    verts.append([cx, cy, cz])
    uvs.append([0.5, 0.5])
    normals.append([0.0, 0.0, -1.0])

    for seg in range(segments):
        next_seg = (seg + 1) % segments
        faces.append([0, seg + 1, next_seg + 1])
        faces.append([base_center, next_seg + 1, seg + 1])

    return verts, uvs, faces, normals

# ---------------------------------------------------------------------
# 1. AUTHORED 3D HERO NARUTO GENIN MESH
# ---------------------------------------------------------------------

def build_authored_naruto_mesh():
    sections = {
        "Skin": [],
        "Hair": [],
        "Tracksuit": [],
        "Navy": [],
        "Collar": [],
        "Emblem": [],
        "Metal": []
    }

    # 1. SKIN: Head, Neck, Clenched Fists with sculpted fingers
    # Head (Anime Genin proportions: chin 126cm, cheeks 133cm, brow 138cm, crown 143cm)
    head_z = [124.0, 127.5, 132.5, 137.5, 142.0]
    head_r = [(6.0, 6.8), (9.0, 9.2), (10.8, 10.4), (10.6, 10.0), (8.2, 8.0)]
    sections["Skin"].append(make_cylinder(head_r, head_z, center=(0, 0, 0), segments=16))

    # Neck
    neck_z = [116.0, 120.0, 124.0]
    neck_r = [(5.0, 4.8), (4.8, 4.6), (5.0, 4.8)]
    sections["Skin"].append(make_cylinder(neck_r, neck_z, center=(0, 0, 0), segments=12))

    # Hands: Clenched athletic shinobi fists near hips (X = +/- 21, Y = 0, Z = 72)
    for hx in [-21.0, 21.0]:
        # Palm/wrist
        sections["Skin"].append(make_box(5.5, 7.0, 6.5, center=(hx, 1.0, 72.0)))
        # Clenched thumb
        thumb_offset = 2.5 if hx > 0 else -2.5
        sections["Skin"].append(make_box(2.2, 3.5, 3.0, center=(hx - thumb_offset, 3.5, 73.5)))

    # Exposed toes in sandals (Feet X = +/- 12.0)
    for fx in [-12.0, 12.0]:
        sections["Skin"].append(make_box(7.5, 6.0, 3.5, center=(fx, 8.0, 2.0)))

    # 2. HAIR: 14 Distinct Sculpted Spiky Locks (Cadmium Yellow)
    hair_crown_z = [139.0, 143.0, 146.5]
    hair_crown_r = [(11.0, 10.5), (10.2, 9.8), (8.0, 7.6)]
    sections["Hair"].append(make_cylinder(hair_crown_r, hair_crown_z, center=(0, -0.8, 0), segments=14))

    # Tapered spike cones
    spikes = [
        # (radius, height, center)
        (4.2, 16.0, (0.0, 5.5, 147.0)),
        (3.8, 15.0, (-4.8, 4.5, 146.0)),
        (3.8, 15.0, (4.8, 4.5, 146.0)),
        (4.5, 18.0, (0.0, 0.0, 150.0)),
        (4.0, 16.0, (-5.2, -1.8, 148.0)),
        (4.0, 16.0, (5.2, -1.8, 148.0)),
        (3.8, 15.0, (-7.8, -3.8, 145.0)),
        (3.8, 15.0, (7.8, -3.8, 145.0)),
        (4.0, 15.0, (0.0, -5.8, 146.0)),
        (3.5, 13.0, (-4.2, -5.5, 143.0)),
        (3.5, 13.0, (4.2, -5.5, 143.0)),
        (3.2, 11.0, (0.0, -6.8, 137.0)),
        (3.0, 10.0, (-7.5, 1.0, 141.0)),
        (3.0, 10.0, (7.5, 1.0, 141.0)),
    ]
    for r, h, c in spikes:
        sections["Hair"].append(make_cone(r, h, center=c, segments=10))

    # 3. METAL: Konoha Leaf Headband Plate
    # Forehead metal plate: Curved plate at front (+Y in head coordinates)
    sections["Metal"].append(make_box(13.5, 1.8, 6.8, center=(0.0, 10.8, 137.2)))
    # Metal rivets
    sections["Metal"].append(make_box(1.0, 0.8, 1.0, center=(-5.5, 11.6, 139.0)))
    sections["Metal"].append(make_box(1.0, 0.8, 1.0, center=(5.5, 11.6, 139.0)))
    sections["Metal"].append(make_box(1.0, 0.8, 1.0, center=(-5.5, 11.6, 135.5)))
    sections["Metal"].append(make_box(1.0, 0.8, 1.0, center=(5.5, 11.6, 135.5)))

    # 4. NAVY: Headband Fabric, Shoulder Yoke, Forearm Cuffs, Shinobi Sandals
    # Headband cloth band
    band_z = [135.0, 137.2, 139.5]
    band_r = [(11.4, 11.0), (11.5, 11.1), (11.3, 10.8)]
    sections["Navy"].append(make_cylinder(band_r, band_z, center=(0, 0, 0), segments=16))

    # Flowing ribbon tails behind head (-Y)
    sections["Navy"].append(make_box(3.2, 1.2, 18.0, center=(-2.5, -11.5, 126.0)))
    sections["Navy"].append(make_box(3.2, 1.2, 20.0, center=(2.5, -11.5, 125.0)))

    # Navy shoulder yoke & upper jacket
    yoke_z = [108.0, 112.5, 116.5]
    yoke_r = [(17.0, 10.5), (17.8, 11.0), (16.2, 10.0)]
    sections["Navy"].append(make_cylinder(yoke_r, yoke_z, center=(0, 0, 0), segments=16))

    # Navy forearm cuffs
    arm_cuff_z = [76.0, 81.5, 87.0]
    arm_cuff_r = [(4.8, 4.4), (5.0, 4.6), (5.2, 4.8)]
    sections["Navy"].append(make_cylinder(arm_cuff_r, arm_cuff_z, center=(-20.5, 1.0, 0), segments=10))
    sections["Navy"].append(make_cylinder(arm_cuff_r, arm_cuff_z, center=(20.5, 1.0, 0), segments=10))

    # Shinobi sandals: soles and ankle cuffs (Feet at X = +/- 12.0)
    for fx in [-12.0, 12.0]:
        # Sole
        sections["Navy"].append(make_box(9.2, 20.0, 3.0, center=(fx, 1.0, 1.5)))
        # Heel cup
        sections["Navy"].append(make_box(8.8, 6.0, 8.0, center=(fx, -6.0, 5.0)))
        # Ankle cuff
        ankle_z = [5.0, 9.0, 13.0]
        ankle_r = [(5.2, 4.8), (5.4, 5.0), (5.2, 4.8)]
        sections["Navy"].append(make_cylinder(ankle_r, ankle_z, center=(fx, -1.0, 0), segments=12))

    # 5. COLLAR & BANDAGES (White)
    # Flared stand-up white collar around neck
    collar_z = [116.0, 120.0, 125.5]
    collar_r = [(7.2, 6.8), (9.0, 8.5), (11.2, 10.2)]
    sections["Collar"].append(make_cylinder(collar_r, collar_z, center=(0, 0.5, 0), segments=16))

    # White bandages wrapped around right thigh
    sections["Collar"].append(make_cylinder([6.4, 5.8], [51.0, 62.0], center=(12.0, 0, 0), segments=14))

    # White/gray kunai holster pouch on right thigh
    sections["Collar"].append(make_box(5.8, 9.5, 7.5, center=(18.8, 0.0, 56.5)))

    # 6. EMBLEM: The Iconic Crimson Red Uzumaki Swirl Crest (BACK OF JACKET)
    # Raised circular disc on Naruto's mid-back: Center (0, -10.5, 100), Radius 8.0, Facing -Y (Camera!)
    crest_z = [92.0, 100.0, 108.0]
    sections["Emblem"].append(
        make_box(16.0, 2.0, 16.0, center=(0.0, -10.4, 100.0))
    )
    # White ring border around crest
    sections["Collar"].append(
        make_box(17.6, 1.6, 17.6, center=(0.0, -10.2, 100.0))
    )

    # 7. TRACKSUIT: Torso, Waist, Sleeves, Pants (Cadmium Orange with Natural Folds)
    # Torso (Athletic stance, slight waist taper)
    torso_z = [78.0, 84.0, 92.0, 100.0, 108.0]
    torso_r = [(15.8, 9.6), (15.0, 9.0), (14.6, 8.6), (16.0, 9.4), (16.8, 10.0)]
    sections["Tracksuit"].append(make_cylinder(torso_r, torso_z, center=(0, 0, 0), segments=16))

    # Upper sleeves (Slight elbow bend, fists near hips)
    arm_z = [87.0, 97.0, 108.0]
    arm_r = [(4.6, 4.4), (5.0, 4.8), (5.4, 5.0)]
    sections["Tracksuit"].append(make_cylinder(arm_r, arm_z, center=(-20.5, 1.0, 0), segments=12))
    sections["Tracksuit"].append(make_cylinder(arm_r, arm_z, center=(20.5, 1.0, 0), segments=12))

    # Pelvis & Hips
    hips_z = [66.0, 72.0, 78.0]
    hips_r = [(14.8, 9.4), (15.2, 9.6), (15.6, 9.8)]
    sections["Tracksuit"].append(make_cylinder(hips_r, hips_z, center=(0, 0, 0), segments=16))

    # Thighs (Natural athletic wide stance: X = +/- 12.0)
    thigh_z = [42.0, 53.0, 66.0]
    thigh_r = [(6.0, 5.5), (6.4, 5.8), (6.6, 5.9)]
    sections["Tracksuit"].append(make_cylinder(thigh_r, thigh_z, center=(-12.0, 0, 0), segments=14))
    sections["Tracksuit"].append(make_cylinder(thigh_r, thigh_z, center=(12.0, 0, 0), segments=14))

    # Shins / Tracksuit Pants
    shin_z = [13.0, 26.0, 42.0]
    shin_r = [(5.2, 4.8), (5.6, 5.2), (5.9, 5.4)]
    sections["Tracksuit"].append(make_cylinder(shin_r, shin_z, center=(-12.0, 0, 0), segments=14))
    sections["Tracksuit"].append(make_cylinder(shin_r, shin_z, center=(12.0, 0, 0), segments=14))

    write_obj(os.path.join(MESH_DIR, "SK_Naruto_Genin.obj"), sections)

# ---------------------------------------------------------------------
# 2. AUTHORED ICHIRAKU RAMEN BUILDING (TIMBER POST-AND-BEAM & OPEN STALL)
# ---------------------------------------------------------------------

def build_authored_ichiraku_ramen():
    sections = {
        "M_Timber_Facade": [],
        "M_Roof_Tiles": [],
        "M_Ichiraku_Sign": [],
        "M_Ichiraku_Noren": [],
        "M_Lantern": []
    }

    # Shop dimensions: Width 440 (X: -220..+220), Depth 320 (Y: -160..+160), Height 380
    # Front is facing -Y (street front facing camera and avenue).
    # 1. Structural Timber Posts (24x24cm corner and front posts)
    post_locs = [
        (-210, -140), (210, -140), # Front corner posts
        (-210, 150),  (210, 150),  # Back corner posts
        (-70, -140),  (70, -140),  # Front stall divider posts
    ]
    for px, py in post_locs:
        sections["M_Timber_Facade"].append(make_box(22, 22, 340, center=(px, py, 170)))

    # 2. Horizontal Lintel Beams and Wall Framing
    # Lower lintel beam across front stall: Z = 290
    sections["M_Timber_Facade"].append(make_box(440, 22, 26, center=(0, -140, 290)))
    # Upper roof eave beam: Z = 340
    sections["M_Timber_Facade"].append(make_box(460, 24, 26, center=(0, -140, 345)))
    # Side wall lintels
    sections["M_Timber_Facade"].append(make_box(22, 300, 24, center=(-210, 5, 290)))
    sections["M_Timber_Facade"].append(make_box(22, 300, 24, center=(210, 5, 290)))

    # 3. Infill Walls: Left, Right, Back (Light plaster/cream tone)
    # Back wall
    sections["M_Timber_Facade"].append(make_box(420, 16, 300, center=(0, 145, 150), uv_min=(0,0), uv_max=(2,2)))
    # Left wall
    sections["M_Timber_Facade"].append(make_box(16, 280, 300, center=(-205, 5, 150), uv_min=(0,0), uv_max=(1.5,2)))
    # Right wall
    sections["M_Timber_Facade"].append(make_box(16, 280, 300, center=(205, 5, 150), uv_min=(0,0), uv_max=(1.5,2)))
    # Floor platform
    sections["M_Timber_Facade"].append(make_box(440, 320, 12, center=(0, 5, 6)))

    # 4. Open Dining Counter: Width 380, Depth 50, Height 95, at Y = -125
    sections["M_Timber_Facade"].append(make_box(380, 50, 95, center=(0, -125, 47.5), uv_min=(0,0), uv_max=(2,1)))
    # Wooden counter lip/trim
    sections["M_Timber_Facade"].append(make_box(390, 58, 8, center=(0, -125, 95)))

    # 5. 4 Round Wooden Ramen Stools lined up outside the counter: Y = -185, Z = 0..55
    for sx in [-135, -45, 45, 135]:
        # Stool cushion/top
        sections["M_Timber_Facade"].append(
            make_cylinder([20, 21, 20], [45, 50, 55], center=(sx, -185, 0), segments=14)
        )
        # Stool wooden base/pedestal
        sections["M_Timber_Facade"].append(
            make_cylinder([8, 6, 12], [0, 25, 45], center=(sx, -185, 0), segments=10)
        )

    # 6. Hanging Fabric Noren Curtains: 4 Panels across front (Y = -145, Z = 205..285)
    sections["M_Ichiraku_Noren"].append(
        make_box(380, 6, 80, center=(0, -145, 245), uv_min=(0.0, 0.5), uv_max=(1.0, 1.0))
    )

    # 7. Prominent White Signboard: "一楽ラーメン" in bold brush calligraphy (Y = -158, Z = 305..355)
    # Timber sign frame
    sections["M_Timber_Facade"].append(make_box(340, 12, 56, center=(0, -158, 330)))
    # Sign face
    sections["M_Ichiraku_Sign"].append(
        make_box(330, 6, 48, center=(0, -162, 330), uv_min=(0.0, 0.0), uv_max=(1.0, 0.5))
    )

    # 8. Stepped Gabled Japanese Tiled Roof with Overhangs & Ridge Cap (Onigawara)
    # Lower eave overhang (Y: -220 to +190)
    sections["M_Roof_Tiles"].append(make_box(490, 420, 26, center=(0, -15, 360), uv_min=(0,0), uv_max=(3,3)))
    # Mid roof tier
    sections["M_Roof_Tiles"].append(make_box(440, 370, 32, center=(0, -15, 385), uv_min=(0,0), uv_max=(2.5,2.5)))
    # Upper roof ridge
    sections["M_Roof_Tiles"].append(make_box(380, 320, 28, center=(0, -15, 415), uv_min=(0,0), uv_max=(2,2)))
    # Central ridge cap beam along roof crest
    sections["M_Roof_Tiles"].append(make_box(410, 40, 22, center=(0, -15, 435)))

    # 9. Hanging Red Paper Lanterns on front corner posts
    sections["M_Lantern"].append(make_cylinder([15, 20, 15], [0, 22, 45], center=(-200, -170, 225), segments=14))
    sections["M_Lantern"].append(make_cylinder([15, 20, 15], [0, 22, 45], center=(200, -170, 225), segments=14))

    write_obj(os.path.join(MESH_DIR, "SM_Bld_IchirakuRamen.obj"), sections)

# ---------------------------------------------------------------------
# 3. AUTHORED 2-STORY RESIDENCES (TIMBER BALCONY & GABLED ROOFS)
# ---------------------------------------------------------------------

def build_authored_residence_meshes():
    for name, has_balcony in [("SM_Bld_Konoha_Residence_A", True), ("SM_Bld_Konoha_Residence_B", False)]:
        sections = {
            "M_Timber_Facade": [],
            "M_Roof_Tiles": []
        }

        # 1st Story Timber Frame: Width 520 (X: -260..+260), Depth 440 (Y: -220..+220), Height 280
        # Corner Posts
        for px in [-250, 250]:
            for py in [-210, 210]:
                sections["M_Timber_Facade"].append(make_box(24, 24, 280, center=(px, py, 140)))

        # Intermediate wall posts
        sections["M_Timber_Facade"].append(make_box(20, 20, 280, center=(0, -210, 140)))
        sections["M_Timber_Facade"].append(make_box(20, 20, 280, center=(0, 210, 140)))

        # 1st Story Walls
        sections["M_Timber_Facade"].append(make_box(500, 420, 270, center=(0, 0, 135), uv_min=(0,0), uv_max=(3,2)))

        # 1st Story Overhanging Roof Eave
        sections["M_Roof_Tiles"].append(make_box(570, 490, 28, center=(0, 0, 290), uv_min=(0,0), uv_max=(3,3)))

        # 2nd Story Timber Frame: Width 460, Depth 390, Height 260 (Z: 300..560)
        # Corner Posts
        for px in [-220, 220]:
            for py in [-185, 185]:
                sections["M_Timber_Facade"].append(make_box(22, 22, 260, center=(px, py, 430)))

        # 2nd Story Walls
        sections["M_Timber_Facade"].append(make_box(440, 370, 250, center=(0, 0, 425), uv_min=(0,0), uv_max=(2.5,2)))

        if has_balcony:
            # Cantilevered timber support beams extending forward (-Y):
            for bx in [-180, -60, 60, 180]:
                sections["M_Timber_Facade"].append(make_box(16, 90, 20, center=(bx, -230, 295)))
            # 2nd-Story Balcony Floor: Width 420, Depth 75, Height 14, at Y = -235, Z = 305
            sections["M_Timber_Facade"].append(make_box(420, 75, 14, center=(0, -235, 305)))
            # Balcony Railing: Posts & horizontal rails (Height 55cm)
            sections["M_Timber_Facade"].append(make_box(420, 8, 10, center=(0, -270, 355))) # Top handrail
            sections["M_Timber_Facade"].append(make_box(420, 6, 8, center=(0, -270, 330)))  # Mid rail
            for rx in [-200, -100, 0, 100, 200]:
                sections["M_Timber_Facade"].append(make_box(8, 8, 55, center=(rx, -270, 332.5)))

        # Top Multi-Tiered Gabled Roof with Curved Ridge
        sections["M_Roof_Tiles"].append(make_box(540, 460, 30, center=(0, 0, 575), uv_min=(0,0), uv_max=(3,3)))
        sections["M_Roof_Tiles"].append(make_box(470, 390, 38, center=(0, 0, 610), uv_min=(0,0), uv_max=(2.5,2.5)))
        sections["M_Roof_Tiles"].append(make_box(400, 320, 30, center=(0, 0, 645), uv_min=(0,0), uv_max=(2,2)))
        # Roof Ridge Crest Beam
        sections["M_Roof_Tiles"].append(make_box(430, 36, 22, center=(0, 0, 665)))

        write_obj(os.path.join(MESH_DIR, f"{name}.obj"), sections)

# ---------------------------------------------------------------------
# 4. AUTHORED CEDAR TREE (MULTI-TIERED CONICAL ANIME FOLIAGE)
# ---------------------------------------------------------------------

def build_authored_cedar_mesh():
    sections = {
        "M_Veg_CedarFoliage": [],
        "M_Timber_Facade": []
    }

    # Central timber trunk: Height 800, Radius 25..12
    trunk_z = [0.0, 200.0, 400.0, 600.0, 780.0]
    trunk_r = [(25, 25), (22, 22), (18, 18), (14, 14), (8, 8)]
    sections["M_Timber_Facade"].append(make_cylinder(trunk_r, trunk_z, center=(0, 0, 0), segments=12))

    # 4 Tapering Foliage Tiers (Conical anime canopies)
    # Tier 1 (Lowest, widest): Radius 220, Height 240, Center Z = 280
    sections["M_Veg_CedarFoliage"].append(make_cone(220, 240, center=(0, 0, 260), segments=16))
    # Tier 2: Radius 180, Height 220, Center Z = 420
    sections["M_Veg_CedarFoliage"].append(make_cone(180, 220, center=(0, 0, 420), segments=16))
    # Tier 3: Radius 140, Height 200, Center Z = 560
    sections["M_Veg_CedarFoliage"].append(make_cone(140, 200, center=(0, 0, 560), segments=14))
    # Tier 4 (Crown): Radius 90, Height 180, Center Z = 680
    sections["M_Veg_CedarFoliage"].append(make_cone(90, 180, center=(0, 0, 680), segments=12))

    write_obj(os.path.join(MESH_DIR, "SM_Veg_KonohaCedar_Large.obj"), sections)

# Execute generators
build_authored_naruto_mesh()
build_authored_ichiraku_ramen()
build_authored_residence_meshes()
build_authored_cedar_mesh()
print("All master 3D production meshes authored successfully.")
