// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawnPoint.generated.h"

UCLASS()
class GOODGAME_API AEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnPoint();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class AEnemy>> EnemyClasses;

	UPROPERTY()
	AEnemy* SpawnedEnemy;

	void SpawnEnemy();
	void SpawnEnemyTimerFinished();

	UFUNCTION()
	void StartSpawnEnemyTimer(AActor* DestroyedActor);

private:
	FTimerHandle SpawnPickupTimer;

	UPROPERTY(EditAnywhere)
	float SpawnEnemyTimeMin;

	UPROPERTY(EditAnywhere)
	float SpawnEnemyTimeMax;
public:

};
