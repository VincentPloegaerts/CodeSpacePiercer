#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "TP_MainMenuWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Continue_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* New_Game_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Options_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Exit_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Yes_Please_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* No_Thanks_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_Tutorial = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TextBlock_3 = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* Square_Fade_Leave= nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* TextBlock_5= nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Yes_B= nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* No_B= nullptr;


	UPROPERTY(EditAnywhere, Category = "menu | loadingScreen")
		TSubclassOf<UUserWidget> loadingScreen = nullptr;
	UPROPERTY(editAnywhere, Category = "menu | optionsScreen")
		TSubclassOf<UUserWidget> optionWidget = nullptr;
	UPROPERTY(EditAnywhere, Category = "menu | LevelPickWidget")
		TSubclassOf<UUserWidget> levelPickWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "menu | loadingTime")
		float loadingTime = 3;
	UPROPERTY()
		FTimerHandle timer;
private:
	virtual void NativeConstruct()override;
private:
	UFUNCTION() void GoToChoosingLevelWidget();
	UFUNCTION() void CallTutorialWindow();
	UFUNCTION() void CallExitWindow();
	UFUNCTION() void HideExitWindow();
	UFUNCTION() void GoToOptions();
	UFUNCTION() void ExitGame();
	UFUNCTION() void GoToTutorial();
	void LaunchTutorial();
	void LaunchFirstLevel();
	void LaunchLevelPickMenu();
};
