#include "TP_RoomManager.h"
#include "TP_Spawner.h"
#include "TP_Room.h"

ATP_RoomManager::ATP_RoomManager()
{
	PrimaryActorTick.bCanEverTick = true;

}
void ATP_RoomManager::BeginPlay()
{
	Super::BeginPlay();
	
}
void ATP_RoomManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_RoomManager::SetRoomOwnerForSpawner(FString _roomName, ATP_Spawner* _spawner)
{
	if (!_spawner)return;
	if (!roomManagerMap.Contains(_roomName))
	{
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, FString::Printf(TEXT("No Room named '%s' in RoomManager, check %s RoomOwnerName"), *_roomName,*_spawner->GetFName().ToString()));
		return;
	}
	
	_spawner->SetRoomOwner(roomManagerMap[_roomName]);
}

void ATP_RoomManager::SetRoomOwnerForEnemy(FString _roomName, ATP_Enemy* _enemy)
{
	if (!_enemy || _roomName.IsEmpty())return;
	if (!roomManagerMap.Contains(_roomName))
	{
		GEngine->AddOnScreenDebugMessage(-1, 30, FColor::Red, FString::Printf(TEXT("No Room named '%s' in RoomManager, check %s RoomOwnerName"), *_roomName, *_enemy->GetFName().ToString()));
		return;
	}
	_enemy->SetRoomOwner(roomManagerMap[_roomName]);
	_enemy->SetRoomOwnerName(_roomName);
}