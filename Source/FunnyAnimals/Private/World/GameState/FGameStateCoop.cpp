// Fill out your copyright notice in the Description page of Project Settings.


#include "World/GameState/FGameStateCoop.h"

#include "Engine/DemoNetDriver.h"

AFGameStateCoop::AFGameStateCoop()
{
	CounterTime = 0;
}

void AFGameStateCoop::SetCounterTime(const int MaxTime) { CounterTime = MaxTime; }

void AFGameStateCoop::UpdateCounterTime(const int DeltaTime) { CounterTime += DeltaTime; }

int AFGameStateCoop::GetCounterTime() const { return CounterTime; }

void AFGameStateCoop::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFGameStateCoop, CounterTime);
}
