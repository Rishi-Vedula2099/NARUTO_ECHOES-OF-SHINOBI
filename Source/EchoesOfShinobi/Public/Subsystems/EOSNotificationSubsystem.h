#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Data/EOSUIDataTypes.h"
#include "EOSNotificationSubsystem.generated.h"

class UEOSUISubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEOSNotificationReceivedDelegate, const FEOSNotificationItem&, Notification);

/**
 * GameInstanceSubsystem managing System Badges, Alerts, and Notification Deep-Linking.
 */
UCLASS()
class ECHOESOFSHINOBI_API UEOSNotificationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	typedef UGameInstanceSubsystem Super;
	UEOSNotificationSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Notification Dispatch
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Notifications")
	void PostNotification(const FEOSNotificationItem& Notification);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Notifications")
	bool MarkNotificationRead(const FString& NotificationID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Notifications")
	bool DismissNotification(const FString& NotificationID);

	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Notifications")
	void ClearAllNotifications();

	// Queries & Badge Checks
	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Notifications")
	int32 GetUnreadNotificationCount() const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Notifications")
	bool HasUnreadNotificationsForSystem(const FString& SourceSystem) const;

	UFUNCTION(BlueprintPure, Category = "EchoesOfShinobi|Notifications")
	TArray<FEOSNotificationItem> GetActiveNotifications() const { return Notifications; }

	// Deep-Link Navigation
	UFUNCTION(BlueprintCallable, Category = "EchoesOfShinobi|Notifications")
	bool ExecuteNotificationDeepLink(const FString& NotificationID, UEOSUISubsystem* UISubsystem);

public:
	UPROPERTY(BlueprintAssignable, Category = "EchoesOfShinobi|Notifications")
	FEOSNotificationReceivedDelegate OnNotificationReceived;

private:
	UPROPERTY()
	TArray<FEOSNotificationItem> Notifications;
};
