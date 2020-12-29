// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FGameState.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFGameState : public AGameStateBase
{
	GENERATED_BODY()

	public:
	AFGameState();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int CounterTime;

public:
	int GetCounterTime() const;
	void SetCounterTime(const int MaxTime);
	void UpdateCounterTime(const int DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnComplete(class APawn *InstigatorPawn);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPreparing();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPlaying();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerWinner();
};
