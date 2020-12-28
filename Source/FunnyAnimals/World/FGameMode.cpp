// Copyright Epic Games, Inc. All Rights Reserved.

#include "FGameMode.h"
#include "FunnyAnimals/World/FGameState.h"
#include "FunnyAnimals/Players/FPlayerController.h"
#include "FunnyAnimals/Players/FPlayerState.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "FunnyAnimals/Actors/FSpawner.h"
#include "FunnyAnimals/Suppot/Helpers/FBlueprintFunctionLibrary.h"
#include "FunnyAnimals/Suppot/Managers/FLevelManager.h"

AFGameMode::AFGameMode()
{
    CurrentGameState = EGameState::UNKNOW;
    GameStateClass = AFGameState::StaticClass();
    PlayerControllerClass = AFPlayerController::StaticClass();
    PlayerStateClass = AFPlayerState::StaticClass();
}

void AFGameMode::BeginPlay()
{
    Super::BeginPlay();

    SetGameState(EGameState::PREPARING);
}

EGameState AFGameMode::GetCurrentGameState() const
{
    return CurrentGameState;
}

void AFGameMode::SetGameState(const EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    HandleGameState(CurrentGameState);

    OnGameStateChange(CurrentGameState);
}

void AFGameMode::Preparing()
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
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFGameMode::SetGameState, EGameState::PLAYING), 1.f, false);
}

void AFGameMode::Playing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameState *GS = GetGameState<AFGameState>())
    {
        GS->MulticastOnPlaying();
        GS->SetCounterTime(LM->GetLevel(0).MaxTime);

        GetWorldTimerManager().SetTimer(Counter_TimerHandle, this, &AFGameMode::StartCounter, 1.f, true);
    }
}

void AFGameMode::Complete()
{
    if (AFGameState *GS = GetGameState<AFGameState>())
    {
        GS->MulticastOnComplete(nullptr);
        GS->MulticastPlayerWinner();
    }
}

void AFGameMode::StartCounter()
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

void AFGameMode::HandleGameState(const EGameState NewGameState)
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
