#include "World/FGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Suppot/Managers/FLevelManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Engine/DemoNetDriver.h"
#include "Suppot/Managers/FWidgetManager.h"

const static FName SESSION_NAME = "SESSION_UE";

UFGameInstance::UFGameInstance()
{
	PlayerSettingSaveSlot = "PlayerSettingSlot";
	bCreateSaveGame = false;
	MaxPlayers = 2;
}

void UFGameInstance::Init()
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFGameInstance::OnJoinSessionsComplete);
		}
	}
}

void UFGameInstance::GameSaveCheck()
{
	bCreateSaveGame = UGameplayStatics::DoesSaveGameExist(PlayerSettingSaveSlot, 0);

	if (bCreateSaveGame)
		ShowMainMenu();
	else
		ShowOptionMenu();
}

// *****************
// Managers
// *****************

class AFLevelManager* UFGameInstance::GetLevelManager()
{
	if (LevelManagerClass && !IsValid(LevelManagerInstance))
		LevelManagerInstance = NewObject<AFLevelManager>(this, LevelManagerClass, TEXT("LevelManager"));

	return IsValid(LevelManagerInstance) ? LevelManagerInstance : LevelManagerInstance = NewObject<AFLevelManager>(this, FName("LevelManager"));
}

AFWidgetManager* UFGameInstance::GetWidgetManager()
{
	if (WidgetManagerClass && !IsValid(WidgetManagerInstance))
		WidgetManagerInstance = NewObject<AFWidgetManager>(this, WidgetManagerClass, TEXT("WidgetManager"));

	return IsValid(WidgetManagerInstance) ? WidgetManagerInstance : WidgetManagerInstance = NewObject<AFWidgetManager>(this, FName("WidgetManager"));
}

// *****************
// UI Widgets
// *****************

void UFGameInstance::ShowOptionMenu() { GetWidgetManager()->ShowOptionMenu(!bCreateSaveGame, GetFirstLocalPlayerController()); }

void UFGameInstance::ShowServerMenu() { GetWidgetManager()->ShowServerMenu(GetFirstLocalPlayerController()); }

void UFGameInstance::ShowHostMenu() { GetWidgetManager()->ShowHostMenu(GetFirstLocalPlayerController()); }

void UFGameInstance::ShowMainMenu() { GetWidgetManager()->ShowMainMenu(GetFirstLocalPlayerController()); }

void UFGameInstance::ShowLoadingScreen() { GetWidgetManager()->ShowLoadingScreen(GetFirstLocalPlayerController()); }

// *****************
// Online Subsystem
// *****************

void UFGameInstance::LaunchLobby(const int NumberOfPlayers, const bool EnableLan, const FText NewServerName)
{
	if (!SessionInterface.IsValid())
		return;

	MaxPlayers = NumberOfPlayers;
	ServerName = NewServerName;
	bEnableLan = EnableLan;

	ShowLoadingScreen();

	if (FNamedOnlineSession* Exist = SessionInterface->GetNamedSession(SESSION_NAME))
		SessionInterface->DestroySession(SESSION_NAME);

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = MaxPlayers;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UFGameInstance::DestroySession() const
{
	if (!SessionInterface.IsValid())
		return;

	SessionInterface->DestroySession(SESSION_NAME);
}

void UFGameInstance::JoinServer(const int SessionToJoinIndex) const
{
	if (!SessionInterface.IsValid() && SessionSearch.IsValid() && SessionSearch->SearchResults.IsValidIndex(SessionToJoinIndex) && SessionSearch->SearchResults[SessionToJoinIndex].IsValid())
		return;

	SessionInterface->JoinSession(0, SESSION_NAME,  SessionSearch->SearchResults[SessionToJoinIndex]);
}

void UFGameInstance::FindSession(const bool EnableLan, const FFoundSessionDelegate& Callback)
{
	if (!SessionInterface.IsValid())
		return;

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 200;
	SessionSearch->bIsLanQuery = true;

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	FoundSessionDelegate = Callback;
}

void UFGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success)
		GetLevelManager()->LoadLobby(GetWorld(), bEnableLan);
	else
		UE_LOG(LogTemp, Warning, TEXT("Cannot create session"));
}

void UFGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("Session with name %s has been destroyed"), *SessionName.ToString());
}

void UFGameInstance::OnFindSessionsComplete(bool Success)
{
	if (SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Find Session complete %d"), SessionSearch->SearchResults.Num());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session search is null"));
	}

	if (Success && SessionInterface.IsValid())
	{
		if (SessionSearch.IsValid() && SessionSearch->SearchResults.Num() > 0)
		{
			for (int i = 0; i < SessionSearch->SearchResults.Num(); ++i)
			{
				if (MaxPlayers != SessionSearch->SearchResults[i].Session.NumOpenPublicConnections)
				{
					int MaxConnection = SessionSearch->SearchResults[i].Session.SessionSettings.NumPublicConnections;
					FoundSessionDelegate.ExecuteIfBound(true, i);
					return;
				}
			}

			FoundSessionDelegate.ExecuteIfBound(false, -1);
		}
		else
		{
			FoundSessionDelegate.ExecuteIfBound(false, -1);
		}
	}
	else
	{
		FoundSessionDelegate.ExecuteIfBound(false, -1);
	}
}

void UFGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type SessionType)
{
	UE_LOG(LogTemp, Warning, TEXT("Join in session completed"));
}

// *****************
// Replication
// *****************

void UFGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFGameInstance, MaxPlayers);
	DOREPLIFETIME(UFGameInstance, ServerName);
}
