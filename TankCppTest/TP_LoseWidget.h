#pragma once

//code majoriterement ecrit par julie 

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TP_LoseWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_LoseWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	//ici on a tous les bouton ou autr de l'ui qui vont reelement servir en code
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(BindWidget))
		UCanvasPanel* Canvas = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Reload = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* MainMenu = nullptr;
	UPROPERTY(EditAnywhere, Category = "LoseWidget | loadingWidget")
		TSubclassOf<UUserWidget> fakeLoading = nullptr;
	UPROPERTY(EditAnywhere, Category = "LoseWidget | loadingTime")
		float loadingTime = 3;
	UPROPERTY()
		FTimerHandle timer;

public:
	virtual void NativeConstruct() override;
private:
	//sont appeler lorsque les boutosn sont clicker
	UFUNCTION() void ReloadButtonPressed();
	UFUNCTION() void MainMenuPressed();

	//on appele la fonction qui vca faire un open level
	UFUNCTION() void RetryLevel();
	UFUNCTION() void GoToMainMenu();
};
