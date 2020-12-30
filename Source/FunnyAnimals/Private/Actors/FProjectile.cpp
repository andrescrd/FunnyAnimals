// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FProjectile.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/AllowWindowsPlatformTypes.h"

// Sets default values
AFProjectile::AFProjectile()
{
	bAutoActivate = false;
	bExplode = false;
	TimeToExplode = 5;
	ParticleSystemScale = 1;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("ForceComp"));
	RadialForceComp->Radius = 600.f;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	RadialForceComp->SetupAttachment(MeshComp);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->bAutoActivate = false;

	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AFProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActivate)
		ActivateExplode();
}


void AFProjectile::ActivateExplode()
{
	if (bExplode)
		return;
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ExplodeCounter, this, &AFProjectile::OnExplode, TimeToExplode, false);
}

void AFProjectile::OnExplode()
{
	bExplode = true;

	RadialForceComp->FireImpulse();
	// DrawDebugSphere(GetWorld(), GetActorLocation(), RadialForceComp->Radius, 12, FColor::Red, false, 5.f, 0, 5.f);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, 10, GetActorLocation(), RadialForceComp->Radius, nullptr, IgnoreActors, this, GetInstigatorController(), true);

	if (ExplodeSoundWave)
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSoundWave, GetActorLocation());

	if (ParticleSystem)
		UGameplayStatics::SpawnEmitterAtLocation(this, ParticleSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(ParticleSystemScale));

	SetLifeSpan(1.f);
}

void AFProjectile::ActivateMovement(const float Speed, const FVector Velocity) const
{
	if (ProjectileMovement->IsActive())
		return;

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->SetVelocityInLocalSpace(Velocity);
	ProjectileMovement->ToggleActive();
}
