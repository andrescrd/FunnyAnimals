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

	ShowLoadingScreen();

	if (FNamedOnlineSession* Exist = SessionInterface->GetNamedSession(SESSION_NAME))
	 	SessionInterface->DestroySession(SESSION_NAME);
	
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = EnableLan;
	SessionSettings.NumPublicConnections = MaxPlayers;

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UFGameInstance::DestroySession() const
{
	if (!SessionInterface.IsValid())
		return;

	SessionInterface->DestroySession(SESSION_NAME);
}

void UFGameInstance::JoinServer(const FSessionResult DesiredSession) const
{
	if (!SessionInterface.IsValid() && DesiredSession.Result.IsValid())
		return;

	SessionInterface->JoinSession(0, SESSION_NAME, DesiredSession.Result);
}

void UFGameInstance::FindSession(const bool EnableLan)
{

	if (!SessionInterface.IsValid())
		return;

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->bIsLanQuery = EnableLan;
	SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());	
}

void UFGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (Success)
		GetLevelManager()->LoadLobby(GetWorld());
	else
		UE_LOG(LogTemp, Warning, TEXT("Cannot create session"));
}

void UFGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("Session with name %s has been destroyed"), *SessionName.ToString());
}

void UFGameInstance::OnFindSessionsComplete(bool Success)
{
	if(Success && SessionInterface.IsValid())
	{
		FSessionResult SessionResult;
		
		if(SessionSearch->SearchResults.Num() > 0)
		{
			for (int i = 0; i < SessionSearch->SearchResults.Num(); ++i)
			{
				if(MaxPlayers != SessionSearch->SearchResults[i].Session.NumOpenPublicConnections)
				{
					SessionResult.Result = SessionSearch->SearchResults[i];
					break;
				}					
			}
		}
		else
		{
			
		}		
	}else
	{
		
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
