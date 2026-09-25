#include "../../Public/UI/EOSShinobiHUD.h"
#include "../../Public/Character/EOSCharacterBase.h"
#include "Engine/Canvas.h"
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

void AEOSShinobiHUD::RegisterHit(int32 HitCount, float HitScore)
{
	CurrentComboHits += HitCount;
	CurrentComboScore += HitScore;
	ComboDecayTimer = 3.0f; // 3 seconds window
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

	// 1. Top-Center Boss Bar (Kakashi Sharingan)
	DrawTargetBossBar(ScreenW, ScreenH);

	// 2. Top-Left Player Status Card (Curved HP & Chakra)
	DrawHealthAndChakraBars(ScreenW, ScreenH, Shinobi);

	// 3. Top-Right Data-Driven Stylized Minimap
	DrawDataDrivenMinimap(ScreenW, ScreenH, Shinobi);

	// 4. Bottom-Right Diamond Jutsu Cluster
	DrawJutsuAndUltimateSlots(ScreenW, ScreenH, Shinobi);

	// 5. Bottom-Left Team 7 Switcher Cards
	DrawSquadSelectionSlots(ScreenW, ScreenH, Shinobi);

	// 6. Center-Right Japanese Calligraphy Combo Counter
	DrawComboDisplay(ScreenW, ScreenH);
}

void AEOSShinobiHUD::DrawTargetBossBar(float ScreenW, float ScreenH)
{
	if (!bHasActiveTarget)
	{
		return;
	}

	const float BarW = 460.0f;
	const float BarH = 16.0f;
	const float BarX = (ScreenW - BarW) * 0.5f;
	const float BarY = 28.0f;

	// Dark Border / Shadow with gold accent
	DrawRect(FLinearColor(0.02f, 0.02f, 0.04f, 0.90f * HUDOpacity), BarX - 6.0f, BarY - 22.0f, BarW + 12.0f, BarH + 30.0f);
	DrawRect(FLinearColor(0.85f, 0.65f, 0.15f, 0.8f * HUDOpacity), BarX - 6.0f, BarY - 22.0f, BarW + 12.0f, 2.0f);

	// Boss Name & Phase Indicator
	DrawText(FString::Printf(TEXT("BOSS: %s  [PHASE 1/2]"), *ActiveTargetName), FLinearColor(1.0f, 0.85f, 0.3f, HUDOpacity), BarX + 4.0f, BarY - 18.0f);

	// Background Slot
	DrawRect(FLinearColor(0.18f, 0.04f, 0.04f, 0.95f * HUDOpacity), BarX, BarY, BarW, BarH);

	// Filled Bar with gradient tone
	const float FillW = BarW * FMath::Clamp(ActiveTargetHealthPercent, 0.0f, 1.0f);
	DrawRect(FLinearColor(0.92f, 0.16f, 0.16f, HUDOpacity), BarX, BarY, FillW, BarH);
	DrawRect(FLinearColor(1.0f, 0.45f, 0.2f, 0.6f * HUDOpacity), BarX, BarY, FillW, BarH * 0.4f);
}

void AEOSShinobiHUD::DrawHealthAndChakraBars(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	// Top-Left Player Status Card
	const float BaseX = 35.0f;
	const float BaseY = 30.0f;
	const float BarW = 280.0f;
	const float BarH = 18.0f;

	float Health = 1000.0f;
	float MaxHealth = 1000.0f;
	float Chakra = 500.0f;
	float MaxChakra = 500.0f;
	FString ActiveForm = TEXT("BASE");

	if (Shinobi)
	{
		if (UEOSAttributeSet* Attr = Shinobi->GetAttributeSet())
		{
			Health = Attr->GetHealth();
			MaxHealth = Attr->GetMaxHealth();
			Chakra = Attr->GetChakra();
			MaxChakra = Attr->GetMaxChakra();
		}
		ActiveForm = Shinobi->GetForms().ActiveFormID;
	}

	// Panel Frame
	DrawRect(FLinearColor(0.04f, 0.04f, 0.07f, 0.88f * HUDOpacity), BaseX - 10.0f, BaseY - 10.0f, BarW + 70.0f, 105.0f);

	// Avatar Circle & Badge
	DrawRect(FLinearColor(0.95f, 0.55f, 0.1f, HUDOpacity), BaseX, BaseY, 32.0f, 32.0f);
	DrawText(TEXT("NARUTO [Lv. 12 Genin]"), FLinearColor(1.0f, 0.95f, 0.8f, HUDOpacity), BaseX + 40.0f, BaseY - 2.0f);
	DrawText(FString::Printf(TEXT("Form: %s"), *ActiveForm), FLinearColor(0.7f, 0.85f, 1.0f, HUDOpacity), BaseX + 40.0f, BaseY + 14.0f);

	// Health Bar (Curved Red)
	const float HP_Y = BaseY + 38.0f;
	DrawRect(FLinearColor(0.25f, 0.04f, 0.04f, 0.95f * HUDOpacity), BaseX, HP_Y, BarW, BarH);
	const float HealthPercent = MaxHealth > 0.0f ? FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) : 0.0f;
	DrawRect(FLinearColor(0.88f, 0.15f, 0.15f, HUDOpacity), BaseX, HP_Y, BarW * HealthPercent, BarH);
	DrawRect(FLinearColor(1.0f, 0.35f, 0.35f, 0.5f * HUDOpacity), BaseX, HP_Y, BarW * HealthPercent, BarH * 0.35f);
	DrawText(FString::Printf(TEXT("HP  %d / %d"), FMath::RoundToInt(Health), FMath::RoundToInt(MaxHealth)), FLinearColor::White, BaseX + 8.0f, HP_Y + 2.0f);

	// Chakra Gauge (Curved Cyan)
	const float CK_Y = HP_Y + BarH + 6.0f;
	DrawRect(FLinearColor(0.04f, 0.12f, 0.28f, 0.95f * HUDOpacity), BaseX, CK_Y, BarW, BarH);
	const float ChakraPercent = MaxChakra > 0.0f ? FMath::Clamp(Chakra / MaxChakra, 0.0f, 1.0f) : 0.0f;
	DrawRect(FLinearColor(0.10f, 0.65f, 0.98f, HUDOpacity), BaseX, CK_Y, BarW * ChakraPercent, BarH);
	DrawRect(FLinearColor(0.45f, 0.88f, 1.0f, 0.5f * HUDOpacity), BaseX, CK_Y, BarW * ChakraPercent, BarH * 0.35f);
	DrawText(FString::Printf(TEXT("CHAKRA  %d / %d"), FMath::RoundToInt(Chakra), FMath::RoundToInt(MaxChakra)), FLinearColor::White, BaseX + 8.0f, CK_Y + 2.0f);
}

void AEOSShinobiHUD::DrawDataDrivenMinimap(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	// Top-Right Minimap
	const float Radius = 65.0f;
	const float CenterX = ScreenW - Radius - 35.0f;
	const float CenterY = Radius + 25.0f;

	// Minimap Disc Background
	DrawRect(FLinearColor(0.03f, 0.04f, 0.08f, 0.90f * HUDOpacity), CenterX - Radius, CenterY - Radius, Radius * 2.0f, Radius * 2.0f);
	DrawRect(FLinearColor(0.75f, 0.65f, 0.25f, 0.70f * HUDOpacity), CenterX - Radius - 2.0f, CenterY - Radius - 2.0f, (Radius * 2.0f) + 4.0f, 2.0f);

	// Compass Card Label
	DrawText(TEXT("KONOHA DISTRICT"), FLinearColor(1.0f, 0.9f, 0.4f, HUDOpacity), CenterX - 55.0f, CenterY - Radius - 18.0f);

	// Center Player Arrow
	DrawRect(FLinearColor(0.1f, 0.85f, 1.0f, HUDOpacity), CenterX - 4.0f, CenterY - 4.0f, 8.0f, 8.0f);

	// Gold Mission Marker (relative to player pos)
	float MarkerAngle = 0.0f;
	if (Shinobi)
	{
		const FVector PlayerPos = Shinobi->GetActorLocation();
		const FVector TargetMissionPos = FVector(2800.0f, 0.0f, 90.0f); // Training Grounds Arena
		const FVector Delta = TargetMissionPos - PlayerPos;
		MarkerAngle = FMath::Atan2(Delta.Y, Delta.X);
	}
	const float MarkerX = CenterX + FMath::Cos(MarkerAngle) * (Radius * 0.65f);
	const float MarkerY = CenterY + FMath::Sin(MarkerAngle) * (Radius * 0.65f);
	DrawRect(FLinearColor(1.0f, 0.82f, 0.05f, HUDOpacity), MarkerX - 5.0f, MarkerY - 5.0f, 10.0f, 10.0f);
	DrawText(TEXT("MISSION"), FLinearColor(1.0f, 0.85f, 0.2f, HUDOpacity), CenterX - 25.0f, CenterY + Radius + 4.0f);
}

void AEOSShinobiHUD::DrawJutsuAndUltimateSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	// Bottom-Right Diamond Jutsu Cluster
	const float ClusterW = 340.0f;
	const float StartX = ScreenW - ClusterW - 30.0f;
	const float StartY = ScreenH - 95.0f;
	const float SlotSize = 52.0f;
	const float Spacing = 12.0f;

	const TCHAR* JutsuTitles[] = { TEXT("[Q]\nRasengan"), TEXT("[E]\nShadowClone"), TEXT("[R]\nBarrage"), TEXT("[C]\nSubstitution"), TEXT("[G]\nAwakening") };
	const FLinearColor SlotColors[] = {
		FLinearColor(0.12f, 0.60f, 0.95f, 0.92f), // Azure Rasengan
		FLinearColor(0.20f, 0.80f, 0.65f, 0.92f), // Shadow clone
		FLinearColor(0.75f, 0.35f, 0.90f, 0.92f), // Uzumaki barrage
		FLinearColor(0.40f, 0.85f, 0.25f, 0.92f), // Substitution log
		FLinearColor(0.95f, 0.25f, 0.15f, 0.92f)  // Vermilion Awakening
	};

	for (int32 i = 0; i < 5; ++i)
	{
		const float X = StartX + (i * (SlotSize + Spacing));
		DrawRect(FLinearColor(0.04f, 0.04f, 0.08f, 0.90f * HUDOpacity), X, StartY, SlotSize, SlotSize);
		DrawRect(SlotColors[i] * HUDOpacity, X + 3.0f, StartY + 3.0f, SlotSize - 6.0f, SlotSize - 6.0f);
		DrawText(JutsuTitles[i], FLinearColor::White, X + 5.0f, StartY + 8.0f);
	}
}

void AEOSShinobiHUD::DrawSquadSelectionSlots(float ScreenW, float ScreenH, AEOSCharacterBase* Shinobi)
{
	// Bottom-Left Team 7 Switcher Cards
	const float StartX = 35.0f;
	const float StartY = ScreenH - 45.0f;
	const float SlotW = 105.0f;
	const float SlotH = 24.0f;
	const float Spacing = 10.0f;

	const TCHAR* SquadNames[] = { TEXT("[1] NARUTO"), TEXT("[2] SASUKE"), TEXT("[3] SAKURA") };
	const FLinearColor TeamColors[] = {
		FLinearColor(1.0f, 0.55f, 0.1f, 1.0f),
		FLinearColor(0.2f, 0.45f, 0.95f, 0.8f),
		FLinearColor(0.95f, 0.4f, 0.6f, 0.8f)
	};

	for (int32 i = 0; i < 3; ++i)
	{
		const float X = StartX + (i * (SlotW + Spacing));
		DrawRect(FLinearColor(0.06f, 0.06f, 0.10f, 0.88f * HUDOpacity), X, StartY, SlotW, SlotH);
		DrawRect(TeamColors[i] * HUDOpacity, X, StartY, 4.0f, SlotH);
		DrawText(SquadNames[i], TeamColors[i], X + 8.0f, StartY + 5.0f);
	}
}

void AEOSShinobiHUD::DrawComboDisplay(float ScreenW, float ScreenH)
{
	if (CurrentComboHits <= 0)
	{
		return;
	}

	const float X = ScreenW - 220.0f;
	const float Y = ScreenH * 0.42f;

	DrawRect(FLinearColor(0.05f, 0.02f, 0.02f, 0.80f * HUDOpacity), X - 12.0f, Y - 12.0f, 200.0f, 75.0f);
	DrawRect(FLinearColor(0.95f, 0.4f, 0.1f, 0.85f * HUDOpacity), X - 12.0f, Y - 12.0f, 4.0f, 75.0f);
	DrawText(FString::Printf(TEXT("%d HITS! COMBO!"), CurrentComboHits), FLinearColor(1.0f, 0.35f, 0.1f, HUDOpacity), X, Y);
	DrawText(FString::Printf(TEXT("SCORE  %.0f"), CurrentComboScore), FLinearColor(1.0f, 0.92f, 0.4f, HUDOpacity), X, Y + 26.0f);
}
