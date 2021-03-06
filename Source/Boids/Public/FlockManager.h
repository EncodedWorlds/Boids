// Copyright ©2020 Samuel Harrison

//README:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Actor placed in the level that stores the perception and steering settings of the boids it controls.
//Used as a way to manipulate the behavior of the entire flock and optimize flock-wide logic changes.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#pragma once

//includes
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlockManager.generated.h"

//forward declares
class UBillboardComponent;
class ABoid;

UCLASS()
class BOIDS_API AFlockManager : public AActor
{
	GENERATED_BODY()

public:
	//default constructor
	AFlockManager();

protected:
	//setup logic called when level starts or spawned
	virtual void BeginPlay() override;

	//COMPONENTS
protected:
	//billboard visuals component
	UPROPERTY(VisibleAnywhere, Category = "Boid|Components")
	UBillboardComponent* FlockManagerBillboard;

	//BOID FLOCK
protected:
	//list of boids in flock
	TArray<ABoid*> BoidsInFlock;

public:
	//adding and removing boids to/from flock when spawned or despawned
	void AddBoidToFlock(ABoid* Boid);
	void RemoveBoidFromFlock(ABoid* Boid);

	//MOVEMENT
	//TODO: add property listener or logic check to ensure max !< min or min !> max when changed in editor
	//TODO: move to a locomotion class/component
protected:
	UPROPERTY(EditAnywhere, Category = "Boid|Movement", meta = (ClampMin = "0.0"))
	float MaxSpeed;
	UPROPERTY(EditAnywhere, Category = "Boid|Movement", meta = (ClampMin = "0.0"))
	float MinSpeed;

public:
	//movement property getters + setters
	UFUNCTION(BlueprintCallable, Category = "Boid|Movement")
	inline float GetMaxSpeed() { return MaxSpeed; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Movement")
	inline float GetMinSpeed() { return MinSpeed; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Movement")
	void SetMaxSpeed(float NewMaxSpeed);
	UFUNCTION(BlueprintCallable, Category = "Boid|Movement")
	void SetMinSpeed(float NewMinSpeed);

	//TODO: logic to transform boid movement from 3d <-> 2d
	//void SetMovementDim(bool b2D){for (ABoid* Boid : BoidsInFlock) Boid->SetActorLocation(x,y, z = 0)}
	//need 2d version of all perception, steering and boid movement logic

	//STEERING
protected:
	//behavioral steering force strengths
	//TODO: add clamping to logical values on changes in editor meta = (ClampMin = "X", ClampMax = "Y")
	UPROPERTY(EditAnywhere, Category = "Boid|Steering")
	float AlignmentStrength;
	UPROPERTY(EditAnywhere, Category = "Boid|Steering")
	float SeparationStrength;
	UPROPERTY(EditAnywhere, Category = "Boid|Steering")
	float CohesionStrength;
	UPROPERTY(EditAnywhere, Category = "Boid|Steering")
	float AvoidanceStrength;

public:
	//steering property getters + setters
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	inline float GetAlignmentStrength() { return AlignmentStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	inline float GetSeparationStrength() { return SeparationStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	inline float GetCohesionStrength() { return CohesionStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	inline float GetAvoidanceStrength() { return AvoidanceStrength; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	void SetAlignmentStrength(float NewAlignmentStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	void SetSeparationStrength(float NewSeparationStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	void SetCohesionStrength(float NewCohesionStrength);
	UFUNCTION(BlueprintCallable, Category = "Boid|Steering")
	void SetAvoidanceStrength(float NewAvoidanceStrength);

	//PERCEPTION
protected:
	//determines the field of view of each perception field to determine if a flockmate is sensed (1.0 boid can only sense things directly in front of it, -1 boid can sense in all directions)
	UPROPERTY(EditAnywhere, Category = "Boid|Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float SeparationFOV;
	UPROPERTY(EditAnywhere, Category = "Boid|Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float AlignmentFOV;
	UPROPERTY(EditAnywhere, Category = "Boid|Perception", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float CohesionFOV;

public:
	//getters + setters
	UFUNCTION(BlueprintCallable, Category = "Boid|Perception")
	inline float GetSeparationFOV() { return SeparationFOV; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Perception")
	inline float GetAlignmentFOV() { return AlignmentFOV; };
	UFUNCTION(BlueprintCallable, Category = "Boid|Perception")
	inline float GetCohesionFOV() { return CohesionFOV; };
	//TODO: add setters for perception FOV's

	//TODO: add logic to get and set boid's perception sensor component radius

	//AVOIDANCE
protected:
	//number of avoidance sensors
	UPROPERTY(EditAnywhere, Category = "Boid|Avoidance", meta = (ClampMin = "0", ClampMax = "1000"))
	int32 NumSensors;
	//golden ratio constant used for spacing the packing points onto the sphere
	const float GoldenRatio = (1.0f + FMath::Sqrt(5.0f)) / 2;
	//range of the avoidance collision sensors
	UPROPERTY(EditAnywhere, Category = "Boid|Avoidance", meta = (ClampMin = "0"))
	float SensorRadius;
	//avoidance sensor directions
	TArray<FVector> AvoidanceSensors;
	//creates the directions that the avoidance sensors point
	void BuildAvoidanceSensors();

public:
	//getters + setters
	inline float GetSensorRadius() { return SensorRadius; }
	inline TArray<FVector> GetAvoidanceSensors() { return AvoidanceSensors; }

	//TODO: add getters and setters for NumSensors, Sensor Radius, etc.

	//FLOCK RESET
	//TODO: add reset function to destroy current flock and generate new one using assigned spawners

	//FLOCK PRESETS
	//TODO: add flock presets that store settings for "general" boid behavior, can be used to reset boid at good starting settings
};
