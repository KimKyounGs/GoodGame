// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoodGame/Weapon/WeaponTypes.h"
#include "Pickup.generated.h"

UCLASS()
class GOODGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:
	// ������: �⺻�� �ʱ�ȭ
	APickup();

	// �� ������ ȣ��: ������ ���³� ȿ�� ���� ������Ʈ
	virtual void Tick(float DeltaTime) override;

	// ���Ͱ� �ı��� �� ȣ��: �ı� �� �߰� �۾��� ����
	virtual void Destroyed() override;

protected:
	// ���Ͱ� �����ǰų� ���۵� �� ȣ��: �ʱ�ȭ �۾� ����
	virtual void BeginPlay() override;

	// ������ �̺�Ʈ�� �߻����� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent, // �������� ������Ʈ
		AActor* OtherActor,                       // �������� �ٸ� ����
		UPrimitiveComponent* OtherComp,           // �������� �ٸ� ������ ������Ʈ
		int32 OtherBodyIndex,                     // �������� �ٵ��� �ε���
		bool bFromSweep,                          // ���� ������ ����
		const FHitResult& SweepResult             // �浹 ��� ����
	);

	// �⺻ ȸ�� �ӵ� ���� (��: �Ⱦ� �������� ȸ�� ȿ��)
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	// �Ⱦ� �������� ������ ����
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	// �Ⱦ� ������ ȹ�� �� ����� ����
	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	// �Ⱦ� �������� �޽� (����)
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	// �Ⱦ� �������� �ð��� ȿ�� ������Ʈ
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PickupEffectComponent;

	// �Ⱦ� ȿ���� ����� Niagara �ý���
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	// ������ Ÿ�̸� �ڵ�: ���� �ð� �� ������ ������ �����ϱ� ���� ���
	FTimerHandle BindOverlapTimer;

	// ������ Ÿ�̸� �ð� ����
	float BindOverlapTime = 0.25f;

	// ������ Ÿ�̸� �Ϸ� �� ȣ��Ǵ� �Լ�
	void BindOverlapTimerFinished();

public:

};
