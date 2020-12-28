// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FObjective.h"
#include "Components/BoxComponent.h"
#include "Characters/FBird.h"
#include "Characters/FWorm.h"
#include "Players/FPlayerState.h"
#include "World/FGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFObjective::AFObjective()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->InitBoxExtent(FVector(128.f));
	RootComponent = Box;

	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFObjective::BeginPlay()
{
	Super::BeginPlay();
}

void AFObjective::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if(!HasAuthority())
		return;;
	
	AFWorm* Worm = Cast<AFWorm>(OtherActor);
	if (Worm == nullptr)
		return;

	if (AFBird* CurrentPawn = Cast<AFBird>(OtherActor->GetInstigator()))
	{
		if (AFPlayerState* PS = CurrentPawn->GetPlayerStateChecked<AFPlayerState>())
			PS->AddScore(CurrentPawn, 1.f);

		Worm->PrepareToDestroy(GetActorLocation());
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Overlap %s"), *OtherActor->GetActorLabel()));
}
