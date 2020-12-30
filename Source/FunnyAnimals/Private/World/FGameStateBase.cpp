// Fill out your copyright notice in the Description page of Project Settings.


#include "World/FGameStateBase.h"
#include "Engine/World.h"
#include "Players/FPlayerControllerGameplay.h"
#include "Players/FPlayerState.h"

void AFGameStateBase::MulticastOnComplete_Implementation(class APawn *InstigatorPawn)
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

void AFGameStateBase::MulticastOnPreparing_Implementation()
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

void AFGameStateBase::MulticastOnPlaying_Implementation()
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

void AFGameStateBase::MulticastPlayerWinner_Implementation()
{
	PlayerArray.Sort([](APlayerState &A, APlayerState &B) {
        return A.GetScore() > B.GetScore();
    });
}
