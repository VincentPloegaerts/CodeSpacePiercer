#pragma once

#include "CoreMinimal.h"
#include "TP_ShootSystemComponent.h"
#include "TP_ShootNukeComponent.generated.h"

class ATP_Enemy;

UCLASS()
class TANKCPPTEST_API UTP_ShootNukeComponent : public UTP_ShootSystemComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	//Call When a enemy is lock
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLock);
	UPROPERTY(EditAnywhere, BlueprintReadWrite,BlueprintAssignable)
		FOnLock onLockEvent;
	//Call When ammo is equal to 0 when try to shoot
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoEmpty);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnAmmoEmpty onAmmoEmpty;
	//Call When nuke was activate call one time
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNukesFired);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnNukesFired onNukesFired;
	//Call When the nuke finish
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNukesLanded);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnNukesLanded onNukesLanded;

#pragma endregion
	UPROPERTY()
		FTimerHandle timerBullet;
	UPROPERTY()
		float time = 0;
	UPROPERTY()
		float bulletCount = 0;
	UPROPERTY()
		int switchBone = 1;
	UPROPERTY()
		int targetAmount = 0;
	UPROPERTY()
		TArray<AActor*> toIgnore;
	//Array Of Enemy Lock
	UPROPERTY()
		TArray<ATP_Enemy*> targetLock;
	//time between 2 bullet
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float timeBetweenShootOneBullet = 0.25f;
	//time for lock
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float timeToLock = 5;
	//size sphereTrace
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		float sizeLock = 1000;
	//Layer Collision nuke detection
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		TArray<TEnumAsByte<EObjectTypeQuery>> objectsLayer;
public:
	FORCEINLINE FOnLock& OnLock() { return onLockEvent; };
	FORCEINLINE FOnNukesFired& OnNukesFired() { return onNukesFired; }
	FORCEINLINE FOnNukesLanded& OnNukesLanded() { return onNukesLanded; }
	FORCEINLINE FOnAmmoEmpty& OnAmmoEmpty() { return onAmmoEmpty; }
	FORCEINLINE bool FullNukeAmmo() { return ammoData.componentAmmo == ammoData.maxAmmo; }
public:
	UTP_ShootNukeComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Targeting until the end of timer all enemy
	virtual void Target();
	//Stop Nuke Empty all Array
	virtual void FinishTarget();
	//Call When targeting is finish
	virtual void Shoot() override;
	//remove ammo
	virtual void RemoveAmmo() override;
public:
	//add ammo
	virtual void AddAmmo(unsigned int _ammoGive) override;
	//Spawn Bullet recursive
	virtual void SpawnBullet();
};