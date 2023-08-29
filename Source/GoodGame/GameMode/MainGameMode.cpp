// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "GoodGame/Character/MainCharacter.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "Components/AudioComponent.h"

AMainGameMode::AMainGameMode()
{
	// Create Audio component and attach to RootComponent
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

}

void AMainGameMode::BeginPlay()
{
	if (AudioComponent && AudioComponent->Sound)
	{
		AudioComponent->SetSound(Music[MusicArrIdx++]);
	}

}

void AMainGameMode::Tick(float DeltaTime)
{
	PlayMusic();

	if (AudioComponent->IsPlaying())
	{
		bMusickPlaying = true;
	}
	else
	{
		bMusickPlaying = false;
	}
}

void AMainGameMode::PlayMusic()
{
	if (bMusickPlaying) return;

	if (MusicArrIdx == Music.Num())
	{
		MusicArrIdx = 0;
	}

	if (AudioComponent && AudioComponent->Sound)
	{
		AudioComponent->SetSound(Music[MusicArrIdx++]);
	}
	
	AudioComponent->Play()
}
