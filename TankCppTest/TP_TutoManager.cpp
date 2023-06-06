#include "TP_TutoManager.h"
#include "TP_TankPlayer.h"
#include "TP_TutoPlatform.h"
#include "Components/SceneComponent.h"
#include "TP_Enemy.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TP_HUD.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
 #include "NiagaraComponent.h"



ATP_TutoManager::ATP_TutoManager()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	positionSpawnFirstEnemy = CreateDefaultSubobject<USceneComponent>("SpawnFirstEnemy");
	positionSpawnSecondEnemy = CreateDefaultSubobject<USceneComponent>("SpawnSecondEnemy");
	positionSpawnThirdEnemy = CreateDefaultSubobject<USceneComponent>("SpawnThirdEnemy");
	finishBoxDashPractice = CreateDefaultSubobject<UBoxComponent>("finishBoxDashPractice");
	positionTPTankDashPractice = CreateDefaultSubobject<USceneComponent>("positionTPTankDashPractice");
	ArrowForDash = CreateDefaultSubobject<UNiagaraComponent>("ArrowForDash");
	positionSpawnFirstEnemy->SetupAttachment(RootComponent);
	positionSpawnSecondEnemy->SetupAttachment(RootComponent);
	positionSpawnThirdEnemy->SetupAttachment(RootComponent);
	finishBoxDashPractice->SetupAttachment(RootComponent);
	positionTPTankDashPractice->SetupAttachment(RootComponent);
	ArrowForDash->SetupAttachment(RootComponent);
}
void ATP_TutoManager::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("begin"));
	ATP_TankPlayer* _tank = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!_tank)return;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("tankplayer exist"));
	player = _tank;
	playerMinigun = player->GetShootMinigunComponent();
	playerPlasma = player->GetShootPlasmaComponent();
	playerNuke = player->GetShootNukeComponent();
	playerDash = player->GetDashComponent();
	player->InputComponent->BindAxis(moveVertical, this, &ATP_TutoManager::AxisForward);
	player->InputComponent->BindAxis(moveHorizontal, this, &ATP_TutoManager::AxisRight);
	player->InputComponent->BindAction(dash, EInputEvent::IE_Pressed, this, &ATP_TutoManager::ActionDash);
	player->InputComponent->BindAction(jumpName, EInputEvent::IE_Pressed, this, &ATP_TutoManager::ActionJump);
	player->InputComponent->BindAction(dash, EInputEvent::IE_Pressed, this, &ATP_TutoManager::ActionDash);
	player->InputComponent->BindAction(shootGatling, EInputEvent::IE_Pressed, this, &ATP_TutoManager::ActionMinigun);
	player->InputComponent->BindAction(shootPlasma, EInputEvent::IE_Pressed, this, &ATP_TutoManager::ActionPlasma);
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Input Set"));
	playerDash->SetIsActivated(false);
	player->SetIsJumpActivated(false);
	playerMinigun->SetIsActivated(false);
	playerPlasma->SetIsActivated(false);
	playerNuke->SetIsActivated(false);
	FTimerHandle timerBegin;
	GetWorld()->GetTimerManager().SetTimer(timerBegin,this, &ATP_TutoManager::SetDialogue,0.1f,false);
}
void ATP_TutoManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATP_TutoManager::SetDialogue()
{
	ATP_HUD* _tempoHud = Cast<ATP_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (!_tempoHud)return;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("HUD are here"));
	dialogueUI = _tempoHud->GetDialogueUI();
	if (!dialogueUI)return;
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("dialogue are here"));
	dialogueUI->SetSpeechText("MoveTuto");
	ArrowForDash->SetVisibility(false);
}
void ATP_TutoManager::MoveTutoFinished()
{
	if (!(isForwardPressed && isLeftPressed && isBackwardPressed && isRightPressed && !isMovementTutoFinished))return;
	isMovementTutoFinished = true;
	playerDash->SetIsActivated(true);
	player->SetIsJumpActivated(true);
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText("DashTuto");
}
void ATP_TutoManager::AxisForward(float _axis)
{
	if (_axis == 0 )return;
	if (_axis > 0)isForwardPressed = true;
	if (_axis < 0)isBackwardPressed = true;
	MoveTutoFinished();
}
void ATP_TutoManager::AxisRight(float _axis)
{
	if (_axis == 0 )return;
	if (_axis > 0)isRightPressed = true;
	if (_axis < 0)isLeftPressed = true;
	MoveTutoFinished();

}

void ATP_TutoManager::DashTutoFinished()
{
	if (!(isDashPressed && isJumpPressed && !isDashTutoFinished && !isDashPracticeFinished))return;
	isDashTutoFinished = true;
	MovePlatforms(PlatformTutoMovement, FVector::DownVector * downOffset);
	if (!dialogueUI)return;
	dialogueUI->SetSpeechText("DashPractice");
	FTimerHandle _tempo;
	GetWorld()->GetTimerManager().SetTimer(_tempo, this,&ATP_TutoManager::TPPlayer, 1.5, false, -1);
	
}
void ATP_TutoManager::ActionDash()
{
	if (!isMovementTutoFinished)return;
	isDashPressed = true;
	DashTutoFinished();
}
void ATP_TutoManager::ActionJump()
{
	if (!isMovementTutoFinished)return;
	isJumpPressed = true;
	DashTutoFinished();
}
void ATP_TutoManager::MovePlatforms(TArray<ATP_TutoPlatform*>& _platformList,FVector _movement)
{
	int _nbrOfPlatforms = _platformList.Num();
	for (int i = 0; i < _nbrOfPlatforms; i++)
	{
		if (!_platformList[i])continue;
		_platformList[i]->Move(_platformList[i]->GetActorLocation() + _movement);
	}
}

void ATP_TutoManager::DashPractice(AActor* _me, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player)return;
	isDashPracticeFinished = true;
	ArrowForDash->SetVisibility(false);
	onDashPracticeFinished.Broadcast();
	finishBoxDashPractice->DestroyComponent();
	playerMinigun->SetIsActivated(true);
	playerPlasma->SetIsActivated(true);
	dialogueUI->SetSpeechText("ShootTuto");
	MovePlatforms(PlatformTutoMovement, FVector::UpVector * downOffset);
}


void ATP_TutoManager::ShootTutoFinished()
{
	if (!(isMinigunPressed && isPlasmaPressed && !isShootTutoFinished))return;
	isShootTutoFinished = true;

	if (!dialogueUI)return;
	dialogueUI->SetSpeechText("MinigunTuto");

	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATP_TutoManager::SpawnFirstEnemy, timeSpawnFirstEnemy, false, -1);

}
void ATP_TutoManager::ActionMinigun()
{
	if (!isDashPracticeFinished)return;
	isMinigunPressed = true;
	ShootTutoFinished();
}
void ATP_TutoManager::ActionPlasma()
{
	if (!isDashPracticeFinished)return;
	isPlasmaPressed = true;
	ShootTutoFinished();

}

void ATP_TutoManager::SpawnFirstEnemy()
{
	if (!dialogueUI)return;
	UWorld* _world = GetWorld();
	if (!_world || !spawnEntityFirstEnemi || !positionSpawnFirstEnemy)return;
	ATP_Enemy* _enemy = _world->SpawnActor<ATP_Enemy>(spawnEntityFirstEnemi, positionSpawnFirstEnemy->GetComponentLocation(), FRotator::ZeroRotator);
	if (!_enemy)return;
	spawnedEnemies.Add(_enemy);
	_enemy->GetHealthComponent()->OnDeathEvent().AddDynamic(this, &ATP_TutoManager::RemoveFirstEnemy);
	_enemy->Activate();
	if (!playerPlasma)return;
	playerPlasma->SetIsActivated(false);
}
void ATP_TutoManager::RemoveFirstEnemy(AActor* _deadActor)
{
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(_deadActor);
	if (!_enemy)return;
	spawnedEnemies.Remove(_enemy);
	if (!playerPlasma)return;
	playerPlasma->SetIsActivated(true);
	dialogueUI->SetSpeechText("PlasmaTuto");
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATP_TutoManager::SpawnSecondEnemy, timeSpawnSecondEnemy, false, -1);

}

void ATP_TutoManager::SpawnSecondEnemy()
{
	if (!dialogueUI)return;
	UWorld* _world = GetWorld();
	if (!_world || !spawnEntitySecondEnemi || !positionSpawnSecondEnemy)return;
	ATP_Enemy* _enemy = _world->SpawnActor<ATP_Enemy>(spawnEntitySecondEnemi, positionSpawnSecondEnemy->GetComponentLocation(), FRotator::ZeroRotator);
	if (!_enemy)return;
	spawnedEnemies.Add(_enemy);
	_enemy->GetHealthComponent()->OnDeathEvent().AddDynamic(this, &ATP_TutoManager::RemoveSecondEnemy);
	_enemy->GetShieldComponent()->OnShieldDestroy().AddDynamic(this, &ATP_TutoManager::ActivateMinigun);
	_enemy->Activate();
	if (!playerMinigun)return;
	playerMinigun->SetIsActivated(false);
}
void ATP_TutoManager::RemoveSecondEnemy(AActor* _deadActor)
{
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(_deadActor);
	if (!_enemy)return;
	spawnedEnemies.Remove(_enemy);
	dialogueUI->SetSpeechText("NukeTuto");
	GetWorld()->GetTimerManager().SetTimer(timer, this, &ATP_TutoManager::SpawnThirdEnemies, timeSpawnThirdEnemy, false, -1);
}

void ATP_TutoManager::SpawnThirdEnemies()
{
	if (!dialogueUI)return;
	UWorld* _world = GetWorld();
	if (!_world || !spawnEntityThirdEnemi || !positionSpawnThirdEnemy)return;
	for (int i = 0; i < numberOfEnemiesThirdTuto; i++)
	{
		ATP_Enemy* _enemy = _world->SpawnActor<ATP_Enemy>(spawnEntityThirdEnemi, positionSpawnThirdEnemy->GetComponentLocation(), FRotator::ZeroRotator);
		if (!_enemy)continue;
		spawnedEnemies.Add(_enemy);
		_enemy->GetHealthComponent()->OnDeathEvent().AddDynamic(this, &ATP_TutoManager::RemoveThirdEnemy);
		_enemy->Activate();
		positionSpawnThirdEnemy->SetWorldLocation(positionSpawnThirdEnemy->GetComponentLocation() + FVector::RightVector * offsetRightSpawnThirdEnemies + FVector::ForwardVector * offsetForwardSpawnThirdEnemies);
	}
	if (!playerMinigun || !playerPlasma || !playerNuke)return;
	playerMinigun->SetIsActivated(false);
	playerPlasma->SetIsActivated(false);
	playerNuke->SetIsActivated(true);
}
void ATP_TutoManager::RemoveThirdEnemy(AActor* _deadActor)
{
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(_deadActor);
	if (!_enemy)return;
	spawnedEnemies.Remove(_enemy);
	if (!(spawnedEnemies.Num() == 0))return;
	playerMinigun->SetIsActivated(true);
	playerPlasma->SetIsActivated(true);
	dialogueUI->SetSpeechText("FinishedTuto");

	UWorld* _world = GetWorld();
	if (!_world || nextLevelName.IsNone())return;
	_world->GetTimerManager().SetTimer(timer, this, &ATP_TutoManager::Changelevel, timeSwitchNextLevel, false, -1);

}

void ATP_TutoManager::ActivateMinigun()
{
	player->GetShootMinigunComponent()->SetIsActivated(true);
}
void ATP_TutoManager::Changelevel()
{
	if (widgetLoad)
	{
		UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetLoad, GetWorld()->GetFirstPlayerController());
		_temp->AddToViewport();
	}
	UGameplayStatics::OpenLevel(GetWorld(), nextLevelName);
}

void ATP_TutoManager::TPPlayer()
{
	ArrowForDash->SetVisibility(true);
	player->SetActorLocation(positionTPTankDashPractice->GetComponentLocation());
	OnActorBeginOverlap.AddDynamic(this, &ATP_TutoManager::DashPractice);
	player->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(FRotator(0, TPRotationYaw, 0));
}

