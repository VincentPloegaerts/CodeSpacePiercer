#pragma once

//code ecrit par julie 

//on ne s'en sert pas on se sert pour l'instant d'un menu presque full bp 

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "TP_LevelPickWidget.generated.h"

UCLASS()
class TANKCPPTEST_API UTP_LevelPickWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Spaceship_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Scientific_Base_B = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UButton* Asteroid_B = nullptr;
	UPROPERTY(EditAnywhere, Category = "LevelPick | loadingScreen")
		TSubclassOf<UUserWidget> loadingScreen = nullptr;
	UPROPERTY(EditAnywhere, Category = "LevelPick | loadingTime")
		float loadingTime = 3;
	UPROPERTY()
		FTimerHandle timer;
private:
	virtual void NativeConstruct()override;

	UFUNCTION() void GoToFirstLevel();
	UFUNCTION() void GoToSecondLevel();
	UFUNCTION() void GoToThirdLevel();
	void LaunchFirstLevel();
	void LaunchSecondLevel();
	void LaunchThirdLevel();
};
