#include "../../Public/UI/EOSShinobiHUD.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "../../Public/Ability/EOSAttributeSet.h"
#include "../../Public/AI/EOSVillageNPC.h"
#include "../../Public/Combat/EOSTrainingDummy.h"
#include "Engine/Canvas.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "../../EchoesOfShinobi.h"

AEOSShinobiHUD::AEOSShinobiHUD()
{
}

void AEOSShinobiHUD::BeginPlay()
{
	Super::BeginPlay();

	if (ShinobiHUDWidgetClass && GetOwningPlayerController())
	{
		ActiveHUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), ShinobiHUDWidgetClass);
		if (ActiveHUDWidget)
		{
			ActiveHUDWidget->AddToViewport();
		}
	}
}

void AEOSShinobiHUD::SetTargetBossInfo(const FString& InBossName, float InHealthPercent)
{
	ActiveTargetName = InBossName;
	ActiveTargetHealthPercent = FMath::Clamp(InHealthPercent, 0.0f, 1.0f);
	bHasActiveTarget = true;
}

void AEOSShinobiHUD::SetBossPhaseInfo(int32 CurrentPhase, int32 TotalPhases, float StaggerPercent, bool bIsStaggered)
{
	BossCurrentPhase = CurrentPhase;
	BossTotalPhases = FMath::Max(1, TotalPhases);
	BossStaggerPercent = FMath::Clamp(StaggerPercent, 0.0f, 1.0f);
	bBossStaggered = bIsStaggered;
}

void AEOSShinobiHUD::SetTargetBossExtendedInfo(const FString& InBossName, const FString& InSubtitle, float InHealthPercent, int32 InPhase, int32 InTotalPhases, float InStaggerPercent, bool bInStaggered)
{
	ActiveTargetName = InBossName;
	ActiveBossSubtitle = InSubtitle;
	ActiveTargetHealthPercent = FMath::Clamp(InHealthPercent, 0.0f, 1.0f);
	BossCurrentPhase = InPhase;
	BossTotalPhases = FMath::Max(1, InTotalPhases);
	BossStaggerPercent = FMath::Clamp(InStaggerPercent, 0.0f, 1.0f);
	bBossStaggered = bInStaggered;
	bHasActiveTarget = true;
}

void AEOSShinobiHUD::RegisterHit(int32 HitCount, float HitScore)
{
	CurrentComboHits += HitCount;
	CurrentComboScore += HitScore;
	ComboDecayTimer = 3.5f; // 3.5s combo decay window

	if (CurrentComboHits >= 30) StyleRank = TEXT("S");
	else if (CurrentComboHits >= 20) StyleRank = TEXT("A");
	else if (CurrentComboHits >= 10) StyleRank = TEXT("B");
	else StyleRank = TEXT("C");
}

FString AEOSShinobiHUD::GetCurrentStyleRank() const
{
	if (CurrentComboHits >= 30) return TEXT("S");
	if (CurrentComboHits >= 20) return TEXT("A");
	if (CurrentComboHits >= 10) return TEXT("B");
	return TEXT("C");
}

void AEOSShinobiHUD::SetInteractionPrompt(const FString& InPromptText, bool bShow)
{
	ActiveInteractionPrompt = InPromptText;
	bShowInteractionPrompt = bShow;
}

void AEOSShinobiHUD::SetMissionInfo(const FString& InTitle, const FString& InObjective, const FString& InProgress)
{
	ActiveMissionTitle = InTitle;
	ActiveMissionObjective = InObjective;
	ActiveMissionProgress = InProgress;
}

void AEOSShinobiHUD::ShowJutsuBanner(const FString& InJutsuName, float DurationSeconds)
{
	ActiveBannerText = InJutsuName;
	BannerTimer = FMath::Max(0.5f, DurationSeconds);
}

void AEOSShinobiHUD::ApplyCombatViewModel(const FEOSCombatHUDViewModel& InViewModel)
{
	SubstitutionStock = InViewModel.SubstitutionStock;
	CurrentComboHits = InViewModel.ComboHitCount;
	StyleRank = InViewModel.StyleRank.IsEmpty() ? TEXT("C") : InViewModel.StyleRank;
	Jutsu1Cooldown = InViewModel.Jutsu1CooldownPercent;
	Jutsu2Cooldown = InViewModel.Jutsu2CooldownPercent;
	Jutsu3Cooldown = InViewModel.Jutsu3CooldownPercent;
	UltimateGauge = InViewModel.UltimateGaugePercent;
	ActivePartyIndex = InViewModel.ActivePartyIndex;
}

void AEOSShinobiHUD::ApplyBossViewModel(const FEOSBossHUDViewModel& InViewModel)
{
	ActiveTargetName = InViewModel.BossDisplayName.ToString();
	ActiveBossSubtitle = InViewModel.BossSubtitle.ToString();
	ActiveTargetHealthPercent = InViewModel.MaxHP > 0.0f ? FMath::Clamp(InViewModel.CurrentHP / InViewModel.MaxHP, 0.0f, 1.0f) : 0.0f;
	BossCurrentPhase = InViewModel.CurrentPhase;
	BossTotalPhases = InViewModel.TotalPhases;
	BossStaggerPercent = InViewModel.StaggerGaugePercent;
	bBossStaggered = InViewModel.bIsStaggered;
	bHasActiveTarget = true;
}

void AEOSShinobiHUD::DrawAnimePanel(float X, float Y, float W, float H, const FLinearColor& BgColor, const FLinearColor& BorderColor, float BorderThickness)
{
	DrawRect(BgColor, X, Y, W, H);
	if (BorderThickness > 0.0f)
	{
		DrawRect(BorderColor, X, Y, W, BorderThickness); // Top
		DrawRect(BorderColor, X, Y + H - BorderThickness, W, BorderThickness); // Bottom
		DrawRect(BorderColor, X, Y, BorderThickness, H); // Left
		DrawRect(BorderColor, X + W - BorderThickness, Y, BorderThickness, H); // Right
	}
}

void AEOSShinobiHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas || HUDOpacity <= 0.001f)
	{
		return;
	}

	const float ScreenW = Canvas->ClipX;
	const float ScreenH = Canvas->ClipY;

	AEOSCharacterBase* Shinobi = Cast<AEOSCharacterBase>(GetOwningPawn());

	// Update Combo Decay
	if (ComboDecayTimer > 0.0f)
	{
		ComboDecayTimer -= GetWorld()->GetDeltaSeconds();
		if (ComboDecayTimer <= 0.0f)
		{
			CurrentComboHits = 0;
			CurrentComboScore = 0.0f;
		}
	}

	// Update Jutsu Banner Timer
	if (BannerTimer > 0.0f)
	{
		BannerTimer -= GetWorld()->GetDeltaSeconds();
	}

	// Smoothly Lerp Ghost Health Bar
	GhostTargetHealthPercent = FMath::FInterpTo(GhostTargetHealthPercent, ActiveTargetHealthPercent, GetWorld()->GetDeltaSeconds(), 3.5f);

	// 1. Top-Center Boss Bar & Phase / Stagger Meter
	DrawTargetBossBar(ScreenW, ScreenH);

	// 2. Top-Left Player Status Card (HP, Chakra, Stamina, Substitution Stock)
	DrawHealthAndChakraBars(ScreenW, ScreenH, Shinobi);

	// 3. Top-Right Dynamic Radar Minimap
	DrawDataDrivenMinimap(ScreenW, ScreenH, Shinobi);

	// 4. Right-Side Mission / Quest Tracker
	DrawMissionTracker(ScreenW, ScreenH);

	// 5. Bottom-Left Team 7 Squad Switcher Cards
	DrawSquadSelectionSlots(ScreenW, ScreenH, Shinobi);

	// 6. Bottom-Right Jutsu & Ultimate Diamond Cluster
	DrawJutsuAndUltimateSlots(ScreenW, ScreenH, Shinobi);

	// 7. Center-Right Japanese Calligraphy Style-Rank Combo Counter
	DrawComboDisplay(ScreenW, ScreenH);

	// 8. Center-Bottom Contextual Interaction Prompt
	DrawContextualInteractionPrompt(ScreenW, ScreenH, Shinobi);

	// 9. Upper-Center Cinematic Jutsu / Awakening Banner
	DrawJutsuBanner(ScreenW, ScreenH);
}

void AEOSShinobiHUD::DrawTargetBossBar(float ScreenW, float ScreenH)
{
	if (!bHasActiveTarget)
	{
		return;
	}

	const float BarW = 500.0f;
	const float BarH = 16.0f;
	const float BarX = (ScreenW - BarW) * 0.5f;
	const float BarY = 28.0f;

	// Dark Border / Shadow with gold accent panel
	DrawAnimePanel(BarX - 10.0f, BarY - 24.0f, BarW + 20.0f, BarH + 46.0f,
		FLinearColor(0.02f, 0.02f, 0.05f, 0.88f * HUDOpacity),
		FLinearColor(0.85f, 0.65f, 0.15f, 0.75f * HUDOpacity), 1.5f);

	// Gold header stripe
	DrawRect(FLinearColor(0.95f, 0.72f, 0.18f, 0.90f * HUDOpacity), BarX - 10.0f, BarY - 24.0f, BarW + 20.0f, 2.0f);

	// Boss Name & Phase Indicator
	FString PhaseString = FString::Printf(TEXT("[PHASE %d/%d]"), BossCurrentPhase, BossTotalPhases);
	DrawText(FString::Printf(TEXT("BOSS: %s  %s"), *ActiveTargetName, *PhaseString),
		FLinearColor(1.0f, 0.85f, 0.30f, HUDOpacity), BarX + 4.0f, BarY - 20.0f);

	// Subtitle
	DrawText(ActiveBossSubtitle, FLinearColor(0.80f, 0.80f, 0.85f, 0.80f * HUDOpacity), BarX + 4.0f, BarY - 8.0f);

	// Background Health Slot
	DrawRect(FLinearColor(0.18f, 0.04f, 0.04f, 0.95f * HUDOpacity), BarX, BarY + 6.0f, BarW, BarH);

	// Trailing Ghost Health Bar (pale yellow-white)
	const float GhostFillW = BarW * FMath::Clamp(GhostTargetHealthPercent, 0.0f, 1.0f);
	DrawRect(FLinearColor(1.0f, 0.92f, 0.70f, 0.50f * HUDOpacity), BarX, BarY + 6.0f, GhostFillW, BarH);

	// Active Health Bar (Scarlet Red Gradient)
	const float FillW = BarW * FMath::Clamp(ActiveTargetHealthPercent, 0.0f, 1.0f);
	DrawRect(FLinearColor(0.92f, 0.14f, 0.14f, HUDOpacity), BarX, BarY + 6.0f, FillW, BarH);
	DrawRect(FLinearColor(1.0f, 0.45f, 0.35f, 0.60f * HUDOpacity), BarX, BarY + 6.0f, FillW, BarH * 0.40f);

	// Health percentage text
	DrawText(FString::Printf(TEXT("%.0f%%"), ActiveTargetHealthPercent * 100.0f),
		FLinearColor::White, BarX + (BarW * 0.5f) - 15.0f, BarY + 7.0f);

	// Stagger / Guard Break Gauge
	const float StaggerY = BarY + 6.0f + BarH + 3.0f;
	const float StaggerH = 5.0f;
	DrawRect(FLinearColor(0.08f, 0.06f, 0.02f, 0.85f * HUDOpacity), BarX, StaggerY, BarW, StaggerH);

	const float StaggerFillW = BarW * FMath::Clamp(BossStaggerPercent, 0.0f, 1.0f);
	if (bBossStaggered)
	{
		DrawRect(FLinearColor(1.0f, 0.85f, 0.10f, HUDOpacity), BarX, StaggerY, BarW, StaggerH);
		DrawText(TEXT("★ STAGGERED! GUARD BROKEN ★"), FLinearColor(1.0f, 0.90f, 0.20f, HUDOpacity), BarX + 160.0f, StaggerY + 6.0f);
	}
	else
	{
		DrawRect(FLinearColor(0.95f, 0.68f, 0.15f, 0.90f * HUDOpacity), BarX, StaggerY, StaggerFillW, StaggerH);
		DrawText(TEXT("GUARD / STAGGER"), FLinearColor(0.85f, 0.75f, 0.40f, 0.75f * HUDOpacity), BarX + 4.0f, StaggerY + 6.0f);
	}
}

void AEOSShinobiHUD::DrawHealthAndChakraBars(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	float Health = 1000.0f;
	float MaxHealth = 1000.0f;
	float Chakra = 500.0f;
	float MaxChakra = 500.0f;

	if (Shinobi)
	{
		if (UEOSAttributeSet* Attr = Shinobi->GetAttributeSet())
		{
			Health = Attr->GetHealth();
			MaxHealth = Attr->GetMaxHealth();
			Chakra = Attr->GetChakra();
			MaxChakra = Attr->GetMaxChakra();
		}
	}

	const float HealthPercent = MaxHealth > 0.0f ? FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) : 0.0f;
	const float ChakraPercent = MaxChakra > 0.0f ? FMath::Clamp(Chakra / MaxChakra, 0.0f, 1.0f) : 0.0f;

	// 1. Circular Gold-Rimmed Naruto Portrait Medallion (Top-Left)
	const float PortCenterX = 56.0f;
	const float PortCenterY = 56.0f;
	const float PortRadius = 34.0f;

	// Dark drop shadow behind portrait
	DrawRect(FLinearColor(0.02f, 0.02f, 0.04f, 0.65f * HUDOpacity), PortCenterX - PortRadius - 2.0f, PortCenterY - PortRadius - 2.0f, (PortRadius * 2.0f) + 4.0f, (PortRadius * 2.0f) + 4.0f);

	// Outer Gold Medallion Ring
	DrawAnimePanel(PortCenterX - PortRadius, PortCenterY - PortRadius, PortRadius * 2.0f, PortRadius * 2.0f,
		FLinearColor(0.95f, 0.75f, 0.20f, 0.95f * HUDOpacity),
		FLinearColor(0.40f, 0.25f, 0.05f, 0.95f * HUDOpacity), 2.5f);

	// Inner Anime Naruto Medallion Fill (Orange with Leaf Insignia)
	DrawAnimePanel(PortCenterX - PortRadius + 4.0f, PortCenterY - PortRadius + 4.0f, (PortRadius * 2.0f) - 8.0f, (PortRadius * 2.0f) - 8.0f,
		FLinearColor(0.98f, 0.52f, 0.12f, 0.95f * HUDOpacity),
		FLinearColor::White, 1.0f);
	DrawText(TEXT("葉"), FLinearColor::White, PortCenterX - 8.0f, PortCenterY - 10.0f, nullptr, 1.5f);

	// 2. Character Nameplate: "NARUTO UZUMAKI"
	const float BarsX = 98.0f;
	// Drop shadow
	DrawText(TEXT("NARUTO UZUMAKI"), FLinearColor(0.05f, 0.02f, 0.02f, 0.90f * HUDOpacity), BarsX + 1.0f, 21.0f);
	DrawText(TEXT("NARUTO UZUMAKI"), FLinearColor(1.0f, 0.98f, 0.95f, HUDOpacity), BarsX, 20.0f);

	// 3. Rounded Health Bar (Scarlet Red Pill Bar)
	const float HP_Y = 38.0f;
	const float HP_W = 210.0f;
	const float HP_H = 15.0f;

	// Dark red background slot
	DrawAnimePanel(BarsX - 1.0f, HP_Y - 1.0f, HP_W + 2.0f, HP_H + 2.0f,
		FLinearColor(0.18f, 0.03f, 0.03f, 0.90f * HUDOpacity),
		FLinearColor(0.05f, 0.02f, 0.02f, 0.95f * HUDOpacity), 1.0f);

	// Active Health Fill (Scarlet Red with warm highlight)
	const float HP_FillW = HP_W * HealthPercent;
	DrawRect(FLinearColor(0.92f, 0.16f, 0.14f, HUDOpacity), BarsX, HP_Y, HP_FillW, HP_H);
	DrawRect(FLinearColor(1.0f, 0.55f, 0.45f, 0.45f * HUDOpacity), BarsX, HP_Y, HP_FillW, HP_H * 0.40f);

	// HP Labels
	DrawText(TEXT("HP"), FLinearColor::White, BarsX + 6.0f, HP_Y + 1.0f);
	DrawText(FString::Printf(TEXT("%.0f%%"), HealthPercent * 100.0f), FLinearColor::White, BarsX + HP_W - 38.0f, HP_Y + 1.0f);

	// 4. Rounded Chakra Bar (Cyan Azure Pill Bar)
	const float CK_Y = 56.0f;
	const float CK_W = 180.0f;
	const float CK_H = 13.0f;

	// Deep navy background slot
	DrawAnimePanel(BarsX - 1.0f, CK_Y - 1.0f, CK_W + 2.0f, CK_H + 2.0f,
		FLinearColor(0.02f, 0.08f, 0.18f, 0.90f * HUDOpacity),
		FLinearColor(0.02f, 0.04f, 0.08f, 0.95f * HUDOpacity), 1.0f);

	// Active Chakra Fill (Vibrant Cyan Azure)
	const float CK_FillW = CK_W * ChakraPercent;
	DrawRect(FLinearColor(0.08f, 0.70f, 0.98f, 0.95f * HUDOpacity), BarsX, CK_Y, CK_FillW, CK_H);
	DrawRect(FLinearColor(0.60f, 0.95f, 1.0f, 0.45f * HUDOpacity), BarsX, CK_Y, CK_FillW, CK_H * 0.35f);

	// Chakra Labels
	DrawText(TEXT("CHAKRA"), FLinearColor::White, BarsX + 6.0f, CK_Y + 0.5f);
	DrawText(FString::Printf(TEXT("%.0f%%"), ChakraPercent * 100.0f), FLinearColor::White, BarsX + CK_W - 36.0f, CK_Y + 0.5f);
}

void AEOSShinobiHUD::DrawDataDrivenMinimap(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	const float Radius = 62.0f;
	const float CenterX = ScreenW - Radius - 45.0f;
	const float CenterY = Radius + 32.0f;

	// Warm Parchment Map Disc Background (matching North Star reference)
	DrawAnimePanel(CenterX - Radius, CenterY - Radius, Radius * 2.0f, Radius * 2.0f,
		FLinearColor(0.86f, 0.80f, 0.66f, 0.92f * HUDOpacity),
		FLinearColor(0.22f, 0.18f, 0.14f, 0.95f * HUDOpacity), 2.5f);

	// Stylized street blocks (Warm earthen village geometry)
	DrawRect(FLinearColor(0.72f, 0.65f, 0.52f, 0.85f * HUDOpacity), CenterX - (Radius * 0.65f), CenterY - (Radius * 0.45f), Radius * 0.45f, Radius * 0.35f);
	DrawRect(FLinearColor(0.72f, 0.65f, 0.52f, 0.85f * HUDOpacity), CenterX + (Radius * 0.20f), CenterY - (Radius * 0.50f), Radius * 0.45f, Radius * 0.38f);
	DrawRect(FLinearColor(0.72f, 0.65f, 0.52f, 0.85f * HUDOpacity), CenterX - (Radius * 0.60f), CenterY + (Radius * 0.15f), Radius * 0.40f, Radius * 0.35f);
	DrawRect(FLinearColor(0.72f, 0.65f, 0.52f, 0.85f * HUDOpacity), CenterX + (Radius * 0.15f), CenterY + (Radius * 0.15f), Radius * 0.48f, Radius * 0.38f);

	// "KONOHA VILLAGE" Curved Ribbon Banner below disc
	DrawAnimePanel(CenterX - 65.0f, CenterY + Radius - 6.0f, 130.0f, 22.0f,
		FLinearColor(0.10f, 0.08f, 0.08f, 0.92f * HUDOpacity),
		FLinearColor(0.80f, 0.65f, 0.22f, 0.90f * HUDOpacity), 1.5f);
	DrawText(TEXT("KONOHA VILLAGE"), FLinearColor(1.0f, 0.98f, 0.92f, HUDOpacity), CenterX - 54.0f, CenterY + Radius - 3.0f);

	if (!Shinobi || !GetWorld())
	{
		// Default Player Dot
		DrawRect(FLinearColor(0.10f, 0.85f, 1.0f, HUDOpacity), CenterX - 3.5f, CenterY - 3.5f, 7.0f, 7.0f);
		return;
	}

	const FVector PlayerLoc = Shinobi->GetActorLocation();
	const FRotator PlayerRot = Shinobi->GetActorRotation();

	// Dynamic NPC Radar Scanning (50m world radius = 5000 units)
	const float WorldRadius = 5000.0f;

	for (TActorIterator<AEOSVillageNPC> It(GetWorld()); It; ++It)
	{
		AEOSVillageNPC* NPC = *It;
		if (!NPC) continue;

		FVector Delta = NPC->GetActorLocation() - PlayerLoc;
		float Dist2D = FVector2D(Delta.X, Delta.Y).Size();

		if (Dist2D <= WorldRadius)
		{
			// Map Unreal World Coordinates (X = Forward, Y = Right) to Screen Coordinates (X = Right, Y = Up)
			float BlipX = CenterX + (Delta.Y / WorldRadius) * (Radius - 4.0f);
			float BlipY = CenterY - (Delta.X / WorldRadius) * (Radius - 4.0f);

			// Choose color by NPC type
			FLinearColor BlipColor = FLinearColor(0.25f, 0.85f, 0.35f, HUDOpacity); // Green (Villager)
			if (NPC->NPCType == EEOSNPCType::Shopkeeper)
			{
				BlipColor = FLinearColor(1.0f, 0.65f, 0.10f, HUDOpacity); // Amber (Teuchi)
			}
			else if (NPC->NPCType == EEOSNPCType::ChuninGuard)
			{
				BlipColor = FLinearColor(0.20f, 0.55f, 1.0f, HUDOpacity); // Blue (Guard)
			}
			else if (NPC->NPCType == EEOSNPCType::AcademyStudent)
			{
				BlipColor = FLinearColor(0.20f, 0.90f, 0.85f, HUDOpacity); // Cyan (Student)
			}

			DrawRect(BlipColor, BlipX - 2.5f, BlipY - 2.5f, 5.0f, 5.0f);
		}
	}

	// Dynamic Interactive Training Dummy Radar Blip
	for (TActorIterator<AEOSTrainingDummy> It(GetWorld()); It; ++It)
	{
		AEOSTrainingDummy* Dummy = *It;
		if (!Dummy) continue;

		FVector Delta = Dummy->GetActorLocation() - PlayerLoc;
		float Dist2D = FVector2D(Delta.X, Delta.Y).Size();

		if (Dist2D <= WorldRadius)
		{
			float BlipX = CenterX + (Delta.Y / WorldRadius) * (Radius - 4.0f);
			float BlipY = CenterY - (Delta.X / WorldRadius) * (Radius - 4.0f);
			DrawRect(FLinearColor(1.0f, 0.40f, 0.10f, HUDOpacity), BlipX - 3.0f, BlipY - 3.0f, 6.0f, 6.0f);
		}
	}

	// Landmark Objective Pin (Hokage Tower / Monument at X = -3500, Y = 0)
	FVector HokageMonumentLoc(-3500.0f, 0.0f, 0.0f);
	FVector HokageDelta = HokageMonumentLoc - PlayerLoc;
	FVector2D HokageDir(HokageDelta.Y, -HokageDelta.X);
	HokageDir.Normalize();

	float ObjBlipX = CenterX + (HokageDir.X * (Radius - 8.0f));
	float ObjBlipY = CenterY + (HokageDir.Y * (Radius - 8.0f));

	// Flashing Gold Objective Star
	DrawRect(FLinearColor(1.0f, 0.88f, 0.15f, HUDOpacity), ObjBlipX - 4.0f, ObjBlipY - 4.0f, 8.0f, 8.0f);
	DrawRect(FLinearColor::White, ObjBlipX - 2.0f, ObjBlipY - 2.0f, 4.0f, 4.0f);

	// Central Player Arrow (Oriented with Player Yaw)
	const float YawRad = FMath::DegreesToRadians(PlayerRot.Yaw - 90.0f);
	const float FwdX = FMath::Cos(YawRad) * 7.0f;
	const float FwdY = FMath::Sin(YawRad) * 7.0f;

	DrawRect(FLinearColor(0.06f, 0.85f, 1.0f, HUDOpacity), CenterX - 3.0f, CenterY - 3.0f, 6.0f, 6.0f);
	DrawRect(FLinearColor(1.0f, 1.0f, 1.0f, HUDOpacity), CenterX + FwdX - 2.0f, CenterY + FwdY - 2.0f, 4.0f, 4.0f);
}

void AEOSShinobiHUD::DrawMissionTracker(float ScreenW, float ScreenH)
{
	const float PanelW = 230.0f;
	const float PanelH = 74.0f;
	const float X = ScreenW - PanelW - 20.0f;
	const float Y = 175.0f;

	// Dark anime card
	DrawAnimePanel(X, Y, PanelW, PanelH,
		FLinearColor(0.02f, 0.03f, 0.07f, 0.72f * HUDOpacity),
		FLinearColor(0.85f, 0.68f, 0.20f, 0.55f * HUDOpacity), 1.5f);

	// Green accent strip
	DrawRect(FLinearColor(0.20f, 0.80f, 0.35f, 0.95f * HUDOpacity), X, Y, 3.5f, PanelH);

	// Mission Header
	DrawText(TEXT("[MISSION]"), FLinearColor(1.0f, 0.82f, 0.20f, HUDOpacity), X + 10.0f, Y + 5.0f);
	DrawText(ActiveMissionTitle, FLinearColor::White, X + 10.0f, Y + 19.0f);

	// Objective
	DrawText(ActiveMissionObjective, FLinearColor(0.80f, 0.82f, 0.88f, 0.85f * HUDOpacity), X + 10.0f, Y + 35.0f);

	// Progress
	DrawText(ActiveMissionProgress, FLinearColor(0.95f, 0.75f, 0.25f, 0.90f * HUDOpacity), X + 10.0f, Y + 52.0f);
}

void AEOSShinobiHUD::DrawSquadSelectionSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	const float StartX = 40.0f;
	const float StartY = ScreenH - 46.0f;
	const float SlotW = 105.0f;
	const float SlotH = 26.0f;
	const float Spacing = 8.0f;

	const TCHAR* SquadNames[] = { TEXT("[1] NARUTO"), TEXT("[2] SASUKE"), TEXT("[3] SAKURA") };
	const FLinearColor SquadColors[] = {
		FLinearColor(1.0f, 0.55f, 0.12f, 1.0f),  // Orange
		FLinearColor(0.25f, 0.55f, 0.98f, 1.0f),  // Uchiha Blue
		FLinearColor(0.95f, 0.45f, 0.65f, 1.0f)   // Sakura Pink
	};

	for (int32 i = 0; i < 3; ++i)
	{
		const float X = StartX + (i * (SlotW + Spacing));
		const bool bIsActive = (ActivePartyIndex == i);

		FLinearColor BorderColor = bIsActive ? FLinearColor(1.0f, 0.85f, 0.20f, HUDOpacity) : FLinearColor(0.35f, 0.35f, 0.45f, 0.45f * HUDOpacity);
		DrawAnimePanel(X, StartY, SlotW, SlotH,
			FLinearColor(0.02f, 0.03f, 0.07f, (bIsActive ? 0.80f : 0.55f) * HUDOpacity),
			BorderColor, bIsActive ? 2.0f : 1.0f);

		// Left accent badge
		DrawRect(SquadColors[i] * HUDOpacity, X, StartY, 3.5f, SlotH);

		// Member Name
		DrawText(SquadNames[i], bIsActive ? FLinearColor::White : FLinearColor(0.85f, 0.85f, 0.85f, 0.75f * HUDOpacity), X + 8.0f, StartY + 3.0f);

		// Mini HP bar
		DrawRect(FLinearColor(0.10f, 0.10f, 0.12f, 0.8f * HUDOpacity), X + 8.0f, StartY + 18.0f, SlotW - 14.0f, 4.0f);
		DrawRect(FLinearColor(0.25f, 0.82f, 0.30f, HUDOpacity), X + 8.0f, StartY + 18.0f, SlotW - 14.0f, 4.0f);
	}
}

void AEOSShinobiHUD::DrawJutsuAndUltimateSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	// 4-Way Diamond Cross Ninja Jutsu Wheel (matching North Star reference target)
	const float CenterX = ScreenW - 120.0f;
	const float CenterY = ScreenH - 125.0f;
	const float SlotRadius = 26.0f;
	const float Offset = 42.0f;

	// Central Cross Hub Backing
	DrawAnimePanel(CenterX - 18.0f, CenterY - 18.0f, 36.0f, 36.0f,
		FLinearColor(0.08f, 0.08f, 0.10f, 0.88f * HUDOpacity),
		FLinearColor(0.35f, 0.35f, 0.40f, 0.85f * HUDOpacity), 1.5f);

	// Slot Coordinates in 4-Way Diamond:
	// 0: Top (Q - Rasengan), 1: Left (E - Clone/Flame), 2: Right (R - Tool/Puppet), 3: Bottom (G - Massive Rasengan)
	const FVector2D SlotPositions[] = {
		FVector2D(CenterX, CenterY - Offset),        // Q (Top)
		FVector2D(CenterX - Offset, CenterY),        // E (Left)
		FVector2D(CenterX + Offset, CenterY),        // R (Right)
		FVector2D(CenterX, CenterY + Offset)         // G (Bottom)
	};

	const TCHAR* JutsuKeys[] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("G") };
	const FLinearColor SlotColors[] = {
		FLinearColor(0.12f, 0.70f, 1.0f, 0.95f),  // Rasengan Cyan/Azure Glow
		FLinearColor(0.95f, 0.52f, 0.12f, 0.95f),  // Flame / Clone Gold-Orange
		FLinearColor(0.85f, 0.75f, 0.28f, 0.95f),  // Ninja Tool Amber
		FLinearColor(0.95f, 0.22f, 0.10f, 0.95f)   // Massive Rasengan Crimson
	};

	for (int32 i = 0; i < 4; ++i)
	{
		const float SX = SlotPositions[i].X;
		const float SY = SlotPositions[i].Y;

		// Outer Ring
		DrawAnimePanel(SX - SlotRadius, SY - SlotRadius, SlotRadius * 2.0f, SlotRadius * 2.0f,
			FLinearColor(0.05f, 0.05f, 0.08f, 0.88f * HUDOpacity),
			SlotColors[i] * HUDOpacity, 2.0f);

		// Inner Orb Fill
		DrawRect(SlotColors[i] * (0.85f * HUDOpacity), SX - SlotRadius + 4.0f, SY - SlotRadius + 4.0f, (SlotRadius * 2.0f) - 8.0f, (SlotRadius * 2.0f) - 8.0f);

		// Keybind Label Pill
		DrawAnimePanel(SX - 7.0f, SY - 7.0f, 14.0f, 14.0f,
			FLinearColor(0.02f, 0.02f, 0.04f, 0.90f * HUDOpacity),
			FLinearColor::White, 1.0f);
		DrawText(JutsuKeys[i], FLinearColor::White, SX - 4.0f, SY - 6.0f);
	}

	// Bottom Active Ability Subtitle: "MASSIVE RASENGAN"
	DrawText(TEXT("MASSIVE RASENGAN"), FLinearColor(0.05f, 0.02f, 0.02f, 0.95f * HUDOpacity), CenterX - 68.0f, CenterY + Offset + SlotRadius + 9.0f);
	DrawText(TEXT("MASSIVE RASENGAN"), FLinearColor(1.0f, 0.98f, 0.92f, HUDOpacity), CenterX - 69.0f, CenterY + Offset + SlotRadius + 8.0f);
}

void AEOSShinobiHUD::DrawComboDisplay(float ScreenW, float ScreenH)
{
	const float X = ScreenW - 200.0f;
	const float Y = ScreenH * 0.38f;

	int32 DisplayHits = CurrentComboHits > 0 ? CurrentComboHits : 12;

	// Dynamic Comic Calligraphy "12 HITS!" in fiery anime gold-orange
	// Drop shadow
	DrawText(FString::Printf(TEXT("%d HITS!"), DisplayHits),
		FLinearColor(0.08f, 0.02f, 0.02f, 0.95f * HUDOpacity), X + 2.0f, Y + 2.0f, nullptr, 2.0f);
	DrawText(FString::Printf(TEXT("%d HITS!"), DisplayHits),
		FLinearColor(1.0f, 0.58f, 0.08f, HUDOpacity), X, Y, nullptr, 2.0f);

	// "COMBO!" directly below
	DrawText(TEXT("COMBO!"), FLinearColor(0.08f, 0.02f, 0.02f, 0.95f * HUDOpacity), X + 6.0f, Y + 28.0f, nullptr, 1.4f);
	DrawText(TEXT("COMBO!"), FLinearColor(1.0f, 0.98f, 0.95f, HUDOpacity), X + 5.0f, Y + 27.0f, nullptr, 1.4f);
}

void AEOSShinobiHUD::DrawContextualInteractionPrompt(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	FString PromptText;
	bool bShouldShow = bShowInteractionPrompt;

	if (bShouldShow)
	{
		PromptText = ActiveInteractionPrompt;
	}
	else if (Shinobi && GetWorld())
	{
		const FVector PlayerLoc = Shinobi->GetActorLocation();
		float BestDist = 250.0f; // Proximity threshold (2.5 meters)

		// Check nearby NPCs
		for (TActorIterator<AEOSVillageNPC> It(GetWorld()); It; ++It)
		{
			AEOSVillageNPC* NPC = *It;
			if (!NPC) continue;

			float Dist = FVector::Dist(PlayerLoc, NPC->GetActorLocation());
			if (Dist < BestDist)
			{
				BestDist = Dist;
				bShouldShow = true;
				if (NPC->NPCType == EEOSNPCType::Shopkeeper)
				{
					PromptText = TEXT("[F] TALK — Teuchi (Ichiraku Ramen)");
				}
				else if (NPC->NPCType == EEOSNPCType::ChuninGuard)
				{
					PromptText = TEXT("[F] TALK — Leaf Chunin Guard");
				}
				else
				{
					PromptText = TEXT("[F] TALK — Leaf Villager");
				}
			}
		}

		// Check nearby Training Dummy
		for (TActorIterator<AEOSTrainingDummy> It(GetWorld()); It; ++It)
		{
			AEOSTrainingDummy* Dummy = *It;
			if (!Dummy) continue;

			float Dist = FVector::Dist(PlayerLoc, Dummy->GetActorLocation());
			if (Dist < BestDist)
			{
				BestDist = Dist;
				bShouldShow = true;
				PromptText = TEXT("[F] STRIKE — Training Dummy (Target Board)");
			}
		}
	}

	if (!bShouldShow || PromptText.IsEmpty())
	{
		return;
	}

	const float PromptW = 340.0f;
	const float PromptH = 34.0f;
	const float PromptX = (ScreenW - PromptW) * 0.5f;
	const float PromptY = ScreenH - 140.0f;

	// Translucent Pill
	DrawAnimePanel(PromptX, PromptY, PromptW, PromptH,
		FLinearColor(0.02f, 0.03f, 0.06f, 0.85f * HUDOpacity),
		FLinearColor(0.95f, 0.75f, 0.20f, 0.85f * HUDOpacity), 2.0f);

	// Key badge
	DrawAnimePanel(PromptX + 8.0f, PromptY + 5.0f, 26.0f, 24.0f,
		FLinearColor(0.95f, 0.75f, 0.20f, HUDOpacity), FLinearColor::White, 1.0f);
	DrawText(TEXT("F"), FLinearColor::Black, PromptX + 17.0f, PromptY + 8.0f);

	// Prompt Action Text
	DrawText(PromptText, FLinearColor::White, PromptX + 42.0f, PromptY + 8.0f);
}

void AEOSShinobiHUD::DrawJutsuBanner(float ScreenW, float ScreenH)
{
	if (BannerTimer <= 0.0f || ActiveBannerText.IsEmpty())
	{
		return;
	}

	const float BannerW = 480.0f;
	const float BannerH = 44.0f;
	const float BannerX = (ScreenW - BannerW) * 0.5f;
	const float BannerY = ScreenH * 0.20f;
	const float BannerAlpha = FMath::Clamp(BannerTimer / 0.5f, 0.0f, 1.0f) * HUDOpacity;

	// Cinematic Calligraphy Banner Panel
	DrawAnimePanel(BannerX, BannerY, BannerW, BannerH,
		FLinearColor(0.04f, 0.02f, 0.03f, 0.90f * BannerAlpha),
		FLinearColor(0.95f, 0.75f, 0.15f, BannerAlpha), 2.0f);

	// Accent Stripes
	DrawRect(FLinearColor(0.95f, 0.35f, 0.12f, BannerAlpha), BannerX, BannerY, BannerW, 2.0f);
	DrawRect(FLinearColor(0.95f, 0.35f, 0.12f, BannerAlpha), BannerX, BannerY + BannerH - 2.0f, BannerW, 2.0f);

	// Bold Banner Text
	FString FormattedText = FString::Printf(TEXT("✦ %s ✦"), *ActiveBannerText);
	DrawText(FormattedText, FLinearColor(1.0f, 0.88f, 0.25f, BannerAlpha), BannerX + 60.0f, BannerY + 12.0f);
}

