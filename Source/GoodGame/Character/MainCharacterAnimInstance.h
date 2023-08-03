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
	// BeginPlay()랑 비슷하게 작동함.
	// 게임 시작 뿐만 아니라 컴파일 등 다양한 시간에 호출됨.
	virtual void NativeInitializeAnimation() override;

	// Tick 함수와 유사하게 작동함.
	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	// 애니메이션 블루프린트에 쓸 몇가지 변수.

	//일반적으로 C++에서 클래스의 멤버 변수는 기본적으로 private으로 선언되어 외부에서 직접 액세스할 수 없습니다. 
	// 그러나 언리얼 엔진에서는 
	//UPROPERTY 매크로의 meta 매개변수를 사용하여 private 멤버 변수에 대한 액세스 권한을 제어할 수 있습니다.
	//이 메타데이터는 변수의 속성, 시리얼라이즈(serialize) 방법, 에디터에서의 표시 방식 등을 제어할 수 있습니다.
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
