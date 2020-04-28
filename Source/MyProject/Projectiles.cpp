#include "Projectiles.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectiles::AProjectiles(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);

	CollisionComp->InitSphereRadius(15.0f);

	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
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
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
	}
}

// Called when the game starts or when spawned
void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

