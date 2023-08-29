// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "Kismet/GameplayStatics.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	BossMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossMesh"));
	SetRootComponent(BossMesh);

}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ABoss::ReceiveDamage);
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoss::ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	AMainPlayController* MainController = Cast<AMainPlayController>(GetWorld()->GetFirstPlayerController());
	if (MainController)
	{
		MainController->SetHUDBossHealth(Health, MaxHealth);
	}
	
	if (Health == 0.f)
	{
		FName LevelName = FName(TEXT("Lobby"));
		UGameplayStatics::OpenLevel(this, LevelName);
	}
}

