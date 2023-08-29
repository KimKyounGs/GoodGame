// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 *
 */
UCLASS()
class GOODGAME_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMainGameMode();
protected:
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Music")
	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Music")
	TArray<class USoundBase*> Music;

	bool bPlaying = false;
	int32 MusicIdx;

	void PlayMusic();
};