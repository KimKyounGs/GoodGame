// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnPoint.h"

AEnemySpawnPoint::AEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	StartSpawnEnemyTimer((AActor*)nullptr);
}

void AEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnPoint::SpawnEnemy()
{
	int32 NumPickupClasses = EnemyClasses.Num();
	if (NumPickupClasses > 0)
	{
		int32 Selection = FMath::RandRange(0, NumPickupClasses - 1);
		SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClasses[Selection], GetActorTransform());

		if (SpawnedEnemy)
		{
			//SpawnedEnemy->OnDestroyed.AddDynamic(this, &AEnemySpawnPoint::StartSpawnPickupTimer);
		}
	}
	StartSpawnEnemyTimer((AActor*)nullptr);
}

void AEnemySpawnPoint::SpawnEnemyTimerFinished()
{
	SpawnEnemy();
}

void AEnemySpawnPoint::StartSpawnEnemyTimer(AActor* DestroyedActor)
{
	const float SpawnTime = FMath::FRandRange(SpawnEnemyTimeMin, SpawnEnemyTimeMax);
	GetWorldTimerManager().SetTimer(
		SpawnPickupTimer,
		this,
		&AEnemySpawnPoint::SpawnEnemyTimerFinished,
		SpawnTime
	);
}
