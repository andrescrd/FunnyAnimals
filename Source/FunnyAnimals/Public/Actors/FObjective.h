// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FObjective.generated.h"

UCLASS()
class FUNNYANIMALS_API AFObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFObjective();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* Box;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AFWorm> WromClass;
	
	virtual void BeginPlay() override;

	virtual  void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
