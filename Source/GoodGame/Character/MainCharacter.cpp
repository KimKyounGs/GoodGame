// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GoodGame/Weapon/Weapon.h"
#include "Components/CapsuleComponent.h"
#include "GoodGame/MainComponents/CombatComponent.h"
#include "GoodGame/MainComponents/BuffComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharacterAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "GoodGame/GoodGame.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "GoodGame/GameMode/MainGameMode.h"
#include "GoodGame/Weapon/WeaponTypes.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	// 나중에 웅크리는 기능을 사용할 때 카메라가 편하게 따라가게 만들기 위해서 Mesh에다가 카메라를 붙임.
	CameraBoom->SetupAttachment(GetMesh());
	// 카메라 떨어져있는 길이.
	CameraBoom->TargetArmLength = 600.f;
	// 마우스 입력에따라 회전할 수 있게함.
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	Buff = CreateDefaultSubobject<UBuffComponent>(TEXT("BuffComponent"));

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionObjectType(ECC_SkeletalMesh);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetCharacterMovement()->RotationRate = FRotator(0.f, 0.f, 850.f);

	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	Health = 100.0f;
	MaxHealth = 100.0f;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AMainCharacter::ReceiveDamage);

	// 타이머 이용해서 늦게 함수 호출.
	// 참고 : https://koreanfoodie.me/1164
	FTimerHandle myTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(myTimerHandle, this, &AMainCharacter::UpdateHUD, 0.2f, false);
	//UpdateHUD();
}

void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffSet(DeltaTime);
}

void AMainCharacter::UpdateHUD()
{
	UpdateHUDHealth();
	UpdateHUDShield();
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 캐릭터 움직임 애니메이션
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);

	// 총 장착 애니메이션
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AMainCharacter::EquipButtonPressed);
	// 기어가는 애니메이션
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::CrouchButtonPressed);

	// 에임 애니메이션
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMainCharacter::AimButtonPressed); 
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMainCharacter::AimButoonReleased);
	// 총 발사 애니메이션
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMainCharacter::FireButtonReleased);
	// 장전 애니메이션
	PlayerInputComponent->BindAction("Reload", IE_Released, this, &AMainCharacter::ReloadButtonPressed);
	// 수류탄 애니메이션
	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &AMainCharacter::GrenadeButtonPressed);
}

void AMainCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
	if (Buff)
	{
		Buff->Character = this;
		Buff->SetInitialSpeeds(
			GetCharacterMovement()->MaxWalkSpeed,
			GetCharacterMovement()->MaxWalkSpeedCrouched
		);
		Buff->SetInitialJumpVelocity(GetCharacterMovement()->JumpZVelocity);
	}
}


void AMainCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AMainCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AMainCharacter::Jump()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void AMainCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}

}

void AMainCharacter::EquipButtonPressed()
{
	if (Combat)
	{
		if (OverlappingWeapon)
		{
			Combat->EquipWeapon(OverlappingWeapon);
		}
		else if (Combat->ShouldSwapWeapons())
		{
			Combat->SwapWeapons();
		}
	}
}


void AMainCharacter::GetOverlappingWeapon(AWeapon* Weapon)
{
	OverlappingWeapon = Weapon;
	/*
	if (Weapon)
	{
		OverlappingWeapon = Weapon;
		OverlappingWeapon->ShowPickupWidget(true);
	}
	else
	{
		OverlappingWeapon = nullptr;
		OverlappingWeapon->ShowPickupWidget(false);
	}
	*/
}

bool AMainCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool AMainCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

void AMainCharacter::AimButtonPressed()
{
	if (Combat)
	{
		Combat->SetAiming(true);
	}
}

void AMainCharacter::AimButoonReleased()
{
	if (Combat)
	{
		Combat->SetAiming(false);
	}
}

void AMainCharacter::TurnInPlace(float DeltaTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("AO_Yaw : %f"), AO_Yaw);
	if (AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if (TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 10.f);
		AO_Yaw = InterpAO_Yaw;
		if (FMath::Abs(AO_Yaw) < 15.f)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
}

void AMainCharacter::AimOffSet(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir) // standing still, not Junmping
	{
		//UE_LOG(LogTemp, Warning, TEXT("AimSpeed = 0"));
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(StartingAimRotation, CurrentAimRotation); // 강의랑 반대로 하니깐 되는데..?
		AO_Yaw = DeltaAimRotation.Yaw;
		if (TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if (Speed > 0.f || bIsInAir) // running, or jumping
	{
		//UE_LOG(LogTemp, Warning, TEXT("AimSpeed > 0"));
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;

	/*
	if (AO_Pitch > 90.f && !IsLocallyControlled()
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
	*/

}


AWeapon* AMainCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}

ECombatState AMainCharacter::GetCombatState() const
{
	if (Combat == nullptr) return ECombatState::ECS_MAX;
	return Combat->CombatState;
}

void AMainCharacter::FireButtonPressed()
{
	if (Combat)
	{
		Combat->FireButtonPressed(true);
	}
}

void AMainCharacter::FireButtonReleased()
{
	if (Combat)
	{
		Combat->FireButtonPressed(false);
	}
}

void AMainCharacter::GrenadeButtonPressed()
{
	if (Combat)
	{
		Combat->ThrowGrenade();
	}
}


void AMainCharacter::PlayFireMontage(bool bAiming)
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Play Montage2"));
		// 에임조준에 있냐 안되어있냐에 따라서 몽타주 섹션 재생을 함. (에임하는 중이면 RifleAim으로 되어있는 섹션 애니메이션을 실행.)
		AnimInstance->Montage_Play(FireWeaponMontage);
		FName SectionName;
		SectionName = bAiming ? FName("RifleAim") : FName("RifleHip");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

void AMainCharacter::ReloadButtonPressed()
{
	if (Combat)
	{
		Combat->Reload();
	}
}

void AMainCharacter::PlayReloadMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	// 애니메이션 인스턴스 받기
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ReloadMontage)
	{
		AnimInstance->Montage_Play(ReloadMontage);
		FName SectionName;

		// 각각 총 장전 애니메이션 다르게 구성.
		switch (Combat->EquippedWeapon->GetWeaponType())
		{
		case EWeaponType::EWT_AssaultRifle:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_RocketLauncher:
			SectionName = FName("Rifle");
			break;
		case EWeaponType::EWT_Pistol:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_SubmachineGun:
			SectionName = FName("Pistol");
			break;
		case EWeaponType::EWT_Shotgun:
			SectionName = FName("Shotgun");
			break;
		case EWeaponType::EWT_SniperRifle:
			SectionName = FName("SniperRifle");
			break;
		case EWeaponType::EWT_GrenadeLauncher:
			SectionName = FName("GrenadeLauncher");
			break;
		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName); // 샷건은 장전중에 쏠 수 있게함.
	}
}

void AMainCharacter::PlayThrowGrenadeMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && ThrowGrenadeMontage)
	{
		//AnimInstance->Montage_Play(ThrowGrenadeMontage);
	}
}

void AMainCharacter::PlayHitReactMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		FName SectionName("FromFront");
		AnimInstance->Montage_JumpToSection(SectionName);
	}
}

FVector AMainCharacter::GetHitTarget() const
{
	if (Combat == nullptr) return FVector();
	return Combat->HitTarget;
}

void AMainCharacter::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{

	// if (bElimmed) return;

	float DamageToHealth = Damage;
	if (Shield > 0.f)
	{
		if (Shield >= Damage)
		{
			Shield = FMath::Clamp(Shield - Damage, 0.f, MaxShield);
			DamageToHealth = 0.f;
		}
		else
		{
			Shield = 0.f;
			DamageToHealth = FMath::Clamp(DamageToHealth - Shield, 0.f, Damage);
		}
	}

	Health = FMath::Clamp(Health - DamageToHealth, 0.f, MaxHealth);

	UpdateHUDHealth();
	UpdateHUDShield();
	PlayHitReactMontage();

	if (Health == 0.f)
	{
		AMainGameMode* MainGameMode = GetWorld()->GetAuthGameMode<AMainGameMode>();
		if (MainGameMode)
		{
			FName LevelName = FName(TEXT("Lobby"));
			UGameplayStatics::OpenLevel(this, LevelName);
			/*
			MainPlayerController = MainPlayerController == nullptr ? Cast<AMainPlayController>(Controller) : MainPlayerController;
			AMainPlayController* AttackerController = Cast<AMainPlayController>(InstigatorController);
			MainGameMode->PlayerEliminated(this, MainPlayerController, AttackerController);
			*/
		}
	}

}

void AMainCharacter::UpdateHUDHealth()
{
	MainPlayerController = MainPlayerController == nullptr ? Cast<AMainPlayController>(Controller) : MainPlayerController;
	if (MainPlayerController)
	{
		MainPlayerController->SetHUDHealth(Health, MaxHealth);
	}
}

void AMainCharacter::UpdateHUDShield()
{
	MainPlayerController = MainPlayerController == nullptr ? Cast<AMainPlayController>(Controller) : MainPlayerController;
	if (MainPlayerController)
	{
		MainPlayerController->SetHUDShield(Shield, MaxShield);
	}
}

void AMainCharacter::HideCameraIfCharacterClose()
{
	if (!IsLocallyControlled()) return;
	if ((FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold)
	{
		GetMesh()->SetVisibility(false);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = true;
		}
	}
	else
	{
		GetMesh()->SetVisibility(true);
		if (Combat && Combat->EquippedWeapon && Combat->EquippedWeapon->GetWeaponMesh())
		{
			Combat->EquippedWeapon->GetWeaponMesh()->bOwnerNoSee = false;
		}
	}
}

void AMainCharacter::Elim()
{
	//DropOrDestroyWeapons();
}

void AMainCharacter::DropOrDestroyWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr) return;
	if (Weapon->bDestroyWeapon)
	{
		Weapon->Destroy();
	}
	else
	{
		Weapon->Dropped();
	}
}

void AMainCharacter::DropOrDestroyWeapons()
{
	if (Combat)
	{
		if (Combat->EquippedWeapon)
		{
			DropOrDestroyWeapon(Combat->EquippedWeapon);
		}
		if (Combat->SecondaryWeapon)
		{
			DropOrDestroyWeapon(Combat->SecondaryWeapon);
		}
	}
}

/*
void AMainCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
	DOREPLIFETIME(AMainCharacter, Health);
}
*/

/*
void AMainCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}
	OverlappingWeapon = Weapon;
	
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickupWidget(true);
		}
	}

}


void AMainCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}
*/
