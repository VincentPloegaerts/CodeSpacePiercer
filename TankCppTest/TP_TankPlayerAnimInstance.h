#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_TankPlayer.h"
#include "TP_TankPlayerAnimInstance.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_TankPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ATP_TankPlayer* owner = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool death = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		bool fireNuke = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		float vertical = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimParam")
		float horizontal = 0;
public:
	UTP_TankPlayerAnimInstance();
private:
	virtual void NativeBeginPlay()override;

	UFUNCTION() void IsDead(AActor* _deadActor);
	UFUNCTION() void FireNuke();
	UFUNCTION() void NukeFired();
	UFUNCTION() void UpdateAxisValues();
};
