#pragma once

#include "CoreMinimal.h"
#include "TP_ShootSystemComponent.h"
#include "NiagaraSystem.h"
#include "TP_ShootTowerSystemComponent.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_ShootTowerSystemComponent : public UTP_ShootSystemComponent
{
	GENERATED_BODY()
	//TimerHandle ShootTimer
	UPROPERTY()
		FTimerHandle shootTimer;
	//amount of bone
	UPROPERTY()
		int boneCount = 0;
	//between 2 bullet timer
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float rateBetweenBullet = 0.1f;
	//
	UPROPERTY()
		float boost = 1;
	//
	UPROPERTY()
		bool isOut = false;
	//
	UPROPERTY()
		bool firstShoot = false;
public:
	UTP_ShootTowerSystemComponent();
	FORCEINLINE float GetRate() { return shootRate; }
	FORCEINLINE void SetRate(float _newRate) { shootRate = _newRate;}
	FORCEINLINE void SetBoost(float _boost) { boost = _boost;}
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Shoot
	virtual void Shoot() override;
	//SpawnBullet
	UFUNCTION() virtual void SpawnBullet();
public:
	//activate shoot
	virtual void ActivateShoot() override;
	//desactivate shoot
	virtual void DesactivateShoot() override;
};
