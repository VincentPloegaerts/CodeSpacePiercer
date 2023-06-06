#include "TP_WinWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TP_TankPlayer.h"
#include "TP_TankCppTestGameInstance.h"

void UTP_WinWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NextLevel->OnClicked.AddDynamic(this, &UTP_WinWidget::NextLevelPressed);
	MainMenu->OnClicked.AddDynamic(this, &UTP_WinWidget::MainMenuPressed);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!_player)
	{
		UGameplayStatics::SetGamePaused(this, true);
		return;
	}
	_player->DesactivatePlayer();
}

void UTP_WinWidget::NextLevelPressed()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!_player)
		UGameplayStatics::SetGamePaused(this, false);
	if (!widgetLoad)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetLoad, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_WinWidget::GoIntoNextLevel, loadingTime, false, -1);
}

void UTP_WinWidget::MainMenuPressed()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!_player)
		UGameplayStatics::SetGamePaused(this, false);
	if (!widgetLoad)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetLoad, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_WinWidget::GoToMenu, loadingTime, false, -1);
}

void UTP_WinWidget::GoIntoNextLevel()
{
	UTP_TankCppTestGameInstance* _gameInstance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	if (_gameInstance)
		_gameInstance->SetCheckpointLevel2(false);
	UGameplayStatics::OpenLevel(GetWorld(), nextLevelName);
}

void UTP_WinWidget::GoToMenu()
{
	UTP_TankCppTestGameInstance* _gameInstance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	if (_gameInstance)
		_gameInstance->SetCheckpointLevel2(false);
	UGameplayStatics::OpenLevel(GetWorld(), "Menu_Scene");
}
