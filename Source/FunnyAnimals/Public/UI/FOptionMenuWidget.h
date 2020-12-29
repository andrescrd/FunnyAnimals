// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FOptionMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API UFOptionMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFOptionMenuWidget(const FObjectInitializer& ObjectInitializer);

protected:

public:
	UPROPERTY(BlueprintReadOnly)
	ESlateVisibility WelcomeMessageVis;
};
