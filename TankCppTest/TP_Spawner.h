#pragma once

/*
* Mostly Vincent
* Spawn anything at it's position
* used to have a range and random spawn point but it was messing with the level design 
* and was not used so it got removed
* used 2 set of timers to 
* 1 spawn x enemies at a rate
* 2 spawn x[n] enemie at a second, faster, rate
*/
#include "CoreMinimal.h"
#include "TP_Enemy.h"
#include "TimerManager.h"
#include "TP_Spawner.generated.h"

class UNavigationSystemV1;
class ATP_Room;
class ATP_RoomManager;

UCLASS()
class TANKCPPTEST_API ATP_Spawner : public ATP_Enemy
{
	GENERATED_BODY()

#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn Entity")
		TSubclassOf<ATP_Enemy>spawnEntity;
	UPROPERTY()
		FTimerHandle spawnrateTick;
	UPROPERTY()
		FTimerHandle spawnTick;
	UPROPERTY()
		UNavigationSystemV1* navigationSystem;

#pragma region Spawn Params
	UPROPERTY()
		bool isActive = false;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn range", meta = (ClampMin = "0", ClampMax = "999999", UIMin = "0", UIMax = "999999"))
		float spawnRange = 1000;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn rate", meta = (ClampMin = "0", ClampMax = "50", UIMin = "0", UIMax = "50"))
		int toSpawnPerWave = 5;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn rate", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
		float spawnRate = 10;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn rate", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
		float timeBetweenEachSpawn = 0.5;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn rate", meta = (ClampMin = "-1", ClampMax = "1000", UIMin = "-1", UIMax = "1000"))
		float firstSpawnDelay = 3;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn Location")
		float offsetUpward = 0;
	UPROPERTY(EditAnywhere, Category = "Spawner | Spawn behaviour")
		bool respawnEntity = false;
	UPROPERTY()
		ATP_Enemy* lastSpawnedEnemy = nullptr;
	UPROPERTY()
		FVector localLocation;
	UPROPERTY()
		int toSpawn = 0;
#pragma endregion
#pragma endregion

#pragma region Engine Methods
public:
	ATP_Spawner();
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

#pragma endregion

#pragma region Activation Methods         
public:
	void ActivateSpawner();
	void DesactivateSpawner();
	FORCEINLINE bool IsActive() { return isActive; }
#pragma endregion

#pragma region Behaviour Methods
protected:
	virtual void SpawnerBehaviour();
	virtual void Spawn();
#pragma endregion

#pragma region Debug
private:
	void DrawDebugSpawner();
#pragma endregion

	/// <summary>
	/// Init everything in spawner for gamelogic and such with some debugs
	/// </summary>
	virtual void Init(UWorld* _world) override;
	virtual void ActorDeath(AActor* _deadActor)override;
};
