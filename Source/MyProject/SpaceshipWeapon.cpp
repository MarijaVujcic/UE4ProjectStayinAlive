// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceshipWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectiles.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASpaceshipWeapon::ASpaceshipWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->WeponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshWepon"));
	RootComponent = this->WeponMesh;

}



void ASpaceshipWeapon::OnFire()
{
	//mozda u Ifu provjerit

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ON FIRE")));
	AActor* MyOwner = GetOwner();  //spaceship
	UWorld* const World = GetWorld();
	if (MyOwner && this->ProjectileClass != NULL && World)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); //vrati mi lokaciju i roticaiju kamere "ispuni"

		FVector TraceEnd = EyeLocation + (EyeRotation.Vector()+1);
		FHitResult Hit;
		Hit.Actor = this->ProjectileClass;
		

		FCollisionQueryParams QueryParams;
		FVector coordinate1 = this->WeponMesh->GetComponentLocation();
		coordinate1.Y += 0.001f;

		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		FVector ShootRotation = EyeRotation.Vector();
		if (GetWorld()->LineTraceSingleByChannel(Hit, coordinate1, TraceEnd, ECC_Visibility, QueryParams))
		{
			//udarilo je u nesto pa damage
			//AActor *HitActor = Hit.GetActor();
			//UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShootRotation, Hit, MyOwner->GetInstigatorController(), this, this->DamageType);

		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed: %f - %f - %f"), coordinate1.X));
		AProjectiles* const Projectile1 = World->SpawnActor<AProjectiles>(this->ProjectileClass, coordinate1, EyeRotation, SpawnParams);

		if (Projectile1)
		{
			Projectile1->InitDirection(ShootRotation+EyeLocation);
		}
	

	}
	

}


