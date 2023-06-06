#pragma once
/*===========================================================
* DEPRECATED DO NOT USE
===========================================================*/
#include "CoreMinimal.h"
#include "TP_Enemy.h"

#include "TP_TowerMissile.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_TowerMissile : public ATP_Enemy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* towerBase = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "TowerMissile | Shoot")
		class UTP_ShootTowerSystemComponent* shootSystem = nullptr;
public:
	ATP_TowerMissile();
private:
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime)override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
