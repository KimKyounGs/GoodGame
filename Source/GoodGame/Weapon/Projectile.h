// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class GOODGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	void StartDestroyTimer();
	void DestroyTimerFinished();
	void SpawnTrailSystem();
	void ExplodeDamage();


	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComp,  // 충돌한 컴포넌트
		AActor* OtherActor,            // 충돌한 액터
		UPrimitiveComponent* OtherComp, // 충돌한 액터의 컴포넌트
		FVector NormalImpulse,         // 충돌 시 가해진 힘
		const FHitResult& Hit          // 충돌 정보 (충돌 지점, 표면 정보 등)
	);

	// 이 발사체가 줄 수 있는 데미지 값
	UPROPERTY(EditAnywhere) // Blueprint에서 값을 조정할 수 있도록 허용
	float Damage = 20.f;

	// 충돌 시 생성될 파티클 효과
	UPROPERTY(EditAnywhere) 
	class UParticleSystem* ImpactParticles;

	// 충돌 시 재생될 사운드
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	// 발사체의 충돌을 감지하는 컴포넌트
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	// 발사체가 날아가는 동안 재생될 Niagara 이펙트 시스템
	UPROPERTY(EditAnywhere) 
	class UNiagaraSystem* TrailSystem;

	// 발사체에 연결된 Niagara 이펙트 컴포넌트 (Trail 이펙트가 이 컴포넌트를 통해 실행됨)
	UPROPERTY() 
	class UNiagaraComponent* TrailSystemComponent;

	// 발사체의 움직임을 제어하는 컴포넌트
	UPROPERTY(VisibleAnywhere) 
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// 발사체의 기본 메시 (모양 및 렌더링)
	UPROPERTY(VisibleAnywhere) 
	UStaticMeshComponent* ProjectileMesh;

	// 폭발 데미지의 내부 반경 (이 반경 내에서는 데미지가 최대)
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;

	// 폭발 데미지의 외부 반경 (이 반경까지 데미지가 감소)
	UPROPERTY(EditAnywhere) 
	float DamageOuterRadius = 500.f;

private:

	UPROPERTY(EditAnywhere)
	UParticleSystem* Tracer;

	class UParticleSystemComponent* TracerComponent;

	FTimerHandle DestroyTimer;

	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;

public:	

};
