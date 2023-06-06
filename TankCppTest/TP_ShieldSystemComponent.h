#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_Bullet.h"
#include "TP_ShieldActor.h"
#include "TP_ShieldSystemComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_ShieldSystemComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	//Call When shield his value equal or inf 0
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShieldDestroy);
	UPROPERTY(EditAnywhere, BlueprintReadWrite,BlueprintAssignable)
		FOnShieldDestroy onShieldDestroy;
	//on hit 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamage);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnDamage onDamage;
	//on value shield modified + or -
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnShieldChange, float, _shield, float, _maxShield);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnShieldChange onShieldChange;
	//Call when shield have a regen value 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegen);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnRegen onRegen;
#pragma endregion
#pragma region Component
	
#pragma endregion
#pragma region ShieldModif
	//actor visuel shield
	UPROPERTY(EditAnywhere, Category = "ShieldComponent | Shield")
		TSubclassOf<ATP_ShieldActor> shields;
	//pointeur current shield actor
	UPROPERTY()
		ATP_ShieldActor* myShieldActor = nullptr;

#pragma endregion
#pragma region Stat
public:
	//shield current value
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "ShieldComponent | Stat", meta = (UMin = "0", ClampMin = "0"))
		float shield = 0;
	//shield max Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ShieldComponent | Stat", meta = (UMin = "0", ClampMin = "0"))
		float maxShield = 200;
private:
	//Value of regen example : regensRate/s
	UPROPERTY(EditAnywhere, Category = "ShieldComponent | Stat")
		float regenRate = 50;
	//waitUntilRegen Start/ReStart
	UPROPERTY(EditAnywhere, Category = "ShieldComponent | Stat")
		float waitUntilRegen = 5.f;
	//if the shield is damageable or not
	UPROPERTY(EditAnywhere, Category = "ShieldComponent | Stat")
		bool isDamageable = true;
	UPROPERTY()
		float time = 0;
#pragma endregion
	UPROPERTY()
		UWorld* world = nullptr;
	//
	UPROPERTY()
		AActor* owner = nullptr;
	//visual effect on hit
	UPROPERTY(EditAnywhere, Category = "Shield | FlashSystem")
		bool canFlash = false;
	//power of flash on hit
	UPROPERTY(EditAnywhere, Category = "Shield | FlashSystem", meta = (ClampMin = "1", ClampMax = "25"))
		float flashPower = 10;
	//time of flash on hit
	UPROPERTY(EditAnywhere, Category = "Shield | FlashSystem")
		float flashTime = 0.05;
	//TimerHandle
	UPROPERTY()
		FTimerHandle timer;
public:
	FORCEINLINE FOnShieldDestroy& OnShieldDestroy() { return onShieldDestroy;}
	FORCEINLINE FOnDamage& OnDamage() { return onDamage; }
	FORCEINLINE FOnShieldChange& OnShieldChange() { return onShieldChange; }
	FORCEINLINE FOnRegen& OnRegen() { return onRegen; }
	FORCEINLINE bool CanRegen() { return time >= waitUntilRegen; }
	FORCEINLINE bool IsBroke() { return shield <= 0; }
	FORCEINLINE float GetShield() { return shield; } ;
	FORCEINLINE float GetMaxShield() { return maxShield; };
	FORCEINLINE bool IsDamageable() { return isDamageable; };
public:
	//Take Damage with the value of damage take
	void TakeDamage(float _damage);
	//Set Value Damageable var
	void SetDamageable(bool _isDamageable);
	//give shieldValue
	void AddShield(float _shieldValue);
	//BreakThe shield
	void DestroyShield();
	//Break the visuel of shield
	void DestroyShieldActor();
	//on hit reset timer regen
	UFUNCTION() void ResetShieldRegen();
public:
	UTP_ShieldSystemComponent();
private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Update shield base on regenRate
	UFUNCTION() void RegenShield();
	void TimeBehaviour();
};
