#pragma once
/*
* Mostly Vincent
* using a TMap for room with a string key to be able to subscribe every actor in the world 
* to its given room
*/
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_RoomManager.generated.h"

class ATP_Room;
class ATP_Spawner;
class ATP_Enemy;

UCLASS()
class TANKCPPTEST_API ATP_RoomManager : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "RoomManager | Linkers | TMap")
		TMap<FString, ATP_Room*> roomManagerMap;
public:	
	ATP_RoomManager();
	void SetRoomOwnerForSpawner(FString _roomName, ATP_Spawner* _spawner);
	void SetRoomOwnerForEnemy(FString _roomName, ATP_Enemy* _enemy);
	FORCEINLINE TMap<FString, ATP_Room*> GetRooms() { return roomManagerMap;  };
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
};
