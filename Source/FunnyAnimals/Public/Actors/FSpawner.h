
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSpawner.generated.h"

UCLASS()
class FUNNYANIMALS_API AFSpawner : public AActor
{
	GENERATED_BODY()

public:
	AFSpawner();

protected:
	bool bIsActive;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UBoxComponent* BoxSpawner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class AActor> SpawnClass;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoActive;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxSpawn;

    FVector GetRandomLocation() const;

	virtual void BeginPlay() override;

public:    
    UFUNCTION(BlueprintCallable)
    void Spawn(int MaxActorsToSpawn);
    UFUNCTION(BlueprintCallable)
	bool IsAutoActive() const;	
};
