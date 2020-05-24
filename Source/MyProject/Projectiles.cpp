#include "Projectiles.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/ProjectileMovementComponent.h"

// life of projectiles namistit, plutaju u svemiru??
AProjectiles::AProjectiles(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);

	CollisionComp->InitSphereRadius(1.0f);

	RootComponent = CollisionComp;
	CollisionComp->SetSimulatePhysics(true);

	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 30000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//this->SetLifeSpan(2.0f); 
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
	if (OtherActor == this->GetOwner()->GetOwner())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
       
	}
	else if (OtherActor && (OtherActor != this) && OtherComp)
	{
		// ovo pise mene
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *OtherActor->GetName()));
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
		this->Destroy();  //definitivno unistiti projectile
	}
}


void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

