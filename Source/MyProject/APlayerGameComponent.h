// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "APlayerGameComponent.generated.h"

// staviti amo total score i current score i zapisivat to
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UAPlayerGameComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UAPlayerGameComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DefaultHealth;
	UPROPERTY(BlueprintReadWrite)
		float MainHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
		float LevelScore;

	UFUNCTION()
	void TakeDamage( AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
};
