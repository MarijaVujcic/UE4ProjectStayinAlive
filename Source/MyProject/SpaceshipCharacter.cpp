
#include "SpaceshipCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "ShooterGame.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"
#include "APlayerGameComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectiles.h"


ASpaceshipCharacter::ASpaceshipCharacter()
{
	this->SpaceshipMesh = GetMesh();
	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	this->SpringArm->SetupAttachment(RootComponent);
	this->Camera = CreateDefaultSubobject<UCameraComponent>("CameraCharacter");
	this->Camera->SetupAttachment(SpringArm);
	this->GameComponent = CreateDefaultSubobject<UAPlayerGameComponent>("GameComponent");
	this->Acceleration = 500.f;
	this->TurnSpeed = 70.f;
	this->MaxSpeed = 10000.f;
	this->MinSpeed = 0.f;
	this->CurrentForwardSpeed = 0.f;
	this->CurrentLRSpeed = 0.f;
	this->CurrentPitchSpeed = 0.f;
	this->CurrentRollSpeed = 0.f;
	this->CurrentYawSpeed = 0.f;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ASpaceshipCharacter::Tick(float DeltaSeconds)
{
	FVector LocalMove = FVector(CurrentForwardSpeed * DeltaSeconds,  CurrentLRSpeed * DeltaSeconds, 0.f);

	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation(0, 0, 0);
	DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
	DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
	DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

	AddActorLocalRotation(DeltaRotation);
	Super::Tick(DeltaSeconds);

}

void ASpaceshipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFoward", this, &ASpaceshipCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveAround", this, &ASpaceshipCharacter::MoveAround);
	PlayerInputComponent->BindAxis("Speed", this, &ASpaceshipCharacter::Speed);

	PlayerInputComponent->BindAxis("TurnYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnPitch", this, &ASpaceshipCharacter::TurnPitch);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceshipCharacter::RollMovement);
}

void ASpaceshipCharacter::MoveFoward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();
				
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
		CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, value*Acceleration, GetWorld()->GetDeltaSeconds(), 2.f);
	}


}

void ASpaceshipCharacter::MoveAround(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		
		AddMovementInput(Direction, value);
		CurrentLRSpeed = 10.f * Acceleration * GetWorld()->GetDeltaSeconds() * value;
	}
	
}

void ASpaceshipCharacter::Speed(float value)
{
	bool bHasInput = !FMath::IsNearlyEqual(value, 0.f);
	float CurrentAcc = (value * Acceleration);
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ASpaceshipCharacter::TurnPitch(float value)
{
	    
	float TargetPitchSpeed = (value * TurnSpeed * 5);
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed));
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 5.f);

}

void ASpaceshipCharacter::RollMovement(float value)
{

	float TargetYawSpeed = (value * TurnSpeed);

	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	const bool bIsTurning = FMath::Abs(value) > 0.2f;
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

FVector ASpaceshipCharacter::GetPawnViewLocation() const
{
	if (this->Camera)
	{
		return this->Camera->GetComponentLocation();

	}
	return Super::GetPawnViewLocation();
}

void ASpaceshipCharacter::SetScore(int32 Total)
{
	this->TotalScore = Total;;
}
