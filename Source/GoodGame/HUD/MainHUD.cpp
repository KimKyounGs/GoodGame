// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "GameFramework/PlayerController.h"
#include "CharacterOverlay.h"
#include "GoodGame/Enemy/EnemyOverlay.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	AddCharacterOverlay();
}

void AMainHUD::AddCharacterOverlay()
{
	// GetOwningPlayerController: 이 함수는 일반적으로 UI 위젯이나 게임 내의 다른 오브젝트들이 현재 어떤 플레이어에 의해 컨트롤되고 있는지 파악할 때 사용
	APlayerController* PlayerController = GetOwningPlayerController();
	if (PlayerController && CharacterOverlayClass)
	{
		CharacterOverlay = CreateWidget<UCharacterOverlay>(PlayerController, CharacterOverlayClass);
		
		CharacterOverlay->AddToViewport();
	}
}

void AMainHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if (GEngine)
	{
		//GEngine의 GameViewport 멤버를 통해 현재 게임의 뷰포트 크기를 가져와 ViewportSize 변수에 저장합니다.
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		//ViewportCenter 변수를 선언하고, 이를 ViewportSize의 절반으로 초기화하여 뷰포트의 중앙 위치를 계산합니다.
		const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

		float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;

		if (HUDPackage.CrosshairsCenter)
		{
			FVector2D Spread(0.f, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsLeft)
		{
			FVector2D Spread(-SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsRight)
		{
			FVector2D Spread(SpreadScaled, 0.f);
			DrawCrosshair(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsTop)
		{
			FVector2D Spread(0.f, -SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
		if (HUDPackage.CrosshairsBottom)
		{
			FVector2D Spread(0.f, SpreadScaled);
			DrawCrosshair(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.CrosshairsColor);
		}
	}
}

void AMainHUD::DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		//크로스헤어 텍스처를 그릴 시작점의 좌표를 계산합니다. 이 좌표는 화면의 중앙에서 텍스처의 절반 크기를 뺀 후 Spread 값을 더해 계산됩니다. 
		// 이렇게 하면 텍스처의 중심이 ViewportCenter에 위치하게 되며, Spread 값에 따라 이동하게 됩니다.
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	/*Texture: 화면에 그릴 텍스처입니다.
	TextureDrawPoint.X, TextureDrawPoint.Y: 텍스처를 그릴 시작점의 X, Y 좌표입니다.
	TextureWidth, TextureHeight : 그릴 텍스처의 너비와 높이입니다.
	0.f, 0.f : 텍스처의 U, V 좌표의 시작점입니다(텍스처 매핑의 시작점).  -> 텍스처의 왼쪽 아래 모서리를 나타냅니다.
	1.f, 1.f : 텍스처의 U, V 좌표의 종료점입니다(텍스처 매핑의 종료점).  -> 텍스처의 오른쪽 위 모서리를 나타냅니다.
	(이 경우 0.f, 0.f에서 시작해서 1.f, 1.f에서 끝나므로, 전체 텍스처 이미지가 사용됩니다. 
	만약 다르게 설정한다면, 텍스처 이미지의 특정 부분만을 그릴 수 있습니다.)
	FLinearColor::White : 텍스처를 그릴 때 사용할 색상입니다.여기서는 흰색으로 설정되어 있습니다.
	*/

	DrawTexture(
		Texture,
		TextureDrawPoint.X,
		TextureDrawPoint.Y,
		TextureWidth,
		TextureHeight,
		0.f,
		0.f,
		1.f,
		1.f,
		CrosshairColor
	);
}