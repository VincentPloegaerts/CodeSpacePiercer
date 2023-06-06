#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Specific AI system, used for the enemy tank
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_AIBehaviorComponent.h"
#include "Components/DecalComponent.h"
#include "TP_AITankBehaviorComponent.generated.h"

class ATP_TankEnemy;

UCLASS()
class TANKCPPTEST_API UTP_AITankBehaviorComponent : public UTP_AIBehaviorComponent
{
	GENERATED_BODY()
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShootPlasma);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnShootPlasma onShootPlasma;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopShootingPlasma);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStopShootingPlasma onStopShootingPlasma;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNormalShooting);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnNormalShooting onNormalShooting;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopNormalShooting);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStopNormalShooting onStopNormalShooting;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharge, bool, _repeat);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnCharge onCharge;


	UPROPERTY()
		UWorld* worldS = nullptr;
	UPROPERTY()
		ATP_TankEnemy* ownerTank = nullptr;
	UPROPERTY()
		bool isDead = false;		
	UPROPERTY()
		FTimerHandle timerAnimPlasma;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Plasma | Animation")
		float timeBeforeRetracting = 1;
	//time for the animation of the plasma canon
	UPROPERTY()
		float timeRetracting = 0;
	UPROPERTY()
		bool retracting = false;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Plasma | Switch Range", meta = (ClampMin = "0", ClampMax = "100000", UIMin = "0", UIMax = "100000"))
		float RangePlasmaActivation = 1200;
	UPROPERTY(VisibleAnywhere, Category = "AI Behaviour | Plasma | Time Activation", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
		float timePlasmaActivation = 0;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Plasma | Time Activation", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
		float timePlasmaActivationMax = 5;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Plasma | Time Activation", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
		float tresholdPlasmaWillContinue = 3;		//if time Plasma > that tank will continue shooting his plasma	
	UPROPERTY(EditAnywhere);
		UDecalComponent* plasmaMaxRange = nullptr;
	UPROPERTY(EditAnywhere);
		UDecalComponent* plasmaCurentCharge = nullptr;
		UPROPERTY()
			bool isLoading = false;
public:
	UTP_AITankBehaviorComponent();
public:
	FORCEINLINE void SetDead() { isDead = true; };
	FORCEINLINE FOnShootPlasma& OnShootPlasma() { return onShootPlasma;}
	FORCEINLINE FOnStopShootingPlasma& OnStopShootingPlasma() { return onStopShootingPlasma; }
	FORCEINLINE FOnNormalShooting& OnNormalShooting() { return onNormalShooting; }
	FORCEINLINE FOnStopNormalShooting& OnStopNormalShooting() { return onStopNormalShooting; }
	FORCEINLINE FOnCharge& OnCharge() { return onCharge; }
	/// <summary>
	/// used to activate tank ai (when entering a room)
	/// </summary>
	virtual void Activation() override;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/// <summary>
	/// change between lost, walk to , attack , use plasma
	/// </summary>
	virtual void AiBehaviour() override;
private:
	/// <summary>
	/// draw the plasma progress debug before the used of decale
	/// </summary>
	void DrawDebugTank(UWorld* _world, FVector _start, FVector _end, float _progress);
	/// <summary>
	/// used to check the distance between the player and the entity and set it accordingly
	/// </summary>
	void setCurrentDistance()override;
	/// <summary>
	/// used to increase the timer of the plasma, set the decale size and fire the plasma if it reach max for timer (could and SHOULD have been split in more fct)
	/// </summary>
	void PlasmaTimerUp();


};
