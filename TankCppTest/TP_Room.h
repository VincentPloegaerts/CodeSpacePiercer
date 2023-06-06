/*
* Mostly Vincent
Logique des pièces de chaque niveaux
gère les room timé dedans aussi

s'active en entrant dedans par une triggerbox


contient des listes pour chaque types d'ennemies
quand l'un meurt il se retire de la liste et une fois que toutes les listes nécessaire 
(tank/tourelles/drones) sont vides, ouvre les portes de la salle
si un boss meurt et qu'il n'y en a plus elle se finit même si il reste des ennemies
pareil pour les générateurs

Possibilité de désactivation de jumppad à l'activation de la room
Possibilité de Cutscene skipable qui désactive le player dans chacune des rooms
*/


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "TP_FakeLoading.h"
#include "TP_WinWidget.h"
#include "TP_Room.generated.h"

class ATP_Enemy;
class ATP_Spawner;
class ATP_Drone;
class ATP_Tower;
class ATP_TankEnemy;
class ATP_Generator;
class ATP_Door;
class UTP_HealthBarBoss;
class UTP_DialogueUI;
class ATP_Jump_Pad;
class UBoxComponent;
class UTP_TimerComponent;
class ATP_BOSS_level3;
class ATP_TriggerCutscene;

UCLASS()
class TANKCPPTEST_API ATP_Room : public AActor
{
	GENERATED_BODY()
protected:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomEnterEvent, UTP_TimerComponent*, _clock);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnRoomEnterEvent onRoomEnterEvent;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomExitEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnRoomExitEvent onRoomExitEvent;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomFinishedEvent);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnRoomFinishedEvent onRoomFinishedEvent;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoomEnemyDestroy);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnRoomEnemyDestroy onRoomEnemyDestroy;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutSceneStart);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnCutSceneStart onCutSceneStart;
#pragma endregion
#pragma region Properties
#pragma region Arrays
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_Enemy*> listOfEnemy;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_Spawner*> listOfSpawner;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_Drone*> listOfDrones;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_Tower*> listOfTowers;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_Generator*> listOfGenerator;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_TankEnemy*> listOfTanks;
	UPROPERTY(VisibleAnywhere, Category = "Room | Enemies | Arrays")
		TArray<ATP_BOSS_level3*> listOfBosses;
	UPROPERTY(EditAnywhere, Category = "Room | Doors")
		TArray<ATP_Door*> entryDoor;
	UPROPERTY(EditAnywhere, Category = "Room | Doors")
		TArray<ATP_Door*> exitDoor;
	UPROPERTY(EditAnywhere, Category = "Room | JumpPad")
		TArray<ATP_Jump_Pad*> listOfJumpPad;
#pragma endregion
	UPROPERTY(EditAnywhere, Category = "Room | Hud Load")
		TSubclassOf<UTP_HealthBarBoss> widgetBoss = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | WinWidget")
		TSubclassOf<UUserWidget> winWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | Last room")
		TSubclassOf<UTP_FakeLoading> widgetLoad = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | CutScene")
		ULevelSequence* cutScene = nullptr;
	UPROPERTY()
		ULevelSequencePlayer* sequencePlayer = nullptr;
	UPROPERTY(EditAnywhere)
		UBoxComponent* triggerBox = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | Activation")
		bool isActive = false;
	UPROPERTY(VisibleAnywhere, Category = "Room | Activation")
		bool isFinished = false;
	UPROPERTY(EditAnywhere, Category = "Room | Timed Room")
		bool isTimed = false;
	UPROPERTY()
		FTimerHandle roomTick;
	UPROPERTY(EditAnywhere, Category = "Room | Timed Room", meta = (ClampMin = "0", ClampMax = "1200", UIMin = "0", UIMax = "1200"))
		float duration = 300;
	UPROPERTY(EditAnywhere, Category = "Room | Enemies | Spawn Limit")
		int spawnLimit = 20;
	UPROPERTY()
		FColor debugColor = FColor::Red;
	UPROPERTY(EditAnywhere, Category = "Room | Last room")
		FName nextLevelName;
	UPROPERTY(EditAnywhere, Category = "Room | Last room")
		float timeNextLevel = 0;
	UPROPERTY()
		FTimerHandle levelTick;
	UPROPERTY(EditAnywhere, Category = "Room | Last room")
		float loadingScreenDuration = 1;

#pragma endregion
	UPROPERTY()
		bool isDoneCutScene = false;
	UPROPERTY(EditAnywhere, Category = "Room | Clock")
		UTP_TimerComponent* clock = nullptr;
	UPROPERTY()
		UTP_DialogueUI* dialogueUI = nullptr;
	UPROPERTY(EditAnywhere, Category = "Room | Text")
		TArray<FString> enterRoomText;
	UPROPERTY(EditAnywhere, Category = "PAS TOUCHE ou on viens te chercher")
		TArray<FString> bindingName;
#pragma region engine methods
public:	
	ATP_Room();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
#pragma endregion

public:
	FORCEINLINE FOnCutSceneStart& OnCutSceneStart() { return onCutSceneStart; }
	FORCEINLINE FOnRoomEnterEvent& OnRoomEnterEvent() { return onRoomEnterEvent; }
	FORCEINLINE FOnRoomEnemyDestroy& OnRoomEnemyDestroy() { return onRoomEnemyDestroy; }
	FORCEINLINE FOnRoomExitEvent& OnRoomExitEvent() { return onRoomExitEvent; }
	FORCEINLINE FOnRoomFinishedEvent& OnRoomFinishedEvent() { return onRoomFinishedEvent; }
#pragma region Array Methods
#pragma region Add
	FORCEINLINE void AddToEnemyArray(ATP_Enemy* _toAdd) { listOfEnemy.Add(_toAdd); }
	void AddToSpawnerArray(ATP_Spawner* _toAdd);
	void AddToDroneArray(ATP_Drone* _toAdd);
	void AddToTowerArray(ATP_Tower* _toAdd);
	void AddToGeneratorArray(ATP_Generator* _toAdd);
	void AddToTankArray(ATP_TankEnemy* _toAdd);
	void AddToBossArray(ATP_BOSS_level3* _toAdd);
#pragma endregion
#pragma region remove
	void RemoveSpawner(ATP_Spawner* _toRemove);
	void RemoveEnemy(ATP_Enemy* _toRemove);
	void RemoveDrone(ATP_Drone* _toRemove);
	void RemoveTower(ATP_Tower* _toRemove);
	void RemoveTank(ATP_TankEnemy* _toRemove);
	void RemoveGenerator(ATP_Generator* _toRemove);
	void RemoveBoss(ATP_BOSS_level3* _toAdd);

	void DestroyAllEnemies();
#pragma endregion
#pragma region Get
	FORCEINLINE int NumberOfEnemyInRoom() { return listOfEnemy.Num(); }
	FORCEINLINE int NumberOfSpawnTillLimit() { return spawnLimit - listOfDrones.Num(); }
	FORCEINLINE bool IsTimerRoom() { return isTimed; }
	bool LeftOnlySpawnerDronesAndGenerator();
#pragma endregion
	void ActivateEnemiesInRoom();
#pragma endregion

#pragma region Room Methods
private:
	UFUNCTION() virtual void EnteringRoom(AActor* _mine, AActor* _other);
	UFUNCTION() void ExitingRoom(AActor* _mine, AActor* _other);
	UFUNCTION() void ActivateRoom();
	UFUNCTION() void DesactivateRoom();
	UFUNCTION() void ActivateAllObjectInRoom();
	UFUNCTION() bool PlayCutScene(ATP_TankPlayer* _player, ULevelSequencePlayer* _sequencePlay);
	UFUNCTION() void ActivateGeneratorUi();
	void DrawRoomDebug(UWorld* _world);
public:
	UFUNCTION() void RoomFinished();
private:
	UFUNCTION() void GoToNextLevel();

#pragma endregion
#pragma region Spawner Methods
protected:
	void ActivateAllSpawners();
	void DesactivateAllSpawners();
	bool AreSpawnersKilled();
#pragma endregion

#pragma region Doors Methods
private:
	void CloseEntryDoors();
	void CloseExitDoors();
	void OpenEntryDoors();
	void OpenExitDoors();
	void DestroyAllDoors();
#pragma endregion
	void EnterRoomText();
	void ActivateJumpPads();
	void DesactivateJumpPads();
	void SetLoadingScreen();
	void GoNextLevelWithoutMenu();
};
