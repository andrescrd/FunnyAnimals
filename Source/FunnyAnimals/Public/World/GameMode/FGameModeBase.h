// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/FTypes.h"

#include "FGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class FUNNYANIMALS_API AFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFGameModeBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGameState CurrentGameState;

	void HandleGameState(const EGameState NewGameState);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// State Game
	EGameState GetCurrentGameState() const;
	void SetGameState(const EGameState NewGameState);
	
	UFUNCTION(BlueprintImplementableEvent)
    void OnGameStateChange(EGameState NewGameState);

	virtual void Preparing() PURE_VIRTUAL(ABaseActor::Preparing,);
	virtual void Playing() PURE_VIRTUAL(ABaseActor::Playing,);
	virtual void Complete() PURE_VIRTUAL(ABaseActor::Complete,);
	virtual void Finish() PURE_VIRTUAL(ABaseActor::Finish,);

};
