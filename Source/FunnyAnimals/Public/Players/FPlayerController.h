// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = PlayerController)
	void OnComplete(class APawn *InstigatorPawn);
	UFUNCTION(BlueprintImplementableEvent, Category = PlayerController)
	void OnPlaying();
	UFUNCTION(BlueprintImplementableEvent, Category = PlayerController)
	void OnLoading();
};
