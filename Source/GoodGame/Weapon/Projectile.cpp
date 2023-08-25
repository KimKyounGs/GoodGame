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
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); // 이 코드는 모든 충돌 채널에 대한 CollisionBox의 반응을 'Ignore'(무시)로 설정합니다. 즉, 기본적으로 모든 유형의 충돌을 무시하도록 설정됩니다.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //이 코드는 'Visibility' 충돌 채널에 대한 CollisionBox의 반응을 'Block'(차단)으로 설정합니다.
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block); //이 코드는 'WorldStatic' 충돌 채널에 대한 CollisionBox의 반응을 'Block'으로 설정합니다.
	CollisionBox->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECollisionResponse::ECR_Block);

}



void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (Tracer)
	{
		// UGameplayStatics::SpawnEmitterAttached : 이 함수는 지정된 배우나 컴포넌트에 파티클 시스템 이밋터 (emitter)를 부착하여 생성합니다.
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer, // 부착할 파티클 시스템 
			CollisionBox, // 파티클 시스템을 부착할 컴포넌트 또는 엑터
			FName(), // 소켓 이름이나 태그가 필요하지 않은 경우 비어 있는 FName()전달
			GetActorLocation(), // 파티클 시스템이 부착될 엑터 위치
			GetActorRotation(), // 파이클 시스템이 부착될 엑터 회전
			EAttachLocation::KeepWorldPosition // 이 파라미터는 이밋터가 월드 공간의 위치를 유지하도록 지정.
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
	* 상대가 때렸을 떄 맞는 애니메이션 재생
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
