#include "World/FGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"
#include "Suppot/Managers/FLevelManager.h"
#include "GameFramework/PlayerController.h"
#include "UI/FOptionMenuWidget.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = FName("SESSION_FA");

UFGameInstance::UFGameInstance()
{
	PlayerSettingSaveSlot = "PlayerSettingSlot";
	bCreateSaveGame = false;
	LobbyMapName = "Lobby";
}

void UFGameInstance::Init()
{
	Subsystem = IOnlineSubsystem::Get();
	SessionInterface = Subsystem->GetSessionInterface();

	if (SessionInterface)
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFGameInstance::HandleOnCreateSession);
}

class AFLevelManager* UFGameInstance::GetLevelManager()
{
	if (LevelManagerClass && !IsValid(LevelManagerInstance))
		LevelManagerInstance = NewObject<AFLevelManager>(this, LevelManagerClass, TEXT("LevelManager"));

	return IsValid(LevelManagerInstance) ? LevelManagerInstance : LevelManagerInstance = NewObject<AFLevelManager>(this, FName("LevelManager"));
}

void UFGameInstance::ChangeWidget(UUserWidget* WidgetWp, const TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass)
		return;

	APlayerController* PC = GetPrimaryPlayerController();

	if (!IsValid(WidgetWp))
		WidgetWp = CreateWidget<UUserWidget>(this, WidgetClass);

	WidgetWp->AddToViewport();
	PC->SetShowMouseCursor(true);
}

void UFGameInstance::GameSaveCheck()
{
	const bool bExist = UGameplayStatics::DoesSaveGameExist(PlayerSettingSaveSlot, 0);

	if (bExist)
		ShowMainMenu();
	else
		ShowOptionMenu();

	bCreateSaveGame = bExist;
}

void UFGameInstance::ShowMainMenu()
{
	ChangeWidget(MainMenuWP, MainMenuClass);
}

void UFGameInstance::ShowOptionMenu()
{
	ChangeWidget(OptionMenuWP, OptionMenuClass);
	if (!bCreateSaveGame)
		OptionMenuWP->WelcomeMessageVis = ESlateVisibility::Visible;
}

void UFGameInstance::ShowHostMenu()
{
	ChangeWidget(HostMenuWP, HostMenuClass);
}

void UFGameInstance::ShowServerMenu()
{
	if (!ServerMenuClass)
		return;

	ServerMenuWP = CreateWidget<UUserWidget>(this, ServerMenuClass);
	ServerMenuWP->AddToViewport();
}

void UFGameInstance::ShowLoadingScreen()
{
	ChangeWidget(LoadingWP, LoadingScreenClass);
}

void UFGameInstance::LaunchLobby(const int NumberOfPlayers, const bool EnableLan, const FText NewServerName)
{
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

void UFGameInstance::HandleOnCreateSession(FName SessionName, bool Success)
{
	if (Success)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("%s: sesion %s was created"), *Subsystem->GetSubsystemName().ToString(), *SessionName.ToString());
#endif

		UGameplayStatics::OpenLevel(this, LobbyMapName, true, FString("listen"));
	}
	else
	{
		//TODO: do something when session failed

#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("Cannot create session"));
#endif
	}
}

void UFGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFGameInstance, MaxPlayers);
	DOREPLIFETIME(UFGameInstance, ServerName);
}
