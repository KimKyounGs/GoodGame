// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GoodGame/Weapon/Weapon.h"
#include "GoodGame/Character/MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 200.f;

}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	}
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceUnderCrosshairs(HitResult);

}

void UCombatComponent::SetAiming(bool bIsAiming)
{
	bAiming = bIsAiming;
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	}
}


void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (Character == nullptr || WeaponToEquip == nullptr)
	{
		return;
	}
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	Character->GetCharacterMovement()->bOrientRotationToMovement = false;
	Character->bUseControllerRotationYaw = true;
	EquippedWeapon->ShowPickupWidget(false);
}


void UCombatComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;

	if (EquippedWeapon == nullptr) return;

	if (Character && bFireButtonPressed)
	{
		Character->PlayFireMontage(bAiming);
		EquippedWeapon->Fire(HitTarget); // �ִϸ��̼�
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	// viewport size�� �ʿ���
	// ȭ�� �߾ӿ� ��ġ�ϴ� �������� ����� ���� ȭ�� �߾� ��ġ�� ���� ��ǥ�� �������� ��ȯ�ؼ� ���.
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	// UGameplayStatics::DeprojectScreenToWorld : ȭ���� Ư�� ��ġ���� ���� ��ǥ���� ��ȯ�� �����ϴ� �Լ��Դϴ�.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	//  ȭ�� �߾ӿ��� �����Ͽ� ���� �������� ���̸� �߻��ϰ� �� ����� ó���ϴ� ���Դϴ�.
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility // � ������Ʈ���� ���̿� �浹�� �� �ִ����� �����Ѵ�.
		);

		// �浹������ ��
		if (!TraceHitResult.bBlockingHit)
		{
			// ImpactPoint�� ���̳� ��ü�� �ٸ� ������Ʈ�� �浹���� �� �� ��Ȯ�� ��ġ�� ��Ÿ���ϴ�.
			// �浹�� �������ϱ� �浹 ��ġ�� End�� ǥ��.
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		// �浹���� ��
		else 
		{
			HitTarget = TraceHitResult.ImpactPoint;
			DrawDebugSphere(
				GetWorld(),
				TraceHitResult.ImpactPoint,
				12.f,
				12,
				FColor::Red
			);
		}
	}

}
