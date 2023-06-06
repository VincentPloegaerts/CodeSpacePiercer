#pragma once

/*
* Mostly Vincent
* used to get gamelogic since it's gettable anywhere
* there's also a bool for drawing debug or not
*/

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TP_GameLogic.h"
#include "TP_FakeLoading.h"
#include "TP_GameMode.generated.h"


UCLASS()
class TANKCPPTEST_API ATP_GameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "GameMode")
		TSoftObjectPtr<ATP_GameLogic> gameLogic = nullptr;
	UPROPERTY(EditAnywhere, Category = "Hud Load")
		TSubclassOf<UTP_FakeLoading> widgetLoad = nullptr;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode")
		bool debugDraw = true;
public:
	FORCEINLINE bool GetDebugDraw() { return debugDraw; };
public:
	ATP_GameMode();
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	FORCEINLINE TSoftObjectPtr<ATP_GameLogic> GetGameLogic() { return gameLogic; }
};
