// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/FTypes.h"
#include "FGameMode.generated.h"

UCLASS()
class FUNNYANIMALS_API AFGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFGameMode();

protected:
	FTimerHandle Counter_TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentGameState;	

	void StartCounter();
	void HandleGameState(const EGameState NewGameState);

public:
	virtual void BeginPlay() override;	

	// State Game
	EGameState GetCurrentGameState() const;
	void SetGameState(const EGameState NewGameState);

	void Preparing();
	void Playing();
	void Complete();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStateChange(EGameState NewGameState);
};
