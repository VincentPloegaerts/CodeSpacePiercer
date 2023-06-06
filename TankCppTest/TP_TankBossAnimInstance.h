#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_BOSS_level3.h"
#include "TP_TankBossAnimInstance.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_TankBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		ATP_BOSS_level3* owner;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool dead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShootingMinigun = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShootingPlasma = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShootingNuke = false;

public:
	UTP_TankBossAnimInstance();
private:
	virtual void NativeBeginPlay()override;

	UFUNCTION() void IsDead(AActor* _deadActor);
	UFUNCTION() void IsShootingPlasma();
	UFUNCTION() void IsNotShootingPlasma();
	UFUNCTION() void IsShootingMinigun();
	UFUNCTION() void IsNotShootingMinigun();
	UFUNCTION() void IsShootingNuke();
	UFUNCTION() void IsNotShootingNuke();
};
