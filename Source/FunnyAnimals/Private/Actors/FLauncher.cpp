// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FLauncher.h"
#include "Actors/FProjectile.h"
#include "Components/BillboardComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/DecalActor.h"
#include "Engine/DemoNetDriver.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFLauncher::AFLauncher()
{
	bAutoActivate = false;
	ProjectileSpeed = 2000;
	TimeOfTimeline = 20;
	TimeBetweenSpawn = FInt32Range::Inclusive(5, 15);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComp"));
	SplineComp->SetupAttachment(Root);

	SpawnerPoint = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnerPoint"));
	SpawnerPoint->SetWorldScale3D(FVector(3));
	SpawnerPoint->SetupAttachment(Root);

	OwnTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OwnTimeline"));

	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(false);
	// bNetLoadOnClient = false;
}

// Called when the game starts or when spawned
void AFLauncher::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActivate)
		Activate();

	SetupTimeline();

	if (EnvQuery != nullptr)
		OwnQueryRequest = FEnvQueryRequest(EnvQuery, this);
}

void AFLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFLauncher::SetupTimeline()
{
	if (Curve == nullptr)
		return;

	FOnTimelineFloat TimelineCallback;
	TimelineCallback.BindUFunction(this, FName("OnTimelineHandler"));
	OwnTimeline->AddInterpFloat(Curve, TimelineCallback);

	const float NewRate = OwnTimeline->GetTimelineLength() / (OwnTimeline->GetTimelineLength() * TimeOfTimeline);
	OwnTimeline->SetPlayRate(NewRate);
	OwnTimeline->SetLooping(true);
	OwnTimeline->SetIgnoreTimeDilation(false);
	OwnTimeline->Play();
}

void AFLauncher::OnTimelineHandler(const float Output) const
{
	const float Distance = SplineComp->GetSplineLength() * Output;
	const FTransform SplineResult = SplineComp->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::Local);
	SpawnerPoint->SetRelativeLocation(SplineResult.GetLocation());
}


void AFLauncher::FindVelocityEQS()
{
	if (EnvQuery != nullptr)
		OwnQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &AFLauncher::OnEQSQueryFinished);
}

void AFLauncher::OnEQSQueryFinished(const TSharedPtr<FEnvQueryResult> Result)
{
	TArray<FVector> Locations;
	Result->GetAllAsLocations(Locations);

	if (Locations.Num() > 0)
	{
		SpawnProjectiles(Locations[0]);
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("%s: There is no result for spawn"), *GetActorLabel());
#endif
	}

	Activate();
}

void AFLauncher::Activate()
{
	if (TimerHandle_Spawner.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Spawner);

	TimeToSpawn = FMath::RandRange(TimeBetweenSpawn.GetLowerBound().GetValue(), TimeBetweenSpawn.GetUpperBound().GetValue());
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Spawner, this, &AFLauncher::FindVelocityEQS, TimeToSpawn, true);

#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("%s: Time to spawn is %d"), *GetActorLabel(), TimeToSpawn);
#endif
}

void AFLauncher::Deactivate()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Spawner);
}

bool AFLauncher::IsAutoActivate() const { return bAutoActivate; }

void AFLauncher::SpawnProjectiles(const FVector EndLocation)
{
	if (ProjectileClass.Num() == 0)
		return;

	if(ActorDecal)
		AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ActorDecal,EndLocation, FRotator::ZeroRotator);		

	FVector Velocity;
	const bool bSuccess = UGameplayStatics::SuggestProjectileVelocity(GetWorld(), Velocity, SpawnerPoint->GetComponentLocation(),
	                                                                  EndLocation,
	                                                                  ProjectileSpeed, true, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace,
	                                                                  FCollisionResponseParams::DefaultResponseParam, TArray<AActor*>(), false);

	if (bSuccess)
	{
		const int32 ProjectileIndex = FMath::RandRange(0, ProjectileClass.Num() -1 );
		AFProjectile* NewProjectile = GetWorld()->SpawnActorDeferred<AFProjectile>(ProjectileClass[ProjectileIndex],
		                                                                           SpawnerPoint->GetComponentTransform(), GetOwner(), GetInstigator(),
		                                                                           ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		
		FTransform NewTransform;
		NewTransform.SetLocation(SpawnerPoint->GetComponentLocation());
		NewProjectile->ActivateMovement(ProjectileSpeed, Velocity);
		NewProjectile->FinishSpawning(NewTransform, true);		
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("%s: Cant find velocity for spawn"), *GetActorLabel());
#endif
	}
}