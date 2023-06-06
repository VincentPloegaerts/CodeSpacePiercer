#include "TP_HUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ATP_HUD::ATP_HUD()
{

}

void ATP_HUD::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* firstPlayerController = GetWorld()->GetFirstPlayerController();
	if (!firstPlayerController)return;
	if (!statWidget)return;
	statWidgetUsed = Cast<UTP_StatsUI>(UWidgetBlueprintLibrary::Create(GetWorld(), statWidget, firstPlayerController));
	if (!statWidgetUsed)return;
	statWidgetUsed->AddToViewport();
	if (!dialogueWidget)return;
	dialogueWidgetUsed = Cast<UTP_DialogueUI>(UWidgetBlueprintLibrary::Create(GetWorld(), dialogueWidget, firstPlayerController));
	if (!dialogueWidgetUsed)return;
	dialogueWidgetUsed->AddToViewport();
}

void ATP_HUD::HideStatsWidget()
{
	statWidgetUsed->SetAllHidden();
}

void ATP_HUD::MakeVisibleStatsWidget()
{
	statWidgetUsed->SetAllVisible();
}
