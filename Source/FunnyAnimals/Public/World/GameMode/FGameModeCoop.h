// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGameModeBase.h"
#include "GameFramework/GameModeBase.h"
#include "FGameModeCoop.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFGameModeCoop : public AFGameModeBase
{
	GENERATED_BODY()

public:
	AFGameModeCoop();

protected:
	FTimerHandle Counter_TimerHandle;
	TArray<AActor*> Spawners;
	TArray<AActor*> Launchers;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void StartCounter();
	void SpawnLaunchers();
	void SpawnWorms();
public:
	virtual void Preparing() override;
	virtual void Playing() override;
	virtual void Complete() override;
	virtual void Finish() override;
};
