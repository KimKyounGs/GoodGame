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
		UPrimitiveComponent* HitComp,  // �浹�� ������Ʈ
		AActor* OtherActor,            // �浹�� ����
		UPrimitiveComponent* OtherComp, // �浹�� ������ ������Ʈ
		FVector NormalImpulse,         // �浹 �� ������ ��
		const FHitResult& Hit          // �浹 ���� (�浹 ����, ǥ�� ���� ��)
	);

	// �� �߻�ü�� �� �� �ִ� ������ ��
	UPROPERTY(EditAnywhere) // Blueprint���� ���� ������ �� �ֵ��� ���
	float Damage = 20.f;

	// �浹 �� ������ ��ƼŬ ȿ��
	UPROPERTY(EditAnywhere) 
	class UParticleSystem* ImpactParticles;

	// �浹 �� ����� ����
	UPROPERTY(EditAnywhere)
	class USoundCue* ImpactSound;

	// �߻�ü�� �浹�� �����ϴ� ������Ʈ
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	// �߻�ü�� ���ư��� ���� ����� Niagara ����Ʈ �ý���
	UPROPERTY(EditAnywhere) 
	class UNiagaraSystem* TrailSystem;

	// �߻�ü�� ����� Niagara ����Ʈ ������Ʈ (Trail ����Ʈ�� �� ������Ʈ�� ���� �����)
	UPROPERTY() 
	class UNiagaraComponent* TrailSystemComponent;

	// �߻�ü�� �������� �����ϴ� ������Ʈ
	UPROPERTY(VisibleAnywhere) 
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// �߻�ü�� �⺻ �޽� (��� �� ������)
	UPROPERTY(VisibleAnywhere) 
	UStaticMeshComponent* ProjectileMesh;

	// ���� �������� ���� �ݰ� (�� �ݰ� �������� �������� �ִ�)
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;

	// ���� �������� �ܺ� �ݰ� (�� �ݰ���� �������� ����)
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
