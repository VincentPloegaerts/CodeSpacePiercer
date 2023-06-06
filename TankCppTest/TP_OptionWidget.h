#pragma once
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Slider.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

#include "TP_OptionWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_OptionWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnPressed);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnReturnPressed onReturnPressed;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* CanvasPanel_0 = nullptr;

	UPROPERTY(EditAnywhere, Category = "Pause")
		TSubclassOf<UUserWidget> widgetPause = nullptr;
	UPROPERTY(EditAnywhere, Category = "Control")
		TSubclassOf<UUserWidget> widgetControl = nullptr;

#pragma region sousMenu
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Video_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Audio_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Controls_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Resolution_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Graphics_B = nullptr;
#pragma endregion
	
#pragma region resolution
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* _640x480_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* _1280x720_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* _1920x1080_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* _2560x1440_B = nullptr;
#pragma endregion

#pragma region graphique

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Low_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Medium_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* High_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Ultra_B = nullptr;
#pragma endregion

#pragma region son

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* Slider_3 = nullptr; //master volume
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		USlider* Slider = nullptr;
#pragma endregion

#pragma region image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square = nullptr; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_2 = nullptr; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_4 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_5 = nullptr; 

#pragma endregion

#pragma region Text
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Master_Volume = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Music_Volume = nullptr;
#pragma endregion


private:
	UPROPERTY()
		bool isSoundClicked = false;
	UPROPERTY()
		bool isResolutionClicked = false;
	UPROPERTY()
		bool isGraphClicked = false;

public:
	virtual void NativeConstruct() override;
private:
#pragma region sousMenu
	UFUNCTION()
		void VideoButtonPressed();
	UFUNCTION()
		void AudioButtonPressed();
	UFUNCTION()
		void ControlsButtonPressed();
	UFUNCTION()
		void ResolutionButtonPressed();
	UFUNCTION()
		void GraphicsButtonPressed();
#pragma endregion
	
#pragma region resolution
	UFUNCTION()
		void _640x480ButtonPressed();
	UFUNCTION()
		void _1280x720ButtonPressed();
	UFUNCTION()
		void _1920x1080ButtonPressed();
	UFUNCTION()
		void _2560x1440ButtonPressed();

#pragma endregion

#pragma region graphique
	UFUNCTION()
		void LowButtonPressed();
	UFUNCTION()
		void MediumButtonPressed();
	UFUNCTION()
		void HighButtonPressed();
	UFUNCTION()
		void UltraButtonPressed();
#pragma endregion

#pragma region son
	UFUNCTION()
		void MasterVolumeChanged();
	UFUNCTION()
		void MusicVolumeChanged();
#pragma endregion
public:
	UFUNCTION() void ReturnKey();
};
