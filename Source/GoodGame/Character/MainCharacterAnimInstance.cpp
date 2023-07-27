// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



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
	bIsCrouched = MainCharacter->bIsCrouched;
	bAiming = MainCharacter->IsAiming();

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
	
}
