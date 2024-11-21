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

// ü�� ȸ���� �����ϴ� �Լ�
void UBuffComponent::Heal(float HealAmount, float HealingTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Heal")); 

    bHealing = true; // ü�� ȸ�� Ȱ��ȭ
    HealingRate = HealAmount / HealingTime; // �ʴ� ȸ���� ���
    AmountToHeal += HealAmount; // �� ȸ���� ü�·� ������Ʈ
}

// �� ������ ȣ��Ǿ� ü���� ���������� ȸ���ϴ� �Լ�
void UBuffComponent::HealRampUp(float DeltaTime)
{
    if (!bHealing || Character == nullptr) return;

    // �̹� �����ӿ��� ȸ���� ü�·� ���
    const float HealThisFrame = HealingRate * DeltaTime;

    // ĳ������ ü���� ������Ű��, �ִ� ü���� �ʰ����� �ʵ��� ����
    Character->SetHealth(FMath::Clamp(Character->GetHealth() + HealThisFrame, 0.f, Character->GetMaxHealth()));

    // HUD�� ü�� ������Ʈ
    Character->UpdateHUDHealth();

    // ���� ȸ�������� �̹� �������� ȸ������ ����
    AmountToHeal -= HealThisFrame;

    // ü���� �ִ뿡 �����ϰų� ���� ȸ������ 0�� �Ǹ� ȸ�� ����
    if (AmountToHeal <= 0.f || Character->GetHealth() >= Character->GetMaxHealth())
    {
        bHealing = false; 
        AmountToHeal = 0.f; 
    }
}

// �� ������ �����ϴ� �Լ�
void UBuffComponent::ReplenishShield(float ShieldAmount, float ReplenishTime)
{
    bReplenishingShield = true; // �� ���� Ȱ��ȭ
    ShieldReplenishRate = ShieldAmount / ReplenishTime; // �ʴ� ���淮 ���
    ShieldReplenishAmount += ShieldAmount; // �� ������ ���� ������Ʈ
}

// �� ������ ȣ��Ǿ� ���� ���������� �����ϴ� �Լ�
void UBuffComponent::ShieldRampUp(float DeltaTime)
{
    if (!bReplenishingShield || Character == nullptr) return;

    // �̹� �����ӿ��� ������ ���� ���
    const float ReplenishThisFrame = ShieldReplenishRate * DeltaTime;

    // ĳ������ ���� ������Ű��, �ִ� ������ �ʰ����� �ʵ��� ����
    Character->SetShield(FMath::Clamp(Character->GetShield() + ReplenishThisFrame, 0.f, Character->GetMaxShield()));

    // HUD�� �� ������Ʈ
    Character->UpdateHUDShield();

    // ���� ���淮���� �̹� �������� ���淮�� ����
    ShieldReplenishAmount -= ReplenishThisFrame;

    // ���� �ִ뿡 �����ϰų� ���� ���淮�� 0�� �Ǹ� ���� ����
    if (ShieldReplenishAmount <= 0.f || Character->GetShield() >= Character->GetMaxShield())
    {
        bReplenishingShield = false; // �� ���� ��Ȱ��ȭ
        ShieldReplenishAmount = 0.f; // ���� ���淮 �ʱ�ȭ
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