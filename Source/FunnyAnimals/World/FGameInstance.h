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
	// virtual void Init() override;

protected:
	// Managers
	UPROPERTY(Transient)
	class AFLevelManager *LevelManagerInstance;

	// Player Config
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFLevelManager> LevelManagerClass;

public:
	UFUNCTION(BlueprintCallable)
	class AFLevelManager *GetLevelManager();

};
