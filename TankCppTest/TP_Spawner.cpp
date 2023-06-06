#include "TP_Spawner.h"
#include "TP_Room.h"
#include "TP_Drone.h"
#include "TP_Tower.h"
#include "TP_TankEnemy.h"
#include "TP_GameMode.h"
#include "TP_GameLogic.h"
#include "TP_RoomManager.h"
#include "TP_AIBehaviorComponent.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"

#pragma region Engine Methods
ATP_Spawner::ATP_Spawner()
{
}
void ATP_Spawner::BeginPlay()
{
	Super::BeginPlay();
	UWorld* _world = GetWorld();
	if (!_world)return;
	_world->GetTimerManager().SetTimer(spawnrateTick, this, &ATP_Spawner::SpawnerBehaviour, spawnRate, true, firstSpawnDelay);
	localLocation = GetActorLocation();
	if (!isActive)
		_world->GetTimerManager().PauseTimer(spawnrateTick);
}
void ATP_Spawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorld()->GetAuthGameMode<ATP_GameMode>()->GetDebugDraw())return;
	DrawDebugSpawner();
}
#pragma endregion
#pragma region Activation
void ATP_Spawner::ActivateSpawner()
{
	isActive = true;
	UWorld* _world = GetWorld();
	if (!_world)return;
	_world->GetTimerManager().UnPauseTimer(spawnrateTick);
}
void ATP_Spawner::DesactivateSpawner()
{
	isActive = false; 
	UWorld* _world = GetWorld();
	if (!_world)return;
	_world->GetTimerManager().SetTimer(spawnrateTick, this, &ATP_Spawner::SpawnerBehaviour, spawnRate, true, firstSpawnDelay);
	_world->GetTimerManager().PauseTimer(spawnrateTick);
}
#pragma endregion

#pragma region Behaviour methods
void ATP_Spawner::SpawnerBehaviour()
{
	if (!isActive)return;
	if (respawnEntity && lastSpawnedEnemy && !lastSpawnedEnemy->IsActorBeingDestroyed())
		return;
	UWorld* _world = GetWorld();
	if (!_world)return;
	ATP_Room* _room = GetRoomOwner();
	if (!_room)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red,FString::Printf(TEXT("Couldn't Get Room for %s"),*GetFName().ToString()));
		return;
	}
	toSpawn = _room->NumberOfSpawnTillLimit();
	toSpawn = toSpawn > toSpawnPerWave ? toSpawnPerWave : toSpawn;
	if (respawnEntity)toSpawn = 1;
	_world->GetTimerManager().SetTimer(spawnTick, this, &ATP_Spawner::Spawn, timeBetweenEachSpawn, true, 0);
}
void ATP_Spawner::Spawn()
{
	UWorld* _world = GetWorld();
	if (!_world)return;
	if (toSpawn <= 0)
	{
		_world->GetTimerManager().PauseTimer(spawnTick);
		return;
	}
	FVector _spawnPoint = FVector::ZeroVector;
	_spawnPoint = GetActorLocation();
	_spawnPoint += FVector::UpVector * offsetUpward;
	ATP_Enemy* _enemy = _world->SpawnActor<ATP_Enemy>(spawnEntity, _spawnPoint, FRotator::ZeroRotator);
	if (!_enemy)return;
	_enemy->SetActorLocation(_spawnPoint);
	toSpawn--;
	_enemy->GetAIBehaviorComponent()->Activation();
	_enemy->Activate();
	roomManager->SetRoomOwnerForEnemy(roomOwnerName, _enemy);
	lastSpawnedEnemy = _enemy;

	ATP_Drone* _drone = Cast<ATP_Drone>(_enemy);
	if (_drone)
	{
		roomOwner->AddToDroneArray(_drone);
		return;
	}
	ATP_Tower* _tower = Cast<ATP_Tower>(_enemy);
	if (_tower)
	{
		roomOwner->AddToTowerArray(_tower);
		return;
	}
	ATP_TankEnemy* _tank = Cast<ATP_TankEnemy>(_enemy);
	if (_tank)
	{
		roomOwner->AddToTankArray(_tank);
		return;
	}
}

#pragma endregion

#pragma region Debug
void ATP_Spawner::DrawDebugSpawner()
{
	UWorld* _world = GetWorld();
	if (!_world)return;
	DrawDebugBox(_world, GetActorLocation(), FVector::OneVector * 100, FColor::Green, false, -1, 0, 2);
}
#pragma endregion

/// <summary>
/// Init everything in spawner for gamelogic and such with some debugs need a super with the beginning in enemy
/// </summary>
void ATP_Spawner::Init(UWorld* _world)
{
	Super::Init(_world);
	ATP_Room* _owner = GetRoomOwner();
	if (!_owner)return;
	_owner->AddToSpawnerArray(this);
}

void ATP_Spawner::ActorDeath(AActor* _deadActor)
{
	GetWorld()->GetTimerManager().ClearTimer(spawnTick);
	Super::ActorDeath(_deadActor);

	ATP_Spawner* _deadSpawner = Cast<ATP_Spawner>(_deadActor);
	if (!_deadSpawner || !_deadSpawner->GetRoomOwner())return;
	_deadSpawner->GetRoomOwner()->RemoveSpawner(_deadSpawner);
}

