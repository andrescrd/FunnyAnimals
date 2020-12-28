// Fill out your copyright notice in the Description page of Project Settings.

#include "FBlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/DataTable.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "FunnyAnimals/World/FGameInstance.h"
#include "FunnyAnimals/World/FGameMode.h"

FCharacterColor UFBlueprintFunctionLibrary::GetColorFromDataTable(class UDataTable *Table)
{
	FCharacterColor Color;

	if (!Table)
		return Color;

	const int RowCount = Table->GetRowNames().Num();
	int RowIndex = FMath::RandRange(0, RowCount - 1);

	Color = *Table->FindRow<FCharacterColor>(Table->GetRowNames()[RowIndex], "");
	return Color;
}

class AFLevelManager *UFBlueprintFunctionLibrary::GetLevelManager(const UObject *WorldContextObject)
{
	UFGameInstance *GI = GetOwnGameInstance(WorldContextObject);
	return GI != nullptr ? GI->GetLevelManager() : nullptr;
}

class UFGameInstance *UFBlueprintFunctionLibrary::GetOwnGameInstance(const UObject *WorldContextObject)
{
	UGameInstance *GI = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GI != nullptr ? Cast<UFGameInstance>(GI) : nullptr;
}

class AFGameMode *UFBlueprintFunctionLibrary::GetOwnGameMode(const UObject *WorldContextObject)
{
	AGameModeBase *GM = UGameplayStatics::GetGameMode(WorldContextObject);
	return GM != nullptr ? Cast<AFGameMode>(GM) : nullptr;
}
