// Fill out your copyright notice in the Description page of Project Settings.


#include "AHealthComponent.h"

// Sets default values for this component's properties
UAHealthComponent::UAHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	this->DefaultHealth = 100;
	this->MainHealth = this->DefaultHealth;
}


// Called when the game starts
void UAHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UAHealthComponent::TakeDamage);
	}
}

void UAHealthComponent::TakeDamage( AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	this->MainHealth = FMath::Clamp(this->MainHealth - Damage, 0.0f, this->DefaultHealth);

}



