// Fill out your copyright notice in the Description page of Project Settings.


#include "FWorm.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "FBird.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "FunnyAnimals/FunnyAnimals.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

AFWorm::AFWorm()
{
	bIsActive = true;
	
	SensComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensComp"));
	SensComp->SightRadius = 600.f;
	SensComp->SetPeripheralVisionAngle(180.f);
	SensComp->OnHearNoise.AddDynamic(this, &AFWorm::HandleHearNoise);
	SensComp->OnSeePawn.AddDynamic(this,&AFWorm::HandleSeePawn);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	PrimaryActorTick.bCanEverTick = false;
}

void AFWorm::BeginPlay()
{
	Super::BeginPlay();
}

void AFWorm::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AAIController* AI = Cast<AAIController>(NewController);
	if (AI != nullptr && BehaviorTreeAsset != nullptr)
		AI->RunBehaviorTree(BehaviorTreeAsset);
}

void AFWorm::MoveToLocation(FVector NewLocation, bool UpdateVelocity)
{
	if (AAIController* AI = Cast<AAIController>(GetController()))
	{
		AI->GetBlackboardComponent()->SetValueAsVector(TEXT("NewLocation"), NewLocation);
		AI->GetBlackboardComponent()->SetValueAsBool(TEXT("ShouldRun"), true);
		AI->GetBlackboardComponent()->SetValueAsBool(TEXT("UpdateVelocity"), UpdateVelocity);		
		
		DrawDebugSphere(GetWorld(), NewLocation, 100.f, 8, FColor::Green, false, 3, 0, 5);		
	}
}

void AFWorm::HandleHearNoise(APawn* OwnInstigator, const FVector& Location, float Volume)
{
	if(!HasAuthority() || !bIsActive || OwnInstigator == this)
		return;
	
	FVector Direction = (GetActorLocation() - Location);
	Direction.Normalize();

	const FVector NewLocation = GetActorLocation() + Direction * 500 * Volume;
	MoveToLocation(NewLocation, true);
	SetInstigator(OwnInstigator);
}

void AFWorm::HandleSeePawn(APawn* OwnPawn)
{
	if(!OwnPawn->IsA(AFBird::StaticClass()))
		return;
	
	FVector NewDirection = GetActorLocation() - OwnPawn->GetActorLocation();
	NewDirection.Normalize();
	
	const FVector NewLocation = GetActorLocation() + (NewDirection * 600.f);
	MoveToLocation(NewLocation);
}

float AFWorm::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	bIsActive = false;
	
	if(AAIController* AI = Cast<AAIController>(GetController()))
		AI->GetBrainComponent()->StopLogic(TEXT("EndMovement"));
	
	SetLifeSpan(2.f);
	return DamageAmount;
}

void AFWorm::PrepareToDestroy(FVector EndLocation)
{
	if(!HasAuthority())
		return;
	
	bIsActive = false;
	AAIController* AI = Cast<AAIController>(GetController());
	AI->GetBrainComponent()->StopLogic(TEXT("EndMovement"));
	AI->MoveToLocation(EndLocation, 0, false);
	SetLifeSpan(3.f);
}

bool AFWorm::GetIsActive() const
{
	return bIsActive;
}
