// Copyright ©2020 Samuel Harrison

//includes
#include "VolumeSpawner.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boid.h"
#include "FlockManager.h"

AVolumeSpawner::AVolumeSpawner()
{
	//disable ticking
	PrimaryActorTick.bCanEverTick = false;

	//setup box component
	VolumeSpawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume Spawner Collision Component"));
	RootComponent = VolumeSpawnerCollision;
	VolumeSpawnerCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VolumeSpawnerCollision->SetCollisionResponseToAllChannels(ECR_Ignore);

	//setup arrow component
	VolumeSpawnerFlowArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Volume Spawner Flow Arrow Component"));
	VolumeSpawnerFlowArrow->SetupAttachment(RootComponent);

	//setup mesh component
	VolumeSpawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Volume Spawner Mesh Component"));
	VolumeSpawnerMesh->SetupAttachment(RootComponent);
	VolumeSpawnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VolumeSpawnerMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//set default number of boids to spawn and interval
	NumBoidsToSpawn = 0;
	SpawnInterval = 1.0f;

	//set type of spawner
	SpawnType = Burst;
}

void AVolumeSpawner::BeginPlay()
{
	Super::BeginPlay();

	//Bind spawn timer delegate
	SpawnTimerDelegate.BindUFunction(this, FName("SpawnBoids"), NumBoidsToSpawn);

	//spawn initial boid flock
	SpawnBoids(NumBoidsToSpawn);
}

void AVolumeSpawner::SpawnBoids(int32 NumBoids)
{
	//check for assigned flock manager
	if (AssignedFlockManager == nullptr)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("No FlockManager found for Boid spawner: %s."), *GetName());
		return;
	}

	//check BoidType is set in BP
	if (BoidType == nullptr)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("BoidType not set for Spawner: %s"), *GetName());
		return;
	}

	//spawn boids
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = VolumeSpawnerFlowArrow->GetComponentRotation();
	FActorSpawnParameters BoidSpawnParams;
	BoidSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BoidSpawnParams.Owner = AssignedFlockManager;

	for (int32 i = 0; i < NumBoids; ++i)
	{
		//spawn boids within volume
		SpawnLocation.X = FMath::FRandRange(-VolumeSpawnerCollision->GetScaledBoxExtent().X, VolumeSpawnerCollision->GetScaledBoxExtent().X);
		SpawnLocation.Y = FMath::FRandRange(-VolumeSpawnerCollision->GetScaledBoxExtent().Y, VolumeSpawnerCollision->GetScaledBoxExtent().Y);
		SpawnLocation.Z = FMath::FRandRange(-VolumeSpawnerCollision->GetScaledBoxExtent().Z, VolumeSpawnerCollision->GetScaledBoxExtent().Z);
		SpawnLocation += this->GetActorLocation();
		AssignedFlockManager->AddBoidToFlock(GetWorld()->SpawnActor<ABoid>(BoidType, SpawnLocation, SpawnRotation, BoidSpawnParams));
	}

	switch (SpawnType)
	{
	case Burst:
		//do nothing, 1 spawn and done
		break;
	case Flow:
		//set timer for continuous spawning over time
		GetWorldTimerManager().SetTimer(SpawnTimerHandle, SpawnTimerDelegate, SpawnInterval, false);
		break;
	default:
		break;
	}
}
