// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "GoodGame/Character/MainCharacter.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "Components/AudioComponent.h"

AMainGameMode::AMainGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	// Create Audio component and attach to RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

}

void AMainGameMode::Tick(float DeltaTime)
{
	PlayMusic();

	if (AudioComponent->IsPlaying())
	{
		bPlaying = true;
	}
	else
	{
		bPlaying = false;
	}
}

void AMainGameMode::PlayMusic()
{
	if (bPlaying) return;

	if (MusicIdx == Music.Num())
	{
		MusicIdx = 0;
	}

	AudioComponent->SetSound(Music[MusicIdx++]);
	if (AudioComponent && AudioComponent->Sound)
	{
		AudioComponent->Play();
	}
}