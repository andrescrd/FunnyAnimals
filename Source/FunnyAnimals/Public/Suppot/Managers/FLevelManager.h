// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "Types/FTypes.h"
#include "FLevelManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FUNNYANIMALS_API AFLevelManager : public AInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FLevelConfig CurrentGameplayLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LobbyMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MainMenuMapName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelConfig> GameplayLevels;

public:
	UFUNCTION(BlueprintCallable)
	void SetGameplayLevels(const TArray<FLevelConfig> NewLevels);
	UFUNCTION(BlueprintCallable)
	TArray<FLevelConfig> GetGameplayLevels() const;
	UFUNCTION(BlueprintCallable)
	FLevelConfig GetCurrentGameplayLevel() const;
	UFUNCTION(BlueprintCallable)
	FLevelConfig GetGameplayLevel(const int Index);
	UFUNCTION(BlueprintCallable)
	void LoadGameplayLevel(class UObject *Context,const FName LevelNameToLoad);
	
	UFUNCTION(BlueprintCallable)
	void LoadLobby(class UObject *Context, const bool EnableLan) const;
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu(class UObject *Context) const;
};
