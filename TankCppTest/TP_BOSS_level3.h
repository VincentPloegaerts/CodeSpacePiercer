#pragma once
#include "CoreMinimal.h"
#include "TP_TankEnemy.h"
#include "TP_ShootTowerSystemComponent.h"
#include "TP_BOSS_level3.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_BOSS_level3 : public ATP_TankEnemy
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
	UTP_ShootTowerSystemComponent* shootMissile = nullptr;
	//multiplie the Speed of the missile by 10 by default 
	UPROPERTY(EditAnywhere, Category = "BoostAttackSpeed")
		float missileSpeedBoost = 10;
	//multiplie the attackSpeed by (1/missileRateBoost) so .1 = *10 to each treshold
	UPROPERTY(EditAnywhere, Category = "BoostAttackSpeed")
		float missileRateBoost = 0.1;
	//multiplie the attackSpeed by (1/miniGunBoost) so .1 = *10 to each treshold
	UPROPERTY(EditAnywhere, Category = "BoostAttackSpeed")
		float miniGunBoost = 0.1;
	//define the time to "activate" the boost
	UPROPERTY(EditAnywhere, Category = "BoostAttackSpeed")
		float timerNoAttackAfterBoost = 3;
public:
	FORCEINLINE UTP_ShootTowerSystemComponent* GetTankNukeSystem() { return shootMissile;}
public:
	ATP_BOSS_level3();
	/// <summary>
	/// used to activate the boss when entering the room
	/// </summary>
	virtual void Activate() override;

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/// <summary>
	/// used to check if the boss as any protection left (like crystal for the ender dragon)
	/// </summary>
	virtual void DefenderDestroy() override;

protected:
	virtual void Init(UWorld* _world) override;
	/// <summary>
	/// best function ever <3
	/// boost stats of the boss after reaching a life treshold (current use is 1/3 and 2/3 hp)
	/// afected stats : - attack speed for mini gun and missiles 
	///					- projectile speed for missiles
	/// </summary>
	UFUNCTION() void Steroid();
	/// <summary>
	/// self explanatory
	/// </summary>
	UFUNCTION()
		void DescativateShoot();
	/// <summary>
	/// self explanatory
	/// </summary>
	UFUNCTION()
		void ActivateShoot();
	/// <summary>
	/// used to kill the actor with a specific way 
	/// </summary>
	virtual void ActorDeath(AActor* _deadActor) override;

};
