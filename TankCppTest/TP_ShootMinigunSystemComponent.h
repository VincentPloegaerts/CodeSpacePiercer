#pragma once

#include "CoreMinimal.h"
#include "TP_ShootSystemComponent.h"
#include "TP_Bullet.h"
#include "Particles/ParticleSystem.h"
#include "NiagaraSystem.h"
#include "TP_ShootMinigunSystemComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKCPPTEST_API UTP_ShootMinigunSystemComponent : public UTP_ShootSystemComponent
{
	GENERATED_BODY()
#pragma region Event
protected:
	//Call When Overheat
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverheat);
	UPROPERTY(EditAnywhere,BlueprintReadWrite, BlueprintAssignable)
		FOnOverheat onOverheat;
	//Call One time finish the overheat
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOverHeatStop);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnOverHeatStop onOverheatStop;
	//Call when onHeatJauge is modified
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHeatChange, float, _heatJauge, float,_heatJaugeMax);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnHeatChange onHeatChange;
	//Call when linetrace is hitting a Actor
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHit, FHitResult, _hitResult);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnHit onHit;
private:
#pragma endregion
	UPROPERTY()
		TArray<AActor*> toIgnore;
protected:
#pragma region Stat
	UPROPERTY()
		bool isOverheated = false;
	//heatJauge var
	UPROPERTY()
		float heatJauge = 0;
	//heatJaugeMax
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float heatJaugeMax = 10;
	//time of cooling example : heatJaugeMax of heatJauge = coolingtime second of cooling time, 10 heatJauge = 3 s cooling
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float coolingTime = 3;
	//damage on shield
	UPROPERTY(EditAnyWhere, Category = "ShootSystem | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float shieldDammage = 5;
	//damage on health
	UPROPERTY(EditAnyWhere, Category = "ShootSystem | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float healthDammage = 5;
	//Object Hit linetrace
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		TArray<TEnumAsByte<EObjectTypeQuery>> objectsLayer;
	//particle pop number
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Particle")
		float particleNb = 4;
	//dispersion particle on impact
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Particle")
		float dispersion = 25.f;
	//particle spawn on hit in location hit linetrace
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Particle")
		UNiagaraSystem* particleOnImpact = nullptr;
	//particle variation shield,metal ,etc...
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Particle")
		TArray<UNiagaraSystem*> allParticleOnImpact;
#pragma endregion
public:
	virtual void Shoot() override;
	FORCEINLINE FOnHeatChange& OnHeatChange() { return onHeatChange; }
public:
	UTP_ShootMinigunSystemComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Deal Damage
	void DealDammage(FHitResult& _result);
	//HeatJauge Update 
	void UpdatHeatJauge(float _DeltaTime);
	//particle to use physical material 
	UFUNCTION() void SwitchMaterial(FHitResult result);
};
