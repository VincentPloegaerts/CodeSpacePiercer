#include "TP_MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTP_MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Square_Tutorial->SetVisibility(ESlateVisibility::Hidden);
	Yes_Please_B->SetVisibility(ESlateVisibility::Hidden);
	No_Thanks_B->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_3->SetVisibility(ESlateVisibility::Hidden);
	TextBlock_5->SetVisibility(ESlateVisibility::Hidden);
	Yes_B->SetVisibility(ESlateVisibility::Hidden);
	No_B->SetVisibility(ESlateVisibility::Hidden);
	Square_Fade_Leave->SetVisibility(ESlateVisibility::Hidden);
	Continue_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::GoToChoosingLevelWidget);
	New_Game_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::CallTutorialWindow);
	Options_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::GoToOptions);
	Exit_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::CallExitWindow);
	Yes_Please_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::GoToTutorial);
	No_Thanks_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::LaunchFirstLevel);
	Yes_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::ExitGame);
	No_B->OnClicked.AddDynamic(this, &UTP_MainMenuWidget::HideExitWindow);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void UTP_MainMenuWidget::GoToChoosingLevelWidget()
{
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_MainMenuWidget::LaunchLevelPickMenu, 2, false, -1);
}

void UTP_MainMenuWidget::CallTutorialWindow()
{
	Square_Tutorial->SetVisibility(ESlateVisibility::Visible);
	Yes_Please_B->SetVisibility(ESlateVisibility::Visible);
	No_Thanks_B->SetVisibility(ESlateVisibility::Visible);
	TextBlock_3->SetVisibility(ESlateVisibility::Visible);
}

void UTP_MainMenuWidget::CallExitWindow()
{
	TextBlock_5->SetVisibility(ESlateVisibility::Visible);
	Yes_B->SetVisibility(ESlateVisibility::Visible);
	No_B->SetVisibility(ESlateVisibility::Visible);
	Square_Fade_Leave->SetVisibility(ESlateVisibility::Visible);
}

void UTP_MainMenuWidget::HideExitWindow()
{
	TextBlock_5->SetVisibility(ESlateVisibility::Hidden);
	Yes_B->SetVisibility(ESlateVisibility::Hidden);
	No_B->SetVisibility(ESlateVisibility::Hidden);
	Square_Fade_Leave->SetVisibility(ESlateVisibility::Hidden);
}

void UTP_MainMenuWidget::GoToOptions()
{
	RemoveFromParent();
	if (!optionWidget)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), optionWidget, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();
}

void UTP_MainMenuWidget::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UTP_MainMenuWidget::GoToTutorial()
{
	if (!loadingScreen)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), loadingScreen, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_MainMenuWidget::LaunchTutorial, loadingTime, false, -1);
}

void UTP_MainMenuWidget::LaunchTutorial()
{
	UGameplayStatics::OpenLevel(this, "Tuto_Main");
}

void UTP_MainMenuWidget::LaunchFirstLevel()
{
	UGameplayStatics::OpenLevel(this, "Entry_Scene");
}

void UTP_MainMenuWidget::LaunchLevelPickMenu()
{
	RemoveFromParent();
	if (!levelPickWidget)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), levelPickWidget, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();
}
