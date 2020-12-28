// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FBird.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Types/FTypes.h"
#include "Suppot/Helpers/FBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PawnNoiseEmitterComponent.h"

AFBird::AFBird()
{
	NoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitterComp"));
}

void AFBird::BeginPlay()
{
	Super::BeginPlay();

	FCharacterColor Color = UFBlueprintFunctionLibrary::GetColorFromDataTable(DataTable);
	SetColor(1, Color.ColorBase, GetMesh()->GetMaterial(0), GetMesh());
	SetColor(2, Color.ColorLight, GetMesh()->GetMaterial(0), GetMesh());
	SetColor(3, Color.ColorDark, GetMesh()->GetMaterial(0), GetMesh());
}

void AFBird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Peck", IE_Pressed, this, &AFBird::Peck);
}

void AFBird::SetColor(const int8 Index, const FLinearColor Color, class UMaterialInterface* MaterialInt, class USkeletalMeshComponent* SkeletalMesh)
{
	UMaterialInstanceDynamic* MatInst = SkeletalMesh->CreateAndSetMaterialInstanceDynamicFromMaterial(Index, MaterialInt);
	MatInst->SetVectorParameterValue("Color", Color);
}

void AFBird::Peck()
{
	if (GetCharacterMovement()->IsFalling())
		return;

	ServerPeck();

	if (PeckAnimation)
	{
		ServerToggleMovement(EMovementMode::MOVE_None);
		const float MontageLength = PlayAnimMontage(PeckAnimation, 1, NAME_None);

		GetWorld()->GetTimerManager().SetTimer(MontageEnd_TimerHandle, this, &AFBird::PeckEnd, MontageLength, false);
	}

	if (CameraShake)
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(CameraShake);

}

void AFBird::ServerPeck_Implementation()
{	
	MakeNoise(1, this, FVector::ZeroVector);
	
	// TArray<AActor*> OverlappingActors;
	// GetOverlappingActors(OverlappingActors, AFItem::StaticClass());
	//
	// if (OverlappingActors.Num() > 0)
	// {
	// 	Cast<AFItem>(OverlappingActors[0])->SelfDestroy(this);
	// 	if (AFPlayerState* PS = GetPlayerStateChecked<AFPlayerState>())
	// 		PS->AddScore(this, 1.f);
	// }
}

bool AFBird::ServerPeck_Validate()
{
	return true;
}

void AFBird::ServerToggleMovement_Implementation(EMovementMode Mode)
{
	GetCharacterMovement()->SetMovementMode(Mode);
}

bool AFBird::ServerToggleMovement_Validate(EMovementMode Mode)
{
	return true;
}

void AFBird::PeckEnd()
{
	ServerToggleMovement(EMovementMode::MOVE_Walking);
	GetWorld()->GetTimerManager().ClearTimer(MontageEnd_TimerHandle);
}
