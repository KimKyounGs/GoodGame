// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AController* OwnerController = OwnerCharacter->Controller;
		/*
		* UGameplayStatics::ApplyDamage(...): UGameplayStatics Ŭ������ ApplyDamage �Լ��� ȣ���Ͽ� ���ظ� �����մϴ�. �� �Լ��� Ư�� ���Ϳ��� ���ظ� �����µ� ���˴ϴ�.
		OtherActor: ���ظ� �ް� �� ��� �����Դϴ�.
		Damage: ������ ���ط��Դϴ�.
		OwnerController: �� ���ظ� �ִ� ĳ������ ��Ʈ�ѷ��Դϴ�.
		this: ���� ��ü�� �������Դϴ�. �Ϲ������� ���ظ� �ִ� ��ü�� ��Ÿ���ϴ�.
		UDamageType::StaticClass(): ������ ���� ������ Ŭ������ �����մϴ�. ���⼭�� UDamageType�� StaticClass() �Լ��� ����Ͽ� �⺻ ���� ������ �����ϰ� �ֽ��ϴ�.
		*/
		if (OwnerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);


}
