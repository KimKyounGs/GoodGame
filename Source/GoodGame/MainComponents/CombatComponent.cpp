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
			FMath::GetMappedRangeValueClamped 함수는 캐릭터의 현재 속도(Velocity.Size())를 WalkSpeedRange에서 VelocityMultiplierRange로 매핑합니다.
			이렇게 계산된 값은 CrosshairVelocityFactor에 저장됩니다.
			*/
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

			if (Character->GetCharacterMovement()->IsFalling())
			{
				// 공중에 있다면 CrosshairInAirFactor를 2.25f로 보간합니다.이는 공중에 있는 동안 크로스헤어가 더 많이 퍼지게 만듭니다.
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
			}
			/*
			CrosshairSpread (크로스헤어의 퍼짐 정도를 나타내는 값)은 
			CrosshairVelocityFactor (캐릭터의 속도에 기반한 퍼짐 요소)와 CrosshairInAirFactor (캐릭터가 공중에 있는지에 기반한 퍼짐 요소)의 합으로 계산됩니다.
			*/

			/*
			* 엎드리면 조준점 퍼지는 것 막기.
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
		EquippedWeapon->Fire(HitTarget); // 애니메이션
	}
}

void UCombatComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	// viewport size가 필요함
	// 화면 중앙에 위치하는 조준점을 만들기 위해 화면 중앙 위치를 월드 좌표와 방향으로 변환해서 사용.
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	// UGameplayStatics::DeprojectScreenToWorld : 화면의 특정 위치에서 월드 좌표로의 변환을 수행하는 함수입니다.
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	//  화면 중앙에서 시작하여 월드 방향으로 레이를 발사하고 그 결과를 처리하는 것입니다.
	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;

		GetWorld()->LineTraceSingleByChannel(
			TraceHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility // 어떤 오브젝트들이 레이와 충돌할 수 있는지를 정의한다.
		);

		// 충돌안했을 때
		if (!TraceHitResult.bBlockingHit)
		{
			// ImpactPoint는 레이나 물체가 다른 오브젝트와 충돌했을 때 그 정확한 위치를 나타냅니다.
			// 충돌을 안했으니깐 충돌 위치를 End로 표시.
			TraceHitResult.ImpactPoint = End;
			HitTarget = End;
		}
		// 충돌했을 때
		else 
		{
			HitTarget = TraceHitResult.ImpactPoint;
		}
	}

}
