// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//An actor that can be placed in the world that despawns Boids that enter it.
//Used with attraction forces from BoidTargetObjects to "pull" Boids into it and emulate flock leaving the world (i.e. entering nest, exiting migration area, etc.).
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VolumeDespawner.generated.h"

//forward declares
class UBoxComponent;
class UStaticMeshComponent;
class ABoid;

UCLASS()
class BOIDS_API AVolumeDespawner : public AActor
{
	GENERATED_BODY()

public:
	//default constructor
	AVolumeDespawner();

protected:
	//called on level start or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
protected:
	//box component detects boid collision and processes it for level "exit" and deletion
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UBoxComponent* VolumeDespawnerCollision;

	//static mesh component used for visuals of boid volume despawner actor
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* VolumeDespawnerMesh;

	//OVERLAP BEGIN EVENT DELEGATE
protected:
	//delegate for target collision component event OnBeginOverlap
	UFUNCTION()
	void OnOutletOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
