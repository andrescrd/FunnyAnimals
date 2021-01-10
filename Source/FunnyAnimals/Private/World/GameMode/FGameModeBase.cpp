// Copyright Epic Games, Inc. All Rights Reserved.

#include "World/GameMode/FGameModeBase.h"


#include "World/GameState/FGameStateSurvive.h"
#include "Players/FPlayerControllerGameplay.h"
#include "Players/FPlayerState.h"
#include "Suppot/Helpers/FBlueprintFunctionLibrary.h"

AFGameModeBase::AFGameModeBase()
{
	CurrentGameState = EGameState::UNKNOW;
	GameStateClass = AFGameStateSurvive::StaticClass();
	PlayerControllerClass = AFPlayerControllerGameplay::StaticClass();
	PlayerStateClass = AFPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}

void AFGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void AFGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

EGameState AFGameModeBase::GetCurrentGameState() const { return CurrentGameState; }

void AFGameModeBase::SetGameState(const EGameState NewGameState)
{
	if (CurrentGameState == NewGameState)
		return;

	CurrentGameState = NewGameState;
	HandleGameState(CurrentGameState);

	OnGameStateChange(CurrentGameState);
}

void AFGameModeBase::HandleGameState(const EGameState NewGameState)
{
	switch (NewGameState)
	{
	case EGameState::PREPARING:
		{
			Preparing();
		}
		break;

	case EGameState::PLAYING:
		{
			Playing();
		}
		break;
	case EGameState::FINISH:
		{
			Finish();
		}
		break;
	case EGameState::COMPLETE:
		{
			Complete();
		}
		break;

	default:
		break;
	}
}
