// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGameStateBase.h"
#include "FGameStateSurvive.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFGameStateSurvive : public AFGameStateBase
{
	GENERATED_BODY()

	public:
	AFGameStateSurvive();

protected:
	UPROPERTY(Replicated, BlueprintReadOnly)
	int CounterTime;
	UPROPERTY(Replicated, BlueprintReadOnly)
	int MaxObjectiveActor;

public:
	int GetCounterTime() const;
	int GetMaxObjectiveActors() const;
	
	void SetCounterTime(const int MaxTime);
	void SetMaxObjectiveActors(const int MaxObjective);
	
	void UpdateCounterTime(const int DeltaTime);
	void UpdateObjectiveActors(const int DeltaActors);	
};
