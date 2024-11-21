#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"), // 가득차있는.
	ECS_Reloading UMETA(DisplayName = "Reloading"), // 재장전
	ECS_ThrowingGrenade UMETA(DisplayName = "Throwing Grenade"), // 수류탄 던지기

	ECS_MAX UMETA(DisplayName = "DefaultMAX") // 디폴트 값
};