#include "TP_LevelPickWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTP_LevelPickWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Spaceship_B->OnClicked.AddDynamic(this, &UTP_LevelPickWidget::GoToFirstLevel);
	Scientific_Base_B->OnClicked.AddDynamic(this, &UTP_LevelPickWidget::GoToSecondLevel);
	Asteroid_B->OnClicked.AddDynamic(this, &UTP_LevelPickWidget::GoToThirdLevel);
}

void UTP_LevelPickWidget::GoToFirstLevel()
{
	if (!loadingScreen)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), loadingScreen, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_LevelPickWidget::LaunchFirstLevel, loadingTime, false, -1);
}

void UTP_LevelPickWidget::GoToSecondLevel()
{
	if (!loadingScreen)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), loadingScreen, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_LevelPickWidget::LaunchSecondLevel, loadingTime, false, -1);
}

void UTP_LevelPickWidget::GoToThirdLevel()
{
	if (!loadingScreen)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), loadingScreen, GetWorld()->GetFirstPlayerController());
	_temp->AddToViewport();
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_LevelPickWidget::LaunchThirdLevel, loadingTime, false, -1);
}

void UTP_LevelPickWidget::LaunchFirstLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Level1_Main");
}

void UTP_LevelPickWidget::LaunchSecondLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Level2_Main");
}

void UTP_LevelPickWidget::LaunchThirdLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Level3_Main");
}
