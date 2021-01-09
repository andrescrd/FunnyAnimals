// Fill out your copyright notice in the Description page of Project Settings.

#include "World/GameState/FGameStateSurvive.h"
#include "Net/UnrealNetwork.h"

AFGameStateSurvive::AFGameStateSurvive()
{
    CounterTime = 0;
    MaxObjectiveActor = 0;
}

void AFGameStateSurvive::SetCounterTime(const int MaxTime) { CounterTime = MaxTime; }

void AFGameStateSurvive::UpdateCounterTime(const int DeltaTime) { CounterTime += DeltaTime; }

int AFGameStateSurvive::GetCounterTime() const { return CounterTime; }

int AFGameStateSurvive::GetMaxObjectiveActors() const { return MaxObjectiveActor; }

void AFGameStateSurvive::SetMaxObjectiveActors(const int MaxObjective) { MaxObjectiveActor = MaxObjective; }

void AFGameStateSurvive::UpdateObjectiveActors(const int DeltaActors) { MaxObjectiveActor+=DeltaActors; }

void AFGameStateSurvive::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AFGameStateSurvive, CounterTime);
    DOREPLIFETIME(AFGameStateSurvive, MaxObjectiveActor);
}
