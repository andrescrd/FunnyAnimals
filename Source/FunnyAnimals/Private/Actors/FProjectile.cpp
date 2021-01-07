// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FProjectile.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Windows/AllowWindowsPlatformTypes.h"

// Sets default values
AFProjectile::AFProjectile()
{
	bAutoActivate = false;
	bIsActive = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetNotifyRigidBodyCollision(true);
	MeshComp->SetGenerateOverlapEvents(false);
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
}

// Called when the game starts or when spawned
void AFProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActivate)
		Activate();
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

void AFProjectile::Activate() { }
