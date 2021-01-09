// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "FGameModeBase.h"
#include "FGameModeSurvive.generated.h"

UCLASS()
class FUNNYANIMALS_API AFGameModeSurvive : public AFGameModeBase
{
	GENERATED_BODY()

public:
	AFGameModeSurvive();

protected:
	FTimerHandle Counter_TimerHandle;
	TArray<AActor *> Spawners;
	TArray<AActor *> Launchers;

	void StartCounter();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// State Game
	void UpdateObjectiveActors(const int DeltaActors);

	virtual void Preparing() override;
	virtual void Playing() override;
	virtual void Complete() override;
	virtual void Finish() override;
};
