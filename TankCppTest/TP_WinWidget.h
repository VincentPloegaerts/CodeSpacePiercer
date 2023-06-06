#pragma once

//code ecrit par julie 

/*ON SE S'en sert pas en jeu mais c'est juste un ui avec deux boutosn qui desactive le joueur 
un des boutosn permet d'aller au niveau suivant et l'autre permet de revenir au menu */

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TP_FakeLoading.h"
#include "TP_WinWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_WinWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* Canvas = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* NextLevel = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* MainMenu = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		FTimerHandle timer;
	UPROPERTY(EditAnywhere, Category = "WinWidget | loadingWidget")
		TSubclassOf<UUserWidget> widgetLoad = nullptr;
	UPROPERTY(EditAnywhere, Category = "WinWidget | loadingTime")
		float loadingTime = 3;
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "NextLevelName")
		FName nextLevelName;
public:
	virtual void NativeConstruct()override;
private:
	UFUNCTION() void NextLevelPressed();
	UFUNCTION() void MainMenuPressed();
	UFUNCTION() void GoIntoNextLevel();
	UFUNCTION() void GoToMenu();
	
};
