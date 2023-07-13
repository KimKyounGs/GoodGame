// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"), // 월드에 무기가 그대로 있을 때의 상태. 아직 아무도 집어들지 않았고 캐릭터가 집어 들고 장비할 수 있는 상태.
	EWS_Equipped UMETA(DisplayName = "Equipped"), // 무기가 장비되어 있음.
	EWS_Dropped UMETA(DisplayName = "Droppped"), // 무기를 가지고 있다가 떨어뜨렸을 때

	EWS_MAX UMETA(DisplayName = "DefaultMAX") // 이렇게 정의된 최대값은 일밙거으로 범위 체크나 반복문에서 사용된다. 또한 열거형의 마지막 항목을 식별함.
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
	class UWidgetComponent* PickupWidget;
public:

};
