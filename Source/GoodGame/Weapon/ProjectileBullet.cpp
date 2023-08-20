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
		* UGameplayStatics::ApplyDamage(...): UGameplayStatics 클래스의 ApplyDamage 함수를 호출하여 피해를 적용합니다. 이 함수는 특정 액터에게 피해를 입히는데 사용됩니다.
		OtherActor: 피해를 받게 될 대상 액터입니다.
		Damage: 적용할 피해량입니다.
		OwnerController: 이 피해를 주는 캐릭터의 컨트롤러입니다.
		this: 현재 객체의 포인터입니다. 일반적으로 피해를 주는 객체를 나타냅니다.
		UDamageType::StaticClass(): 적용할 피해 유형의 클래스를 지정합니다. 여기서는 UDamageType의 StaticClass() 함수를 사용하여 기본 피해 유형을 지정하고 있습니다.
		*/
		if (OwnerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("ApplyDamage"));
			UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerController, this, UDamageType::StaticClass());
		}
	}

	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);


}
