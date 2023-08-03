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

	// ĳ������ ȸ���� �޾ƿͼ� ������ ������ �Ŀ� ���� ���� YawOffset�� ���� ����.
	FRotator AimRotation = MainCharacter->GetBaseAimRotation(); // ���� ȸ����
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(MainCharacter->GetVelocity()); // ĳ���� ȸ����
	//YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	
	//* �̷��� �ϸ� ���� �ε巴�� �Ѵٰ� ��.
	FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.0f);
	YawOffset = DeltaRotation.Yaw;
	

	// Lean ������ �����Ӱ� ������ �ִ� �� ����.
	// ĳ���� ȸ���ϴ� ���� �ε巴�� ��
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = MainCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaTime; // ȸ���� ��ȭ���� ���. Yaw���� ���� (��, �� ������ ������ ���� ��ȯ) -> Delta�� ������ ��ȭ���� ���
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f); // �������� -> �ε巴�� ���ο� �������� ��ȯ
	Lean = FMath::Clamp(Interp, -90.f, 90.f); // ���̰� ���� �̷��� �ϸ� ĳ������ ���Ⱑ ���ϰ� ����Ǵ� �� ����.
	
	AO_Yaw = MainCharacter->GetAO_Yaw();
	AO_Pitch = MainCharacter->GetAO_Pitch();

	if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && MainCharacter->GetMesh())
	{
		/*
		*  1 :ù ��° ���� 'EquippedWeapon'�̶�� ���� ������ ���⿡�� ������ �޽ø� �����ͼ� �װ��� "LeftHandSocket"�̶�� ������ ��ȯ�� �����ɴϴ�. �� ��ȯ�� ��ġ�� ȸ���� �����ϸ�, ���� ���������� ����� ��ġ�� ������ ��Ÿ���ϴ�.
		*  3 : �� �ٿ����� 'MainCharacter'�� �޽ø� ������ 'hand_r'�̶�� ��(����)�� �������� ��ȯ�� �����մϴ�. �� ��ȯ�� 'LeftHandTransform'�� ��ġ�� 0�� ȸ���� �Է����� �޾�, 'OutPosition'�� 'OutRotation'�� ����� �����մϴ�.
		*  4,5 : 'LeftHandTransform'�� ��ġ�� ȸ���� ���� 'OutPosition' �� 'OutRotation'���� �����մϴ�. �̴� ĳ������ �޼տ� ���⸦ ��Ȯ�� ��ġ��Ű�� ȸ����Ű�� �� ���˴ϴ�.
		*/
		LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), ERelativeTransformSpace::RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		MainCharacter->GetMesh()->TransformToBoneSpace(FName("RightHand"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));

	}
}
