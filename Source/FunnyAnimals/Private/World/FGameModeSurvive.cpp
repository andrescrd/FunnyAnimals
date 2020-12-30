// Copyright Epic Games, Inc. All Rights Reserved.

#include "World/FGameModeSurvive.h"
#include "World/FGameStateSurvive.h"
#include "Players/FPlayerControllerGameplay.h"
#include "Players/FPlayerState.h"
#include "TimerManager.h"
#include "Actors/FLauncher.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/FSpawner.h"
#include "Suppot/Helpers/FBlueprintFunctionLibrary.h"
#include "Suppot/Managers/FLevelManager.h"

AFGameModeSurvive::AFGameModeSurvive()
{
    CurrentGameState = EGameState::UNKNOW;
    GameStateClass = AFGameStateSurvive::StaticClass();
    PlayerControllerClass = AFPlayerControllerGameplay::StaticClass();
    PlayerStateClass = AFPlayerState::StaticClass();    
}

void AFGameModeSurvive::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(EGameState::PREPARING);
}

EGameState AFGameModeSurvive::GetCurrentGameState() const
{
    return CurrentGameState;
}

void AFGameModeSurvive::SetGameState(const EGameState NewGameState)
{
    if (CurrentGameState == NewGameState)
        return;

    CurrentGameState = NewGameState;
    HandleGameState(CurrentGameState);

    OnGameStateChange(CurrentGameState);
}

void AFGameModeSurvive::Preparing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
        GS->MulticastOnPreparing();

    TArray<AActor *> Spawners;
    UGameplayStatics::GetAllActorsOfClass(this, AFSpawner::StaticClass(), Spawners);

    for (int i = 0; i < Spawners.Num(); i++)
    {
        AFSpawner *Spawner = Cast<AFSpawner>(Spawners[i]);
        if (!Spawner->IsAutoActive())
            Spawner->Spawn(LM->GetGameplayLevel(0).MaxItemToSpawn);
    }

    TArray<AActor *> Launchers;
    UGameplayStatics::GetAllActorsOfClass(this, AFLauncher::StaticClass(), Launchers);

    for (int i = 0; i < Launchers.Num(); ++i)
    {
        AFLauncher* Launcher = Cast<AFLauncher>(Launchers[i]);
        if(!Launcher->IsAutoActivate())
            Launcher->Activate();        
    }
    
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFGameModeSurvive::SetGameState, EGameState::PLAYING), 1.f, false);
}

void AFGameModeSurvive::Playing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
    {
        GS->MulticastOnPlaying();
        GS->SetCounterTime(LM->GetGameplayLevel(0).MaxTime);

        GetWorldTimerManager().SetTimer(Counter_TimerHandle, this, &AFGameModeSurvive::StartCounter, 1.f, true);
    }
}

void AFGameModeSurvive::Complete() const
{
    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
    {
        GS->MulticastOnComplete(nullptr);
        GS->MulticastPlayerWinner();
    }
}

void AFGameModeSurvive::StartCounter()
{
    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
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

void AFGameModeSurvive::HandleGameState(const EGameState NewGameState)
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
