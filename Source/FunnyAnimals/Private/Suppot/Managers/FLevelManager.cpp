// Fill out your copyright notice in the Description page of Project Settings.

#include "Suppot/Managers/FLevelManager.h"
#include "Types/FTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void AFLevelManager::SetLevels(TArray<FLevelConfig> NewLevels)
{
    Levels = NewLevels;
}

TArray<FLevelConfig> AFLevelManager::GetLevels() const
{
    return Levels;
}

FLevelConfig AFLevelManager::GetCurrentLevel() const
{
    return CurrentLevel;
}

FLevelConfig AFLevelManager::GetLevel(int Index)
{
    return Levels.IsValidIndex(Index) ? Levels[Index] : FLevelConfig();
}

void AFLevelManager::LoadLevel(class UObject *context, FName LevelNameToLoad)
{
    for (int i = 0; i < Levels.Num(); i++)
    {
        if (Levels[i].LevelName.IsEqual(LevelNameToLoad))
        {
            CurrentLevel = Levels[i];
            break;
        }
    }

    UWorld *World = GEngine->GetWorldFromContextObjectChecked(context);
    UE_LOG(LogTemp, Warning, TEXT("Level to load %s"), *LevelNameToLoad.ToString());
    UGameplayStatics::OpenLevel(World, LevelNameToLoad, true);
}
