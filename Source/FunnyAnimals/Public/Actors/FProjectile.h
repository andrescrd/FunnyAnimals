// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FProjectile.generated.h"

UCLASS(Abstract)
class FUNNYANIMALS_API AFProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URadialForceComponent* RadialForceComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoActivate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsActive;
	
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ActivateMovement(const float Speed, const FVector Velocity) const;
	UFUNCTION(BlueprintCallable)
	virtual void Activate();
};
