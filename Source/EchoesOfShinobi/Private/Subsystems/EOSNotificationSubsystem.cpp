#include "../../Public/Subsystems/EOSNotificationSubsystem.h"
#include "../../Public/Subsystems/EOSUISubsystem.h"
#include "../../EchoesOfShinobi.h"

UEOSNotificationSubsystem::UEOSNotificationSubsystem()
{
}

void UEOSNotificationSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogEOSUI, Log, TEXT("UEOSNotificationSubsystem Initialized. Notification & Badge Routing Ready."));
}

void UEOSNotificationSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UEOSNotificationSubsystem::PostNotification(const FEOSNotificationItem& Notification)
{
	Notifications.Add(Notification);
	if (OnNotificationReceived.IsBound())
	{
		OnNotificationReceived.Broadcast(Notification);
	}
	UE_LOG(LogEOSUI, Log, TEXT("[Notification] Posted: %s from %s (Target: %s)"),
		*Notification.NotificationID, *Notification.SourceSystem, *Notification.DeepLinkTargetScreenID);
}

bool UEOSNotificationSubsystem::MarkNotificationRead(const FString& NotificationID)
{
	for (FEOSNotificationItem& Item : Notifications)
	{
		if (Item.NotificationID == NotificationID)
		{
			Item.bIsRead = true;
			return true;
		}
	}
	return false;
}

bool UEOSNotificationSubsystem::DismissNotification(const FString& NotificationID)
{
	for (int32 i = 0; i < Notifications.Num(); ++i)
	{
		if (Notifications[i].NotificationID == NotificationID)
		{
			Notifications.RemoveAt(i);
			return true;
		}
	}
	return false;
}

void UEOSNotificationSubsystem::ClearAllNotifications()
{
	Notifications.Empty();
}

int32 UEOSNotificationSubsystem::GetUnreadNotificationCount() const
{
	int32 Count = 0;
	for (const FEOSNotificationItem& Item : Notifications)
	{
		if (!Item.bIsRead)
		{
			Count++;
		}
	}
	return Count;
}

bool UEOSNotificationSubsystem::HasUnreadNotificationsForSystem(const FString& SourceSystem) const
{
	for (const FEOSNotificationItem& Item : Notifications)
	{
		if (!Item.bIsRead && Item.SourceSystem == SourceSystem)
		{
			return true;
		}
	}
	return false;
}

bool UEOSNotificationSubsystem::ExecuteNotificationDeepLink(const FString& NotificationID, UEOSUISubsystem* UISubsystem)
{
	if (!UISubsystem)
	{
		return false;
	}

	for (FEOSNotificationItem& Item : Notifications)
	{
		if (Item.NotificationID == NotificationID)
		{
			Item.bIsRead = true;
			if (!Item.DeepLinkTargetScreenID.IsEmpty())
			{
				return UISubsystem->PushScreen(Item.DeepLinkTargetScreenID, Item.DeepLinkContextID);
			}
			return true;
		}
	}
	return false;
}
