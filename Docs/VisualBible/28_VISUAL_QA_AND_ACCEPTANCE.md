# NARUTO: ECHOES OF SHINOBI — VISUAL BIBLE
## MODULE 28: VISUAL QA & ACCEPTANCE CRITERIA

> **Document ID:** EOS-VB-MOD-28  
> **Target Engine:** Unreal Engine 5.8 (Win64 Development)  
> **Master Reference:** [Master_Visual_Production_Board.jpg](../../References/Phase13/Master_Visual_Production_Board.jpg)  
> **Test Suite:** `FEOSVisualAssetValidationTest`

---

## 1. Automated Visual Asset Validation Rules

A visual test does **NOT** pass merely because an actor spawns or an actor exists in memory.

### 1.1 Non-Negotiable Test Assertions
```cpp
// Automation Test: FEOSVisualAssetValidationTest

// 1. Character Pawn Validation
TestNotNull(TEXT("SkeletalMeshComponent must exist"), CharacterPawn->GetMesh());
TestNotNull(TEXT("SkeletalMeshAsset must NOT be nullptr"), CharacterPawn->GetMesh()->GetSkeletalMeshAsset());
TestNotNull(TEXT("Character Material must NOT be nullptr"), CharacterPawn->GetMesh()->GetMaterial(0));
TestNotNull(TEXT("AnimInstance must NOT be nullptr"), CharacterPawn->GetMesh()->GetAnimInstance());
TestTrue(TEXT("Collision Capsule must be enabled"), CharacterPawn->GetCapsuleComponent()->GetCollisionEnabled() != ECollisionEnabled::NoCollision);

// 2. Environment Mesh Validation
TestNotNull(TEXT("Building StaticMesh must NOT be nullptr"), BuildingActor->GetStaticMeshComponent()->GetStaticMesh());
TestNotNull(TEXT("Building Material must NOT be nullptr"), BuildingActor->GetStaticMeshComponent()->GetMaterial(0));
TestTrue(TEXT("Building must have solid collision"), BuildingActor->GetStaticMeshComponent()->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block);

// 3. Lighting & Camera Validation
TestNotNull(TEXT("Directional Sun must exist"), SunActor);
TestNotNull(TEXT("SkyLight must exist"), SkyLightActor);
TestTrue(TEXT("Camera Boom TargetArmLength must be in [300, 600]"), CameraBoom->TargetArmLength >= 300.0f);
```

### 1.2 Automated Failure Criteria
The test runner immediately flags a test as `FAIL` or `BLOCKED` if any required visual asset resolves to:
- `nullptr`
- `missing soft reference`
- `missing package`
- `missing skeleton`
- `missing animation blueprint`
- `missing material`
- `missing texture dependency`
- `missing collision`
- `placeholder/debug asset`
- `Engine/BasicShapes asset` (`Cube`, `Plane`, `Sphere`, `Cylinder`)
- `default mannequin`
- `checkerboard material`
- `unlit fallback material`

> [!CAUTION]
> This strict check prevents false-positive test passes where an engine cube or unweighted mesh satisfies a naive `Mesh != nullptr` assertion. Every asset must resolve to a valid production asset path and type.

---

## 2. Screenshot Acceptance Protocol: VISUAL_ACCEPTANCE_PHASE13

### 2.1 Acceptance Camera Framing
- **Camera Position:** Elevated third-person action-RPG camera over the player character's right shoulder (Arm Length: 420cm, Pitch: -14°, Socket Offset: Y=50, Z=25).
- **Field of View:** 80 degrees.
- **Framing Requirements:**
  1. **Player Character:** In sharp focus at lower-third right; visible Naruto Genin mesh with spiky hair, headband, orange tracksuit, and cel-shading.
  2. **Konoha Building:** Ichiraku Ramen facade visible in midground with white/navy noren curtains and glowing red lantern.
  3. **Road & Curb:** Cobblestone street paving with stone curbs beneath player's sandals.
  4. **NPC:** Shopkeeper Teuchi or civilian villager standing at counter.
  5. **Vegetation:** Green Konoha cedar tree branches and floating cherry blossom petals visible in frame.
  6. **Skyline Landmark:** Hokage Monument cliff face with carved stone faces readable in background.
  7. **HUD:** Existing authoritative `EOSShinobiHUD` displaying curved health and chakra gauges bound to GAS attributes.

### 2.2 Rejection Checklist
The acceptance screenshot is automatically **REJECTED (FAIL)** if any of the following are visible:
- [ ] Any grey fog or empty void in the camera volume
- [ ] Default engine checkerboard texture
- [ ] Default mannequin mesh (grey/silver generic mannequin)
- [ ] Primitive graybox cube, cylinder, or plane (unless specifically outside boundary and hidden)
- [ ] Invisible or floating actors
- [ ] Unlit black geometry or blown-out 100% white burnout
- [ ] Missing character limbs, head, or hair

---

## 3. Performance Signoff Protocol (GTX 1650 Target)

Visual acceptance is contingent upon maintaining solid performance in a packaged Development Win64 build:

| Metric | Pass Threshold | Conditional Pass | Immediate Fail |
| :--- | :--- | :--- | :--- |
| **Average Frame Rate (1080p)** | >= 60.0 FPS | 50.0 – 59.9 FPS | < 50.0 FPS |
| **1% Low Frame Rate** | >= 45.0 FPS | 38.0 – 44.9 FPS | < 38.0 FPS |
| **GPU Frame Time** | <= 16.6 ms | 16.7 – 20.0 ms | > 20.0 ms |
| **Game Thread Frame Time** | <= 10.0 ms | 10.1 – 14.0 ms | > 14.0 ms |
| **Total Draw Calls** | <= 1,800 calls | 1,801 – 2,500 calls | > 2,500 calls |
| **Visible Triangle Count** | <= 1,500,000 tris | 1,500,001 – 2,500,000 tris | > 2,500,000 tris |
| **VRAM Consumption** | <= 3.2 GB | 3.2 – 3.7 GB | > 3.8 GB (GTX 1650 OOM) |

---

## 4. Final Signoff Status Declarations

No human or AI agent may mark Phase 13 visually complete without evidence. Only the following statuses may be declared:
- `SYSTEM_IMPLEMENTED_VISUAL_PENDING`: Gameplay architecture is authoritative and passing, but production 3D assets are awaiting import/rendering.
- `VISUAL_ASSET_PARTIAL`: 1 to 6 assets are rendering in smoke test, remainder pending authoring.
- `VISUAL_RUNTIME_PASS`: All 17 vertical slice elements render simultaneously in Unreal Engine with passing automated validation and acceptable performance.
