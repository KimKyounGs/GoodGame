// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameMode.h"
#include "GoodGame/Character/MainCharacter.h"
#include "GoodGame/PlayerController/MainPlayController.h"
#include "Components/AudioComponent.h"

AMainGameMode::AMainGameMode()
{
	// Create Audio component and attach to RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);

}

void AMainGameMode::BeginPlay()
{
	int32 Selection = FMath::RandRange(0, Music.Num() - 1);
	AudioComponent->SetSound(Music[Selection]);
	if (AudioComponent && AudioComponent->Sound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Play Music"));
		AudioComponent->Play();
	}

}

void AMainGameMode::PlayerEliminated(AMainCharacter* ElimmedCharacter, AMainPlayController* VictimController, AMainPlayController* AttackerController)
{

}
