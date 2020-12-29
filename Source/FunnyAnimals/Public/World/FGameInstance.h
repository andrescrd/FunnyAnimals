// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API UFGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFGameInstance();

protected:
	class UUserWidget* MainMenuWP;
	class UFOptionMenuWidget* OptionMenuWP;
	class UUserWidget* HostMenuWP;
	class UUserWidget* ServerMenuWP;
	class UUserWidget* LoadingWP;
	
	// Managers
	UPROPERTY(Transient)
	class AFLevelManager *LevelManagerInstance;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFLevelManager> LevelManagerClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString PlayerSettingSaveSlot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool bCreateSaveGame;

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

	void ChangeWidget(class UUserWidget* WidgetWp,const TSubclassOf<class UUserWidget> WidgetClass);
	
public:
	UFUNCTION(BlueprintCallable)
	class AFLevelManager *GetLevelManager();

	UFUNCTION(BlueprintCallable)
	void GameSaveCheck();
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();
	UFUNCTION(BlueprintCallable)
	void ShowOptionMenu();
	UFUNCTION(BlueprintCallable)
	void ShowHostMenu();
	UFUNCTION(BlueprintCallable)
	void ShowServerMenu();
	UFUNCTION(BlueprintCallable)
	void ShowLoadingScreen();
};
