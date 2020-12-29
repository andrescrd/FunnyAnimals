// Fill out your copyright notice in the Description page of Project Settings.

#include "World/FGameState.h"
#include "Engine/World.h"
#include "Players/FPlayerControllerGameplay.h"
#include "Players/FPlayerState.h"
#include "Net/UnrealNetwork.h"

AFGameState::AFGameState()
{
    CounterTime = 0;
}

void AFGameState::MulticastOnComplete_Implementation(class APawn *InstigatorPawn)
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AFPlayerControllerGameplay *PC = Cast<AFPlayerControllerGameplay>(It->Get());

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
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AFPlayerControllerGameplay *PC = Cast<AFPlayerControllerGameplay>(It->Get());

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
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AFPlayerControllerGameplay *PC = Cast<AFPlayerControllerGameplay>(It->Get());

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

void AFGameState::SetCounterTime(const int MaxTime)
{
    CounterTime = MaxTime;
}

void AFGameState::UpdateCounterTime(const int DeltaTime)
{
    CounterTime += DeltaTime;
}

int AFGameState::GetCounterTime() const
{
    return CounterTime;
}

void AFGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFGameState, CounterTime);
}
