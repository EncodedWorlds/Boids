// Copyright ©2020 Samuel Harrison


#include "BoidCageSpawner.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boid.h"
#include "FlockManager.h"

ABoidCageSpawner::ABoidCageSpawner()
{
	//disable actor ticking
	PrimaryActorTick.bCanEverTick = false;

	//setup cage collision component
	CageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Cage Collision Component"));
	RootComponent = CageCollision;
	CageCollision->SetBoxExtent(FVector(2500));
	CageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CageCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	CageCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//setup cage mesh component
	CageMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cage Mesh Component"));
	CageMesh->SetupAttachment(RootComponent);
	CageMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CageMesh->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));

	//set initial flock size in cage
	NumBoidsToSpawn = 0;
}

void ABoidCageSpawner::BeginPlay()
{
	Super::BeginPlay();

	//bind collision component overlap event to delegate that gets called to perform logic
	if (CageCollision)
	{
		CageCollision->OnComponentEndOverlap.AddDynamic(this, &ABoidCageSpawner::OnCageOverlapEnd);
	}

	//spawn flock of boids
	SpawnBoids(NumBoidsToSpawn);
}

void ABoidCageSpawner::OnCageOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check if actor leaving cage is a valid boid and not being destroyed
	ABoid* EscapingBoid = Cast<ABoid>(OtherActor);
	if (EscapingBoid)
	{
		if (EscapingBoid->IsActorBeingDestroyed()) { return; }

		FVector NewBoidLocation = EscapingBoid->GetActorLocation();
		//check which side(s) of the cage the boid is leaving
		//exited front side
		if (EscapingBoid->GetActorLocation().X > this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X)
		{
			//move boid to back side
			NewBoidLocation.X = this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X;
			NewBoidLocation.Y = FMath::Clamp(NewBoidLocation.Y, this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y, this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y);
			NewBoidLocation.Z = FMath::Clamp(NewBoidLocation.Z, this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z, this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z);
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//exited back side
		else if (EscapingBoid->GetActorLocation().X < this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X)
		{
			//move boid to front side
			NewBoidLocation.X = this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X;
			NewBoidLocation.Y = FMath::Clamp(NewBoidLocation.Y, this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y, this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y);
			NewBoidLocation.Z = FMath::Clamp(NewBoidLocation.Z, this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z, this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z);
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//exited right side
		else if (EscapingBoid->GetActorLocation().Y > this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y)
		{
			//move boid to left side
			NewBoidLocation.X = FMath::Clamp(NewBoidLocation.X, this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X, this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X);
			NewBoidLocation.Y = this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y;
			NewBoidLocation.Z = FMath::Clamp(NewBoidLocation.Z, this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z, this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z);
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//exited left side
		else if (EscapingBoid->GetActorLocation().Y < this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y)
		{
			//move boid to right side
			NewBoidLocation.X = FMath::Clamp(NewBoidLocation.X, this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X, this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X);
			NewBoidLocation.Y = this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y;
			NewBoidLocation.Z = FMath::Clamp(NewBoidLocation.Z, this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z, this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z);
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//exited top side
		else if (EscapingBoid->GetActorLocation().Z > this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z)
		{
			//move boid to bottom side
			NewBoidLocation.X = FMath::Clamp(NewBoidLocation.X, this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X, this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X);
			NewBoidLocation.Y = FMath::Clamp(NewBoidLocation.Y, this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y, this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y);
			NewBoidLocation.Z = this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z;
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//exited bottom side
		else if (EscapingBoid->GetActorLocation().Z < this->GetActorLocation().Z - CageCollision->GetScaledBoxExtent().Z)
		{
			//move boid to top side
			NewBoidLocation.X = FMath::Clamp(NewBoidLocation.X, this->GetActorLocation().X - CageCollision->GetScaledBoxExtent().X, this->GetActorLocation().X + CageCollision->GetScaledBoxExtent().X);
			NewBoidLocation.Y = FMath::Clamp(NewBoidLocation.Y, this->GetActorLocation().Y - CageCollision->GetScaledBoxExtent().Y, this->GetActorLocation().Y + CageCollision->GetScaledBoxExtent().Y);
			NewBoidLocation.Z = this->GetActorLocation().Z + CageCollision->GetScaledBoxExtent().Z;
			EscapingBoid->SetActorLocation(NewBoidLocation);
		}
		//unexpected exit occurred, remove boid from flock, destroy it, and spawn new one
		else
		{
			EscapingBoid->GetFlockManager()->RemoveBoidFromFlock(EscapingBoid);
			GetWorld()->DestroyActor(EscapingBoid);
			SpawnBoids(1);
		}
	}
}

void ABoidCageSpawner::SpawnBoids(int32 NumBoids)
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
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters BoidSpawnParams;
	BoidSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	BoidSpawnParams.Owner = AssignedFlockManager;

	for (int i = 0; i < NumBoids; ++i)
	{
		//spawn boids randomly within cage
		SpawnLocation.X = FMath::FRandRange(-CageCollision->GetScaledBoxExtent().X, CageCollision->GetScaledBoxExtent().X);
		SpawnLocation.Y = FMath::FRandRange(-CageCollision->GetScaledBoxExtent().Y, CageCollision->GetScaledBoxExtent().Y);
		SpawnLocation.Z = FMath::FRandRange(-CageCollision->GetScaledBoxExtent().Z, CageCollision->GetScaledBoxExtent().Z);
		SpawnLocation += this->GetActorLocation();
		SpawnRotation = FMath::VRand().ToOrientationRotator();
		AssignedFlockManager->AddBoidToFlock(GetWorld()->SpawnActor<ABoid>(BoidType, SpawnLocation, SpawnRotation, BoidSpawnParams));
	}
}
