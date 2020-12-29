#include "World/FGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Suppot/Managers/FLevelManager.h"
#include "GameFramework/PlayerController.h"
#include "UI/FOptionMenuWidget.h"

UFGameInstance::UFGameInstance()
{
	PlayerSettingSaveSlot = "PlayerSettingSlot";
	bCreateSaveGame = false;
}

class AFLevelManager* UFGameInstance::GetLevelManager()
{
	if (LevelManagerClass && !IsValid(LevelManagerInstance))
		LevelManagerInstance = NewObject<AFLevelManager>(this, LevelManagerClass, TEXT("LevelManager"));

	return IsValid(LevelManagerInstance) ? LevelManagerInstance : LevelManagerInstance = NewObject<AFLevelManager>(this, FName("LevelManager"));
}


void  UFGameInstance::ChangeWidget(UUserWidget* WidgetWp, const TSubclassOf<UUserWidget> WidgetClass)
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
	if (!OptionMenuClass)
		return;

	APlayerController* PC = GetPrimaryPlayerController();

	if (!IsValid(OptionMenuWP))
		OptionMenuWP = CreateWidget<UFOptionMenuWidget>(this, OptionMenuClass);

	OptionMenuWP->AddToViewport();
	PC->SetShowMouseCursor(true);

	if (!bCreateSaveGame)
		OptionMenuWP->WelcomeMessageVis = ESlateVisibility::Visible;
}

void UFGameInstance::ShowHostMenu()
{
	if (!HostMenuClass)
		return;

	if (!IsValid(HostMenuWP))
		HostMenuWP = CreateWidget<UUserWidget>(this, HostMenuClass);

	HostMenuWP->AddToViewport();
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
	if (!LoadingScreenClass)
		return;

	if (!IsValid(LoadingWP))
		LoadingWP = CreateWidget<UUserWidget>(this, LoadingScreenClass);

	LoadingWP->AddToViewport();
}
