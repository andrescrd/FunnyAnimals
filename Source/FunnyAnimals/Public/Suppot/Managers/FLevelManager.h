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
	FLevelConfig CurrentLevel;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FLevelConfig> Levels;

public:
	UFUNCTION(BlueprintCallable)
	void LoadLevel(class UObject *context, FName LevelNameToLoad);
	UFUNCTION(BlueprintCallable)
	void SetLevels(TArray<FLevelConfig> NewLevels);
	UFUNCTION(BlueprintCallable)
	TArray<FLevelConfig> GetLevels() const;
	UFUNCTION(BlueprintCallable)
	FLevelConfig GetCurrentLevel() const;
	UFUNCTION(BlueprintCallable)
	FLevelConfig GetLevel(int Index);
};
