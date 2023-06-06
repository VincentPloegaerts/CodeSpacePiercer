#pragma once

#include "CoreMinimal.h"
#include "TP_ShootSystemComponent.h"
#include "NiagaraSystem.h"
#include "TP_ShootPlasmaComponent.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_ShootPlasmaComponent : public UTP_ShootSystemComponent
{
	GENERATED_BODY()
protected:
	//Call when you try to Shoot without Ammo 
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoEmpty);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnAmmoEmpty onAmmoEmpty;
private:
	//Timer between pressed button and bullet spawn
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float timeToShoot = 0.25f;
	UPROPERTY()
		FTimerHandle timerPlasma;
public:
	FORCEINLINE FOnAmmoEmpty& OnAmmoEmpty() { return onAmmoEmpty; }
	FORCEINLINE bool FullPlasmaAmmo() { return ammoData.componentAmmo == ammoData.maxAmmo; }
public:
	UTP_ShootPlasmaComponent();
	//Spawn Bullet
	virtual void Shoot() override;
	void SpawnBullet();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	//Remove One Ammo
	virtual void RemoveAmmo() override;
public:
	//Add a amount of _ammoGive
	virtual void AddAmmo(unsigned int _ammoGive) override;
	UFUNCTION() void ShootAOEPlasma();
};
