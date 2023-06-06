#pragma once
/*===========================================================
* Avenant Remi
* prog 1 2020/2021
* 
* Basic AI movement system, used for the drone
* 
===========================================================*/
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationSystem.h"
#include "TP_TankPlayer.h"
#include "TP_AIMovementComponent.generated.h"

//class ATP_TankEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_AIMovementComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneStuned);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneStuned onDroneStuned;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventDroneUnStuned);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FEventDroneUnStuned onDroneUnStuned;
#pragma endregion
	UPROPERTY()
		AController* controller = nullptr;
	UPROPERTY()
		ATP_TankPlayer* target = nullptr;
	UPROPERTY()
		ATP_GameCharacter* owner = nullptr;

#pragma region rotation
	/// Angle Step of the rotation
	UPROPERTY(EditAnywhere, Category = "Enemy | AIRotateAroundStats")
		float rotationAngle = 10;
	/// Speed of the rotation
	UPROPERTY(EditAnywhere, Category = "Enemy | AIRotateAroundStats")
		float rotationSpeed = 5;
	/// Maximum distance the entity will start rotation around the player
	UPROPERTY(EditAnywhere, Category = "Enemy | AIRotateAroundStats")
		float rotationDistance = 300;
#pragma endregion

#pragma region stun
	/// Stun timer, set at value and decrease to 0
	UPROPERTY(EditAnywhere, Category = "Enemy | Stun")
		float timerStuned = 0;
	/// If Active the owner will be stunned for the time on top (set timer first for it to work)
	UPROPERTY(EditAnywhere, Category = "Enemy | Stun")
		bool isStuned = false;
	/// If set at true the owner can be stunned
	UPROPERTY(EditAnywhere, Category = "Enemy | Stun")
		bool isStunable = false;
#pragma endregion

#pragma region movement
	/// Owner movement speed
	UPROPERTY(EditAnywhere, Category = "Enemy | MovementOnMesh")
		float movementSpeed = 300;
	/// Power multiplier of jump
	UPROPERTY(EditAnywhere, Category = "Enemy | JumpMovement")
		float jumpStrength = 300;
	/// 65k/70k = falling ; 100k ~= jump ; 150k ~= jump + dash
	UPROPERTY(EditAnywhere, Category = "Enemy | JumpMovement", meta = (ClampMin = "50000", ClampMax = "10000000", UIMin = "50000", UIMax = "10000000"))
		float maxJumpDist = 100000;
	/// Minimum so the owner will not jump and fall on the same platform (at least less often)
	UPROPERTY()
		float minJumpDist = 50000;
#pragma endregion



//	UPROPERTY()
		//ATP_TankEnemy* isTank = nullptr;
#pragma endregion

	UPROPERTY()
		float currentRotationAngle = 0;
	UPROPERTY()
		FVector targetPos = FVector(0, 0, 0);
	UPROPERTY()
		bool updateTargetPos = true;
	UPROPERTY()
		FVector lastAerialPosOffTarget = FVector(0, 0, 0);
	UPROPERTY()
		FVector beginPos = FVector(0, 0, 0);
	UPROPERTY()
		int modeActionEnCours = 0;
	UPROPERTY()
		bool isBeginPosAttributed = false;
	UPROPERTY()
		FVector toCompare = FVector::ZeroVector;
public:
	FORCEINLINE FEventDroneStuned& OnDroneStuned() { return onDroneStuned; }
	FORCEINLINE FEventDroneUnStuned& OnDroneUnStuned() { return onDroneUnStuned; }
	FORCEINLINE bool GetIsStuned() {return isStuned;}
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// update the enemy when stuned (decrease the timer) 
	/// </summary>
	void StunBehaviour(float _DeltaTime);
	/// <summary>
	/// Make the ai jump when it reach the last known point after the player jumped
	/// </summary>
	void MoveToJump();
	/// <summary>
	/// check if the drone as a faster path to target the player (mostly if the player jump behind the drone) so it will not jump uselessly
	/// </summary>
	void PathComparator();
	/// <summary>
	/// Walk toward the player to get closer
	/// </summary>
	virtual void MoveToTarget(ATP_TankPlayer* _target);
	/// <summary>
	/// Walk around the player when in a range
	/// </summary>
	void MoveAroundTarget(float _deltaTime, ATP_TankPlayer* _target);
	/// <summary>
	/// disable movement, clearly not the best solution
	/// </summary>
	void StopMoving();
	/// <summary>
	/// make the enemy look at the player
	/// </summary>
	void LookAt(ATP_TankPlayer* _target);

#pragma region DelayedFunc
	/// <summary>
	/// reset some stats, used after the player jumped 
	/// </summary>
	UFUNCTION()
		void ResetStats();
	/// <summary>
	/// get the last known pos of the player when he leave the ground
	/// </summary>
	UFUNCTION()
		void GetLastAerialPosOffTarget();
#pragma endregion


public:
	UTP_AIMovementComponent();
	/// <summary>
	/// Used to stun the enemy for a said time
	/// </summary>
	UFUNCTION()		void Stunned(float _timeStuned);
	/// <summary>
	/// make special movement on different state (attack and move to basic)
	/// </summary>
	UFUNCTION()		void SetActOnGoing(int _index);
	/// <summary>
	/// used when the player jumped to call 'GetLastAerialPosOffTarget'
	/// </summary>
	UFUNCTION()		virtual void TargetFalling();
	/// <summary>
	/// get the position of the player raycast to the ground
	/// </summary>
	UFUNCTION()		void GetRayCastImpactLocation(FVector _impactPos);
};
