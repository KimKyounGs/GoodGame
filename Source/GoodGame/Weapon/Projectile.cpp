// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "GoodGame/Character/MainCharacter.h"
#include "GoodGame/GoodGame.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // �� �ڵ�� ��� �浹 ä�ο� ���� CollisionBox�� ������ 'Ignore'(����)�� �����մϴ�. ��, �⺻������ ��� ������ �浹�� �����ϵ��� �����˴ϴ�.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //�� �ڵ�� 'Visibility' �浹 ä�ο� ���� CollisionBox�� ������ 'Block'(����)���� �����մϴ�.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block); //�� �ڵ�� 'WorldStatic' �浹 ä�ο� ���� CollisionBox�� ������ 'Block'���� �����մϴ�.
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

}



void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		// UGameplayStatics::SpawnEmitterAttached : �� �Լ��� ������ ��쳪 ������Ʈ�� ��ƼŬ �ý��� �̹��� (emitter)�� �����Ͽ� �����մϴ�.
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer, // ������ ��ƼŬ �ý��� 
			CollisionBox, // ��ƼŬ �ý����� ������ ������Ʈ �Ǵ� ����
			FName(), // ���� �̸��̳� �±װ� �ʿ����� ���� ��� ��� �ִ� FName()����
			GetActorLocation(), // ��ƼŬ �ý����� ������ ���� ��ġ
			GetActorRotation(), // ����Ŭ �ý����� ������ ���� ȸ��
			EAttachLocation::KeepWorldPosition // �� �Ķ���ʹ� �̹��Ͱ� ���� ������ ��ġ�� �����ϵ��� ����.
		);
	}

	CollisionBox->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	/*
	* ��밡 ������ �� �´� �ִϸ��̼� ���
	*/
	if (MainCharacter)
	{
		MainCharacter->PlayHitReactMontage();
	}

	Destroy();
}



void AProjectile::Destroyed()
{
	Super::Destroyed();

	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}
