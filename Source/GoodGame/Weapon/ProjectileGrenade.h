// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileGrenade.generated.h"

/**
 * 
 */
UCLASS()
class GOODGAME_API AProjectileGrenade : public AProjectile
{
	GENERATED_BODY()
public:
	AProjectileGrenade();
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;

	// 투사체가 튀게하는 함수
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity); 
private:

	UPROPERTY(EditAnywhere)
	USoundCue* BounceSound;
};
