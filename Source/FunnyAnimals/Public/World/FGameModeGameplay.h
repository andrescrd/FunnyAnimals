// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/FTypes.h"
#include "FGameModeGameplay.generated.h"

UCLASS()
class FUNNYANIMALS_API AFGameModeGameplay : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFGameModeGameplay();

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
	void Complete() const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameStateChange(EGameState NewGameState);
};
