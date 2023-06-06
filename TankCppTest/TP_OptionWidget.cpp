#include "TP_OptionWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_TankPlayer.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UTP_OptionWidget::NativeConstruct()
{
	Video_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::VideoButtonPressed);
	Audio_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::AudioButtonPressed);
	Controls_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::ControlsButtonPressed);
	Resolution_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::ResolutionButtonPressed);
	Graphics_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::GraphicsButtonPressed);

	_640x480_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::_640x480ButtonPressed);
	_1280x720_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::_1280x720ButtonPressed);
	_1920x1080_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::_1920x1080ButtonPressed);
	_2560x1440_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::_2560x1440ButtonPressed);

	Low_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::LowButtonPressed);
	Medium_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::MediumButtonPressed);
	High_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::HighButtonPressed);
	Ultra_B->OnClicked.AddDynamic(this, &UTP_OptionWidget::UltraButtonPressed);

	Slider_3->OnMouseCaptureEnd.AddDynamic(this, &UTP_OptionWidget::MasterVolumeChanged);
	Slider->OnMouseCaptureEnd.AddDynamic(this, &UTP_OptionWidget::MusicVolumeChanged);

	onReturnPressed.AddDynamic(this, &UTP_OptionWidget::ReturnKey);
}
#pragma region sousMenu
void UTP_OptionWidget::VideoButtonPressed()
{
	
	Controls_B->SetVisibility(ESlateVisibility::Hidden);
	Audio_B->SetVisibility(ESlateVisibility::Hidden);
	Video_B->SetVisibility(ESlateVisibility::Hidden);
	Square_2->SetVisibility(ESlateVisibility::Hidden);
	Square_4->SetVisibility(ESlateVisibility::Hidden);
	Square_5->SetVisibility(ESlateVisibility::Hidden);
	Slider->SetVisibility(ESlateVisibility::Hidden);
	Slider_3->SetVisibility(ESlateVisibility::Hidden);
	Master_Volume->SetVisibility(ESlateVisibility::Hidden);
	Music_Volume->SetVisibility(ESlateVisibility::Hidden);

	Resolution_B->SetVisibility(ESlateVisibility::Visible);
	Graphics_B->SetVisibility(ESlateVisibility::Visible);
	
}

void UTP_OptionWidget::AudioButtonPressed()
{
	if (!isSoundClicked)
	{
		Slider->SetVisibility(ESlateVisibility::Visible);
		Slider_3->SetVisibility(ESlateVisibility::Visible);
		Master_Volume->SetVisibility(ESlateVisibility::Visible);
		Music_Volume->SetVisibility(ESlateVisibility::Visible);

		isSoundClicked = true;
	}
	else
	{
		Slider->SetVisibility(ESlateVisibility::Hidden);
		Slider_3->SetVisibility(ESlateVisibility::Hidden);
		Master_Volume->SetVisibility(ESlateVisibility::Hidden);
		Music_Volume->SetVisibility(ESlateVisibility::Hidden);

		isSoundClicked = false;
	}
}

void UTP_OptionWidget::ControlsButtonPressed()
{
	RemoveFromParent();
	if (!widgetControl)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetControl, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();

	ATP_TankPlayer* _tempoPlayer = Cast<ATP_TankPlayer>(GetOwningPlayerPawn());
	if (!_tempoPlayer)return;
	_tempoPlayer->SetControlWidget(_temp);
}

void UTP_OptionWidget::ResolutionButtonPressed()
{
	if (isGraphClicked)
	{
		Low_B->SetVisibility(ESlateVisibility::Hidden);
		Medium_B->SetVisibility(ESlateVisibility::Hidden);
		High_B->SetVisibility(ESlateVisibility::Hidden);
		Ultra_B->SetVisibility(ESlateVisibility::Hidden);
		isGraphClicked = false;
	}
	if (!isResolutionClicked)
	{
		_640x480_B->SetVisibility(ESlateVisibility::Visible);
		_1280x720_B->SetVisibility(ESlateVisibility::Visible);
		_1920x1080_B->SetVisibility(ESlateVisibility::Visible);
		_2560x1440_B->SetVisibility(ESlateVisibility::Visible);
		isResolutionClicked = true;
	}
	else
	{
		_640x480_B->SetVisibility(ESlateVisibility::Hidden);
		_1280x720_B->SetVisibility(ESlateVisibility::Hidden);
		_1920x1080_B->SetVisibility(ESlateVisibility::Hidden);
		_2560x1440_B->SetVisibility(ESlateVisibility::Hidden);

		isResolutionClicked = false;
	}
}

void UTP_OptionWidget::GraphicsButtonPressed()
{
	if (isResolutionClicked)
	{
		_640x480_B->SetVisibility(ESlateVisibility::Hidden);
		_1280x720_B->SetVisibility(ESlateVisibility::Hidden);
		_1920x1080_B->SetVisibility(ESlateVisibility::Hidden);
		_2560x1440_B->SetVisibility(ESlateVisibility::Hidden);

		isResolutionClicked = false;
	}
	if (!isGraphClicked)
	{
		Low_B->SetVisibility(ESlateVisibility::Visible);
		Medium_B->SetVisibility(ESlateVisibility::Visible);
		High_B->SetVisibility(ESlateVisibility::Visible);
		Ultra_B->SetVisibility(ESlateVisibility::Visible);
		isGraphClicked = true;
	}
	else
	{
		Low_B->SetVisibility(ESlateVisibility::Hidden);
		Medium_B->SetVisibility(ESlateVisibility::Hidden);
		High_B->SetVisibility(ESlateVisibility::Hidden);
		Ultra_B->SetVisibility(ESlateVisibility::Hidden);
		isGraphClicked = false;
	}
}
#pragma endregion

#pragma region resolution
void UTP_OptionWidget::_640x480ButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.setRes 640x480");
}

void UTP_OptionWidget::_1280x720ButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.setRes 1280x720");
}

void UTP_OptionWidget::_1920x1080ButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.setRes 1920x1080");
}

void UTP_OptionWidget::_2560x1440ButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.setRes 2560x1440");
}
#pragma endregion

#pragma region graphique
void UTP_OptionWidget::LowButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.LightFunctionQuality=0");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.Streaming.MipBias=2.5");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ScreenPercentage 50");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ViewDistanceScale 0.4");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.PostProcessAAQuality 0");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.MotionBlurQuality=0");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.TranslucencyLightingVolumeDim=24");
}

void UTP_OptionWidget::MediumButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.LightFunctionQuality=1");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.Streaming.MipBias=1");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ScreenPercentage 75");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ViewDistanceScale 0.6");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.PostProcessAAQuality 2");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.MotionBlurQuality=3");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.TranslucencyLightingVolumeDim=32");
}

void UTP_OptionWidget::HighButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.LightFunctionQuality=1");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.Streaming.MipBias=0");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ScreenPercentage 90");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ViewDistanceScale 0.8");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.PostProcessAAQuality 4");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.MotionBlurQuality=3");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.TranslucencyLightingVolumeDim=48");
}

void UTP_OptionWidget::UltraButtonPressed()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.LightFunctionQuality=1");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.Streaming.MipBias=0");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ScreenPercentage 100");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.ViewDistanceScale 1");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.PostProcessAAQuality 6");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.MotionBlurQuality=4");
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "r.TranslucencyLightingVolumeDim=64");
}
#pragma endregion

#pragma region son
void UTP_OptionWidget::MasterVolumeChanged()
{

}

void UTP_OptionWidget::MusicVolumeChanged()
{

}
#pragma endregion

void UTP_OptionWidget::ReturnKey()
{
	RemoveFromParent();
	if (!widgetPause)return;
	UUserWidget* _temp = UWidgetBlueprintLibrary::Create(GetWorld(), widgetPause, GetWorld()->GetFirstPlayerController());
	if (!_temp)return;
	_temp->AddToViewport();
	ATP_TankPlayer* _tempoPlayer = Cast<ATP_TankPlayer>(GetOwningPlayerPawn());
	if (!_tempoPlayer)return;
	_tempoPlayer->SetPauseWidget(_temp);

}