// Copyright Epic Games, Inc. All Rights Reserved.

#include "World/GameMode/FGameModeSurvive.h"
#include "World/GameState/FGameStateSurvive.h"
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

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.f;
}

void AFGameModeSurvive::BeginPlay()
{
    Super::BeginPlay();
    SetGameState(EGameState::PREPARING);
}

void AFGameModeSurvive::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);    
}

void AFGameModeSurvive::Preparing()
{
    AFLevelManager *LM = UFBlueprintFunctionLibrary::GetLevelManager(this);

    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
        GS->MulticastOnPreparing();

    UpdateObjectiveActors(LM->GetGameplayLevel(0).MaxItemToSpawn);    
   
    UGameplayStatics::GetAllActorsOfClass(this, AFSpawner::StaticClass(), Spawners);

    for (int i = 0; i < Spawners.Num(); i++)
    {
        AFSpawner *Spawner = Cast<AFSpawner>(Spawners[i]);
        if (!Spawner->IsAutoActive())
            Spawner->Spawn(LM->GetGameplayLevel(0).MaxItemToSpawn);
    }

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

void AFGameModeSurvive::Complete()
{
    if (AFGameStateSurvive *GS = GetGameState<AFGameStateSurvive>())
    {
        GS->MulticastOnComplete(nullptr);
        GS->MulticastPlayerWinner();

        for (AActor* Launcher : Launchers)
            Cast<AFLauncher>(Launcher)->Deactivate();
    }
}

void AFGameModeSurvive::Finish() { Complete(); }

void AFGameModeSurvive::UpdateObjectiveActors(const int DeltaActors)
{
    AFGameStateSurvive* GS = GetGameState<AFGameStateSurvive>();
    GS->UpdateObjectiveActors(DeltaActors);

    if(GS->GetMaxObjectiveActors() <= 0)
        SetGameState(EGameState::GAME_OVER);
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
