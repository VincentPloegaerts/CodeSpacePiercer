#pragma once
/*===========================================================
* tank Group
* prog 1 2020/2021
*
* Tank enemy entity, used in boss fight and other big room
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_EnemyBoss.h"
#include "TP_AITankMovementComponent.h"
#include "TP_ShootAIMinigunSystemComponent.h"
#include "TP_AIBehaviorComponent.h"
#include "NiagaraSystem.h"
#include "TP_TankEnemy.generated.h"

class UTP_ShootPlasmaComponent;
class UTP_ShootMinigunSystemComponent;
class UTP_AITankBehaviorComponent;
class UTP_DialogueUI;

UCLASS()
class TANKCPPTEST_API ATP_TankEnemy : public ATP_EnemyBoss
{
	GENERATED_BODY()
protected:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTankEnnemyMove);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTankEnnemyMove onTankEnnemyMove;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTankEnnemyAttack);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTankEnnemyAttack onTankEnnemyAttack;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTankEnnemyTargetAcquired);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTankEnnemyTargetAcquired onTankEnnemyTargetAcquired;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTankEnnemyTargetLost);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTankEnnemyTargetLost onTankEnnemyTargetLost;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHalfLife);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnHalfLife onHalfLife;

#pragma endregion

	UPROPERTY()
		UTP_DialogueUI* dialogueUI = nullptr;
	UPROPERTY(EditAnywhere, Category = "TankEnemy | Text")
		TArray<FString> deathText;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* meshTurret = nullptr;
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* meshRadar = nullptr;
	UPROPERTY(EditAnywhere, Category = "TankEnemy")
		UNiagaraComponent* plasmaLoading = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_AITankMovementComponent* aiMovement = nullptr;
	UPROPERTY(EditAnywhere)
		UTP_ShootPlasmaComponent* plasmaComponent;
	UPROPERTY(EditAnywhere)
		UTP_ShootAIMinigunSystemComponent* aiMinigunShootComponent = nullptr;
	UPROPERTY()
		bool shieldWasRestored = false;
	UPROPERTY(EditAnywhere, Category = "TankEnemy");
		bool doRadarMeshRotate = true;

	UPROPERTY(EditAnywhere);
	UDecalComponent* plasmaMaxRange = nullptr;
	UPROPERTY(EditAnywhere);
	UDecalComponent* plasmaCurentCharge = nullptr;
	UPROPERTY()
		UMaterialInstanceDynamic* materialBaseTank;
	UPROPERTY()
		UMaterialInstanceDynamic* materialTurretTank;
	UPROPERTY()
		UMaterialInstanceDynamic* materialRadarTank;
public:
	ATP_TankEnemy();
public:
	FORCEINLINE UTP_ShootPlasmaComponent* GetPlasmaComponent() { return plasmaComponent; }
	FORCEINLINE UTP_ShootMinigunSystemComponent* GetMinigunComponent() { return aiMinigunShootComponent; }
	FORCEINLINE USkeletalMeshComponent* GetMeshTurret() {return meshTurret;}
	FORCEINLINE USkeletalMeshComponent* GetSkeletalMesh() { return meshTurret; }

	FORCEINLINE FEventTankEnnemyMove& OnTankEnnemyMove() { return onTankEnnemyMove; }
	FORCEINLINE FEventTankEnnemyAttack& OnTankEnnemyAttack() { return onTankEnnemyAttack; }
	FORCEINLINE FEventTankEnnemyTargetAcquired& OnTankEnnemyTargetAcquired() { return onTankEnnemyTargetAcquired; }
	FORCEINLINE FEventTankEnnemyTargetLost& OnTankEnnemyTargetLost() { return onTankEnnemyTargetLost; }
	FORCEINLINE FOnHalfLife& OnHalfLife() { return onHalfLife; }

	FORCEINLINE UDecalComponent* GetPlasmaMaxRange() { return plasmaMaxRange; }
	FORCEINLINE UDecalComponent* GetPlasmaCurrentRange() { return plasmaCurentCharge; }
	/// <summary>
	/// used to force destroy the tank mostly used in timed room
	/// </summary>
	virtual void LifeSpanExpired() override;
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//----------behavior calling fct------
	UFUNCTION()	void Lost();
	UFUNCTION() void Attack();
	UFUNCTION()	void Detected();
	//------------------------------------
	/// <summary>
	/// clean death fct , need to be used
	/// </summary>
	virtual void ActorDeath(AActor* _deadActor) override;
	/// <summary>
	/// used to instantly restore the shield at mid life
	/// </summary>
	UFUNCTION() void RestoreShield(bool _isMidLife);
	virtual void Init(UWorld* _world) override;
	/// <summary>
	/// text when the tank die
	/// </summary>
	void DeathText();
protected:
	/// <summary>
	/// change the material when the health change (lerp clean and used texture)
	/// </summary>
	virtual void UpdateMaterial(float _health, float _maxHealth) override;
};
