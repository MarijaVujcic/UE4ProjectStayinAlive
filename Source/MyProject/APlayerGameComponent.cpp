#include "APlayerGameComponent.h"

UAPlayerGameComponent::UAPlayerGameComponent()
{

	this->DefaultHealth = 100;
	this->LevelScore = 0;
	this->MainHealth = this->DefaultHealth;
}

void UAPlayerGameComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UAPlayerGameComponent::TakeDamage);
	}
}

// pogledat ovo take damage cini se malo cudno sa onim u projectiles
void UAPlayerGameComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}
	this->MainHealth = FMath::Clamp(this->MainHealth - Damage, 0.0f, this->DefaultHealth);
}



