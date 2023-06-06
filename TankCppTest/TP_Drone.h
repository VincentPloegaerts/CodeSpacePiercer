#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Drone system, basic enemy, used everywhere
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_Enemy.h"
#include "TP_AIMovementComponent.h"
#include "TP_ShootAIMinigunSystemComponent.h"
#include "TP_Drone.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_Drone : public ATP_Enemy
{
	GENERATED_BODY()
#pragma region Event
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneMove);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneMove onDroneMove;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneAttack);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneAttack onDroneAttack;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneTargetAcquired);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneTargetAcquired onDroneTargetAcquired;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneTargetLost);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneTargetLost onDroneTargetLost;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBumped);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnBumped onBumped;
#pragma endregion
	UPROPERTY(EditAnywhere)
		bool asTarget = false;
	UPROPERTY(EditAnywhere)
		bool isAttacking = false;
	UPROPERTY(EditAnywhere, Category = "Enemy | AiMovement")
		UTP_AIMovementComponent* aiMovement = nullptr;
	UPROPERTY(EditAnywhere, Category = "Enemy | AiShootComponent")
		UTP_ShootAIMinigunSystemComponent* aiShoot = nullptr;
	UPROPERTY()
		UMaterialInstanceDynamic* materialMeshDrone;
public:
	UPROPERTY()
		bool wasBumped = false;
public:
	ATP_Drone();

public:
	FORCEINLINE FEventDroneMove& OnDroneMove() { return onDroneMove; }
	FORCEINLINE FEventDroneAttack& OnDroneAttack() { return onDroneAttack; }
	FORCEINLINE FEventDroneTargetAcquired& OnDroneTargetAcquired() { return onDroneTargetAcquired; }
	FORCEINLINE FEventDroneTargetLost& OnDroneTargetLost() { return onDroneTargetLost; }
	FORCEINLINE UTP_AIMovementComponent* GetAiMovement() { return aiMovement; }
	FORCEINLINE FOnBumped& OnBumped() { return onBumped; }
	FORCEINLINE bool isBumped() { return wasBumped; }
	/// <summary>
	/// destroy drone after a timed room mostly
	/// </summary>
	virtual void LifeSpanExpired() override;
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// change the behavior
	/// </summary>
	UFUNCTION() void Lost();
	UFUNCTION()	void Attack();
	UFUNCTION()	void Detected();
	//-----------------------------------
	/// <summary>
	/// make a special death for entity
	/// </summary>
	virtual void ActorDeath(AActor* _deadActor) override;
	/// <summary>
	/// init the drone
	/// </summary>
	virtual void Init(UWorld* _world) override;
protected:
	/// <summary>
	/// lerp the texture from good as new to broken
	/// </summary>
	virtual void UpdateMaterial(float _health, float _maxHealth) override;

};
