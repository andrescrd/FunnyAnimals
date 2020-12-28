// Fill out your copyright notice in the Description page of Project Settings.


#include "FPlayerState.h"
#include "GameFramework/Pawn.h"

void AFPlayerState::AddScore(class APawn *PawnInstigator, float NewScore)
{
    float ResultScore = PawnInstigator->GetPlayerState()->GetScore() + NewScore;
    PawnInstigator->GetPlayerState()->SetScore(ResultScore);
}