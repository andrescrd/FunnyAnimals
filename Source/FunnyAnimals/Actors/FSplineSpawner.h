// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FSplineSpawner.generated.h"

UCLASS()
class FUNNYANIMALS_API AFSplineSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFSplineSpawner();

protected:
	float Size;
	
	UPROPERTY(BlueprintReadOnly, Category=Setting)
	TArray<class UInstancedStaticMeshComponent*> InstancedStaticMeshes;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Setting)
	class USplineComponent* SplinComp;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Setting)
	TArray<class UStaticMesh*> StaticMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Setting)
	FRandomStream Scale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Setting)	
	FFloatRange ScaleRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Setting)
	float Spacing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category=Setting)
	bool RandomRotation;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	void Initialize();
	void SetupTransform();
};
