#pragma once

#include "CoreMinimal.h"
#include "TP_Bullet.h"
#include "Components/ActorComponent.h"
#include "TP_HealthComponent.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FHealthStats{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Stats | MaxHealth")
		float maxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats | Health")
		float health = 100.f;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_HealthComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
		FHealthStats healthStats;
	UPROPERTY()
		bool midLife = false;
	UPROPERTY()
		bool oneThird = false;
	UPROPERTY()
		bool twoThird = false;
#pragma endregion
public:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamage);
	UPROPERTY(EditAnywhere, BlueprintReadWrite,BlueprintAssignable)
		FOnTakeDamage onTakeDamage;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMidLife,bool,_midLife);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnMidLife onMidLife;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOneThirdLife);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnOneThirdLife onOneThirdLife;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTwoThirdLife);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnTwoThirdLife onTwoThirdLife;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathEvent,AActor*,_deadActor);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnDeathEvent onDeathEvent;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddLife);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnAddLife onAddLife;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLifeChange, float ,_health,float , _maxHealth);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnLifeChange onLifeChange;
#pragma endregion
public:
	FORCEINLINE FOnDeathEvent& OnDeathEvent() { return onDeathEvent;}
	FORCEINLINE FOnLifeChange& OnLifeChange() { return onLifeChange;}
	FORCEINLINE FOnAddLife& OnAddLife() { return onAddLife;}
	FORCEINLINE FOnTakeDamage& OnTakeDamage() { return onTakeDamage;}
	FORCEINLINE FOnMidLife& OnMidLife() { return onMidLife;}
	FORCEINLINE FOnOneThirdLife& OnOneThirdLife() { return onOneThirdLife;}
	FORCEINLINE FOnTwoThirdLife& OnTwoThirdLife() { return onTwoThirdLife;}
	FORCEINLINE bool IsDead() { return healthStats.health <= 0; }
	FORCEINLINE bool IsLifeFull() { return healthStats.health == healthStats.maxHealth; }
	FORCEINLINE bool IsLifeHalfFull() { return healthStats.health <= (healthStats.maxHealth)/2; }
public:
	FORCEINLINE FHealthStats* GetStats() { return &healthStats; }
public:	
	UTP_HealthComponent();

private:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void TakeDamage(float _damage);
	void AddStat(float _health);
	void KillActor();
private:
};