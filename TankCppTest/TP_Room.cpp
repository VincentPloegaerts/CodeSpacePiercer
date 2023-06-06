#include "TP_Room.h"
#include "TP_Drone.h"
#include "TP_Spawner.h"
#include "TP_TankEnemy.h"
#include "TP_Tower.h"
#include "TP_Door.h"
#include "Components/BoxComponent.h"
#include "TP_Spawner.h"
#include "TP_AIBehaviorComponent.h"
#include "DrawDebugHelpers.h"
#include "TP_Generator.h"
#include "TP_GameMode.h"
#include "TP_HealthBarBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TP_DialogueUI.h"
#include "TP_HUD.h"
#include "TP_Jump_Pad.h"
#include "TP_TimerComponent.h"
#include "TP_BOSS_level3.h"
#include "TP_TriggerCutscene.h"
#include "TP_TankCppTestGameInstance.h"

/*
engine methods tick,begin play and constructor that initialize everything
there's nothing in the tick for the rooms *dab*
*/
#pragma region Engine Methods
ATP_Room::ATP_Room()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	clock = CreateDefaultSubobject<UTP_TimerComponent>(TEXT("TimerComponent"));
	if (triggerBox)triggerBox->SetupAttachment(RootComponent);
}
void ATP_Room::BeginPlay()
{
	Super::BeginPlay();
	ALevelSequenceActor* idfk = nullptr;
	sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), cutScene, FMovieSceneSequencePlaybackSettings(), idfk);

	ATP_HUD* _tempoHud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!_tempoHud)return;
	dialogueUI = _tempoHud->GetDialogueUI();

	onCutSceneStart.AddDynamic(_tempoHud, &ATP_HUD::HideStatsWidget);

	OnActorBeginOverlap.AddDynamic(this, &ATP_Room::EnteringRoom);
	OnActorEndOverlap.AddDynamic(this, &ATP_Room::ExitingRoom);
	onRoomFinishedEvent.AddDynamic(this, &ATP_Room::RoomFinished);
}
void ATP_Room::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorld()->GetAuthGameMode<ATP_GameMode>()->GetDebugDraw())return;
	DrawRoomDebug(GetWorld());
}
#pragma endregion

/*
Methods to fill and empty the arrays
calls the finishedroom methods if conditions are met
*/
#pragma region Array Methods
#pragma region Add
void ATP_Room::AddToSpawnerArray(ATP_Spawner* _toAdd)
{
	if (!_toAdd)return;
	listOfSpawner.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
void ATP_Room::AddToDroneArray(ATP_Drone* _toAdd)
{
	if (!_toAdd)return;
	listOfDrones.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
void ATP_Room::AddToTowerArray(ATP_Tower* _toAdd)
{
	if (!_toAdd)return;
	listOfTowers.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
void ATP_Room::AddToTankArray(ATP_TankEnemy* _toAdd)
{
	if (!_toAdd)return;
	listOfTanks.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
void ATP_Room::AddToGeneratorArray(ATP_Generator* _toAdd)
{
	if (!_toAdd)return;
	listOfGenerator.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
void ATP_Room::AddToBossArray(ATP_BOSS_level3* _toAdd)
{
	if (!_toAdd)return;
	listOfBosses.AddUnique(_toAdd);
	listOfEnemy.AddUnique(_toAdd);
}
#pragma endregion
#pragma region Remove
void ATP_Room::RemoveSpawner(ATP_Spawner* _toRemove)
{
	listOfSpawner.Remove(_toRemove);
	if (listOfSpawner.Contains(nullptr))
		listOfSpawner.Remove(nullptr);
	RemoveEnemy(_toRemove);
}
void ATP_Room::RemoveDrone(ATP_Drone* _toRemove)
{
	listOfDrones.Remove(_toRemove);
	if (listOfDrones.Contains(nullptr))
		listOfDrones.Remove(nullptr);
	RemoveEnemy(_toRemove);
}
void ATP_Room::RemoveTower(ATP_Tower* _toRemove)
{
	listOfTowers.Remove(_toRemove);
	if (listOfTowers.Contains(nullptr))
		listOfTowers.Remove(nullptr);
	RemoveEnemy(_toRemove);
}
void ATP_Room::RemoveTank(ATP_TankEnemy* _toRemove)
{
	listOfTanks.Remove(_toRemove);
	if (listOfTanks.Contains(nullptr))
		listOfTanks.Remove(nullptr);
	RemoveEnemy(_toRemove);
}
void ATP_Room::RemoveEnemy(ATP_Enemy* _toRemove)
{
	listOfEnemy.Remove(_toRemove);
	if (listOfEnemy.Contains(nullptr))
		listOfEnemy.Remove(nullptr);
	if (isTimed)return;
	int _nbrOfSpawner = listOfSpawner.Num(), _nbrOfEnemy = listOfEnemy.Num();
	if (_nbrOfEnemy <= _nbrOfSpawner + listOfDrones.Num())
	{
		DesactivateAllSpawners();
	}
	if (_nbrOfEnemy <= _nbrOfSpawner)
		OnRoomFinishedEvent().Broadcast();
	onRoomEnemyDestroy.Broadcast();
}
void ATP_Room::RemoveGenerator(ATP_Generator* _toRemove)
{
	listOfGenerator.Remove(_toRemove);
	if (listOfEnemy.Contains(nullptr))
		listOfEnemy.Remove(nullptr);
	RemoveEnemy(_toRemove);
	if (listOfGenerator.Num() <= 0 && listOfBosses.Num() <= 0)
		RoomFinished();
}
void ATP_Room::RemoveBoss(ATP_BOSS_level3* _toRemove)
{
	listOfBosses.Remove(_toRemove);
	if (listOfEnemy.Contains(nullptr))
		listOfEnemy.Remove(nullptr);
	RemoveEnemy(_toRemove);
	if (listOfBosses.Num() <= 0)
		RoomFinished();

}
void ATP_Room::DestroyAllEnemies()
{
	int nbrOfEnemies = listOfEnemy.Num();
	for (int i = 0; i < nbrOfEnemies; i++)
	{
		if (!listOfEnemy.IsValidIndex(0) || !listOfEnemy[0] || !listOfEnemy.Contains(listOfEnemy[0]))continue;
		UTP_ShieldSystemComponent* _shieldComponent = listOfEnemy[0]->GetShieldComponent();
		if (_shieldComponent)
			_shieldComponent->DestroyShield();
		UTP_HealthComponent* _healthComponent = listOfEnemy[0]->GetHealthComponent();
		if (_healthComponent)
			_healthComponent->KillActor();
	}
}
#pragma endregion
bool ATP_Room::LeftOnlySpawnerDronesAndGenerator()
{
	if (listOfTowers.Num() > 0)return false;
	if (listOfTanks.Num() > 0)return false;
	//if (listOfEnemy.Num() > listOfDrones.Num() + listOfSpawner.Num() + listOfGenerator.Num())return false;
	return true;
}
void ATP_Room::ActivateEnemiesInRoom()
{
	int _nbrOfEnemies = listOfEnemy.Num();
	for (int i = 0; i < _nbrOfEnemies; i++)
	{
		if (!listOfEnemy.Contains(listOfEnemy[i]) || !listOfEnemy[i])continue;
		listOfEnemy[i]->Activate();
	}
}
#pragma endregion

/*
* Methods used when entering room and finishing it
* activate everything, play cutscene and close doors at start
* 
* kill everythings that's left and open door at end
* then destroy the door after a few second to liberate some used perf
*/
#pragma region Room Methods
void ATP_Room::EnteringRoom(AActor* _mine, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player || isActive || isFinished)return;
	if (!PlayCutScene(_player, sequencePlayer))
		ActivateAllObjectInRoom();
	CloseEntryDoors();
	CloseExitDoors();
	EnterRoomText();
	if (isTimed)
	{
		GetWorld()->GetTimerManager().SetTimer(roomTick, this, &ATP_Room::RoomFinished, duration, false, -1);
		clock->SetTimer(duration);
	}
	onRoomEnterEvent.Broadcast(clock);
	debugColor = FColor::Blue;
	if(!sequencePlayer)
		ActivateGeneratorUi();
}
void ATP_Room::ExitingRoom(AActor* _mine, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	debugColor = FColor::Red;
	if (!_player || !isFinished)return;
	DesactivateAllSpawners();
	DestroyAllDoors();

}
void ATP_Room::ActivateRoom()
{
	isActive = true;
	ActivateEnemiesInRoom();

}
void ATP_Room::DesactivateRoom()
{
	isActive = false;
}
bool ATP_Room::PlayCutScene(ATP_TankPlayer* _player, ULevelSequencePlayer* _sequencePlay)
{
	UWorld* _world = GetWorld();
	if (!_world)return false;
	if (!_player || isDoneCutScene || !sequencePlayer)return false;
	_sequencePlay->Play();
	onCutSceneStart.Broadcast();
	_player->DesactivatePlayer();
	_player->GetMovementComponent()->Velocity = FVector(0,0,0);
	if (widgetBoss && listOfGenerator.Num() != 0)
	{
		widgetBoss.GetDefaultObject()->SetGeneratorOwner(listOfGenerator[0]);
		UTP_HealthBarBoss* _temp = Cast<UTP_HealthBarBoss>(UWidgetBlueprintLibrary::Create(GetWorld(), widgetBoss, GetWorld()->GetFirstPlayerController()));
		if (_temp)
			_temp->EraseTheWidget();
	}
	ATP_HUD* _myHud = Cast<ATP_HUD>(_world->GetFirstPlayerController()->GetHUD());
	if (_myHud)
	{
		_sequencePlay->OnFinished.AddDynamic(_myHud, &ATP_HUD::MakeVisibleStatsWidget);
		_sequencePlay->OnStop.AddDynamic(_myHud, &ATP_HUD::MakeVisibleStatsWidget);
	}
	_sequencePlay->OnStop.AddDynamic(this, &ATP_Room::ActivateGeneratorUi);
	_sequencePlay->OnStop.AddDynamic(_player, &ATP_TankPlayer::ActivatePlayer);
	_sequencePlay->OnStop.AddDynamic(this, &ATP_Room::ActivateAllObjectInRoom);
	_sequencePlay->OnFinished.AddDynamic(_player, &ATP_TankPlayer::ActivatePlayer);
	_sequencePlay->OnFinished.AddDynamic(this, &ATP_Room::ActivateAllObjectInRoom);
	_sequencePlay->OnFinished.AddDynamic(this, &ATP_Room::ActivateGeneratorUi);
	_player->InputComponent->BindAction("SkipSequence", EInputEvent::IE_Pressed, _sequencePlay, &ULevelSequencePlayer::Stop).bExecuteWhenPaused = true;;
	isDoneCutScene = true;
	return true;
}
void ATP_Room::ActivateGeneratorUi()
{
	if (listOfGenerator.Num() == 0)return;
	if (!widgetBoss)return;
	widgetBoss.GetDefaultObject()->SetGeneratorOwner(listOfGenerator[0]);
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetBoss, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
}

void ATP_Room::ActivateAllObjectInRoom()
{
	DesactivateJumpPads();
	ActivateRoom();
	ActivateAllSpawners();
}
void ATP_Room::DrawRoomDebug(UWorld* _world)
{
	if (!_world)return;
	ATP_GameMode* _gameMode = _world->GetAuthGameMode<ATP_GameMode>();
	if (!_gameMode || _gameMode->GetDebugDraw())return;
	DrawDebugBox(_world, GetActorLocation(), triggerBox->GetScaledBoxExtent(), debugColor, false, -1, 0, 10);
}
void ATP_Room::RoomFinished()
{
	DesactivateRoom();
	ActivateJumpPads();
	isFinished = true;
	DestroyAllEnemies();
	DestroyAllDoors();
	UWorld* _world = GetWorld();
	if (!_world)return;
	if (!nextLevelName.IsNone())
	{
		UTP_TankCppTestGameInstance* _gameInstance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
		if (_gameInstance)
		{
			_gameInstance->LevelFinished(nextLevelName);
		}
		if (!winWidget)
		{
			GetWorld()->GetTimerManager().SetTimer(levelTick, this, &ATP_Room::SetLoadingScreen, timeNextLevel, false, -1);

			return;
		}
		_world->GetTimerManager().SetTimer(levelTick, this, &ATP_Room::GoToNextLevel, timeNextLevel, false, -1);
	}
}
void ATP_Room::GoToNextLevel()
{
	if (!winWidget)return;
	UTP_WinWidget* _temp = Cast<UTP_WinWidget>(UWidgetBlueprintLibrary::Create(GetWorld(), winWidget, GetWorld()->GetFirstPlayerController()));
	if (!_temp)return;
	_temp->nextLevelName = nextLevelName;
	_temp->AddToViewport();
}
void ATP_Room::SetLoadingScreen()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (_player)
		_player->DesactivatePlayer();
	if (!widgetLoad)
	{
		GoNextLevelWithoutMenu();
		return;
	}
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetLoad, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(levelTick, this, &ATP_Room::GoNextLevelWithoutMenu, loadingScreenDuration, false, -1);
}
void ATP_Room::GoNextLevelWithoutMenu()
{
	UGameplayStatics::OpenLevel(GetWorld(), nextLevelName);
}
#pragma endregion

/* used at the beginning to activate spawners because the rest wasn't done
shouldn't be useful anymore but still there and the activate wasn't the same for spawners at first
*/
#pragma region Spawner Methods
void ATP_Room::ActivateAllSpawners()
{
	int _size = listOfSpawner.Num();
	for (int i = 0; i < _size; i++)
	{
		if (!listOfSpawner[i])	continue;
		listOfSpawner[i]->ActivateSpawner();
	}
}
void ATP_Room::DesactivateAllSpawners()
{
	int _size = listOfSpawner.Num();

	for (int i = 0; i < _size; i++)
	{
		if (!listOfSpawner[i])	continue;
		listOfSpawner[i]->DesactivateSpawner();
	}
}
//might need change with destructible spawners
bool ATP_Room::AreSpawnersKilled()
{
	for (int i = 0; i < listOfSpawner.Num(); i++)
	{
		if (!listOfSpawner[i])continue;
		if (listOfSpawner[i]->IsActive())
			return false;
	}
	return true;
}
#pragma endregion

/*
* methods called in room methods to use doors
* 
*/
#pragma region Door Methods
void ATP_Room::CloseEntryDoors()
{
	int _size = entryDoor.Num();
	if (_size == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No entry doors for %s"), *GetFName().ToString()));
		return;
	}
	for (int i = 0; i < _size; i++)
	{
		if (!entryDoor[i])continue;
		entryDoor[i]->CloseDoor();
	}
}
void ATP_Room::CloseExitDoors()
{
	int _size = exitDoor.Num();
	if (_size == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No exit doors for %s"), *GetFName().ToString()));
		return;
	}
	for (int i = 0; i < _size; i++)
	{
		if (!exitDoor[i])continue;
		exitDoor[i]->CloseDoor();
	}
}
void ATP_Room::OpenEntryDoors()
{
	int _size = entryDoor.Num();
	if (_size == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No entry doors for %s"), *GetFName().ToString()));
		return;
	}
	for (int i = 0; i < _size; i++)
	{
		if (!entryDoor[i])continue;
		entryDoor[i]->OpenDoor();
	}
}
void ATP_Room::OpenExitDoors()
{
	int _size = exitDoor.Num();
	if (_size == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No exit doors for %s"), *GetFName().ToString()));
		return;
	}
	for (int i = 0; i < _size; i++)
	{
		if (!exitDoor[i])continue;
		exitDoor[i]->OpenDoor();
	}
}
void ATP_Room::DestroyAllDoors()
{
	int _sizeEntry = entryDoor.Num(), _sizeExit = exitDoor.Num();
	if (_sizeExit == 0)
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No exit doors for %s"), *GetFName().ToString()));
	if (_sizeEntry == 0)
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("No entry doors for %s"), *GetFName().ToString()));
	for (int i = 0; i < _sizeExit; i++)
	{
		if (!exitDoor[i])continue;
		exitDoor[i]->OnDoorExplode().Broadcast();
	}
	for (int i = 0; i < _sizeEntry; i++)
	{
		if (!entryDoor[i])continue;
		entryDoor[i]->OnDoorExplode().Broadcast();

	}
}
#pragma endregion

/*
* activate jump and show dialogues texts
*/
void ATP_Room::EnterRoomText()
{
	if (!dialogueUI)
	{
		ATP_HUD* _hud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		if (!_hud)return;
		dialogueUI = _hud->GetDialogueUI();
	}

	int _len = enterRoomText.Num();
	if (_len == 0)return;
	int _rand = FMath::RandRange(0, _len - 1);
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText(enterRoomText[_rand]);
}
void ATP_Room::ActivateJumpPads()
{
	int _nbrOfJumpPads = listOfJumpPad.Num();
	for (int i = 0; i < _nbrOfJumpPads; i++)
	{
		if (listOfJumpPad[i])
			listOfJumpPad[i]->ActivateJumpPad();
	}
}
void ATP_Room::DesactivateJumpPads()
{
	int _nbrOfJumpPads = listOfJumpPad.Num();
	for (int i = 0; i < _nbrOfJumpPads; i++)
	{
		if (listOfJumpPad[i])
			listOfJumpPad[i]->DesactivateJumpPad();
	}
}