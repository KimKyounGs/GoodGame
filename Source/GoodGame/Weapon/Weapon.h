// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"), // ���忡 ���Ⱑ �״�� ���� ���� ����. ���� �ƹ��� ������� �ʾҰ� ĳ���Ͱ� ���� ��� ����� �� �ִ� ����.
	EWS_Equipped UMETA(DisplayName = "Equipped"), // ���Ⱑ ���Ǿ� ����.
	EWS_Dropped UMETA(DisplayName = "Droppped"), // ���⸦ ������ �ִٰ� ����߷��� ��

	EWS_MAX UMETA(DisplayName = "DefaultMAX") // �̷��� ���ǵ� �ִ밪�� �ϓK������ ���� üũ�� �ݺ������� ���ȴ�. ���� �������� ������ �׸��� �ĺ���.
};

UCLASS()
class GOODGAME_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		EWeaponState WeaponState;

	//UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	//class UWidgetComponent* PickupWidget;
public:

};