

# VISUAL DIAGNOSTIC MANDATE

Project: NARUTO: ECHOES OF SHINOBI
Engine: Unreal Engine 5.8

## SUPREME DIRECTIVES

### Directive 1: Visual Identity IS the Product
NARUTO: ECHOES OF SHINOBI is not being developed as a technically functional Unreal prototype that will receive its visual identity later. The visual identity is part of the product itself.

### Directive 2: Fix the Correct Layer
Do not attempt to solve missing visual assets through lighting, exposure, fog, emissive intensity, post-processing or arbitrary material changes.

- Missing assets = ASSET-PRODUCTION problem
- Incorrect rendering = RENDERING problem
- Incorrect animation = ANIMATION problem
- Incorrect camera framing = CAMERA problem
- Incorrect UI = UI problem

Diagnose and fix the correct layer.

## NEVER ASSUME

```
actor exists              ≠  visual exists
mesh component exists     ≠  mesh exists
light exists              ≠  lighting is correct
material exists           ≠  material is correct
C++ compiles              ≠  feature works
asset reference exists    ≠  asset loads
HUD exists                ≠  UI is production-ready
jutsu ability exists      ≠  jutsu visually works
AI node exists            ≠  AI works
animation asset exists    ≠  animation works
```

## NEVER USE THESE AS PRODUCTION ASSETS

- Unreal default mannequin
- Engine/BasicShapes
- Primitive cube, cylinder, cone, sphere, plane
- Checkerboard material
- Debug emissive material
- Untextured fallback mesh
- Pitch-black silhouette character
- Blinding white blown-out wall
- Debug VFX
- Generated placeholder geometry

## NEVER DECLARE VISUAL COMPLETE BECAUSE

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

## VISUAL COMPLETION REQUIRES

The intended production-quality visual asset is actually rendered in the running game, matching the EOS Visual DNA, passing all 6 visual gates, and matching the gameplay reference in overall visual language.
