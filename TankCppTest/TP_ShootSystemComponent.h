#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Components/ActorComponent.h"
#include "TP_ShootSystemComponent.generated.h"

class ATP_Bullet;
//Ammo Stokage struct
USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Ammo Data | Component Ammo")
		int componentAmmo = 0;
	UPROPERTY(EditAnywhere, Category = "Ammo Data | Component Max Ammo")
		int maxAmmo = 0;
	FAmmoData() {};
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_ShootSystemComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	//Call On Bullet Spawn/Launch
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnShootEvent onShootEvent;
	//Call When ActivateShoot example : first click until the release and re click
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartShootEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStartShootEvent onStartShootEvent;
	//Call When DesactivateShoot example : when release
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopShootEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStopShootEvent onStopShootEvent;
	//Call when the Jauge of reload is moving/changing
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReload,float, _reloadValue,float,_maxReloadValue);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnReload onReload;
	//Call when you add a munition or remove a munition
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMunitionChange, int, _munitionValue);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
	FOnMunitionChange onMunitionChange;
#pragma endregion
#pragma region Stat
	UPROPERTY()
		float shootTime = 0;
	//shootRate bullet per second example : 0.5f = 2 bullet/s
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat", meta = (ClampMin = "0.001", UIMin = "0.001"))
		float shootRate = 0.5f;
	//Use In Minigun System : LineTrace Range, NukeSystem : SphereTrace Range
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat", meta = (ClampMin = "0", UIMin = "0"))
		float attackRange = 1000;
	//Type of bullet spawn
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		TSubclassOf<ATP_Bullet> bulletType;
	//Struct AmmoStokage
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		FAmmoData ammoData;
	//particle emmit onn bullet spawn
	UPROPERTY(EditAnywhere, Category = "ShootSystem | Stat")
		UNiagaraSystem* particleOnShoot = nullptr;
#pragma endregion
	UPROPERTY(EditAnywhere, Category = "ShootSystem | BonesName")
		USkeletalMeshComponent* meshShoot = nullptr;
	//Locations of Spawn Bullet with bones name
	UPROPERTY(EditAnywhere, Category = "ShootSystem | BonesName")
		TArray<FName> bonesName;
	//Can Shoot
	UPROPERTY()
		bool canShoot = true;
	//example triggerShoot = true when click is pressed
	UPROPERTY()
		bool isTriggerShoot = false;
	UPROPERTY()
		class ATP_GameCharacter* owner = nullptr;
	UPROPERTY()
		bool isActivated = true;
public:
	FORCEINLINE FOnReload& OnReload() { return onReload; };
	FORCEINLINE FOnMunitionChange& OnMunitionChange() { return onMunitionChange; };
	FORCEINLINE FOnShootEvent& OnShootEvent() { return onShootEvent; };
	FORCEINLINE FOnStartShootEvent& OnStartShootEvent() { return onStartShootEvent; };
	FORCEINLINE FOnStopShootEvent& OnStopShootEvent() { return onStopShootEvent; };
	FORCEINLINE virtual bool& GetShootActivate() { return isTriggerShoot; };
	FORCEINLINE virtual bool IsActivated() { return isActivated; };
	FORCEINLINE virtual void SetIsActivated(bool _status) { isActivated = _status; };
	//Activate shoot : example on pressed button
	UFUNCTION() virtual void ActivateShoot();
	//Desactivate shoot : example released button
	UFUNCTION() virtual void DesactivateShoot();
	//add ammo
	virtual void AddAmmo(unsigned int _ammoGive);
	//Remove One Ammo
	virtual void RemoveAmmo();
	virtual void SetMeshShoot(USkeletalMeshComponent* _meshShoot);
public:	
	UTP_ShootSystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//SpawnBullet
	UFUNCTION() virtual void Shoot();
	//Reload after Shooting
	void CoolDown(float _DeltaTime);
};