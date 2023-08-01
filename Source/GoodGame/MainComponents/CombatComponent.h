// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


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

private:
	
	class AMainCharacter* Character;

	AWeapon* EquippedWeapon;


	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bAiming;


public:	
	



		
};
