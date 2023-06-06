// Fill out your copyright notice in the Description page of Project Settings.

/* 
* Mostly Vincent
* used to get different managers through gamemode 
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TP_DialogueManager.h"
#include "TP_GameLogic.generated.h"

class ATP_RoomManager;

UCLASS()
class TANKCPPTEST_API ATP_GameLogic : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "GameLogic")
		ATP_RoomManager* roomManager;
	UPROPERTY(EditAnywhere, Category = "GameLogic")
		ATP_DialogueManager* dialogueManager;
public:	
	ATP_GameLogic();
public:
	FORCEINLINE ATP_RoomManager* GetRoomManager() { return roomManager; }
	FORCEINLINE ATP_DialogueManager* GetDialogueManager() { return dialogueManager; }
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

};
