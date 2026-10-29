---
trigger: always_on
description: "Non-negotiable visual completion and runtime validation rules for NARUTO: ECHOES OF SHINOBI."
---

# EOS VISUAL COMPLETION LAW

Project: NARUTO: ECHOES OF SHINOBI  
Engine: Unreal Engine 5.8  

The agent MUST distinguish between:
1. Gameplay framework
2. Runtime scaffolding
3. Placeholder visualization
4. Production visual implementation

These are NOT equivalent.

## ABSOLUTE RULE
Never declare a visual feature complete because:
- a C++ actor exists
- a Blueprint actor exists
- an Outliner entry exists
- a HUD exists
- a camera exists
- a primitive mesh renders
- a material exists
- a light exists
- a scene loads
- a screenshot can be captured

A visual feature is COMPLETE only when the intended production-quality visual asset is actually rendered in the running game.

## NON-NEGOTIABLE VISUAL QUALITY RULES
1. A visible character must not be mistaken for a mannequin, robot, debug actor or generic placeholder.
2. A production environment must not contain unexplained floating spheres, test primitives, disconnected platforms or exposed blockout geometry.
3. A sky gradient is not a finished environment.
4. A functioning HUD is not proof that the gameplay experience is finished.
5. Correct material colors are not proof of correct character modeling.
6. Correct actor rotation is not proof of correct architecture.
7. A populated registry is not proof of implemented assets.
8. A successful build is not proof of visual acceptance.
9. A screenshot is not proof of playable functionality.
10. Every accepted visual change must be verified in the real runtime.
11. Never hide missing assets by silently substituting unrelated meshes.
12. Preserve existing valid gameplay systems and fix the smallest responsible layer.
13. Every missing production asset must have a precise authoring specification and status.
14. Keep debug content in dedicated development/QA environments.
15. Maintain a coherent global visual identity across the entire game.
16. Build one gold-standard vertical slice before mass-producing characters, stages and worlds.
17. No task is complete until its acceptance criteria are verified.
18. Report actual implementation, not intended implementation.

## PLACEHOLDER FAILURE CONDITIONS
The following are NOT production visual assets:
- Unreal default mannequin
- Engine/BasicShapes
- Primitive cube, cylinder, cone, sphere, plane
- Checkerboard material
- Debug emissive material
- Untextured fallback mesh
- Pitch-black silhouette character
- Blinding white blown-out wall or surface
- Debug VFX
- Generated placeholder geometry
- Missing skeletal mesh
- Missing animation
- Missing material
- Missing texture
- Missing collision
- Missing background landmark (Hokage Monument)
- Empty black skybox or void surrounding the scene

These may only be used in isolated development tests.
They MUST NOT be used to claim Golden Visual Slice completion.

## VISUAL STYLE AUTHORITY
The target visual direction is:
STYLIZED 3D ANIME + PAINTERLY ENVIRONMENT + WARM CINEMATIC DAYLIGHT + READABLE CEL/STYLIZED MATERIALS + NATURAL SHINOBI MOVEMENT + AUTHORED KONOHA ARCHITECTURE + VEGETATION + NPC LIFE + BACKGROUND LANDMARKS + CONTROLLED CHAKRA/JUTSU VFX + THIRD-PERSON ACTION CAMERA.

The project does NOT require photorealism.
Do not add realism merely because Unreal supports it.
Do not use lighting technology as a substitute for missing art.

## CRITICAL RULE: DO NOT SOLVE MISSING ART WITH LIGHTING
If the character is missing or silhouette: fix the character asset and material binding.  
If the building is missing: fix the building asset.  
If the background is missing: fix the environment backdrop.  
If the material is wrong or glowing: fix the material parameters.  
If the animation is wrong: fix the animation state machine.  

Do NOT compensate by adding lights, bloom, emissive materials, fog, exposure changes, or rendering effects.

## ADVANCEMENT GATE
If the runtime screenshot shows:
- missing characters or placeholder mannequin silhouettes
- primitive buildings or white untextured slabs
- primitive roads or stepped cylinders
- missing vegetation
- missing NPCs
- missing background (pitch black void)
- overexposure or pitch black crushed shadows
- broken materials
- missing animation
- debug VFX

then:
`VISUAL GATE = FAIL`

The agent MUST NOT declare the phase complete.
The agent MUST NOT proceed to other features or expansion worlds.
The agent must identify the FIRST blocking dependency and fix that dependency before moving on.
