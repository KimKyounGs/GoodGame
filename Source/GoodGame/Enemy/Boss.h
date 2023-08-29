// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boss.generated.h"

UCLASS()
class GOODGAME_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoss();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Property")
	UStaticMeshComponent* BossMesh;

	UPROPERTY(EditAnywhere, Category = "Property")
	float MaxHealth = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Property")
	float Health = 5000.f;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
};
