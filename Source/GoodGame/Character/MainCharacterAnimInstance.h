// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GoodGame/MainCharacterTypes/TurningInPlace.h"
#include "MainCharacterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class GOODGAME_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// BeginPlay()�� ����ϰ� �۵���.
	// ���� ���� �Ӹ� �ƴ϶� ������ �� �پ��� �ð��� ȣ���.
	virtual void NativeInitializeAnimation() override;

	// Tick �Լ��� �����ϰ� �۵���.
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	// �ִϸ��̼� �������Ʈ�� �� ��� ����.

	//�Ϲ������� C++���� Ŭ������ ��� ������ �⺻������ private���� ����Ǿ� �ܺο��� ���� �׼����� �� �����ϴ�. 
	// �׷��� �𸮾� ���������� 
	//UPROPERTY ��ũ���� meta �Ű������� ����Ͽ� private ��� ������ ���� �׼��� ������ ������ �� �ֽ��ϴ�.
	//�� ��Ÿ�����ʹ� ������ �Ӽ�, �ø��������(serialize) ���, �����Ϳ����� ǥ�� ��� ���� ������ �� �ֽ��ϴ�.
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AMainCharacter* MainCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bWeaponEquipped;

	class AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float Lean;


	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;


	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ETurningInPlace TurningInPlace;
};
