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
	// GetOwningPlayerController: �� �Լ��� �Ϲ������� UI �����̳� ���� ���� �ٸ� ������Ʈ���� ���� � �÷��̾ ���� ��Ʈ�ѵǰ� �ִ��� �ľ��� �� ���
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
		//GEngine�� GameViewport ����� ���� ���� ������ ����Ʈ ũ�⸦ ������ ViewportSize ������ �����մϴ�.
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		//ViewportCenter ������ �����ϰ�, �̸� ViewportSize�� �������� �ʱ�ȭ�Ͽ� ����Ʈ�� �߾� ��ġ�� ����մϴ�.
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
		//ũ�ν���� �ؽ�ó�� �׸� �������� ��ǥ�� ����մϴ�. �� ��ǥ�� ȭ���� �߾ӿ��� �ؽ�ó�� ���� ũ�⸦ �� �� Spread ���� ���� ���˴ϴ�. 
		// �̷��� �ϸ� �ؽ�ó�� �߽��� ViewportCenter�� ��ġ�ϰ� �Ǹ�, Spread ���� ���� �̵��ϰ� �˴ϴ�.
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	/*Texture: ȭ�鿡 �׸� �ؽ�ó�Դϴ�.
	TextureDrawPoint.X, TextureDrawPoint.Y: �ؽ�ó�� �׸� �������� X, Y ��ǥ�Դϴ�.
	TextureWidth, TextureHeight : �׸� �ؽ�ó�� �ʺ�� �����Դϴ�.
	0.f, 0.f : �ؽ�ó�� U, V ��ǥ�� �������Դϴ�(�ؽ�ó ������ ������).  -> �ؽ�ó�� ���� �Ʒ� �𼭸��� ��Ÿ���ϴ�.
	1.f, 1.f : �ؽ�ó�� U, V ��ǥ�� �������Դϴ�(�ؽ�ó ������ ������).  -> �ؽ�ó�� ������ �� �𼭸��� ��Ÿ���ϴ�.
	(�� ��� 0.f, 0.f���� �����ؼ� 1.f, 1.f���� �����Ƿ�, ��ü �ؽ�ó �̹����� ���˴ϴ�. 
	���� �ٸ��� �����Ѵٸ�, �ؽ�ó �̹����� Ư�� �κи��� �׸� �� �ֽ��ϴ�.)
	FLinearColor::White : �ؽ�ó�� �׸� �� ����� �����Դϴ�.���⼭�� ������� �����Ǿ� �ֽ��ϴ�.
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