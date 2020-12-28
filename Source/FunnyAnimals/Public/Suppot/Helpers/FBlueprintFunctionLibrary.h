// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/FTypes.h"
#include "FBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API UFBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FCharacterColor GetColorFromDataTable(class UDataTable *Table);

	UFUNCTION(BlueprintCallable)
	static class AFLevelManager *GetLevelManager(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class UFGameInstance *GetOwnGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static class AFGameMode *GetOwnGameMode(const UObject* WorldContextObject);
};
