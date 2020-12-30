// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "FWidgetManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FUNNYANIMALS_API AFWidgetManager : public AInfo
{
	GENERATED_BODY()

protected:
	class UUserWidget* MainMenuWP;
	class UFOptionMenuWidget* OptionMenuWP;
	class UUserWidget* HostMenuWP;
	class UUserWidget* ServerMenuWP;
	class UUserWidget* LoadingWP;

	// Widgets
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Widgets)
	TSubclassOf<class UUserWidget> MainMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Widgets)
	TSubclassOf<class UUserWidget> OptionMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Widgets)
	TSubclassOf<class UUserWidget> HostMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Widgets)
	TSubclassOf<class UUserWidget> ServerMenuClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Widgets)
	TSubclassOf<class UUserWidget> LoadingScreenClass;

	void ChangeWidget(class UUserWidget* WidgetWp, const TSubclassOf<class UUserWidget> WidgetClass, class APlayerController* PC) const;

public:

	UFUNCTION(BlueprintCallable)
	void ShowMainMenu(class APlayerController* PC) const;
	UFUNCTION(BlueprintCallable)
	void ShowOptionMenu(const bool ShowWelcome,class APlayerController* PC) const;
	UFUNCTION(BlueprintCallable)
	void ShowHostMenu(class APlayerController* PC) const;
	UFUNCTION(BlueprintCallable)
	void ShowServerMenu(class APlayerController* PC);
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen(class APlayerController* PC) const;
};
