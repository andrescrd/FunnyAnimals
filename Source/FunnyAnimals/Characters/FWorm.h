// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FunnyAnimals/Characters/FCharacter.h"
#include "FWorm.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFWorm : public AFCharacter
{
	GENERATED_BODY()

public:
	AFWorm();

protected:
	FTimerHandle TimerHandle_HearNooise;
	bool bIsActive;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UBehaviorTree* BehaviorTreeAsset;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPawnSensingComponent* SensComp;

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	void MoveToLocation(FVector NewLocation, bool UpdateVelocity = false);

	UFUNCTION()
	void HandleHearNoise(APawn* OwnInstigator, const FVector& Location, float Volume);
	UFUNCTION()
	void HandleSeePawn(APawn* OwnPawn);

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
public:
	void PrepareToDestroy(FVector EndLocation);

	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const;
};