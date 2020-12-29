// Copyright Epic Games, Inc. All Rights Reserved.

#include "World/FGameModeGameplay.h"
#include "World/FGameState.h"
#include "Players/FPlayerController.h"
#include "Players/FPlayerState.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/FSpawner.h"
#include "Suppot/Helpers/FBlueprintFunctionLibrary.h"
#include "Suppot/Managers/FLevelManager.h"

AFGameModeGameplay::AFGameModeGameplay()
{
    CurrentGameState = EGameState::UNKNOW;
    GameStateClass = AFGameState::StaticClass();
    PlayerControllerClass = AFPlayerController::StaticClass();
    PlayerStateClass = AFPlayerState::StaticClass();
}

void AFGameModeGameplay::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(EGameState::PREPARING);
}

EGameState AFGameModeGameplay::GetCurrentGameState() const
{
    return CurrentGameState;
}

void AFGameModeGameplay::SetGameState(const EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    HandleGameState(CurrentGameState);

    OnGameStateChange(CurrentGameState);
}

void AFGameModeGameplay::Preparing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameState *GS = GetGameState<AFGameState>())
        GS->MulticastOnPreparing();

    TArray<AActor *> Spawners;
    UGameplayStatics::GetAllActorsOfClass(this, AFSpawner::StaticClass(), Spawners);

    for (int i = 0; i < Spawners.Num(); i++)
    {
        AFSpawner *Spawner = Cast<AFSpawner>(Spawners[i]);
        if (!Spawner->IsActive())
            Spawner->Spawn(LM->GetLevel(0).MaxItemToSpawn);
    }

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFGameModeGameplay::SetGameState, EGameState::PLAYING), 1.f, false);
}

void AFGameModeGameplay::Playing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameState *GS = GetGameState<AFGameState>())
    {
        GS->MulticastOnPlaying();
        GS->SetCounterTime(LM->GetLevel(0).MaxTime);

        GetWorldTimerManager().SetTimer(Counter_TimerHandle, this, &AFGameModeGameplay::StartCounter, 1.f, true);
    }
}

void AFGameModeGameplay::Complete()
{
    if (AFGameState *GS = GetGameState<AFGameState>())
    {
        GS->MulticastOnComplete(nullptr);
        GS->MulticastPlayerWinner();
    }
}

void AFGameModeGameplay::StartCounter()
{
    if (AFGameState *GS = GetGameState<AFGameState>())
    {
        if (GS->GetCounterTime() > 0)
        {
            GS->UpdateCounterTime(-1);
        }
        else
        {
            GetWorldTimerManager().ClearTimer(Counter_TimerHandle);
            SetGameState(EGameState::FINISH);
        }
    }
}

void AFGameModeGameplay::HandleGameState(const EGameState NewGameState)
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
    case EGameState::GAME_OVER:
    {
        Complete();
    }
    break;

    default:
        break;
    }
}
