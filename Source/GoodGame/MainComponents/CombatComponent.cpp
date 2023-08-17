// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GoodGame/Weapon/Weapon.h"
#include "GoodGame/Character/MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "GoodGame/HUD/MainHUD.h"

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

	SetHUDCrosshairs(DeltaTime);

	if (Character)
	{
		FHitResult HitResultForHand;
		TraceUnderCrosshairs(HitResultForHand);
		HitTargetForHand = HitResultForHand.ImpactPoint;
	}

}

void UCombatComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (Character == nullptr || Character->Controller == nullptr) return;

	Controller = Controller == nullptr ? Cast<AMainPlayController>(Character->Controller) : Controller;
	if (Controller)
	{
		HUD = HUD == nullptr ? Cast<AMainHUD>(Controller->GetHUD()) : HUD;
		if (HUD)
		{
			FHUDPackage HUDPackage;
			if (EquippedWeapon)
			{
				HUDPackage.CrosshairsCenter = EquippedWeapon->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedWeapon->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedWeapon->CrosshairsRight;
				HUDPackage.CrosshairsBottom = EquippedWeapon->CrosshairsBottom;
				HUDPackage.CrosshairsTop = EquippedWeapon->CrosshairsTop;
			}
			else
			{
				HUDPackage.CrosshairsCenter = nullptr;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
			}

			// Calculate crosshair spread

			// [0, 600] -> [0, 1]
			FVector2D WalkSpeedRange(0.f, Character->GetCharacterMovement()->MaxWalkSpeed);
			FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = Character->GetVelocity();
			Velocity.Z = 0.f;

			/* 
			FMath::GetMappedRangeValueClamped �Լ��� ĳ������ ���� �ӵ�(Velocity.Size())�� WalkSpeedRange���� VelocityMultiplierRange�� �����մϴ�.
			�̷��� ���� ���� CrosshairVelocityFactor�� ����˴ϴ�.
			*/
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling())
			{
				// ���߿� �ִٸ� CrosshairInAirFactor�� 2.25f�� �����մϴ�.�̴� ���߿� �ִ� ���� ũ�ν��� �� ���� ������ ����ϴ�.
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}
			/*
			CrosshairSpread (ũ�ν������ ���� ������ ��Ÿ���� ��)�� 
			CrosshairVelocityFactor (ĳ������ �ӵ��� ����� ���� ���)�� CrosshairInAirFactor (ĳ���Ͱ� ���߿� �ִ����� ����� ���� ���)�� ������ ���˴ϴ�.
			*/

			/*
			* ���帮�� ������ ������ �� ����.
			* if (Character->GetCharacterMovement()->IsCrouching())  CrosshairVelocityFactor = 0.f;
			*/


			HUDPackage.CrosshairSpread = CrosshairVelocityFactor + CrosshairInAirFactor;

			HUD->SetHUDPackage(HUDPackage);
		}
	}
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
		}
	}

}
