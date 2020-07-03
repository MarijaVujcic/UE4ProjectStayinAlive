#include "Projectiles.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectiles::AProjectiles()
{
	PrimaryActorTick.bCanEverTick = true;
	this->SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComp");
	
	this->SphereCollisionComponent->InitSphereRadius(1.0f);

	this->RootComponent = SphereCollisionComponent;
	this->SphereCollisionComponent->SetSimulatePhysics(true);
	this->ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMeshComponent");
	this->ProjectileMesh->SetupAttachment(this->RootComponent);

	this->ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileComp");
	this->ProjectileMovement->UpdatedComponent = SphereCollisionComponent;
	this->ProjectileMovement->InitialSpeed = 10000.f;
	this->ProjectileMovement->MaxSpeed = 30000.f;
	this->ProjectileMovement->bRotationFollowsVelocity = true;
	this->ProjectileMovement->bShouldBounce = true;
	this->ProjectileMovement->Bounciness = 0.3f;

	this->SphereCollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	this->SphereCollisionComponent->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);
}

void AProjectiles::InitDirection(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}

void AProjectiles::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && (OtherActor != this) && OtherActor && (OtherActor != this->GetOwner()->GetOwner()))
	{
		TSubclassOf<UDamageType> DamageType;
		AController* ControllerDamage = this->GetOwner()->GetOwner()->GetInstigatorController();
		AActor* Damager = GetInstigator();
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(this->DamageAmount, DamageEvent, ControllerDamage, this);
	}
	this->Destroy();
}

void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
}
void AProjectiles::Tick(float DeltaTime)
{

}