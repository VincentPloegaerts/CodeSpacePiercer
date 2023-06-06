#include "TP_TankCppTestGameInstance.h"

void UTP_TankCppTestGameInstance::LevelFinished(FName nextLevelName)
{
	if (nextLevelName == level1Name)
		isLevel1Finished = true;
	else if (nextLevelName == level2Name)
		isLevel2Finished = true;
	return;
}
