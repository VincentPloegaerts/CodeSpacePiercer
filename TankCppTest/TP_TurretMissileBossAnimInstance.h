#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_BOSS_level3.h"
#include "TP_TurretMissileBossAnimInstance.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_TurretMissileBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ATP_BOSS_level3* owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShooting = false;
public:
	UTP_TurretMissileBossAnimInstance();
private:
	virtual void NativeBeginPlay() override;

	UFUNCTION() void IsShooting();
	UFUNCTION() void IsNotShooting();
};
