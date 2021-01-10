// Fill out your copyright notice in the Description page of Project Settings.


#include "World/GameMode/FGameModeCoop.h"


#include "Actors/FLauncher.h"
#include "Actors/FSpawner.h"
#include "Characters/FBird.h"
#include "Kismet/GameplayStatics.h"
#include "Players/FPlayerControllerGameplay.h"
#include "Players/FPlayerState.h"
#include "Suppot/Helpers/FBlueprintFunctionLibrary.h"
#include "Suppot/Managers/FLevelManager.h"
#include "World/GameState/FGameStateCoop.h"
#include "World/GameState/FGameStateSurvive.h"

AFGameModeCoop::AFGameModeCoop()
{
	CurrentGameState = EGameState::UNKNOW;
	GameStateClass = AFGameStateCoop::StaticClass();
	PlayerControllerClass = AFPlayerControllerGameplay::StaticClass();
	PlayerStateClass = AFPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.f;
}

void AFGameModeCoop::BeginPlay()
{
	Super::BeginPlay();
	SetGameState(EGameState::PREPARING);
}

void AFGameModeCoop::Tick(float DeltaSeconds) { Super::Tick(DeltaSeconds); }

void AFGameModeCoop::Preparing()
{
	if (AFGameStateCoop* GS = GetGameState<AFGameStateCoop>())
		GS->MulticastOnPreparing();

	// UpdateObjectiveActors(LM->GetGameplayLevel(0).MaxItemToSpawn);

	SpawnLaunchers();

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &AFGameModeCoop::SetGameState, EGameState::PLAYING), 5.f, false);
}

void AFGameModeCoop::Playing()
{
	if (AFGameStateCoop* GS = GetGameState<AFGameStateCoop>())
		GS->MulticastOnPlaying();

	SpawnWorms();
}

void AFGameModeCoop::Complete()
{
}

void AFGameModeCoop::Finish()
{
}

void AFGameModeCoop::StartCounter()
{
}

void AFGameModeCoop::UpdateObjectiveActors(class APawn* OwnPawn, const int DeltaActors)
{
	if (AFPlayerState* PS = Cast<AFPlayerState>(OwnPawn->GetPlayerState()))
	{
		PS->AddScore(OwnPawn, DeltaActors);

		if (PS->GetScore() <= 0)
			SetGameState(EGameState::COMPLETE);
	}
}

// *****************************
// Spawners
// *****************************

void AFGameModeCoop::SpawnLaunchers()
{
	UGameplayStatics::GetAllActorsOfClass(this, AFLauncher::StaticClass(), Launchers);

	for (int i = 0; i < Launchers.Num(); ++i)
	{
		AFLauncher* Launcher = Cast<AFLauncher>(Launchers[i]);
		if (!Launcher->IsAutoActivate())
			Launcher->Activate();
	}
}

void AFGameModeCoop::SpawnWorms()
{
	AFLevelManager* LM = UFBlueprintFunctionLibrary::GetLevelManager(this);
	UGameplayStatics::GetAllActorsOfClass(this, AFSpawner::StaticClass(), Spawners);

	if (AFGameStateCoop* GS = GetGameState<AFGameStateCoop>())
		GS->MulticastOnPlaying();

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AFPlayerControllerGameplay* PC = Cast<AFPlayerControllerGameplay>(It->Get()))
		{
			if (APawn* Pawn = PC->GetPawn())
			{
				AFBird* CurrentBird = Cast<AFBird>(Pawn);

				for (int i = 0; i < Spawners.Num(); i++)
				{
					AFSpawner* Spawner = Cast<AFSpawner>(Spawners[i]);
					if (!Spawner->IsAutoActive())
						Spawner->Spawn(LM->GetGameplayLevel(0).MaxItemToSpawn / 2, CurrentBird);
				}


				if (AFPlayerState* PS = Cast<AFPlayerState>(PC->PlayerState))
					PS->AddScore(Pawn, LM->GetGameplayLevel(0).MaxItemToSpawn / 2 * Spawners.Num());
			}
		}
	}
}
