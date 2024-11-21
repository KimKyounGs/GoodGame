// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMyWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"

void AProjectileMyWeapon::Fire(const FVector& HitTarget)
{
    // �θ� Ŭ������ Fire �Լ� ȣ��
    Super::Fire(HitTarget);

    // ���⸦ ������ Pawn �������� (�߻� ��ü)
    APawn* InstigatorPawn = Cast<APawn>(GetOwner());

    // ���� ���̷�Ż �޽ÿ��� "MuzzleFlash"��� �̸��� ������ ������
    const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
    if (MuzzleFlashSocket) 
    {
        // ������ ��ġ�� ȸ�� ���� (Transform)�� ������
        FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());

        // �߻� ���� ���: ���� �÷��� ���� ��ġ���� ��ǥ ���������� ����
        FVector ToTarget = HitTarget - SocketTransform.GetLocation();
        // ��ǥ ���͸� ȸ�������� ��ȯ
        FRotator TargetRotation = ToTarget.Rotation();

        // �߻�ü Ŭ������ InstigatorPawn�� ��ȿ���� Ȯ��
        if (ProjectileClass && InstigatorPawn)
        {
            // �߻�ü ���� �� �ʿ��� �Ķ���� ����
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = GetOwner();          // �߻�ü �����ڴ� ���� ������
            SpawnParams.Instigator = InstigatorPawn; // �߻�ü�� Instigator ���� (�߻� ��ü)

            // ���� ���� ��������
            UWorld* World = GetWorld();
            if (World) 
            {
                // �߻�ü(Projectile) ����
                World->SpawnActor<AProjectile>(
                    ProjectileClass,                // ������ �߻�ü Ŭ����
                    SocketTransform.GetLocation(),  // �߻�ü ���� ��ġ (���� �÷��� ���� ��ġ)
                    TargetRotation,                 // �߻�ü�� �ʱ� ȸ���� (��ǥ ����)
                    SpawnParams                     // ���� �Ķ����
                );
            }
        }
    }
}