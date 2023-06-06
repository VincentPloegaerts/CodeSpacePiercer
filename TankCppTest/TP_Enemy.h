#pragma once
/*===========================================================
* prog 1 2020/2021
*
* Drone system, basic enemy, used everywhere
*
===========================================================*/
#include "CoreMinimal.h"
#include "TP_GameCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "TP_Enemy.generated.h"

class UTP_AIBehaviorComponent;
class ATP_RoomManager;
class ATP_Room;

UCLASS()
class TANKCPPTEST_API ATP_Enemy : public ATP_GameCharacter
{
	GENERATED_BODY()
protected:
#pragma region Component
	UPROPERTY(EditAnywhere, Category = "Enemy | Anim")
		UNiagaraComponent* niagaraFxMidLife = nullptr;
	UPROPERTY(EditAnywhere, Category = "Enemy | AiBehavior")
		UTP_AIBehaviorComponent* aiBehaviour = nullptr;
	UPROPERTY(EditAnywhere, Category = "Enemy | SphereColision")
		USphereComponent* sphereComponent = nullptr;

#pragma endregion
	UPROPERTY(VisibleAnywhere, Category = "Room | Room owner")
		ATP_Room* roomOwner = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | Room Name")
		FString roomOwnerName;
	UPROPERTY(VisibleAnywhere, Category = "Room | Room Manager")
		ATP_RoomManager* roomManager;
	UPROPERTY(EditAnywhere, Category = "Enemy | Anim")
		UNiagaraSystem* niagaraOnDeath = nullptr;
	UPROPERTY(EditAnywhere, Category = "Enemy | Anim")
		UNiagaraSystem* enemyExplosionFx = nullptr;
	//timer before entity despawn
	UPROPERTY(EditAnywhere, Category = "Enemy | Anim", meta = (ClampMin = "0.01", ClampMax = "2000", UIMin = "0.01", UIMax = "2000"))
		float deathAnimDuration = 1;
	//can the player colide with the dead body
	UPROPERTY(EditAnywhere, Category = "Enemy | Anim")
		bool deathAnimCollisionEnable = false;


public:
	FORCEINLINE UTP_AIBehaviorComponent* GetAIBehaviorComponent() { return aiBehaviour;  }
	FORCEINLINE ATP_Room* GetRoomOwner() { return roomOwner; }
	FORCEINLINE FString GetRoomOwnerName() { return roomOwnerName; }
	FORCEINLINE ATP_RoomManager* GetRoomManager() { return roomManager; }
	FORCEINLINE USphereComponent* GetSphereComponent() { return sphereComponent; }

	FORCEINLINE void SetRoomOwner(ATP_Room* _room) { roomOwner=_room; }
	FORCEINLINE void SetRoomOwnerName(FString _name) { roomOwnerName = _name; }
	FORCEINLINE void SetRoomManager(ATP_RoomManager* _roomManager) { roomManager= _roomManager; }

public:
	ATP_Enemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/// <summary>
	/// init the enemy
	/// </summary>
	virtual void Init(UWorld* _world);
	/// <summary>
	/// play the death FX
	/// </summary>
	UFUNCTION() void DeathFx(AActor* _b);
public:
	FVector GetSphereScale();
	/// <summary>
	/// init the drone
	/// </summary>
	virtual void ActorDeath(AActor* _deadActor) override;
	/// <summary>
	/// activate enemy
	/// </summary>
	virtual void Activate() override;
	/// <summary>
	/// desactivate enemy
	/// </summary>
	virtual void Desactivate() override;
};
