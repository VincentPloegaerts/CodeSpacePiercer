#pragma once

#include "CoreMinimal.h"
#include "TP_Enemy.h"
#include "TP_EnemyBoss.generated.h"


UCLASS(Abstract)
class TANKCPPTEST_API ATP_EnemyBoss : public ATP_Enemy
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Generator | Behaviour")
		TArray<ATP_Enemy*> listOfProtectors;
	UPROPERTY(EditAnywhere, Category = "Generator | Behaviour")
		bool generatorWithProtection = false;
	//le temps ou lon peut le taper
	UPROPERTY(EditAnywhere, Category = "Generator | Parameter")
		float timeDamageable = 5;
	UPROPERTY()
		float time = 0;
	UPROPERTY(EditAnywhere, Category = "Generator | Parameter")
		bool isDead = false;
public:
	ATP_EnemyBoss();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() virtual void DefenderDestroy();
	bool AreProtectorsDead();
	
	virtual void CoolDownRestoreShield(float _deltaTime);
	virtual void RestoreShield();
};
