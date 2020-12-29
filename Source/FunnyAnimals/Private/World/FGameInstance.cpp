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
	if (!MainMenuClass)
		return;
	
	APlayerController * PC = GetPrimaryPlayerController();

	if(!IsValid(MainMenuWP))
		MainMenuWP = CreateWidget<UUserWidget>(this, MainMenuClass);

	MainMenuWP->AddToViewport();
	PC->SetShowMouseCursor(true);
}

void UFGameInstance::ShowOptionMenu()
{
	if (!OptionMenuClass)
		return;
	
	APlayerController * PC = GetPrimaryPlayerController();

	if(!IsValid(OptionMenuWP))
		OptionMenuWP = CreateWidget<UFOptionMenuWidget>(this, OptionMenuClass);

	OptionMenuWP->AddToViewport();
	PC->SetShowMouseCursor(true);

	if(!bCreateSaveGame)
		OptionMenuWP->WelcomeMessageVis = ESlateVisibility::Visible;	
}
