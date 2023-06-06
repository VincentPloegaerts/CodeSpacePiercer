#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_TankEnemyAnimInstance.generated.h"

class ATP_TankEnemy;

UCLASS()
class TANKCPPTEST_API UTP_TankEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		int state = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool isShooting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool isShootingPlasma = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool startCharging = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool stopCharging = false;

	UPROPERTY()
		ATP_TankEnemy* owner = nullptr;
	UPROPERTY()
		FTimerHandle timer;


public:
	UTP_TankEnemyAnimInstance();

private:
	virtual void NativeBeginPlay() override;

	UFUNCTION() void IsIdle();
	void IsCharging();
	UFUNCTION() void ReachedMidHP();
	UFUNCTION() void IsDead(AActor* _actor);
	UFUNCTION() void IsShooting();
	UFUNCTION() void IsNotShooting();
	UFUNCTION() void IsShootingPlasma();
	UFUNCTION() void IsNotShootingPlasma();
};
