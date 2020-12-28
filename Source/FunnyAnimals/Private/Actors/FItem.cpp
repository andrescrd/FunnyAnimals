// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/FItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFItem::AFItem()
{
    Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	Collider->InitCapsuleSize(60.f,70.f);
	RootComponent = Collider;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	SetReplicates(true);
}

void AFItem::BeginPlay()
{
	Super::BeginPlay();
}

void AFItem::SelfDestroy(APawn* ActorInstigator)
{
	Destroy();
}
