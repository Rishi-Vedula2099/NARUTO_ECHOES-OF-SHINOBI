# NARUTO: ECHOES OF SHINOBI
# 07 — JUTSU & VFX BIBLE
## *Authoritative Jutsu Visual & VFX Specification*

---

## Core Principle

Every jutsu is a complete visual sequence with preparation, execution, impact, and recovery. VFX must be readable, stylized, and never overpower character visibility.

---

## Jutsu Visual Sequence

```
PREPARATION
    → HAND SIGNS (optional per jutsu)
    → CHAKRA BUILDUP (visible gathering effect)
    → JUTSU FORMATION (technique takes shape)
    → RELEASE (execution)
    → IMPACT (on target)
    → SECONDARY EFFECT (environmental, knockback, etc.)
    → RECOVERY (cooldown visual, character returns to ready state)
```

---

## Rasengan — Golden Jutsu Test

The first complete jutsu pipeline. Must be fully validated before any other jutsu.

### Rasengan Visual Breakdown

| Phase | Visual | VFX | Camera | Duration |
|---|---|---|---|---|
| Charge Start | Right hand extends, left hand cups | Blue chakra wisps gather | Slight pull-in | 0.3s |
| Chakra Gather | Spinning energy forms in palm | Swirling blue sphere grows, wind trails | Tighten on hand | 0.5s |
| Formation | Rasengan fully formed, stable | Bright rotating sphere, particle ring | Return to gameplay | 0.2s |
| Lunge | Naruto dashes forward | Motion trail, wind distortion | Follow dash | 0.3s |
| Impact | Palm strikes target | Explosion of blue energy, spiral outward | Camera shake | 0.15s |
| Knockback | Target flies backward | Debris, dust cloud, impact crater | Wide shot | 0.3s |
| Recovery | Naruto pulls back, stance reset | Residual energy dissipates | Return normal | 0.4s |

### Rasengan Data Contract
```
JutsuID:          "JUTSU_RASENGAN"
CharacterID:      "CHAR_NARUTO"
JutsuType:        CLOSE_RANGE
Element:          NONE (pure chakra)
ChakraCost:       30
Cooldown:         8.0s
CastTime:         0.8s
ChargeTime:       0.5s
ReleaseTime:      0.3s
HitWindow:        0.15s
RecoveryWindow:   0.4s
Damage:           HIGH
Knockback:        STRONG
```

---

## VFX Style Rules

### Visual Hierarchy (Priority Order)
```
1. CHARACTER MOTION (always visible through VFX)
2. ACTION (attack/dodge readable)
3. JUTSU EFFECT (spectacular but doesn't obscure gameplay)
4. IMPACT (brief, intense, then clears)
5. RECOVERY (dissipation, return to clarity)
```

### VFX DO
- Use Niagara for all particle systems
- Match anime color language (blue chakra, orange fire, etc.)
- Keep effects additive/translucent, not opaque
- Scale effects proportionally to camera distance
- Add dynamic light contribution (subtle, capped intensity)
- Include audio sync points

### VFX DO NOT
- Overuse bloom (max 0.45 intensity)
- Create screen-filling opaque effects
- Use realistic explosion/smoke (stylized only)
- Make VFX so bright they blow out exposure
- Use debug/placeholder VFX in production
- Create effects that hide the character completely

---

## Jutsu VFX Color Language

| Element | Primary Color | Secondary Color | Emissive |
|---|---|---|---|
| Chakra (raw) | Blue (#4488FF) | Cyan (#66CCFF) | Soft glow |
| Wind | White-green (#CCFFCC) | Light blue | Slash trails |
| Fire | Orange (#FF8833) | Red-yellow | Warm glow |
| Lightning | White-purple (#CCCCFF) | Electric blue | Crackling |
| Water | Deep blue (#3366CC) | Cyan-white foam | Wet surface |
| Earth | Brown-gold (#CC9933) | Rocky gray | Dust |
| Nine-Tails | Red-orange (#FF4422) | Dark crimson | Intense |
| Sage | Orange-gold (#FFAA33) | Nature green | Warm pulse |

---

## Jutsu Data Contract (Per Jutsu)

```cpp
USTRUCT(BlueprintType)
struct FEOSJutsuVisualProfile
{
    GENERATED_BODY()

    UPROPERTY() FName JutsuID;
    UPROPERTY() FName CharacterID;
    UPROPERTY() EEOSJutsuType JutsuType;       // CLOSE, MID, LONG, AREA
    UPROPERTY() EEOSElement Element;
    UPROPERTY() float ChakraCost;
    UPROPERTY() float Cooldown;
    UPROPERTY() float CastTime;
    UPROPERTY() float ChargeTime;
    UPROPERTY() float ReleaseTime;
    UPROPERTY() float HitWindow;
    UPROPERTY() float RecoveryWindow;
    UPROPERTY() TSoftObjectPtr<UAnimMontage> AnimationMontage;
    UPROPERTY() TSoftObjectPtr<UNiagaraSystem> ChargeVFX;
    UPROPERTY() TSoftObjectPtr<UNiagaraSystem> ReleaseVFX;
    UPROPERTY() TSoftObjectPtr<UNiagaraSystem> ImpactVFX;
    UPROPERTY() TSoftObjectPtr<USoundBase> ChargeAudio;
    UPROPERTY() TSoftObjectPtr<USoundBase> ReleaseAudio;
    UPROPERTY() TSoftObjectPtr<USoundBase> ImpactAudio;
    UPROPERTY() FName CameraProfile;
    UPROPERTY() FGameplayTagContainer GameplayTags;
    UPROPERTY() float BaseDamage;
    UPROPERTY() EEOSKnockbackType KnockbackType;
};
```

---

## Hand-Sign / QTE System

### Jutsu Learning Path
```
Character Advancement
    → Jutsu Eligibility (level/rank requirement)
    → Jutsu Trial (practice mode)
    → Hand-Sign Practice (learn the sequence)
    → Chakra Execution (control the power)
    → Mastery (unlock full jutsu)
    → Jutsu Available in Combat
```

### QTE Modes
- Hand-sign recognition (match the sign)
- Hand-sign sequence (correct order)
- Timed input (rhythm-based)
- Chakra control (hold precision)
- Timing release (release at right moment)
- Reaction input (dodge/counter prompt)
- Practical combat test (use in real fight)

### Failure Flow
```
ATTEMPT
    → FAIL
    → PRACTICE XP GAINED
    → INCREASED FAMILIARITY
    → RETRY (easier)
    → EVENTUAL MASTERY
```

Not: "You failed. Try again." → But: progressive improvement.

### Input Support
- Keyboard + Mouse
- Controller (gamepad)
- Mobile (future — touch input)

### Canon Compliance
- Use known canon hand-sign sequences
- If canon data unavailable: `TODO_AUTHORING`
- Never invent non-canon hand-sign sequences
