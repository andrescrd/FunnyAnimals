// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/FCharacter.h"
#include "FBird.generated.h"

/**
 * 
 */
UCLASS()
class FUNNYANIMALS_API AFBird : public AFCharacter
{
	GENERATED_BODY()

public:
	AFBird();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	TSubclassOf<class UCameraShakeBase> CameraShake;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UAnimMontage *PeckAnimation;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UPawnNoiseEmitterComponent* NoiseEmitterComp;
	
	void Peck();	
	UFUNCTION(Server, Reliable, WithValidation)
    void ServerPeck();
	UFUNCTION(Server, Reliable, WithValidation)
    void ServerToggleMovement(EMovementMode Mode);
	
	UFUNCTION()
    void PeckEnd();
	
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = GamePlay)
	class UDataTable *DataTable;

	void SetColor(const int8 Index,const FLinearColor Color, class UMaterialInterface *MaterialInt, class USkeletalMeshComponent *SkeletalMesh);
};
