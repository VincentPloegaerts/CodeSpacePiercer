#include "TP_GameMode.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


ATP_GameMode::ATP_GameMode()
{
}

void ATP_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	if (!widgetLoad)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetLoad, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
}
