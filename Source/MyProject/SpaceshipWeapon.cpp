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
		FVector Coordinate = this->WeponMesh->GetComponentLocation();
		Coordinate.Y += 0.001f;
		FRotator ShootRotation = MyOwner->GetActorRotation();
		ShootRotation.Pitch += 10.f;
		FVector ShootDirection = ShootRotation.Vector();  
		FTransform SpawnTM(Coordinate);

		AProjectiles* const Projectile = Cast<AProjectiles>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, ProjectileClass, SpawnTM));

		if (Projectile)
		{
			Projectile->Instigator = Instigator;
			Projectile->SetOwner(this);
			Projectile->InitDirection(ShootDirection); 
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTM);
		}
	}
}


