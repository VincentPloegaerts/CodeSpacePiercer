#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tp_StatsUI.h"
#include "TP_DialogueUI.h"
#include "TP_HUD.generated.h"

UCLASS()
class TANKCPPTEST_API ATP_HUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Hud | Widget")
		TSubclassOf<UTP_StatsUI> statWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "Hud | Widget")
		TSubclassOf<UTP_DialogueUI> dialogueWidget = nullptr;

	UPROPERTY()
		UTP_StatsUI* statWidgetUsed = nullptr;
	UPROPERTY()
		UTP_DialogueUI* dialogueWidgetUsed = nullptr;


public:
	FORCEINLINE UTP_StatsUI* GetStatWidget() { return statWidgetUsed; }
	FORCEINLINE UTP_DialogueUI* GetDialogueUI() { return dialogueWidgetUsed;}
public:
	ATP_HUD();
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION() void HideStatsWidget();
	UFUNCTION() void MakeVisibleStatsWidget();
};
