// Fill out your copyright notice in the Description page of Project Settings.


#include "TP_DialogueComponent.h"
#include "Components/Image.h"
#include "TP_DialogueUI.h"


UTP_DialogueComponent::UTP_DialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UTP_DialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	UWorld* _world = GetWorld();
	if (!_world)return;
	APlayerController* _player = _world->GetFirstPlayerController();
	if (!_player)return;
	//ATP_HUD* _UI = Cast<ATP_HUD>(_player->GetHUD());
	//if (!_UI)return;
	//dialogueUi = _UI->GetStatWidget();
}

void UTP_DialogueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ShowDialogueHUD();
}

void UTP_DialogueComponent::ShowDialogueHUD()
{
	//with hud pointers and maybe in that class idrk bring dialogue box where it needs to be
	if (!dialogueUi)return;
	//dialogueUi->SetMunNuke(FText::FromString("test"));
	
}
