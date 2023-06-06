#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_TankPlayer.h"
#include "TP_TankPlayerTurretAnimInstance.generated.h"


UCLASS()
class TANKCPPTEST_API UTP_TankPlayerTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ATP_TankPlayer* owner = nullptr;
	UPROPERTY()
		FTimerHandle timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShootingMinigun = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShootingPlasma = false;
public:
	UTP_TankPlayerTurretAnimInstance();
private:
	virtual void NativeBeginPlay()override;
	UFUNCTION() void IsShootingMinigun();
	UFUNCTION() void StopShootingMinigun();
	UFUNCTION() void IsShootingPlasma();
	UFUNCTION() void IsNotShootingPlasma();
};
