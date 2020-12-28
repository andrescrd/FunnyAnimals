// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FItem.generated.h"

UCLASS()
class FUNNYANIMALS_API AFItem : public AActor
{
	GENERATED_BODY()

public:
	AFItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent *Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent *StaticMesh;

	virtual void BeginPlay() override;

public:
	void SelfDestroy(class APawn* ActorInstigator);
};
