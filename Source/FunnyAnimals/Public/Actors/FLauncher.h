// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/Actor.h"
#include "Math/Range.h"
#include "FLauncher.generated.h"

UCLASS()
class FUNNYANIMALS_API AFLauncher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFLauncher();

protected:
	FTimerHandle TimerHandle_Spawner;
	FEnvQueryRequest OwnQueryRequest;
	int TimeToSpawn; 
	
	UPROPERTY(BlueprintReadOnly)
	class USceneComponent* Root;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTimelineComponent* OwnTimeline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USplineComponent* SplineComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBillboardComponent* SpawnerPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<class AFProjectile>> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AActor> ActorDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCurveFloat* Curve;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UEnvQuery* EnvQuery;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoActivate;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeOfTimeline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInt32Range TimeBetweenSpawn;
	
	virtual void BeginPlay() override;
	void SetupTimeline();
	void SpawnProjectiles(const FVector EndLocation);	
	void FindVelocityEQS();
	void OnEQSQueryFinished(const TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void OnTimelineHandler(const float Output) const;	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Activate();
	void Deactivate();

	UFUNCTION(BlueprintCallable)
	bool IsAutoActivate() const;
};
