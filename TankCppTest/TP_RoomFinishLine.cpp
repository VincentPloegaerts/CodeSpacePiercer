// Fill out your copyright notice in the Description page of Project Settings.

#include "TP_RoomFinishLine.h"
#include "Components/BoxComponent.h"
#include "TP_Room.h"
#include "TP_TankPlayer.h"

ATP_RoomFinishLine::ATP_RoomFinishLine()
{
	finishLine = CreateDefaultSubobject<UBoxComponent>(TEXT("Finish Line"));
	if (finishLine)finishLine->SetupAttachment(RootComponent);
}

void ATP_RoomFinishLine::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ATP_RoomFinishLine::Crossedline);
	if (!room)return;
}

void ATP_RoomFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATP_RoomFinishLine::Crossedline(AActor* _me, AActor* _other)
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_other);
	if (!_player || !room)return;
	room->RoomFinished();
	if (this)Destroy();
}
