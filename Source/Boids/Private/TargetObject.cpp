// Copyright ©2020 Samuel Harrison

//includes
#include "TargetObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boid.h"

ATargetObject::ATargetObject()
{
	//enable ticking
	PrimaryActorTick.bCanEverTick = true;

	//setup collision component
	TargetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Target Collision Component"));
	RootComponent = TargetCollision;
	TargetCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TargetCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//setup mesh component
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Target Mesh Component"));
	TargetMesh->SetupAttachment(RootComponent);
	TargetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TargetMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//setup target force
	TargetForceStrength = 0.0f;
	bProximityScaled = false;
	bInvertProximityScaling = false;
	bHasRange = true;
}

void ATargetObject::BeginPlay()
{
	Super::BeginPlay();

	//bind collision component overlap events to delegates
	if (TargetCollision)
	{
		TargetCollision->OnComponentBeginOverlap.AddDynamic(this, &ATargetObject::OnTargetOverlapBegin);
		TargetCollision->OnComponentEndOverlap.AddDynamic(this, &ATargetObject::OnTargetOverlapEnd);
	}
}

void ATargetObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//apply target force to affected boids
	ApplyTargetForces();
}

void ATargetObject::OnTargetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//check if actor entering target range is a boid and valid
	ABoid* EnteringBoid = Cast<ABoid>(OtherActor);
	if (EnteringBoid)
	{
		BoidsInRange.Add(EnteringBoid);
	}
}

void ATargetObject::OnTargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//check if actor exiting target range is a boid and valid
	ABoid* ExitingBoid = Cast<ABoid>(OtherActor);
	if (ExitingBoid)
	{
		BoidsInRange.Remove(ExitingBoid);
	}
}

void ATargetObject::ApplyTargetForces()
{
	FVector TargetForce = FVector::ZeroVector;

	if (bHasRange)
	{
		//get list of overlapping boids
		for (ABoid* Boid : BoidsInRange)
		{
			//get unscaled force
			TargetForce = this->GetActorLocation() - Boid->GetActorLocation();
			TargetForce = TargetForce.GetSafeNormal();
			TargetForce *= TargetForceStrength;
			TargetForce -= Boid->GetBoidVelocity();

			//add force to boid target force stack
			Boid->AddTargetForce(TargetForce);
			//TODO: add distance factor
			if (bProximityScaled)
			{
				if (bInvertProximityScaling)
				{

				}
			}
		}
	}
	else
	{
		//get ALL boids

		//TODO: calculate unscaled force
		//TODO: add distance factor
	}
}
