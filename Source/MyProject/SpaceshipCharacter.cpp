// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceshipCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "CrosshairHUD.h"
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
#include <exception>
#include "GameFramework/CharacterMovementComponent.h"
#include "Projectiles.h"
// Sets default values
ASpaceshipCharacter::ASpaceshipCharacter()
{
	// Create static mesh component
	//this->SpaceshipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipSkelet"));
//	this->SpaceshipSkeletalMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());	// Set static mesh
//	RootComponent = this->SpaceshipMesh;

	this->SpringArm = CreateDefaultSubobject<USpringArmComponent>("ArmComponent");
	this->SpringArm->SetupAttachment(RootComponent);
	this->Camera = CreateDefaultSubobject<UCameraComponent>("CameraCharacter");
	this->Camera->SetupAttachment(SpringArm);


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

	/*if (this->CurrentForwardSpeed > 0)
	{
		this->CurrentForwardSpeed = this->CurrentForwardSpeed - (this->CurrentForwardSpeed / 5);
	}
	if (this->CurrentLRSpeed > 0)
	{
		this->CurrentLRSpeed = this->CurrentLRSpeed - (this->CurrentLRSpeed / 5);
	}*/
	Super::Tick(DeltaSeconds);

}


void ASpaceshipCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveFoward", this, &ASpaceshipCharacter::MoveFoward);
	PlayerInputComponent->BindAxis("MoveAround", this, &ASpaceshipCharacter::MoveAround);
	PlayerInputComponent->BindAxis("Speed", this, &ASpaceshipCharacter::Speed);

	//napravit svoju funkciju za Yaw
	PlayerInputComponent->BindAxis("TurnYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnPitch", this, &ASpaceshipCharacter::TurnPitch);
	PlayerInputComponent->BindAxis("Roll", this, &ASpaceshipCharacter::RollMovement);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASpaceshipCharacter::OnFire);

}

void ASpaceshipCharacter::MoveFoward(float value)
{
	if ((Controller != NULL) && (value != 0.0f))
	{
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

void ASpaceshipCharacter::OnFire()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("USLO")));

	if (ProjectileClass1 != NULL && ProjectileClass2 != NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Speed: %f - %f - %f"), CurrentForwardSpeed, CurrentYawSpeed, CurrentRollSpeed));
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the camera to find the final muzzle position
		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRot;
		MuzzleRotation.Pitch += 10.0f;
		UWorld* const World = GetWorld();

		if (World)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %f - %f - %f"), CurrentForwardSpeed, CurrentYawSpeed, CurrentRollSpeed));

			FVector coordinate1 = GetActorLocation();
			FVector coordinate2 = GetActorLocation();
			coordinate1.X += 25;
			coordinate2.X += 25;
			coordinate1.Y += -55;
			coordinate2.Y += 55;

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AProjectiles* const Projectile1 = World->SpawnActor<AProjectiles>(this->ProjectileClass1, coordinate1, MuzzleRotation, SpawnParams);
			AProjectiles* const Projectile2 = World->SpawnActor<AProjectiles>(this->ProjectileClass2, coordinate2, MuzzleRotation, SpawnParams);

			if (Projectile1)
			{
				// find launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile1->InitDirection(LaunchDir);
				Projectile2->InitDirection(LaunchDir);
			}
		}
	}


}

float ASpaceshipCharacter::getCurrentForwardSpeed()
{
	return CurrentForwardSpeed;
}

