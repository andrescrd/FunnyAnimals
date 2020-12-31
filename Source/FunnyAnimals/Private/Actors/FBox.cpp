// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FBox.h"

#include "Characters/FWorm.h"
#include "Kismet/GameplayStatics.h"

AFBox::AFBox()
{
	MeshComp->GetBodyInstance()->bLockXRotation = true;
	MeshComp->GetBodyInstance()->bLockYRotation = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AFBox::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other->IsA(AFWorm::StaticClass()) && FVector::DotProduct(HitNormal, FVector::UpVector) > 0.9)
		UGameplayStatics::ApplyDamage(Other, 10, GetInstigatorController(), this, nullptr);

	SetLifeSpan(3.f);
}
