// Fill out your copyright notice in the Description page of Project Settings.

#include "FGameState.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "FunnyAnimals/Players/FPlayerController.h"
#include "FunnyAnimals/Players/FPlayerState.h"
#include "Net/UnrealNetwork.h"

AFGameState::AFGameState()
{
    CounterTime = 0;
}

void AFGameState::MulticastOnComplete_Implementation(class APawn *InstigatorPawn)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPlayerController *PC = Cast<AFPlayerController>(It->Get());

        if (PC && PC->IsLocalController())
        {
            PC->OnComplete(InstigatorPawn);

            if (APawn *Pawn = PC->GetPawn())
                Pawn->DisableInput(nullptr);
        }
    }
}

void AFGameState::MulticastOnPreparing_Implementation()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPlayerController *PC = Cast<AFPlayerController>(It->Get());

        if (PC && PC->IsLocalController())
        {
            PC->OnLoading();

            if (APawn *Pawn = PC->GetPawn())
                Pawn->DisableInput(nullptr);
        }
    }
}

void AFGameState::MulticastOnPlaying_Implementation()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
    {
        AFPlayerController *PC = Cast<AFPlayerController>(It->Get());

        if (PC && PC->IsLocalController())
        {
            PC->OnPlaying();

            if (APawn *Pawn = PC->GetPawn())
                Pawn->EnableInput(nullptr);
        }
    }
}

void AFGameState::MulticastPlayerWinner_Implementation()
{
    PlayerArray.Sort([](APlayerState &A, APlayerState &B) {
        return A.GetScore() > B.GetScore();
    });
}

void AFGameState::SetCounterTime(int MaxTime)
{
    CounterTime = MaxTime;
}

void AFGameState::UpdateCounterTime(int DeltaTime)
{
    CounterTime += DeltaTime;
}

int AFGameState::GetCounterTime()
{
    return CounterTime;
}

void AFGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFGameState, CounterTime);
}
