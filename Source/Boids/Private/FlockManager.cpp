// Copyright ©2020 Samuel Harrison

//includes
#include "FlockManager.h"
#include "Components/BillboardComponent.h"
#include "Boid.h"
#include "Kismet/KismetMathLibrary.h"

AFlockManager::AFlockManager()
{
	//disable ticking
	PrimaryActorTick.bCanEverTick = false;

	//setup billboard visual component
	FlockManagerBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("FlockManager Billboard Component"));
	RootComponent = FlockManagerBillboard;

	//TODO: set to "general" preset settings that are a good starting default
	//set speed settings
	MaxSpeed = 700.0f;
	MinSpeed = 300.0f;

	//default behavioral steering strengths
	AlignmentStrength = 200.0f;
	SeparationStrength = 30.0f;
	CohesionStrength = 5.0f;
	AvoidanceStrength = 10000.0f;

	//default perception angles
	SeparationFOV = -1.0f;
	AlignmentFOV = 0.5f;
	CohesionFOV = -0.5f;

	//default avoidance properties
	NumSensors = 100;
	SensorRadius = 300.0f;
	BuildAvoidanceSensors();
}

void AFlockManager::BeginPlay()
{
	Super::BeginPlay();
}

void AFlockManager::AddBoidToFlock(ABoid* Boid)
{
	if (Boid)
	{
		//add new boid to flock
		BoidsInFlock.AddUnique(Boid);
	}
}

void AFlockManager::RemoveBoidFromFlock(ABoid* Boid)
{
	if (Boid)
	{
		//remove despawned boid from flock
		BoidsInFlock.Remove(Boid);
	}
}

void AFlockManager::SetMaxSpeed(float NewMaxSpeed)
{
	if (NewMaxSpeed < 0)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("Request to change Boid Max Speed to negative value ignored in FlockManager: %s."), *GetName());
		return;
	}

	//update max speed to new setting
	MaxSpeed = NewMaxSpeed;

	//check if new Max is lower than Min, lower Min to new speed to avoid Min > Max error behavior
	if (MaxSpeed < MinSpeed)
	{
		MinSpeed = MaxSpeed;
	}
}

void AFlockManager::SetMinSpeed(float NewMinSpeed)
{
	if (NewMinSpeed < 0)
	{
		//log warning to console
		UE_LOG(LogTemp, Warning, TEXT("Request to change Boid Max Speed to negative value ignored in FlockManager: %s."), *GetName());
		return;
	}

	//update min speed to new setting
	MinSpeed = NewMinSpeed;

	//check if new Min is higher than Max, raise Max to new speed to avoid Min > Max error behavior
	if (MinSpeed > MaxSpeed)
	{
		MaxSpeed = MinSpeed;
	}
}

void AFlockManager::SetAlignmentStrength(float NewAlignmentStrength)
{
	AlignmentStrength = NewAlignmentStrength;
}

void AFlockManager::SetSeparationStrength(float NewSeparationStrength)
{
	SeparationStrength = NewSeparationStrength;
}

void AFlockManager::SetCohesionStrength(float NewCohesionStrength)
{
	CohesionStrength = NewCohesionStrength;
}

void AFlockManager::SetAvoidanceStrength(float NewAvoidanceStrength)
{
	AvoidanceStrength = NewAvoidanceStrength;
}

void AFlockManager::BuildAvoidanceSensors()
{
	//empty sensor array
	AvoidanceSensors.Empty();

	//theta angle of rotation on xy plane around z axis (yaw) around sphere
	float theta;
	//phi angle of rotation (~pitch) around sphere
	float phi;
	//assuming unit sphere (radius = 1) to simplify calculations
	//direction vector pointing from the center to point on sphere surface
	FVector SensorDirection;

	for (int32 i = 0; i < NumSensors; ++i)
	{
		//calculate the spherical coordinates of the direction vectors endpoint
		theta = 2 * UKismetMathLibrary::GetPI() * GoldenRatio * i;
		phi = FMath::Acos(1 - (2 * float(i) / NumSensors));

		//convert point on sphere to cartesian coordinates xyz
		SensorDirection.X = FMath::Cos(theta)*FMath::Sin(phi);
		SensorDirection.Y = FMath::Sin(theta)*FMath::Sin(phi);
		SensorDirection.Z = FMath::Cos(phi);
		//add direction to list of sensors for avoidance
		AvoidanceSensors.Emplace(SensorDirection);
	}
}

