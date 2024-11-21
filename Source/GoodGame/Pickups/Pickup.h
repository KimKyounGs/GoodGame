// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoodGame/Weapon/WeaponTypes.h"
#include "Pickup.generated.h"

UCLASS()
class GOODGAME_API APickup : public AActor
{
	GENERATED_BODY()
	
public:
	// 생성자: 기본값 초기화
	APickup();

	// 매 프레임 호출: 오버랩 상태나 효과 등을 업데이트
	virtual void Tick(float DeltaTime) override;

	// 액터가 파괴될 때 호출: 파괴 시 추가 작업을 정의
	virtual void Destroyed() override;

protected:
	// 액터가 스폰되거나 시작될 때 호출: 초기화 작업 수행
	virtual void BeginPlay() override;

	// 오버랩 이벤트가 발생했을 때 호출되는 함수
	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent, // 오버랩된 컴포넌트
		AActor* OtherActor,                       // 오버랩된 다른 액터
		UPrimitiveComponent* OtherComp,           // 오버랩된 다른 액터의 컴포넌트
		int32 OtherBodyIndex,                     // 오버랩된 바디의 인덱스
		bool bFromSweep,                          // 스윕 오버랩 여부
		const FHitResult& SweepResult             // 충돌 결과 정보
	);

	// 기본 회전 속도 설정 (예: 픽업 아이템의 회전 효과)
	UPROPERTY(EditAnywhere)
	float BaseTurnRate = 45.f;

private:
	// 픽업 아이템의 오버랩 영역
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	// 픽업 아이템 획득 시 재생될 사운드
	UPROPERTY(EditAnywhere)
	class USoundCue* PickupSound;

	// 픽업 아이템의 메시 (외형)
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	// 픽업 아이템의 시각적 효과 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* PickupEffectComponent;

	// 픽업 효과에 사용할 Niagara 시스템
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* PickupEffect;

	// 오버랩 타이머 핸들: 일정 시간 후 오버랩 동작을 제어하기 위해 사용
	FTimerHandle BindOverlapTimer;

	// 오버랩 타이머 시간 설정
	float BindOverlapTime = 0.25f;

	// 오버랩 타이머 완료 후 호출되는 함수
	void BindOverlapTimerFinished();

public:

};
