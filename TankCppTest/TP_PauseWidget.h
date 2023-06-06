#pragma once
#include "CoreMinimal.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TP_PauseWidget.generated.h"
class UTP_OptionWidget;
UCLASS()
class TANKCPPTEST_API UTP_PauseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopPauseMenu);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStopPauseMenu onStopPause;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartPauseMenu);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStartPauseMenu onStartPause;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* CanvasPanel_0 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Resume_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Option_B_1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Quit_B = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Exit_B = nullptr;

	UPROPERTY(EditAnywhere, Category = "Option")
		TSubclassOf<UTP_OptionWidget> widgetOption = nullptr;
public:
	virtual void NativeConstruct() override;
	FORCEINLINE TSubclassOf<UTP_OptionWidget> GetOption() { return widgetOption; }
private:

	UFUNCTION() 
		void OptionButtonPressed();
	UFUNCTION() 
		void QuitGameButtonPressed();
	UFUNCTION()
		void MainMenuButtonPressed();
public:
	UFUNCTION() 
		void ResumeButtonPressed();
};
