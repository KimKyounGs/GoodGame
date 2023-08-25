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

	// 프로그래스바 정말 힘들다.
	if (HealthWidget && HealthWidget->GetUserWidgetObject())
	{
		UUserWidget* UserWidget = HealthWidget->GetUserWidgetObject();

		// 이제 UserWidget에서 프로그래스바를 찾습니다.
		// 여기서 "ProgressBar"는 프로그래스바 위젯의 이름이며, 이는 UMG 에디터에서 설정할 수 있습니다.
		HealthBar = Cast<UProgressBar>(UserWidget->GetWidgetFromName("HealthBar"));
	}


	// 이거 때문에 오래걸렸네 아오 ~!~!!~!~!~!~!~!~!~!~!~!~!~!~!~!
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
