// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GoodGame/HUD/MainHUD.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOODGAME_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	friend class AMainCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);
protected:
	virtual void BeginPlay() override;

	void SetAiming(bool bIsAiming);

	void FireButtonPressed(bool bPressed);

	void Fire();

	// Line trace 
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	
	class AMainCharacter* Character;
	// ���߿� �� ���� �ĸ� �� �κ� ���� �����ֱ�.
	class AMainPlayController* Controller;
	class AMainHUD* HUD;

	AWeapon* EquippedWeapon;


	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bAiming;

	bool bFireButtonPressed;

	FVector HitTarget;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	
	FVector HitTargetForHand;

	FHUDPackage HUDPackage;

	/**
	* Aiming and FOV
	*/

	// Field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/**
	* Automatic fire
	*/

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();


public:	
	



		
};
