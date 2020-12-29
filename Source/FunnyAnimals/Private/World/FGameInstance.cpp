#include "World/FGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"
#include "Suppot/Managers/FLevelManager.h"
#include "GameFramework/PlayerController.h"
#include "UI/FOptionMenuWidget.h"
// #include "FindSessionsCallbackProxy.h"
// #include "CreateSessionCallbackProxy.h"


const static FName SESSION_NAME = "SESSION_UE";

UFGameInstance::UFGameInstance()
{
	PlayerSettingSaveSlot = "PlayerSettingSlot";
	bCreateSaveGame = false;
	LobbyMapName = "Lobby";
}

void UFGameInstance::Init()
{
	// if(IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	// {
	// 	SessionInterface = Subsystem->GetSessionInterface();
	//
	// 	if (SessionInterface.IsValid())
	// 		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFGameInstance::HandleOnCreateSession);
	// }
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

	if (PC)
		PC->SetShowMouseCursor(true);
}

void UFGameInstance::GameSaveCheck()
{
	bCreateSaveGame = UGameplayStatics::DoesSaveGameExist(PlayerSettingSaveSlot, 0);

	if (bCreateSaveGame)
		ShowMainMenu();
	else
		ShowOptionMenu();
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

// void UFGameInstance::LaunchLobby(const int NumberOfPlayers, const bool EnableLan, const FText& NewServerName)
// {
// 	MaxPlayers = NumberOfPlayers;
// 	ServerName = NewServerName;
//
// 	ShowLoadingScreen();
// 	//
// 	// if (FNamedOnlineSession* Exist = SessionInterface->GetNamedSession(SESSION_NAME))
// 	// 	SessionInterface->DestroySession(SESSION_NAME);
//
// 	const auto SessionSettings = MakeShareable(new FOnlineSessionSettings());	
// 	SessionSettings.Object->bIsLANMatch = EnableLan;	
// 	 SessionSettings.Object->NumPublicConnections = MaxPlayers;
//
// 	SessionInterface->CreateSession(0, SESSION_NAME, *SessionSettings.Object);
// }
//
// void UFGameInstance::HandleOnCreateSession(FName SessionName, bool Success)
// {
// 	if (Success)
// 	{
// 		UGameplayStatics::OpenLevel(GetWorld(),LobbyMapName,true, FString("listen"));
// 	}	
// 	else
// 	{
// 		//TODO: do something when session failed
//
// #if WITH_EDITOR
// 		UE_LOG(LogTemp, Warning, TEXT("Cannot create session"));
// #endif
// 	}
// }

void UFGameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFGameInstance, MaxPlayers);
	DOREPLIFETIME(UFGameInstance, ServerName);
}
