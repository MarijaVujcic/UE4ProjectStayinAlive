// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceshipWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Projectiles.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

ASpaceshipWeapon::ASpaceshipWeapon()
{
	this->WeponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshWepon"));
	RootComponent = this->WeponMesh;
}

void ASpaceshipWeapon::OnFire()
{
	AActor* MyOwner = GetOwner();
	UWorld* const World = GetWorld();

	if (MyOwner && this->ProjectileClass != NULL && World)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); //vrati mi lokaciju i roticaiju kamere "ispuni"

		FVector coordinate = this->WeponMesh->GetComponentLocation();
		coordinate.Y += 0.001f;
		FRotator ShootRotation = MyOwner->GetActorRotation();
		ShootRotation.Pitch += 10.f;
		FVector ShootDirection = ShootRotation.Vector();  
		FTransform SpawnTM(coordinate);  //lokacija za spawn 

		AProjectiles* const Projectile = Cast<AProjectiles>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM)); // SPAWN PROJECTILE

		if (Projectile)
		{
			Projectile->Instigator = Instigator;
			Projectile->SetOwner(this);
			Projectile->InitDirection(ShootDirection); //init velocity
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
			
		}

	}
	
}


