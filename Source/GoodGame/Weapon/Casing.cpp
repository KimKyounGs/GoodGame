// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
    PrimaryActorTick.bCanEverTick = false;

    // 탄피의 메시 컴포넌트를 생성 및 초기화
    CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));

    // 탄피 메시를 루트 컴포넌트로 설정
    SetRootComponent(CasingMesh);

    // 카메라 채널과의 충돌을 무시하도록 설정
    CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    // 물리 시뮬레이션 활성화
    CasingMesh->SetSimulatePhysics(true);

    // 중력 적용 활성화
    CasingMesh->SetEnableGravity(true);

    // 리지드 바디 충돌 이벤트를 활성화
    CasingMesh->SetNotifyRigidBodyCollision(true);

    // 탄피가 배출될 때 가해질 충격량 설정
    ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
    Super::BeginPlay();

    // 컴포넌트 충돌 이벤트(OnComponentHit)에 함수를 바인딩
    CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

    // GetActorForwardVector(): 탄피가 향하는 방향을 나타내는 벡터
    // ShellEjectionImpulse: 배출되는 속도 크기
    CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);

}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (ShellSound)
    {
        // 탄피가 충돌한 위치에서 ShellSound를 재생
        UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
    }

    // 충돌 후 탄피 액터를 제거
    Destroy();
}
