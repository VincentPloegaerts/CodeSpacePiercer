#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
*
* Basic AI system, used for the drone
*
===========================================================*/
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_TankPlayer.h"
#include "TP_AIBehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_AIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTargetLost);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTargetLost onTargetLost;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTargetNotInReach);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTargetNotInReach onTargetNotInReachAnymore;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventTargetInReach);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventTargetInReach onTargetReached;

#pragma endregion
#pragma region
	UPROPERTY(VisibleAnywhere, Category = "AI Behaviour | Range")
		float distanceTarget = 0;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Range")
		float rangeAgro = 2500;
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Range")
		float maxZRange = 0;
	UPROPERTY()
		int previousStatus = 0;
	UPROPERTY()
		ATP_GameCharacter* owner = nullptr;
	UPROPERTY()
		ATP_TankPlayer* target = nullptr;
#pragma endregion
protected:
	UPROPERTY(EditAnywhere, Category = "AI Behaviour | Range")
		float rangeAttack = 2000;
	UPROPERTY()
		FTimerHandle timerCheckDistance;
	UPROPERTY(EditAnywhere, Category = "AiBehavior | Reaction")
		float reactionTime = 0.5f;
	UPROPERTY(EditAnywhere, Category = "AiBehavior | Reaction")
		float reactionTimeMin = 0.2f;
	UPROPERTY(EditAnywhere, Category = "AiBehavior | Reaction")
		float reactionTimeMax = 0.8f;

public:
	FORCEINLINE FEventTargetLost& OnTargetLost() {	return onTargetLost	;}
	FORCEINLINE FEventTargetNotInReach& OnTargetNotInReachAnymore() { return onTargetNotInReachAnymore; }
	FORCEINLINE FEventTargetInReach& OnTargetReached() { return onTargetReached; }
	FORCEINLINE ATP_TankPlayer* GetTarget() { return target; }
	/// <summary>
	/// used to activate drones ai (when entering a room)
	/// </summary>
	UFUNCTION() virtual void Activation();
public:	
	UTP_AIBehaviorComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/// <summary>
	/// used to change between lost, walk to and attack 
	/// </summary>
	virtual void AiBehaviour();
	/// <summary>
	/// used to check the distance between the player and the entity and set it accordingly
	/// </summary>
	virtual void setCurrentDistance();
private:
	/// <summary>
	/// use to show debugs
	/// </summary>
	void DrawDebug();
};
