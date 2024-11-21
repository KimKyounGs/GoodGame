// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMyWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileMyWeapon::Fire(const FVector& HitTarget)
{
    // 부모 클래스의 Fire 함수 호출
    Super::Fire(HitTarget);

    // 무기를 소유한 Pawn 가져오기 (발사 주체)
    APawn* InstigatorPawn = Cast<APawn>(GetOwner());

    // 무기 스켈레탈 메시에서 "MuzzleFlash"라는 이름의 소켓을 가져옴
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
    if (MuzzleFlashSocket) 
    {
        // 소켓의 위치와 회전 정보 (Transform)를 가져옴
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        // 발사 방향 계산: 머즐 플래시 소켓 위치에서 목표 지점까지의 벡터
        FVector ToTarget = HitTarget - SocketTransform.GetLocation();
        // 목표 벡터를 회전값으로 변환
        FRotator TargetRotation = ToTarget.Rotation();

        // 발사체 클래스와 InstigatorPawn이 유효한지 확인
        if (ProjectileClass && InstigatorPawn)
        {
            // 발사체 스폰 시 필요한 파라미터 설정
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();          // 발사체 소유자는 무기 소유자
            SpawnParams.Instigator = InstigatorPawn; // 발사체의 Instigator 설정 (발사 주체)

            // 현재 월드 가져오기
            UWorld* World = GetWorld();
            if (World) 
            {
                // 발사체(Projectile) 스폰
                World->SpawnActor<AProjectile>(
                    ProjectileClass,                // 스폰할 발사체 클래스
                    SocketTransform.GetLocation(),  // 발사체 생성 위치 (머즐 플래시 소켓 위치)
                    TargetRotation,                 // 발사체의 초기 회전값 (목표 방향)
                    SpawnParams                     // 스폰 파라미터
                );
            }
        }
    }
}