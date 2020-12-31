// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FExplosive.h"

#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AFExplosive::AFExplosive()
{
	TimeToExplode = 5;
	ParticleSystemScale = 1;
	PrimaryActorTick.bCanEverTick = false;
}

void AFExplosive::Activate()
{
	if (bIsActive)
		return;

	bIsActive = true;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExplodeCounter, this, &AFExplosive::OnExplode, TimeToExplode, false);
}

void AFExplosive::OnExplode()
{
	RadialForceComp->FireImpulse();

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, 10, GetActorLocation(), RadialForceComp->Radius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

	if (ExplodeSoundWave)
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSoundWave, GetActorLocation());

	if (ParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(ParticleSystemScale));

	SetLifeSpan(1.f);
}

void AFExplosive::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Activate();
}
