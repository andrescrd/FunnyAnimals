// Fill out your copyright notice in the Description page of Project Settings.

#include "FSpawner.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "FunnyAnimals/Actors/FItem.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFSpawner::AFSpawner()
{
	bAutoActive = false;
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

void AFSpawner::Spawn(int MaxActorsToSpawn)
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
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		AActor* NewItem = GetWorld()->SpawnActor<AActor>(SpawnClass, Location, Rotation, SpawnParameters);

		if (NewItem == nullptr)
			MaxActorsToSpawn--;
	}
}

bool AFSpawner::IsActive()
{
	return bAutoActive;
}

FVector AFSpawner::GetRandomLocation() const
{
	const FVector Origin = BoxSpawner->Bounds.Origin;
	const FVector Extent = BoxSpawner->Bounds.BoxExtent;
	return UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
}

// FVector AFSpawner::GetRandomLocation()
// {
//     FHitResult OutHit;
//     FVector Start = FVector(FMath::RandRange(-2000.0f, 2000.0f), FMath::RandRange(-2000.0f, 2000.0f), 4000.0f);
//     FVector End = Start - FVector(0, 0, 8000.f);
//     FCollisionQueryParams Params;
//     Params.AddIgnoredActor(this);
//     GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel ::ECC_Visibility, Params);
//
//     return OutHit.Location;
// }
