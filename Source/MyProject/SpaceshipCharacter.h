#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpaceshipCharacter.generated.h"

UCLASS()
class MYPROJECT_API ASpaceshipCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASpaceshipCharacter();

	virtual FVector GetPawnViewLocation() const override;

	UPROPERTY(Category = "Camera", EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(Category = "Camera", EditAnywhere)
		class UCameraComponent* Camera; 
	UPROPERTY(Category = "Body", VisibleAnywhere, BluePrintReadOnly)
		class USkeletalMeshComponent* SpaceshipMesh;
	UPROPERTY(Category = "Game", VisibleAnywhere, BluePrintReadOnly)
		int32 TotalScore;
	UPROPERTY(Category = "Game", VisibleAnywhere, BluePrintReadOnly)
		int32 LevelScore;


	UFUNCTION(BlueprintCallable)
		void SetScore(int32 Total);
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void MoveFoward(float value);
	void MoveAround(float value);
	void Speed(float value);
	void TurnRate(float value);
	void TurnPitch(float value);
	void PitchMovement(float value);
	void YawMovement(float value);
	void RollMovement(float value);
	
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
