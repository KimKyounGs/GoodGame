// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // �� �ڵ�� ��� �浹 ä�ο� ���� CollisionBox�� ������ 'Ignore'(����)�� �����մϴ�. ��, �⺻������ ��� ������ �浹�� �����ϵ��� �����˴ϴ�.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //�� �ڵ�� 'Visibility' �浹 ä�ο� ���� CollisionBox�� ������ 'Block'(����)���� �����մϴ�.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block); //�� �ڵ�� 'WorldStatic' �浹 ä�ο� ���� CollisionBox�� ������ 'Block'���� �����մϴ�. ��

}


void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

