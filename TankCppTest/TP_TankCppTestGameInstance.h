// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TP_TankCppTestGameInstance.generated.h"

/**
* Mostly Vincent
 * used to save everything between levels and also a checkpoint
 * Save system gets everything from here
 */
UCLASS()
class TANKCPPTEST_API UTP_TankCppTestGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance | bool level 2")
		bool checkpointLevel2 = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance | Level Finished")
		bool isLevel1Finished = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance | Level Finished")
		bool isLevel2Finished = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance | Level Names")
		FName level1Name = "Level2_Main";
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameInstance | Level Names")
		FName level2Name = "Level3_Main";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance | Difficulty", meta = (ClampMin = "0", ClampMax = "2"))
		int difficulty = 1;


	

public:
	FORCEINLINE bool GetCheckpointLevel2() { return checkpointLevel2; }
	FORCEINLINE void SetCheckpointLevel2(bool _status) {  checkpointLevel2 = _status; }
	FORCEINLINE int GetDifficulty() { return difficulty; }
public:
	void LevelFinished(FName nextLevelName);
};
