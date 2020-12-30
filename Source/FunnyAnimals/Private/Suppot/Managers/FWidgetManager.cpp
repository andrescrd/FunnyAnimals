// Fill out your copyright notice in the Description page of Project Settings.


#include "Suppot/Managers/FWidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "UI/FOptionMenuWidget.h"

void AFWidgetManager::ChangeWidget(UUserWidget* WidgetWp, const TSubclassOf<UUserWidget> WidgetClass, APlayerController* PC) const
{
	if (!WidgetClass)
		return;
	
	if (!IsValid(WidgetWp))
		WidgetWp = CreateWidget<UUserWidget>(PC->GetWorld(), WidgetClass);

	WidgetWp->AddToViewport();

	if (PC)
		PC->SetShowMouseCursor(true);
}

void AFWidgetManager::ShowOptionMenu(const bool ShowWelcome, APlayerController* PC) const
{
	ChangeWidget(OptionMenuWP, OptionMenuClass, PC);
	if (ShowWelcome)
		OptionMenuWP->WelcomeMessageVis = ESlateVisibility::Visible;
}

void AFWidgetManager::ShowServerMenu( APlayerController* PC)
{
	if (!ServerMenuClass)
		return;

	ServerMenuWP = CreateWidget<UUserWidget>(PC->GetWorld(), ServerMenuClass);
	ServerMenuWP->AddToViewport();
}

void AFWidgetManager::ShowHostMenu(APlayerController* PC) const { ChangeWidget(HostMenuWP, HostMenuClass,PC); }

void AFWidgetManager::ShowMainMenu(APlayerController* PC) const { ChangeWidget(MainMenuWP, MainMenuClass, PC); }

void AFWidgetManager::ShowLoadingScreen(APlayerController* PC) const { ChangeWidget(LoadingWP, LoadingScreenClass,PC); }

