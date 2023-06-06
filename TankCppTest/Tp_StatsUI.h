#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanel.h"
#include "Components/CircularThrobber.h"
#include "TP_TankPlayer.h"
#include "TP_TimerComponent.h"
#include "TP_StatsUI.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClockStart);


UCLASS()
class TANKCPPTEST_API UTP_StatsUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnClockStart onClockStart;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* SequencePanel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UCanvasPanel* PlayerPanel;
	UPROPERTY()
		UTP_TimerComponent* clock = nullptr;
#pragma region MaterialInstance
	UPROPERTY( )UMaterialInstanceDynamic* healthMaterial = nullptr;
	UPROPERTY() UMaterialInstanceDynamic* shieldMaterial = nullptr;
	UPROPERTY() UMaterialInstanceDynamic* gatlingMaterial = nullptr;
	UPROPERTY() UMaterialInstanceDynamic* plasmaMaterial = nullptr;
	UPROPERTY() UMaterialInstanceDynamic* nukeMaterial = nullptr;
#pragma endregion
#pragma region Textblock
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Mun_Plasma_HUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Mun_Nuke_HUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* MinuteText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* SecondText;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* minuteHeure;
#pragma endregion
#pragma region Parametre
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float speedColorChangeMinigunBar = 1.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorGatlingBar = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorPlasmaBar = FLinearColor::Blue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorNukeBar = FLinearColor::Green;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorHealthBar = FLinearColor::Red;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FLinearColor colorShieldBar = FLinearColor::Blue;
#pragma endregion
#pragma region Image
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* OverHeatImage = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* WarningImage = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* AimCursor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* HealthBar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* ShieldBar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* GatlingBar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* PlasmaBar = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
		UImage* NukeBar = nullptr;
	UPROPERTY()
		bool ratioWarning = false;
	UPROPERTY(EditAnywhere, Category = "Stat")
		float speedBlinking = 0.15f;
	UPROPERTY()
		float ratioOverHeat = 0;
	UPROPERTY()
		ATP_TankPlayer* player = nullptr;
#pragma endregion
public:
	virtual void NativeConstruct() override;
	UFUNCTION() void ActivateTimer(UTP_TimerComponent* _clock);
	UFUNCTION() void Timer(float a, float b);
	UFUNCTION() void TimerHidden();
	UFUNCTION() void SetHealth(float _currentHealth, float _maxHealth);
	UFUNCTION() void SetShield(float _currentShield, float _maxShield);
	UFUNCTION() void SetGatlingJauge(float _overheatValue, float _overheatMaxValue);
	UFUNCTION() void SetPlasmaJauge(float _plasmaValue, float _plasmaMaxValue);
	UFUNCTION() void SetNukeJauge(float _nukeValue, float _nukeMaxValue);
	UFUNCTION() void SetMunitionPlasma(int _plasmaMunValue);
	UFUNCTION() void SetMinitionNuke(int _nukeMunValue);
	UFUNCTION() void SetVisibleOverHeat();
	UFUNCTION() void SetHiddenOverHeat();
	UFUNCTION() void SetAllHidden();
	UFUNCTION() void SetAllVisible();

	FORCEINLINE FOnClockStart& OnClockStart() { return onClockStart; }
};
