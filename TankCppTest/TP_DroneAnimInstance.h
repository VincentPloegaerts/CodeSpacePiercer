#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TP_DroneAnimInstance.generated.h"


class ATP_Drone;

UCLASS()
class TANKCPPTEST_API UTP_DroneAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "EnemyParam")
		bool isStuned = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		int death = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool isShooting = false;
	//false = idle, true walkToward
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool isInRange = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyParam")
		bool isBumped = false;
	UPROPERTY()
		ATP_Drone* owner = nullptr;

	UPROPERTY()
		FTimerHandle timer;
public : 
	UTP_DroneAnimInstance();
private:

	virtual void NativeBeginPlay() override;

	UFUNCTION()		void SetStuned();
	UFUNCTION()		void SetUnStuned();
	UFUNCTION()		void SetDeath(AActor* _deadActor);
	UFUNCTION()		void SetIsShooting();
	UFUNCTION()		void SetNotShooting();
	UFUNCTION()		void SetWalkTo();
	UFUNCTION()		void SetLost();
	UFUNCTION()     void IsBumped();
	UFUNCTION()     void IsNotBumped();


};
