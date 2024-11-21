// Fill out your copyright notice in the Description page of Project Settings.


#include "HitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GoodGame/Character/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "WeaponTypes.h"

#include "DrawDebugHelpers.h"

void AHitScanWeapon::Fire(const FVector& HitTarget)
{
    // 부모 클래스(Weapon)의 Fire 함수 호출
    Super::Fire(HitTarget);

    // 무기의 소유자를 APawn으로 캐스팅
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn == nullptr) return;

    // 소유자의 컨트롤러를 가져옴 (데미지 주체로 사용)
    AController* InstigatorController = OwnerPawn->GetController();

    // 총구 소켓(MuzzleFlash)의 위치 가져오기
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if (MuzzleFlashSocket)
    {
        // 소켓의 변환(위치와 회전)을 가져옴
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
        FVector Start = SocketTransform.GetLocation(); // 발사 시작 위치

        // 발사 궤적 계산 및 충돌 판정
        FHitResult FireHit;
        WeaponTraceHit(Start, HitTarget, FireHit); // 레이저 발사

        // 충돌된 대상에게 데미지를 적용
        if (InstigatorController)
        {
            UGameplayStatics::ApplyDamage(
                FireHit.GetActor(), // 데미지를 받을 대상
                Damage,             // 데미지 양
                InstigatorController, // 데미지를 가한 주체
                this,               // 데미지를 가한 무기
                UDamageType::StaticClass() // 데미지 타입
            );
        }

        // 충돌 지점에 파티클 효과를 생성
        if (ImpactParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),                 // 월드 참조
                ImpactParticles,            // 사용할 파티클
                FireHit.ImpactPoint,        // 충돌 지점
                FireHit.ImpactNormal.Rotation() // 충돌 지점의 표면 방향
            );
        }

        // 충돌 지점에 사운드 효과를 재생
        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,                      // 사운드 소유 객체
                HitSound,                  // 사용할 사운드
                FireHit.ImpactPoint        // 재생 위치
            );
        }

        // 총구에서 머즐 플래시 효과를 생성
        if (MuzzleFlash)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),                 // 월드 참조
                MuzzleFlash,                // 사용할 파티클
                SocketTransform             // 총구의 위치와 회전
            );
        }

        // 총구에서 발사 사운드를 재생
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,                       // 사운드 소유 객체
                FireSound,                  // 사용할 사운드
                GetActorLocation()          // 총기의 위치에서 재생
            );
        }
    }
}


void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
    // 월드 객체 가져오기
    UWorld* World = GetWorld();
    if (World)
    {
        // 레이캐스트의 끝점 계산
        FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget)
            : TraceStart + (HitTarget - TraceStart) * 1.25f;

        // 레이캐스트 실행 (충돌된 객체 정보를 OutHit에 저장)
        World->LineTraceSingleByChannel(
            OutHit,                  // 충돌 정보를 저장할 변수
            TraceStart,              // 레이캐스트 시작 지점
            End,                     // 레이캐스트 끝 지점
            ECollisionChannel::ECC_Visibility // 충돌 채널 (가시성)
        );

        FVector BeamEnd = End; // 레이캐스트 끝 지점
        if (OutHit.bBlockingHit) // 충돌이 발생하면
        {
            BeamEnd = OutHit.ImpactPoint; // 충돌 지점을 끝점으로 설정
        }

        // 빔(발사 궤적) 이펙트 생성
        if (BeamParticles)
        {
            // 빔 파티클 효과 생성
            UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
                World,                    // 월드 참조
                BeamParticles,            // 사용할 파티클
                TraceStart,               // 발사 시작 지점
                FRotator::ZeroRotator,    // 회전값 (없음)
                true                      // 지속 여부
            );
            if (Beam)
            {
                // 빔 파티클의 타겟 지점을 충돌 지점으로 설정
                Beam->SetVectorParameter(FName("Target"), BeamEnd);
            }
        }
    }
}

FVector AHitScanWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget)
{
	FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	FVector RandVec = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	FVector EndLoc = SphereCenter + RandVec;
	FVector ToEndLoc = EndLoc - TraceStart;

	
	/*
	DrawDebugSphere(GetWorld(), SphereCenter, SphereRadius, 12, FColor::Red, true);
	DrawDebugSphere(GetWorld(), EndLoc, 4.f, 12, FColor::Orange, true);
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size()),
		FColor::Cyan,
		true);
	*/
	return FVector(TraceStart + ToEndLoc * TRACE_LENGTH / ToEndLoc.Size());
}