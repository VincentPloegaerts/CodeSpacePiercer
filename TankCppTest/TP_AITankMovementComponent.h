#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Specific AI movement system, used for the enemy tank
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_AIMovementComponent.h"
#include "TP_AITankMovementComponent.generated.h"


class ATP_TankEnemy;
UCLASS()
class TANKCPPTEST_API UTP_AITankMovementComponent : public UTP_AIMovementComponent
{
	GENERATED_BODY()
private:
		UPROPERTY()
		ATP_TankEnemy* isTank = nullptr;
		UPROPERTY()
			USkeletalMeshComponent* skeletalTurretMesh = nullptr;
#pragma region tankEnemyOnly
		/// forward vector mutiplier
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Body", meta = (ClampMin = "0.1", ClampMax = "100", UIMin = "0.1", UIMax = "100"))
			float forwardSpeedMultiplier = 0.5;
		///Body rotation speed
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Body", meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
			float rotationLerpValueBody = 0.1;

		///Turret rotation speed
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Body", meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
			float rotationLerpValueTurret = 0.03;
		/// if true the turret can fully rotate otherwise stop at the cap only for Yaw
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Turret")
			bool canTurretMakeFullRotationYaw = true;
		/// cap for the turret rotation Yaw
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Turret")
			float turretRotationCapYaw = 40;

		/// if true the turret can fully rotate otherwise stop at the cap only for Pitch
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Turret")
			bool canTurretMakeFullRotationPitch = false;
		/// cap for the turret rotation Pitch
		UPROPERTY(EditAnywhere, Category = "Enemy | TankOnly | Turret")
			float turretRotationCapPitch = 20;
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// move toward the target and do this F***ing slow rotation (turret faster than body)
	/// </summary>
	void MoveToTarget(ATP_TankPlayer* _target) override;
	/// <summary>
	/// check if player falling, no more use in the current states, should have been removed
	/// </summary>
	void TargetFalling() override;
};
