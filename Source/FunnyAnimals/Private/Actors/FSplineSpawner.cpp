// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FSplineSpawner.h"
#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFSplineSpawner::AFSplineSpawner()
{
	ScaleRange = FFloatRange::Inclusive(0.8, 1.5);
	Scale.Initialize(200);
	Spacing = 0;
	RandomRotation = false;

	SplinComp = CreateDefaultSubobject<USplineComponent>(TEXT("Splin"));
	RootComponent = SplinComp;

	PrimaryActorTick.bCanEverTick = false;
}

void AFSplineSpawner::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InstancedStaticMeshes.Empty();

	if (StaticMeshes.Num() == 0)
		return;

	Initialize();
	SetupTransform();	
}

void AFSplineSpawner::Initialize()
{
	Size=0;
	FVector Min;
	FVector Max;

	for (UStaticMesh* Item : StaticMeshes)
	{
		UInstancedStaticMeshComponent* InstancedStaticMesh = NewObject<UInstancedStaticMeshComponent>(this);
		InstancedStaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		InstancedStaticMesh->RegisterComponent();
		InstancedStaticMesh->SetFlags(EObjectFlags::RF_Transactional);
		InstancedStaticMesh->bCastDynamicShadow = false;
		InstancedStaticMesh->CreationMethod = EComponentCreationMethod::SimpleConstructionScript;
		InstancedStaticMesh->SetStaticMesh(Item);

		InstancedStaticMeshes.Add(InstancedStaticMesh);

		InstancedStaticMesh->GetLocalBounds(Min, Max);
		Size += FMath::Abs(Min.X) + FMath::Abs(Max.X) + Spacing;
	}

	Size = (Size / StaticMeshes.Num());
}

void AFSplineSpawner::SetupTransform()
{
	const float MinScale = ScaleRange.GetLowerBound().GetValue();
	const float MaxScale = ScaleRange.GetUpperBound().GetValue();
	const int LastIndex = FMath::TruncToInt(SplinComp->GetSplineLength() / Size);

	for (int i = 0; i <= LastIndex; ++i)
	{
		FRotator NewRotation = FRotator::ZeroRotator;
		FTransform NewTransform = SplinComp->GetTransformAtDistanceAlongSpline(i * Size, ESplineCoordinateSpace::World);

		if (!RandomRotation)
		{
			FTransform NextTransform = SplinComp->GetTransformAtDistanceAlongSpline((i + 1) * Size, ESplineCoordinateSpace::World);
			NewRotation = UKismetMathLibrary::FindLookAtRotation(NewTransform.GetLocation(), NextTransform.GetLocation());
		}
		else
		{
			NewRotation = FRotator(0, FMath::RandRange(0, 360), 0);
		}

		FVector NewScale = FVector(MinScale + (MaxScale - MinScale) * Scale.FRand());

		NewTransform.SetRotation(NewRotation.Quaternion());
		NewTransform.SetScale3D(NewScale);
		const int MeshIndex = FMath::RandRange(0, InstancedStaticMeshes.Num() - 1);
		InstancedStaticMeshes[MeshIndex]->AddInstanceWorldSpace(NewTransform);
	}
}
