// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceshipCharacter.generated.h"

UCLASS()
class MYPROJECT_API ASpaceshipCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpaceshipCharacter();

	UPROPERTY(Category = "Camera", EditAnywhere)
		class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = "Camera", EditAnywhere)
		class UCameraComponent* Camera;

	/*UPROPERTY(Category = "Body", BlueprintReadWrite)
		class UStaticMeshComponent* SpaceshipMesh;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* NoseThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* TailThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* LeftWingRoolThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* RightWingRoolThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* RollThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* NoseYawThrust;

	UPROPERTY(Category = "Physic", VisibleDefaultsOnly, BlueprintReadWrite)
		class APhysicsThruster* TailYawThrust;*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//functions for movments
	void MoveFoward(float value);
	void MoveAround(float value);
	void Speed(float value);
	void TurnRate(float value);
	void TurnPitch(float value);
	void PitchMovement(float value);
	void YawMovement(float value);
	void RollMovement(float value);
	
	UFUNCTION()
		void OnFire();
	/** Gun muzzle's offset from the camera location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere , Category = Projectile)
		TSubclassOf<class AProjectiles> ProjectileClass1;
	UPROPERTY(EditAnywhere, Category = Projectile)
		TSubclassOf<class AProjectiles> ProjectileClass2;
	float getCurrentForwardSpeed();

private:

	/** How quickly forward speed changes */
	UPROPERTY(Category = Plane, EditAnywhere)
		float Acceleration;

	/** How quickly pawn can steer */
	UPROPERTY(Category = Plane, EditAnywhere)
		float TurnSpeed;

	/** Max forward speed */
	UPROPERTY(Category = Pitch, EditAnywhere)
		float MaxSpeed;

	/** Min forward speed */
	UPROPERTY(Category = Yaw, EditAnywhere)
		float MinSpeed;

	/** Current forward speed */
	UPROPERTY(Category = Yaw, EditAnywhere)
	float CurrentForwardSpeed;

	/** Current yaw speed */
	float CurrentYawSpeed;

	/** Current pitch speed */
	float CurrentPitchSpeed;

	/** Current roll speed */
	float CurrentRollSpeed;

	/* Current Left Right speed*/
	float CurrentLRSpeed;

	float PitchValue;
	float YawValue;
	float RollValue;
	bool IsFoward;
};
