// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceshipWeapon.generated.h"

UCLASS()
class MYPROJECT_API ASpaceshipWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	ASpaceshipWeapon();

protected:

	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category="Components")
		class USkeletalMeshComponent* WeponMesh;
	UFUNCTION(BlueprintCallable, Category = "Shooting")
	void OnFire();


	/** Gun muzzle's offset from the camera location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	/** Projectile class to spawn */
	UPROPERTY(EditAnywhere, Category = "Shooting")
		TSubclassOf<class AProjectiles> ProjectileClass;

public:	
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly,Category = "Shooting")
		TSubclassOf<class UDamageType> DamageType;

};
