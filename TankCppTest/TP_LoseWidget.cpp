#include "TP_LoseWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TP_TankCppTestGameInstance.h"


void UTP_LoseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UGameplayStatics::SetGamePaused(this, true);
	Reload->OnClicked.AddDynamic(this, &UTP_LoseWidget::ReloadButtonPressed);
	MainMenu->OnClicked.AddDynamic(this, &UTP_LoseWidget::MainMenuPressed);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UTP_LoseWidget::ReloadButtonPressed()
{
	UGameplayStatics::SetGamePaused(this, false);
	if (!fakeLoading)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), fakeLoading, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_LoseWidget::RetryLevel, loadingTime, false, -1);
}

void UTP_LoseWidget::MainMenuPressed()
{
	UGameplayStatics::SetGamePaused(this, false);
	if (!fakeLoading)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), fakeLoading, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_LoseWidget::GoToMainMenu, loadingTime, false, -1);
}

void UTP_LoseWidget::RetryLevel()
{
	FName _tempo = FName(*UGameplayStatics::GetCurrentLevelName(GetWorld()));
	UGameplayStatics::OpenLevel(GetWorld(), _tempo);
}

void UTP_LoseWidget::GoToMainMenu()
{
	UTP_TankCppTestGameInstance* _gameInstance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	if (_gameInstance)
		_gameInstance->SetCheckpointLevel2(false);
	UGameplayStatics::OpenLevel(GetWorld(), "Menu_Scene");
}
