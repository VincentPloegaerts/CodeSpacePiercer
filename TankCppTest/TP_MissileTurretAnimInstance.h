#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_Tower.h"
#include "TP_MissileTurretAnimInstance.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_MissileTurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
		UPROPERTY()
		ATP_Tower* owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool isShooting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animparam")
		int death = 0;
public:
	UTP_MissileTurretAnimInstance();
private:
	virtual void NativeBeginPlay() override;

	UFUNCTION() void IsDead(AActor* _deadActor);
	UFUNCTION() void IsShooting();
	UFUNCTION() void IsNotShooting();
};
