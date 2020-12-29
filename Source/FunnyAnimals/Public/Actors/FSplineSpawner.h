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
	
	UPROPERTY(BlueprintReadOnly)
	TArray<class UInstancedStaticMeshComponent*> InstancedStaticMeshes;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* SplinComp;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UStaticMesh*> StaticMeshes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRandomStream Scale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	
	FFloatRange ScaleRange;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Spacing;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool RandomRotation;
	
	virtual void OnConstruction(const FTransform& Transform) override;
	void Initialize();
	void SetupTransform();
};
