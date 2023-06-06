// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "TP_TriggerCutscene.generated.h"

/*
* Mostly Vincent
* simply trigger a cutscene and desactivate player while doing so 
* skipable and reactivate player at the end
*/
UCLASS()
class TANKCPPTEST_API ATP_TriggerCutscene : public ATriggerBox
{
	GENERATED_BODY()
protected:
		DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutSceneStart);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnCutSceneStart onCutSceneStart;

	UPROPERTY()
		bool isDone = false;
	UPROPERTY(EditAnywhere,Category = "CutScene | Cinematic")
		ULevelSequence* cutScene = nullptr;
	UPROPERTY()
		ULevelSequencePlayer* sequencePlayer = nullptr;
	UPROPERTY()
		FTimerHandle sequenceTimer;
public:
	FORCEINLINE FOnCutSceneStart& OnCutSceneStart() { return onCutSceneStart; }
public:
	ATP_TriggerCutscene();


private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() void EnterBox(AActor* _mine, AActor* _other);
};
