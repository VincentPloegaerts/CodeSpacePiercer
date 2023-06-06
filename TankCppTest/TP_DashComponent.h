// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TP_DashComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_DashComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDash);
	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintReadWrite)
		FOnDash onDash;


	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		FName boneName = "";
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		UNiagaraSystem* dashEffect = nullptr;
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		float dashPower = 1500;
	UPROPERTY(EditAnywhere, Category= "DashComponent | Stat")
		float cooldown = 3;
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		float  jumpPower = 300.f;

	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		FVector offSet = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		FRotator offSetRotator = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		float  dashSpeedDivider = 2;
	UPROPERTY(EditAnywhere, Category = "DashComponent | Stat")
		float  delayBeforDashSlow = 0.2;
	UPROPERTY(EditAnywhere, Category = "DashComponent | KB")
		float maxVelocity = 100000;
	UPROPERTY(EditAnywhere, Category = "DashComponent | KB")
		float kbPower = 500;
	UPROPERTY(EditAnywhere, Category = "DashComponent | KB")
		float kbBumpUp = 20;
	UPROPERTY(EditAnywhere, Category = "DashComponent | KB")
		float sphereKBSize = 10;
	UPROPERTY(EditAnywhere, Category = "DashComponent | KB")
		float delayedDash = 0.01f;

	UPROPERTY()
		float time = 0;
	UPROPERTY()
		bool canDash = true;
	UPROPERTY()
		bool isActivated = true;
	UPROPERTY()
		UCharacterMovementComponent* charaComponent = nullptr;
	UPROPERTY() FVector currentDash = FVector::ZeroVector;
	UPROPERTY()
		bool dashKBOn = false;
public:
	UPROPERTY() float vertical = 0 ;
	UPROPERTY() float horizontal = 0;

public:
	FORCEINLINE FOnDash& OnDash() { return onDash; }
	FORCEINLINE bool IsCoolDawnDone() { return time >= cooldown;}
	FORCEINLINE bool IsActivated() { return isActivated; }
	FORCEINLINE void SetIsActivated(bool _status) {  isActivated = _status; }

public:
	UTP_DashComponent();

private: 
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION() void KnockBack();
	UFUNCTION() void timeKB();
	UFUNCTION() void kbZoneOff();
	void FindOwnerMovementComponent();
public:
	void TimeProgress();
	void Dash();
	void DashForward();
	void DashSideway();
	void BaseDash();
	void DashSlow();
};
