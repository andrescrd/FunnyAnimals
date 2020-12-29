// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
// #include "OnlineSubsystem.h"
// #include "FindSessionsCallbackProxy.h"
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
	// IOnlineSessionPtr SessionInterface;

	class UUserWidget* MainMenuWP;
	class UFOptionMenuWidget* OptionMenuWP;
	class UUserWidget* HostMenuWP;
	class UUserWidget* ServerMenuWP;
	class UUserWidget* LoadingWP;

	// Properties
	UPROPERTY(Replicated, BlueprintReadWrite)
	int MaxPlayers;
	UPROPERTY(Replicated, BlueprintReadWrite)
	FText ServerName;

	// Managers
	UPROPERTY(Transient)
	class AFLevelManager* LevelManagerInstance;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFLevelManager> LevelManagerClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString PlayerSettingSaveSlot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bCreateSaveGame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LobbyMapName;

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

	void ChangeWidget(class UUserWidget* WidgetWp, const TSubclassOf<class UUserWidget> WidgetClass);
	// void HandleOnCreateSession(FName SessionName, bool Success);

	virtual void Init() override;
	
public:
	UFUNCTION(BlueprintCallable)
	class AFLevelManager* GetLevelManager();

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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LaunchLobby(int NumberOfPlayers, bool EnableLan, const FText& NewServerName);
	//  UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	// void JoinServer(const FBlueprintSessionResult& SessionToJoin);
	
};
