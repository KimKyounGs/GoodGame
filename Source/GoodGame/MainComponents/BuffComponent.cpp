// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffComponent.h"
#include "GoodGame/Character/MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UBuffComponent::UBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UBuffComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UBuffComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealRampUp(DeltaTime);
	ShieldRampUp(DeltaTime);
}

// 체력 회복을 시작하는 함수
void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Heal")); 

    bHealing = true; // 체력 회복 활성화
    HealingRate = HealAmount / HealingTime; // 초당 회복량 계산
    AmountToHeal += HealAmount; // 총 회복할 체력량 업데이트
}

// 매 프레임 호출되어 체력을 점진적으로 회복하는 함수
void UBuffComponent::HealRampUp(float DeltaTime)
{
    if (!bHealing || Character == nullptr) return;

    // 이번 프레임에서 회복할 체력량 계산
    const float HealThisFrame = HealingRate * DeltaTime;

    // 캐릭터의 체력을 증가시키며, 최대 체력을 초과하지 않도록 제한
    Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));

    // HUD에 체력 업데이트
    Character->UpdateHUDHealth();

    // 남은 회복량에서 이번 프레임의 회복량을 차감
    AmountToHeal -= HealThisFrame;

    // 체력이 최대에 도달하거나 남은 회복량이 0이 되면 회복 종료
    if (AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
    {
        bHealing = false; 
        AmountToHeal = 0.f; 
    }
}

// 방어막 보충을 시작하는 함수
void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
    bReplenishingShield = true; // 방어막 보충 활성화
    ShieldReplenishRate = ShieldAmount / ReplenishTime; // 초당 보충량 계산
    ShieldReplenishAmount += ShieldAmount; // 총 보충할 방어막량 업데이트
}

// 매 프레임 호출되어 방어막을 점진적으로 보충하는 함수
void UBuffComponent::ShieldRampUp(float DeltaTime)
{
    if (!bReplenishingShield || Character == nullptr) return;

    // 이번 프레임에서 보충할 방어막량 계산
    const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;

    // 캐릭터의 방어막을 증가시키며, 최대 방어막량을 초과하지 않도록 제한
    Character->SetShield(FMath::Clamp(Character->GetShield() + ReplenishThisFrame, 0.f, Character->GetMaxShield()));

    // HUD에 방어막 업데이트
    Character->UpdateHUDShield();

    // 남은 보충량에서 이번 프레임의 보충량을 차감
    ShieldReplenishAmount -= ReplenishThisFrame;

    // 방어막이 최대에 도달하거나 남은 보충량이 0이 되면 보충 종료
    if (ShieldReplenishAmount <= 0.f || Character->GetShield() >= Character->GetMaxShield())
    {
        bReplenishingShield = false; // 방어막 보충 비활성화
        ShieldReplenishAmount = 0.f; // 남은 보충량 초기화
    }
}

void UBuffComponent::SetInitialSpeeds(float BaseSpeed, float CrouchSpeed)
{
	InitialBaseSpeed = BaseSpeed;
	InitialCrouchSpeed = CrouchSpeed;
}

void UBuffComponent::BuffSpeed(float BuffBaseSpeed, float BuffCrouchSpeed, float BuffTime)
{
	if (Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(
		SpeedBuffTimer,
		this,
		&UBuffComponent::ResetSpeeds,
		BuffTime
	);

	if (Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = BuffBaseSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeedCrouched = BuffCrouchSpeed;
	}
}

void UBuffComponent::ResetSpeeds()
{
	if (Character == nullptr || Character->GetCharacterMovement() == nullptr) return;

	Character->GetCharacterMovement()->MaxWalkSpeed = InitialBaseSpeed;
	Character->GetCharacterMovement()->MaxWalkSpeedCrouched = InitialCrouchSpeed;
}

void UBuffComponent::SetInitialJumpVelocity(float Velocity)
{
	InitialJumpVelocity = Velocity;
}

void UBuffComponent::BuffJump(float BuffJumpVelocity, float BuffTime)
{
	if (Character == nullptr) return;

	Character->GetWorldTimerManager().SetTimer(
		JumpBuffTimer,
		this,
		&UBuffComponent::ResetJump,
		BuffTime
	);

	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = BuffJumpVelocity;
	}
}


void UBuffComponent::ResetJump()
{
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->JumpZVelocity = InitialJumpVelocity;
	}
}