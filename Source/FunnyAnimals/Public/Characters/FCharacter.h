// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FCharacter.generated.h"

UCLASS()
class FUNNYANIMALS_API AFCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFCharacter();

protected:
	FTimerHandle MontageEnd_TimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent *CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent *FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);	

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

public:
	FORCEINLINE class USpringArmComponent *GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent *GetFollowCamera() const { return FollowCamera; }
};
