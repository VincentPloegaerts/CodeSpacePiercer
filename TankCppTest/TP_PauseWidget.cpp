#include "TP_PauseWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_OptionWidget.h"
#include "TP_TankPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "TP_TankCppTestGameInstance.h"


void UTP_PauseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UWidget::RemoveFromParent();
	UGameplayStatics::SetGamePaused(this, true);
	onStartPause.Broadcast();
	Resume_B->OnClicked.AddDynamic(this, &UTP_PauseWidget::ResumeButtonPressed);
	Option_B_1->OnClicked.AddDynamic(this, &UTP_PauseWidget::OptionButtonPressed);
	Quit_B->OnClicked.AddDynamic(this, &UTP_PauseWidget::QuitGameButtonPressed);
	Exit_B->OnClicked.AddDynamic(this, &UTP_PauseWidget::MainMenuButtonPressed);

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

}

void UTP_PauseWidget::ResumeButtonPressed()
{
	RemoveFromParent();
	UGameplayStatics::SetGamePaused(this, false);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	onStopPause.Broadcast();

	ATP_TankPlayer* _tempoPlayer = Cast<ATP_TankPlayer>(GetOwningPlayerPawn());
	if (!_tempoPlayer)return;
	_tempoPlayer->SetOptionWidget(nullptr);
	_tempoPlayer->SetPauseWidget(nullptr);
	_tempoPlayer->SetControlWidget(nullptr);
}

void UTP_PauseWidget::OptionButtonPressed()
{
	
	RemoveFromParent();

	if (!widgetOption)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetOption, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();

	ATP_TankPlayer* _tempoPlayer = Cast<ATP_TankPlayer>(GetOwningPlayerPawn());
	if (!_tempoPlayer)return;
	_tempoPlayer->SetOptionWidget(_temp);
	
}

void UTP_PauseWidget::QuitGameButtonPressed()
{
	UKismetSystemLibrary::QuitGame(this,GetWorld()->GetFirstPlayerController(),EQuitPreference::Quit,false);
}

void UTP_PauseWidget::MainMenuButtonPressed()
{
	UTP_TankCppTestGameInstance* _gameInstance = Cast<UTP_TankCppTestGameInstance>(GetGameInstance());
	if (_gameInstance)
		_gameInstance->SetCheckpointLevel2(false);
	UGameplayStatics::OpenLevel(GetWorld(), "Menu_Scene");
}
