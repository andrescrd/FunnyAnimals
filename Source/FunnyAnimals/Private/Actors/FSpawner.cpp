// Fill out your copyright notice in the Description page of Project Settings.
#include "Actors/FSpawner.h"


#include "Characters/FWorm.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFSpawner::AFSpawner()
{
	bAutoActive = false;
	bIsActive = false;
	MaxSpawn = 10;

	BoxSpawner = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxSpawner"));
	BoxSpawner->InitBoxExtent(FVector(1024, 1024, 64));
	RootComponent = BoxSpawner;

	SetReplicates(false);
}

void AFSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActive)
		Spawn(MaxSpawn);
}

void AFSpawner::Spawn(int MaxActorsToSpawn, class AFBird* ParentBird)
{
	if (!HasAuthority())
		return;

	for (int i = 0; i < MaxActorsToSpawn; i++)
	{
		FVector Location = GetRandomLocation();

		if (Location.IsZero())
		{
			MaxActorsToSpawn--;
			continue;
		}

		FRotator Rotation = FRotator(0, FMath::RandRange(0.f, 360.f), 0);

		FTransform Transform = FTransform();
		Transform.SetLocation(Location);
		Transform.SetRotation(Rotation.Quaternion());

		AActor* NewItem = GetWorld()->SpawnActorDeferred<AActor>(SpawnClass, Transform, this, GetInstigator(), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (NewItem != nullptr)
		{
			if (ParentBird != nullptr && NewItem->IsA(AFWorm::StaticClass()))
				Cast<AFWorm>(NewItem)->SetParent(ParentBird);

			NewItem->FinishSpawning(Transform, true);
		}
		else
		{
			MaxActorsToSpawn--;
		}
	}
}

bool AFSpawner::IsAutoActive() const { return bAutoActive; }

FVector AFSpawner::GetRandomLocation() const
{
	const FVector Origin = BoxSpawner->Bounds.Origin;
	const FVector Extent = BoxSpawner->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}
