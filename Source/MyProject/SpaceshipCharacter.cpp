// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Controller.h"
#include "Engine/Engine.h"
#include <stdio.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectiles.h"
// Sets default values
ASpaceshipCharacter::ASpaceshipCharacter()
{
	// Create static mesh component
	this->SpaceshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipSkelet"));
//	this->SpaceshipSkeletalMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
	RootComponent = this->SpaceshipMesh;

	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	this->SpringArm->SetupAttachment(RootComponent);
	this->SpringArm->TargetArmLength = 250.0f;
	this->SpringArm->bEnableCameraLag = false;
	this->SpringArm->CameraLagSpeed = 15.0f;

	this->Camera = CreateDefaultSubobject<UCameraComponent>("CameraCharacter");
	this->Camera->SetupAttachment(SpringArm);

	SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
	SpringArm->bEnableCameraLag = false;	// Do not allow camera to lag
	SpringArm->CameraLagSpeed = 15.f;

	Acceleration = 500.f;
	TurnSpeed = 70.f;
	MaxSpeed = 4000.f;
	MinSpeed = 0.f;
	CurrentForwardSpeed = 0.f;

	//ProjectileClass = CreateDefaultSubobject<AProjectiles>("Projectiless");
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

	if (this->CurrentForwardSpeed > 0)
	{
		this->CurrentForwardSpeed = this->CurrentForwardSpeed - (this->CurrentForwardSpeed / 5);
	}
	if (this->CurrentLRSpeed > 0)
	{
		this->CurrentLRSpeed = this->CurrentLRSpeed - (this->CurrentLRSpeed / 5);
	}
	Super::Tick(DeltaSeconds);

}

// Called to bind functionality to input
void ASpaceshipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	// Bind our control axis' to callback functions
	PlayerInputComponent->BindAxis("MoveFoward", this, &ASpaceshipCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveAround", this, &ASpaceshipCharacter::MoveAround);
	PlayerInputComponent->BindAxis("Speed", this, &ASpaceshipCharacter::Speed);

	//napravit svoju funkciju za Yaw
	PlayerInputComponent->BindAxis("TurnYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnPitch", this, &ASpaceshipCharacter::TurnPitch);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceshipCharacter::RollMovement);

//	InputComponent->BindAction("Shoot", IE_Pressed, this, &ASpaceshipCharacter::OnFire);

}

void ASpaceshipCharacter::MoveFoward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
				
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, value);
		CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, value*Acceleration, GetWorld()->GetDeltaSeconds(), 2.f);
	}


}

void ASpaceshipCharacter::MoveAround(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		
		AddMovementInput(Direction, value);
		CurrentLRSpeed = 50.f * Acceleration * GetWorld()->GetDeltaSeconds() * value;
	
	}
	
}
void ASpaceshipCharacter::Speed(float value)
{
	//stajanje bas ne radi nesto??
	bool bHasInput = !FMath::IsNearlyEqual(value, 0.f);
	float CurrentAcc = (value * Acceleration);
	float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * CurrentAcc);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Speed: %f - %f - %f"), CurrentForwardSpeed, CurrentYawSpeed, CurrentRollSpeed));

	CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, MinSpeed, MaxSpeed);
}

void ASpaceshipCharacter::TurnPitch(float value)
{
	    
	float TargetPitchSpeed = (value * TurnSpeed * 5
		);
	TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed));
	CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 5.f);

}

void ASpaceshipCharacter::RollMovement(float value)
{
	// Target yaw speed is based on input
	float TargetYawSpeed = (value * TurnSpeed);

	// Smoothly interpolate to target yaw speed
	CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

	// Is there any left/right input?
	const bool bIsTurning = FMath::Abs(value) > 0.2f;

	// If turning, yaw value is used to influence roll
	// If not turning, roll to reverse current roll value.
	float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) : (GetActorRotation().Roll * -2.f);

	// Smoothly interpolate roll speed
	CurrentRollSpeed = FMath::FInterpTo(CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ASpaceshipCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		// Get the camera transform
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRot;
		MuzzleRotation.Pitch += 10.0f;          // skew the aim upwards a bit
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			AProjectiles* const Projectile = World->SpawnActor<AProjectiles>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// find launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}


}

