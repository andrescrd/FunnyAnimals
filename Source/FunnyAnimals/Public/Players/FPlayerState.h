// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	static void AddScore(class APawn *PawnInstigator, float Score);
};
