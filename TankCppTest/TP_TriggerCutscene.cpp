// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_TriggerCutscene.h"
#include "TP_TankPlayer.h"
#include "TP_HUD.h"

ATP_TriggerCutscene::ATP_TriggerCutscene()
{
	

}
void ATP_TriggerCutscene::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATP_TriggerCutscene::EnterBox);
	ALevelSequenceActor* idfk = nullptr;
	sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), cutScene, FMovieSceneSequencePlaybackSettings(), idfk);
}
void ATP_TriggerCutscene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATP_TriggerCutscene::EnterBox(AActor* _mine, AActor* _other)
{
	UWorld* _world = GetWorld();
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player || isDone || !sequencePlayer || !_world)return;
	ATP_HUD* _myHud = Cast<ATP_HUD>(_world->GetFirstPlayerController()->GetHUD());
	if (!_myHud)return;
	onCutSceneStart.AddDynamic(_myHud, &ATP_HUD::HideStatsWidget);

	sequencePlayer->Play();
	_player->DesactivatePlayer();
	_player->GetMovementComponent()->Velocity = FVector(0, 0, 0);
	onCutSceneStart.Broadcast();
	sequencePlayer->OnStop.AddDynamic(_myHud, &ATP_HUD::MakeVisibleStatsWidget);
	sequencePlayer->OnStop.AddDynamic(_player, &ATP_TankPlayer::ActivatePlayer);
	sequencePlayer->OnFinished.AddDynamic(_myHud, &ATP_HUD::MakeVisibleStatsWidget);
	sequencePlayer->OnFinished.AddDynamic(_player, &ATP_TankPlayer::ActivatePlayer);
	_player->InputComponent->BindAction("SkipSequence", EInputEvent::IE_Pressed, sequencePlayer, &ULevelSequencePlayer::Stop).bExecuteWhenPaused = true;
	isDone = true;
}