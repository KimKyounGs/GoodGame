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
    // �θ� Ŭ����(Weapon)�� Fire �Լ� ȣ��
    Super::Fire(HitTarget);

    // ������ �����ڸ� APawn���� ĳ����
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn == nullptr) return;

    // �������� ��Ʈ�ѷ��� ������ (������ ��ü�� ���)
    AController* InstigatorController = OwnerPawn->GetController();

    // �ѱ� ����(MuzzleFlash)�� ��ġ ��������
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName("MuzzleFlash");
    if (MuzzleFlashSocket)
    {
        // ������ ��ȯ(��ġ�� ȸ��)�� ������
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
        FVector Start = SocketTransform.GetLocation(); // �߻� ���� ��ġ

        // �߻� ���� ��� �� �浹 ����
        FHitResult FireHit;
        WeaponTraceHit(Start, HitTarget, FireHit); // ������ �߻�

        // �浹�� ��󿡰� �������� ����
        if (InstigatorController)
        {
            UGameplayStatics::ApplyDamage(
                FireHit.GetActor(), // �������� ���� ���
                Damage,             // ������ ��
                InstigatorController, // �������� ���� ��ü
                this,               // �������� ���� ����
                UDamageType::StaticClass() // ������ Ÿ��
            );
        }

        // �浹 ������ ��ƼŬ ȿ���� ����
        if (ImpactParticles)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),                 // ���� ����
                ImpactParticles,            // ����� ��ƼŬ
                FireHit.ImpactPoint,        // �浹 ����
                FireHit.ImpactNormal.Rotation() // �浹 ������ ǥ�� ����
            );
        }

        // �浹 ������ ���� ȿ���� ���
        if (HitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,                      // ���� ���� ��ü
                HitSound,                  // ����� ����
                FireHit.ImpactPoint        // ��� ��ġ
            );
        }

        // �ѱ����� ���� �÷��� ȿ���� ����
        if (MuzzleFlash)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                GetWorld(),                 // ���� ����
                MuzzleFlash,                // ����� ��ƼŬ
                SocketTransform             // �ѱ��� ��ġ�� ȸ��
            );
        }

        // �ѱ����� �߻� ���带 ���
        if (FireSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,                       // ���� ���� ��ü
                FireSound,                  // ����� ����
                GetActorLocation()          // �ѱ��� ��ġ���� ���
            );
        }
    }
}


void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit)
{
    // ���� ��ü ��������
    UWorld* World = GetWorld();
    if (World)
    {
        // ����ĳ��Ʈ�� ���� ���
        FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget)
            : TraceStart + (HitTarget - TraceStart) * 1.25f;

        // ����ĳ��Ʈ ���� (�浹�� ��ü ������ OutHit�� ����)
        World->LineTraceSingleByChannel(
            OutHit,                  // �浹 ������ ������ ����
            TraceStart,              // ����ĳ��Ʈ ���� ����
            End,                     // ����ĳ��Ʈ �� ����
            ECollisionChannel::ECC_Visibility // �浹 ä�� (���ü�)
        );

        FVector BeamEnd = End; // ����ĳ��Ʈ �� ����
        if (OutHit.bBlockingHit) // �浹�� �߻��ϸ�
        {
            BeamEnd = OutHit.ImpactPoint; // �浹 ������ �������� ����
        }

        // ��(�߻� ����) ����Ʈ ����
        if (BeamParticles)
        {
            // �� ��ƼŬ ȿ�� ����
            UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
                World,                    // ���� ����
                BeamParticles,            // ����� ��ƼŬ
                TraceStart,               // �߻� ���� ����
                FRotator::ZeroRotator,    // ȸ���� (����)
                true                      // ���� ����
            );
            if (Beam)
            {
                // �� ��ƼŬ�� Ÿ�� ������ �浹 �������� ����
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