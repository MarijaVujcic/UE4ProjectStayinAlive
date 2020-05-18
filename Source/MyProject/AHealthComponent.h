// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UAHealthComponent : public UActorComponent
{
	GENERATED_BODY()
		// staviti razlicite bodove za razlicite objekte koji me demagaju
public:	
	// Sets default values for this component's properties
	UAHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultHealth;
	UPROPERTY(BlueprintReadWrite)
		float MainHealth;

	UFUNCTION()
	void TakeDamage( AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
