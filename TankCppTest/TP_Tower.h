#pragma once
/*===========================================================
* Tank Team
* prog 1 2020/2021
*
* Drone system, basic enemy, used everywhere
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_Enemy.h"
#include "Components/BoxComponent.h"
#include "TP_Tower.generated.h"

class UTP_ShootTowerSystemComponent;

UCLASS()
class TANKCPPTEST_API ATP_Tower : public ATP_Enemy
{
	GENERATED_BODY()
protected:
		//Call distance to target is <= attackRange
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
			FOnAttack onAttack;
		//Call distance To Target is too much
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetLost);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
			FOnTargetLost onTargetLost;
#pragma region Component

	UPROPERTY(EditAnywhere)
		UBoxComponent* triggerBox = nullptr;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* towerBase = nullptr;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* towerTurret = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_ShootTowerSystemComponent* shootAiSystem = nullptr;
#pragma endregion
	UPROPERTY(EditAnywhere, Category = "Tower | Stat")
		float anticipationSpeed = 0.25f;
	UPROPERTY(EditAnywhere, Category = "Tower | Stat")
		float minTurretInclinaison = -20.f;
	UPROPERTY(EditAnywhere, Category = "Tower | Stat")
		float maxTurretInclinaison = 90.f;
	UPROPERTY()
		bool canRotate = false;
	UPROPERTY()
		ATP_TankPlayer* target = nullptr;
	UPROPERTY()
		FVector localLocation;
	UPROPERTY()
		UMaterialInstanceDynamic* materialTowerBase;
	UPROPERTY()
		UMaterialInstanceDynamic* materialTowerTurret;
public:
	FORCEINLINE UTP_ShootTowerSystemComponent* GetShootTowerSystemComponent() { return shootAiSystem; };
	FORCEINLINE virtual USkeletalMeshComponent* GetSkeletalMesh() { return GetMesh(); };
	FORCEINLINE FOnAttack& OnAttack() { return onAttack; }
	FORCEINLINE FOnTargetLost& OnTargetLost() { return onTargetLost; }
public:
	ATP_Tower();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// make the tower rotate tower terget
	/// </summary>
	void RotationTower(FVector _start, FVector _target);
	//---------------behavior--
	UFUNCTION() void Lost();
	UFUNCTION() void Attack();
	//-------------------------
	/// <summary>
	/// init of the tower
	/// </summary>
	void Init(UWorld* _world) override;
	/// <summary>
	/// make a special death for entity
	/// </summary>
	virtual void ActorDeath(AActor* _deadActor) override;
protected:
	/// <summary>
	/// lerp the texture from good as new to broken
	/// </summary>
	virtual void UpdateMaterial(float _health, float _maxHealth) override;
};
