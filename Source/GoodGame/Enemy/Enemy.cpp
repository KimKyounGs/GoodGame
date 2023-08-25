// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/WidgetComponent.h"
#include "GoodGame/Enemy/EnemyOverlay.h"
#include "Components/ProgressBar.h"
#include "GameFramework/Actor.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	HealthWidget->SetupAttachment(RootComponent);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AEnemy::ReceiveDamage);

	// ���α׷����� ���� �����.
	if (HealthWidget && HealthWidget->GetUserWidgetObject())
	{
		UUserWidget* UserWidget = HealthWidget->GetUserWidgetObject();

		// ���� UserWidget���� ���α׷����ٸ� ã���ϴ�.
		// ���⼭ "ProgressBar"�� ���α׷����� ������ �̸��̸�, �̴� UMG �����Ϳ��� ������ �� �ֽ��ϴ�.
		HealthBar = Cast<UProgressBar>(UserWidget->GetWidgetFromName("HealthBar"));
	}


	// �̰� ������ �����ɷȳ� �ƿ� ~!~!!~!~!~!~!~!~!~!~!~!~!~!~!~!
	//EnemyOverlay = CreateWidget<UEnemyOverlay>(GetWorld()->GetFirstPlayerController(), EnemyOverlayClass);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::UpdateHealth()
{
	const float HealthPercent = Health / MaxHealth;
	HealthBar->SetPercent(HealthPercent);
}

void AEnemy::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UpdateHealth();
}
