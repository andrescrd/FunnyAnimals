// Fill out your copyright notice in the Description page of Project Settings.

#include "Suppot/Managers/FLevelManager.h"
#include "Types/FTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

void AFLevelManager::SetGameplayLevels(const TArray<FLevelConfig> NewLevels)
{
    GameplayLevels = NewLevels;
}

TArray<FLevelConfig> AFLevelManager::GetGameplayLevels() const
{
    return GameplayLevels;
}

FLevelConfig AFLevelManager::GetCurrentGameplayLevel() const
{
    return CurrentGameplayLevel;
}

FLevelConfig AFLevelManager::GetGameplayLevel(const int Index)
{
    return GameplayLevels.IsValidIndex(Index) ? GameplayLevels[Index] : FLevelConfig();
}

void AFLevelManager::LoadGameplayLevel(class UObject *Context, const FName LevelNameToLoad)
{
    for (int i = 0; i < GameplayLevels.Num(); i++)
    {
        if (GameplayLevels[i].MapName.IsEqual(LevelNameToLoad))
        {
            CurrentGameplayLevel = GameplayLevels[i];
            break;
        }
    }

    UWorld *World = GEngine->GetWorldFromContextObjectChecked(Context);
    UE_LOG(LogTemp, Warning, TEXT("Level to load %s"), *LevelNameToLoad.ToString());
    UGameplayStatics::OpenLevel(World, LevelNameToLoad, true);
}

void AFLevelManager::LoadLobby(class UObject *Context) const
{
    UWorld *World = GEngine->GetWorldFromContextObjectChecked(Context);
    UGameplayStatics::OpenLevel(World, LobbyMapName, true, FString("listen"));
}

void AFLevelManager::LoadMainMenu(class UObject *Context) const
{
    UWorld *World = GEngine->GetWorldFromContextObjectChecked(Context);
    UGameplayStatics::OpenLevel(World, MainMenuMapName, true);
}
