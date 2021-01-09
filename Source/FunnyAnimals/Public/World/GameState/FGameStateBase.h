// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FGameStateBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FUNNYANIMALS_API AFGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UFUNCTION(NetMulticast, Reliable)
    void MulticastOnComplete(class APawn *InstigatorPawn);
	UFUNCTION(NetMulticast, Reliable)
    void MulticastOnPreparing();
	UFUNCTION(NetMulticast, Reliable)
    void MulticastOnPlaying();
	UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayerWinner();	
};
