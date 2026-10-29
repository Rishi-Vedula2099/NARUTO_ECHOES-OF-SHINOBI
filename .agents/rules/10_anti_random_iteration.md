

# ANTI-RANDOM-ITERATION RULE

Project: NARUTO: ECHOES OF SHINOBI
Engine: Unreal Engine 5.8

## ABSOLUTE RULE

ANTIGRAVITY MUST NOT ITERATE RANDOMLY ON VISUAL PROBLEMS.

Instead, follow this exact diagnostic flow:

```
OBSERVE (capture current state)
    ↓
IDENTIFY FAILURE CATEGORY
    ↓
CLASSIFY ROOT CAUSE (which layer is broken?)
    ↓
FIX ROOT CAUSE (fix the correct layer)
    ↓
RUN VISUAL TEST
    ↓
CAPTURE SCREENSHOT
    ↓
COMPARE AGAINST VISUAL ACCEPTANCE
    ↓
PASS / FAIL
```

## Failure Category Classification

| Symptom | Failure Category | Correct Fix Layer |
|---|---|---|
| Black screen / void | LIGHTING / SKY | Fix DirectionalLight, SkyAtmosphere, SkyLight |
| White blown-out scene | EXPOSURE | Fix PostProcess exposure, light intensity |
| Character silhouette (black) | MATERIAL / MESH | Fix character material binding, mesh assignment |
| Missing character | ASSET / REFERENCE | Fix mesh soft reference, spawn logic |
| Missing building | ASSET / LEVEL | Import/place building mesh |
| Missing texture | ASSET / MATERIAL | Import texture, assign to material |
| Checkerboard material | ASSET / MATERIAL | Import proper texture, fix material |
| Mannequin visible | ASSET / REFERENCE | Fix character mesh reference |
| BasicShapes visible | ASSET / REPLACEMENT | Replace with production mesh |
| No vegetation | ASSET / LEVEL | Place authored foliage meshes |
| No NPCs | ACTOR / SPAWN | Create and place NPC actors |
| Crushed shadows | LIGHTING | Fix SkyLight, shadow bias, ambient fill |
| Extreme bloom | POST-PROCESS | Reduce bloom intensity (≤ 0.45) |
| Camera too close | CAMERA | Fix SpringArm length |
| Camera too far | CAMERA | Fix SpringArm length |
| Debug UI | UI | Replace with production UMG widgets |

## DO NOT Do These

- DO NOT change lighting to compensate for missing assets
- DO NOT increase emissive to compensate for dark scenes
- DO NOT add fog to hide missing geometry
- DO NOT add bloom to mask material problems
- DO NOT change exposure to fix material issues
- DO NOT add point lights to fix global illumination failures
- DO NOT declare "improved" without screenshot evidence
- DO NOT proceed to next phase when current phase fails

## After Every Visual Change

1. State what was changed and why
2. Classify the root cause
3. Provide before/after evidence
4. Report PASS or FAIL against the visual reference

## Log Requirement

Every visual failure and its resolution must be logged in:
`Docs/Reports/VISUAL_FAILURE_MEMORY.md`
