// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
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
	IOnlineSessionPtr SessionInterface;

	// Properties
	UPROPERTY(Replicated, BlueprintReadWrite)
	int MaxPlayers;
	UPROPERTY(Replicated, BlueprintReadWrite)
	FText ServerName;
	
	// Managers
	UPROPERTY(Transient)
	class AFLevelManager* LevelManagerInstance;
	UPROPERTY(Transient)
	class AFWidgetManager* WidgetManagerInstance;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFLevelManager> LevelManagerClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFWidgetManager> WidgetManagerClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString PlayerSettingSaveSlot;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bCreateSaveGame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LobbyMapName;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete( FName SessionName, EOnJoinSessionCompleteResult::Type SessionType);
	
	virtual void Init() override;	
public:
	UFUNCTION(BlueprintCallable)
	class AFLevelManager* GetLevelManager();
	UFUNCTION(BlueprintCallable)
	class AFWidgetManager* GetWidgetManager();

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

	UFUNCTION(BlueprintCallable)
	void LaunchLobby(int NumberOfPlayers, bool EnableLan, const FText NewServerName);
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	// UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	// void JoinServer(const FBlueprintSessionResult& SessionToJoin);	
};
