// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayController.generated.h"

/**
 * 
 */
UCLASS()
class GOODGAME_API AMainPlayController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
protected:
	virtual void BeginPlay() override;

private:
	class AMainHUD* MainHUD;

};
