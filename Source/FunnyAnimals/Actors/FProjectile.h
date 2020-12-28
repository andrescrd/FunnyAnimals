// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FProjectile.generated.h"

UCLASS()
class FUNNYANIMALS_API AFProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFProjectile();

protected:
	FTimerHandle TimerHandle_ExplodeCounter;
	bool bExplode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent* Root;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class URadialForceComponent* RadialForceComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int TimeToExplode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoActivate;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnExplode();

public:
	UFUNCTION(BlueprintCallable)
	void ActivateMovement(const float Speed, const FVector Velocity) const;
	UFUNCTION(BlueprintCallable)
	void ActivateExplode();
};
