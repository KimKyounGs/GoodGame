// Fill out your copyright notice in the Description page of Project Settings.


#include "Casing.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

ACasing::ACasing()
{
    PrimaryActorTick.bCanEverTick = false;

    // ź���� �޽� ������Ʈ�� ���� �� �ʱ�ȭ
    CasingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CasingMesh"));

    // ź�� �޽ø� ��Ʈ ������Ʈ�� ����
    SetRootComponent(CasingMesh);

    // ī�޶� ä�ΰ��� �浹�� �����ϵ��� ����
    CasingMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    // ���� �ùķ��̼� Ȱ��ȭ
    CasingMesh->SetSimulatePhysics(true);

    // �߷� ���� Ȱ��ȭ
    CasingMesh->SetEnableGravity(true);

    // ������ �ٵ� �浹 �̺�Ʈ�� Ȱ��ȭ
    CasingMesh->SetNotifyRigidBodyCollision(true);

    // ź�ǰ� ����� �� ������ ��ݷ� ����
    ShellEjectionImpulse = 10.f;
}

void ACasing::BeginPlay()
{
    Super::BeginPlay();

    // ������Ʈ �浹 �̺�Ʈ(OnComponentHit)�� �Լ��� ���ε�
    CasingMesh->OnComponentHit.AddDynamic(this, &ACasing::OnHit);

    // GetActorForwardVector(): ź�ǰ� ���ϴ� ������ ��Ÿ���� ����
    // ShellEjectionImpulse: ����Ǵ� �ӵ� ũ��
    CasingMesh->AddImpulse(GetActorForwardVector() * ShellEjectionImpulse);

}

void ACasing::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (ShellSound)
    {
        // ź�ǰ� �浹�� ��ġ���� ShellSound�� ���
        UGameplayStatics::PlaySoundAtLocation(this, ShellSound, GetActorLocation());
    }

    // �浹 �� ź�� ���͸� ����
    Destroy();
}
