// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/GameState/FGameStateBase.h"
#include "FGameStateCoop.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFGameStateCoop : public AFGameStateBase
{
	GENERATED_BODY()

public:
	AFGameStateCoop();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int CounterTime;
	
public:
	int GetCounterTime() const;
	void SetCounterTime(const int MaxTime);
	void UpdateCounterTime(const int DeltaTime);
};
