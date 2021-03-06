// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//An actor that can be placed in the world to attract/repel Boids by applying steering forces on all Boids within its range.
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetObject.generated.h"

//forward declares
class USphereComponent;
class UStaticMeshComponent;
class ABoid;

UCLASS()
class BOIDS_API ATargetObject : public AActor
{
	GENERATED_BODY()

public:
	//default constructor
	ATargetObject();

protected:
	//setup logic called when level starts or spawned
	virtual void BeginPlay() override;

public:
	//called to update each frame
	virtual void Tick(float DeltaTime) override;

	//COMPONENTS
protected:
	//target collision component
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	USphereComponent* TargetCollision;

	//target static mesh component for visuals
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UStaticMeshComponent* TargetMesh;

	//OVERLAP EVENT DELEGATES
protected:
	//delegate for target collision component event OnBeginOverlap
	UFUNCTION()
	void OnTargetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	//delegate for target collision component event OnEndOverlap
	UFUNCTION()
	void OnTargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//BOID FORCES
protected:
	//force strength, (+)attract, (-)repel
	UPROPERTY(EditAnywhere, Category = "Boid|TargetForce")
	float TargetForceStrength;

	//set force to scale with distance from boid to target (i.e. closer to target, stronger the force)
	UPROPERTY(EditAnywhere, Category = "Boid|TargetForce")
	bool bProximityScaled;

	//set if proximity scaling is inverted (i.e. further from target yields stronger force)
	UPROPERTY(EditAnywhere, Category = "Boid|TargetForce")
	bool bInvertProximityScaling;

	//set if force has a range or limitless range (i.e. only affects overlapping boids or all boids universally)
	UPROPERTY(EditAnywhere, Category = "Boid|TargetForce")
	bool bHasRange;

	//list of boids in range
	TArray<ABoid*> BoidsInRange;

	//add target forces to the affected boids
	void ApplyTargetForces();
};
