#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_TurretAnimInstance.generated.h"

class ATP_Tower;

UCLASS()
class TANKCPPTEST_API UTP_TurretAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretParam")
		bool isShooting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurretParam")
		int death = 0;

	UPROPERTY()
		ATP_Tower* owner = nullptr;
public:
	UTP_TurretAnimInstance();
private:
	virtual void NativeBeginPlay() override;

	UFUNCTION() void SetDeath(AActor* _deadActor);
	UFUNCTION() void SetIsShooting();
	UFUNCTION() void SetNotShooting();
};
