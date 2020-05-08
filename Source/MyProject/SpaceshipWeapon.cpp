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

		//FCollisionQueryParams QueryParams;
		FVector coordinate = this->WeponMesh->GetComponentLocation();
		coordinate.Y += 0.001f;
		FRotator ShootRotation = MyOwner->GetActorRotation();
		ShootRotation.Pitch += 10.f;
		FVector ShootDirection = ShootRotation.Vector();  
		
		FTransform SpawnTM(coordinate);  //lokacija za spawn 


		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Z: %f -x %f - %f"), ShootDirection.Z, ShootDirection.X));
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


