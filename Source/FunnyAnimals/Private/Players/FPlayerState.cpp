// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/FPlayerState.h"
#include "GameFramework/Pawn.h"

void AFPlayerState::AddScore(class APawn* PawnInstigator, const float NewScore)
{
	const float ResultScore = PawnInstigator->GetPlayerState()->GetScore() + NewScore;
	PawnInstigator->GetPlayerState()->SetScore(ResultScore);
}
