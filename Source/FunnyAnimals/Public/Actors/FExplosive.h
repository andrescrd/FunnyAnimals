// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FProjectile.h"
#include "GameFramework/Actor.h"
#include "FExplosive.generated.h"

UCLASS()
class FUNNYANIMALS_API AFExplosive : public AFProjectile
{
	GENERATED_BODY()
	
public:	
	AFExplosive();

protected:
	FTimerHandle TimerHandle_ExplodeCounter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundWave* ExplodeSoundWave;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* ParticleSystem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(CalmpMin= "0.5", UIMin="0.5"))
	float ParticleSystemScale;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int TimeToExplode;
	
	UFUNCTION()
    void OnExplode();

	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
public:	
	virtual void Activate() override;	
};
