// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayController.h"
#include "GoodGame/HUD/MainHUD.h"
#include "GoodGame/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void AMainPlayController::BeginPlay()
{
	Super::BeginPlay();

	MainHUD = Cast<AMainHUD>(GetHUD());

}

void AMainPlayController::SetHUDHealth(float Health, float MaxHealth)
{
	MainHUD = MainHUD == nullptr ? Cast<AMainHUD>(GetHUD()) : MainHUD;
	MainHUD->AddCharacterOverlay();

	bool bHUDValid = MainHUD &&
		MainHUD->CharacterOverlay &&
		MainHUD->CharacterOverlay->HealthBar &&
		MainHUD->CharacterOverlay->HealthText;

	if (MainHUD->CharacterOverlay) {
		UE_LOG(LogTemp, Warning, TEXT("Can Overlay"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't Overlay"));
	}
	if (bHUDValid)
	{
		UE_LOG(LogTemp, Warning, TEXT("Good Game"));
		const float HealthPercent = Health / MaxHealth;
		UE_LOG(LogTemp, Warning, TEXT("%f"), HealthPercent);
		MainHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		// CeilToInt = 반올림 함수.
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		// FText로 변환할 때 이렇게 사용.
		MainHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Bad Game"));
	}
	
}
