// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GoodGame/Weapon/Weapon.h"



void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
}

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MainCharacter == nullptr)
	{
		MainCharacter = Cast<AMainCharacter>(TryGetPawnOwner());
	}

	if (MainCharacter == nullptr) return;

	FVector Velocity = MainCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = MainCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = MainCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = MainCharacter->IsWeaponEquipped();
	EquippedWeapon = MainCharacter->GetEquippedWeapon();
	bIsCrouched = MainCharacter->bIsCrouched;
	bAiming = MainCharacter->IsAiming();
	TurningInPlace = MainCharacter->GetTurningInPlace();

	// 캐릭터의 회전을 받아와서 변수에 저장한 후에 값에 따라 YawOffset의 값을 조정.
	FRotator AimRotation = MainCharacter->GetBaseAimRotation(); // 에임 회전값
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity()); // 캐릭터 회전값
	//YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	
	//* 이렇게 하면 좀더 부드럽게 한다고 함.
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.0f);
	YawOffset = DeltaRotation.Yaw;
	

	// Lean 구현시 프레임과 관련이 있는 것 같음.
	// 캐릭터 회전하는 것을 부드럽게 함
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MainCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime; // 회전의 변화율을 계산. Yaw값의 차이 (즉, 두 프레임 사이의 방향 전환) -> Delta로 나누어 변화율을 계산
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f); // 보간연산 -> 부드럽게 새로운 방향으로 전환
	Lean = FMath::Clamp(Interp, -90.f, 90.f); // 사이값 제한 이렇게 하면 캐릭터의 기울기가 과하게 변경되는 것 방지.
	
	AO_Yaw = MainCharacter->GetAO_Yaw();
	AO_Pitch = MainCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && MainCharacter->GetMesh())
	{
		/*
		*  1 :첫 번째 줄은 'EquippedWeapon'이라는 현재 장착된 무기에서 무기의 메시를 가져와서 그것의 "LeftHandSocket"이라는 소켓의 변환을 가져옵니다. 이 변환은 위치와 회전을 포함하며, 월드 공간에서의 상대적 위치와 방향을 나타냅니다.
		*  3 : 이 줄에서는 'MainCharacter'의 메시를 가져와 'hand_r'이라는 본(뼈대)의 공간으로 변환을 수행합니다. 이 변환은 'LeftHandTransform'의 위치와 0의 회전을 입력으로 받아, 'OutPosition'과 'OutRotation'에 결과를 저장합니다.
		*  4,5 : 'LeftHandTransform'의 위치와 회전을 각각 'OutPosition' 및 'OutRotation'으로 설정합니다. 이는 캐릭터의 왼손에 무기를 정확히 위치시키고 회전시키는 데 사용됩니다.
		*/
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		MainCharacter->GetMesh()->TransformToBoneSpace(FName("RightHand"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

	}
}
