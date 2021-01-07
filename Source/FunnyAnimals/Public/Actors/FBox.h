// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/FProjectile.h"
#include "FBox.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFBox : public AFProjectile
{
	GENERATED_BODY()

public:
	AFBox();

protected:
	virtual  void Tick(float DeltaSeconds) override;
public:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
