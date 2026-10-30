import os

cpp_path = r"Source/EchoesOfShinobi/Private/Character/EOSCharacterBase.cpp"

with open(cpp_path, "r", encoding="utf-8") as f:
    content = f.read().replace("\r\n", "\n")

target1 = """void AEOSCharacterBase::SetupStylizedVisualMesh()
{
	// Vertical slice character visual presentation:
	// Eliminates TECHNICAL_DEBUG_ASSETS (primitives / checkerboard) from player view.
	// Uses authored stylized skeletal mesh or stylized character asset with anime cel-shaded material.
	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		CharMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		CharMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}"""

replacement1 = """void AEOSCharacterBase::ResolveProductionVisualDefaults()
{
	VisualProfile.CharacterID = TEXT("CHAR_NARUTO");
	VisualProfile.EraID = TEXT("ERA_GENIN");
	VisualProfile.FormID = TEXT("FORM_BASE");
	VisualProfile.CostumeID = TEXT("COSTUME_CLASSIC_TRACKSUIT");
	VisualProfile.AnimationProfileID = TEXT("ANIM_NARUTO_GENIN");
	VisualProfile.MaterialProfileID = TEXT("MAT_CEL_TOON_SHINOBI");
	VisualProfile.VFXProfileID = TEXT("VFX_NARUTO_DEFAULT");

	// Canonical proof-of-pipeline mesh & ABP (Naruto Genin)
	VisualProfile.SkeletalMeshAsset = TSoftObjectPtr<USkeletalMesh>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Meshes/SK_Naruto_Genin_Rigged/SkeletalMeshes/SK_Naruto_Genin.SK_Naruto_Genin")));
	VisualProfile.AnimBlueprintClass = TSoftClassPtr<UAnimInstance>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Anims/ABP_Naruto_Genin.ABP_Naruto_Genin_C")));

	// 7 Canonical Material Slots
	VisualProfile.MaterialInstances.Empty();
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Skin.MI_Naruto_Skin"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Navy.MI_Naruto_Navy"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Hair.MI_Naruto_Hair"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Collar.MI_Naruto_Collar"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Tracksuit.MI_Naruto_Tracksuit"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Emblem.MI_Naruto_Emblem"))));
	VisualProfile.MaterialInstances.Add(TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/Characters/Naruto/Materials/MI_Naruto_Metal.MI_Naruto_Metal"))));

	// Canonical Niagara Jutsu Asset (Rasengan)
	JutsuVFXAsset = TSoftObjectPtr<UNiagaraSystem>(FSoftObjectPath(TEXT("/Game/VFX/Niagara/Jutsu/NS_Rasengan_Orb_Master.NS_Rasengan_Orb_Master")));
}

void AEOSCharacterBase::ApplyMeshComponentLayout()
{
	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		CharMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -96.0f));
		CharMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

void AEOSCharacterBase::SetupStylizedVisualMesh()
{
	ApplyMeshComponentLayout();

	if (USkeletalMeshComponent* CharMesh = GetMesh())
	{
		if (USkeletalMesh* MeshAsset = VisualProfile.SkeletalMeshAsset.LoadSynchronous())
		{
			CharMesh->SetSkeletalMeshAsset(MeshAsset);
		}

		if (UClass* AnimClass = VisualProfile.AnimBlueprintClass.LoadSynchronous())
		{
			CharMesh->SetAnimInstanceClass(AnimClass);
		}

		for (int32 SlotIdx = 0; SlotIdx < VisualProfile.MaterialInstances.Num(); ++SlotIdx)
		{
			if (UMaterialInterface* Mat = VisualProfile.MaterialInstances[SlotIdx].LoadSynchronous())
			{
				CharMesh->SetMaterial(SlotIdx, Mat);
			}
		}
	}
}

void AEOSCharacterBase::SpawnJutsuVFXAtSocket()
{
	if (!JutsuVFXAsset.IsNull())
	{
		UNiagaraSystem* LoadedVFX = JutsuVFXAsset.LoadSynchronous();
		if (LoadedVFX && GetMesh())
		{
			if (JutsuVFXComponent)
			{
				JutsuVFXComponent->SetAsset(LoadedVFX);
				JutsuVFXComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_rSocket"));
				JutsuVFXComponent->Activate(true);
			}
			else
			{
				JutsuVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
					LoadedVFX,
					GetMesh(),
					TEXT("hand_rSocket"),
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::SnapToTarget,
					true
				);
			}
		}
	}
}"""

if target1 in content:
    content = content.replace(target1, replacement1)
    print("Part 1 replaced successfully.")
else:
    print("Part 1 TARGET NOT FOUND.")

# Part 2: ValidateVisualAssetIntegrity at the end of the file
validation_code = """
TArray<FEOSVisualAssetSlotVerdict> AEOSCharacterBase::ValidateVisualAssetIntegrity(bool bLogResults) const
{
	TArray<FEOSVisualAssetSlotVerdict> Verdicts;

	// 1. SkeletalMesh Slot
	{
		FEOSVisualAssetSlotVerdict MeshVerdict;
		MeshVerdict.AssetID = VisualProfile.CharacterID;
		MeshVerdict.AssetSlot = TEXT("SkeletalMesh");
		MeshVerdict.ResolvedPath = VisualProfile.SkeletalMeshAsset.ToSoftObjectPath().ToString();
		MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (MeshVerdict.ResolvedPath.IsEmpty())
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			MeshVerdict.bPassed = false;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenEngineBasicShape(MeshVerdict.ResolvedPath))
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenBasicShape;
			MeshVerdict.bPassed = false;
		}
		else if (FEOSVisualRectificationStatus::IsForbiddenDefaultMannequin(MeshVerdict.ResolvedPath))
		{
			MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenMannequin;
			MeshVerdict.bPassed = false;
		}
		else
		{
			UObject* LoadedObj = VisualProfile.SkeletalMeshAsset.ToSoftObjectPath().ResolveObject();
			if (!LoadedObj)
			{
				LoadedObj = VisualProfile.SkeletalMeshAsset.LoadSynchronous();
			}
			if (LoadedObj != nullptr)
			{
				MeshVerdict.bPassed = true;
			}
			else
			{
				MeshVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				MeshVerdict.bPassed = false;
			}
		}
		Verdicts.Add(MeshVerdict);
	}

	// 2. AnimBlueprint Slot
	{
		FEOSVisualAssetSlotVerdict AnimVerdict;
		AnimVerdict.AssetID = VisualProfile.CharacterID;
		AnimVerdict.AssetSlot = TEXT("AnimBlueprint");
		AnimVerdict.ResolvedPath = VisualProfile.AnimBlueprintClass.ToSoftObjectPath().ToString();
		AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (AnimVerdict.ResolvedPath.IsEmpty())
		{
			AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			AnimVerdict.bPassed = false;
		}
		else
		{
			UClass* LoadedClass = VisualProfile.AnimBlueprintClass.LoadSynchronous();
			if (LoadedClass != nullptr)
			{
				AnimVerdict.bPassed = true;
			}
			else
			{
				AnimVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				AnimVerdict.bPassed = false;
			}
		}
		Verdicts.Add(AnimVerdict);
	}

	// 3. JutsuVFX Slot
	{
		FEOSVisualAssetSlotVerdict VFXVerdict;
		VFXVerdict.AssetID = VisualProfile.CharacterID;
		VFXVerdict.AssetSlot = TEXT("JutsuVFX");
		VFXVerdict.ResolvedPath = JutsuVFXAsset.ToSoftObjectPath().ToString();
		VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (VFXVerdict.ResolvedPath.IsEmpty())
		{
			VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			VFXVerdict.bPassed = false;
		}
		else
		{
			UObject* LoadedVFX = JutsuVFXAsset.LoadSynchronous();
			if (LoadedVFX != nullptr)
			{
				VFXVerdict.bPassed = true;
			}
			else
			{
				VFXVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
				VFXVerdict.bPassed = false;
			}
		}
		Verdicts.Add(VFXVerdict);
	}

	// 4. MaterialInstances Slot
	{
		FEOSVisualAssetSlotVerdict MatVerdict;
		MatVerdict.AssetID = VisualProfile.CharacterID;
		MatVerdict.AssetSlot = TEXT("MaterialInstances");
		MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailNone;

		if (VisualProfile.MaterialInstances.Num() == 0)
		{
			MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingSoftReference;
			MatVerdict.bPassed = false;
		}
		else
		{
			bool bAllMatsValid = true;
			FString CombinedPaths;
			for (const auto& MatSoftPtr : VisualProfile.MaterialInstances)
			{
				const FString PathStr = MatSoftPtr.ToSoftObjectPath().ToString();
				CombinedPaths += PathStr + TEXT(";");
				if (FEOSVisualRectificationStatus::IsForbiddenPlaceholderMaterial(PathStr))
				{
					MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailForbiddenPlaceholderMaterial;
					bAllMatsValid = false;
					break;
				}
				if (!MatSoftPtr.LoadSynchronous())
				{
					MatVerdict.FailureCode = FEOSVisualRectificationStatus::FailMissingPackage;
					bAllMatsValid = false;
				}
			}
			MatVerdict.ResolvedPath = CombinedPaths;
			MatVerdict.bPassed = bAllMatsValid;
		}
		Verdicts.Add(MatVerdict);
	}

	if (bLogResults)
	{
		for (const FEOSVisualAssetSlotVerdict& V : Verdicts)
		{
			UE_LOG(LogEOSCore, Log, TEXT("Visual Integrity Gate [%s - %s]: %s (Failure: %s) -> %s"),
				*V.AssetID, *V.AssetSlot, V.bPassed ? TEXT("PASS") : TEXT("FAIL"), *V.FailureCode, *V.ResolvedPath);
		}
	}

	return Verdicts;
}
"""

if "ValidateVisualAssetIntegrity" not in content:
    content = content + "\n" + validation_code
    print("Part 2 (ValidateVisualAssetIntegrity) added.")
else:
    print("ValidateVisualAssetIntegrity already present.")

# Part 3: In CastJutsuSlot slot 0, trigger SpawnJutsuVFXAtSocket()
target_jutsu0 = 'JutsuID = TEXT("JUTSU_NARUTO_RASENGAN"); // Slot 1: [Q] Rasengan (CANON_VERIFIED)'
if target_jutsu0 in content:
    # also call SpawnJutsuVFXAtSocket() inside the chakra check if possible, or right in case 0
    replacement_jutsu0 = 'JutsuID = TEXT("JUTSU_NARUTO_RASENGAN"); // Slot 1: [Q] Rasengan (CANON_VERIFIED)\n\t\tSpawnJutsuVFXAtSocket();'
    content = content.replace(target_jutsu0, replacement_jutsu0)
    print("Part 3 (Rasengan VFX hook) added.")

with open(cpp_path, "w", encoding="utf-8") as f:
    f.write(content)
print("File saved successfully.")
