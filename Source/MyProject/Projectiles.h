// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles.generated.h"

UCLASS()
class MYPROJECT_API AProjectiles : public AActor
{
	GENERATED_BODY()
	
public:	

	AProjectiles();

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		class USphereComponent* SphereCollisionComponent;

	/** Projectile movement component */
	UPROPERTY(Category = "Movement",VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(Category = "Body", VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float DamageAmount;

	/** inits velocity of the projectile in the shoot direction */
	void InitDirection(const FVector& ShootDirection);

	/** called when projectile hits something */
	UFUNCTION()
		void OnHit(class UPrimitiveComponent* HitComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
