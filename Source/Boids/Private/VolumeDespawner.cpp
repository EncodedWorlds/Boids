// Copyright ©2020 Samuel Harrison

//includes
#include "VolumeDespawner.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Boid.h"
#include "FlockManager.h"

AVolumeDespawner::AVolumeDespawner()
{
	//disable ticking
	PrimaryActorTick.bCanEverTick = false;

	//setup box component
	VolumeDespawnerCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume Despawner Collision Component"));
	RootComponent = VolumeDespawnerCollision;
	VolumeDespawnerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	VolumeDespawnerCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	VolumeDespawnerCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//setup static mesh component
	VolumeDespawnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Volume Despawner Mesh Component"));
	VolumeDespawnerMesh->SetupAttachment(RootComponent);
	VolumeDespawnerMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	VolumeDespawnerMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AVolumeDespawner::BeginPlay()
{
	Super::BeginPlay();

	//bind collision component overlap event to delegate that gets called to perform despawn logic
	if (VolumeDespawnerCollision)
	{
		VolumeDespawnerCollision->OnComponentBeginOverlap.AddDynamic(this, &AVolumeDespawner::OnOutletOverlapBegin);
	}
}

void AVolumeDespawner::OnOutletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//check if entering actor is a boid and valid
	ABoid* EnteringBoid = Cast<ABoid>(OtherActor);
	if (EnteringBoid)
	{
		//remove boid from flock and destroy it
		EnteringBoid->GetFlockManager()->RemoveBoidFromFlock(EnteringBoid);
		EnteringBoid->Destroy();
	}
}
