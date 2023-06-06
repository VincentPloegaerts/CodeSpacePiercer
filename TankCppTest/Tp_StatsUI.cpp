#include "TP_StatsUI.h"
#include "TP_GameMode.h"
#include "TP_RoomManager.h"
#include "TP_Room.h"

void UTP_StatsUI::NativeConstruct()
{
	Super::NativeConstruct();
	player = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (!player)return;
	player->GetShootMinigunComponent()->OnHeatChange().AddDynamic(this, &UTP_StatsUI::SetGatlingJauge);
	player->GetShootNukeComponent()->OnReload().AddDynamic(this, &UTP_StatsUI::SetNukeJauge);
	player->GetShootNukeComponent()->OnMunitionChange().AddDynamic(this, &UTP_StatsUI::SetMinitionNuke);
	player->GetShootPlasmaComponent()->OnReload().AddDynamic(this, &UTP_StatsUI::SetPlasmaJauge);
	player->GetShootPlasmaComponent()->OnMunitionChange().AddDynamic(this, &UTP_StatsUI::SetMunitionPlasma);
	player->GetHealthComponent()->OnLifeChange().AddDynamic(this, &UTP_StatsUI::SetHealth);
	player->GetShieldComponent()->OnShieldChange().AddDynamic(this, &UTP_StatsUI::SetShield);
	healthMaterial = HealthBar->GetDynamicMaterial();
	shieldMaterial = ShieldBar->GetDynamicMaterial();
	gatlingMaterial = GatlingBar->GetDynamicMaterial();
	plasmaMaterial = PlasmaBar->GetDynamicMaterial();
	nukeMaterial = NukeBar->GetDynamicMaterial();
	UWorld* _world = GetWorld();
	if (!_world)return;
	ATP_GameMode* _gameMode = _world->GetAuthGameMode<ATP_GameMode>();
	if (!_gameMode)return;
	if(!_gameMode->GetGameLogic())return;
	if(!_gameMode->GetGameLogic()->GetRoomManager())return;
	TMap<FString, ATP_Room*> mapRoom = _gameMode->GetGameLogic()->GetRoomManager()->GetRooms();
	for (auto& Elem : mapRoom)
	{
		if (Elem.Value->IsTimerRoom())
		{
			Elem.Value->OnRoomEnterEvent().AddDynamic(this, &UTP_StatsUI::ActivateTimer);
			UE_LOG(LogTemp, Warning, TEXT("Room clock nb"));
		}
	}
}
void UTP_StatsUI::ActivateTimer(UTP_TimerComponent* _clock)
{
	if (!_clock)return;
	onClockStart.Broadcast();
	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("time connard"));
	MinuteText->SetVisibility(ESlateVisibility::Visible);
	SecondText->SetVisibility(ESlateVisibility::Visible);
	minuteHeure->SetVisibility(ESlateVisibility::Visible);
	clock = nullptr;
	clock = _clock;
	clock->OnTimerChange().AddDynamic(this,&UTP_StatsUI::Timer);
	clock->OnFinishTimer().AddDynamic(this,&UTP_StatsUI::TimerHidden);
}
void UTP_StatsUI::TimerHidden()
{
	UE_LOG(LogTemp,Warning,TEXT("Hidden"));
	MinuteText->SetText(FText::AsNumber(0));
	SecondText->SetText(FText::AsNumber(0));
	MinuteText->SetVisibility(ESlateVisibility::Hidden);
	SecondText->SetVisibility(ESlateVisibility::Hidden);
	minuteHeure->SetVisibility(ESlateVisibility::Hidden);
}
void UTP_StatsUI::Timer(float a,float b)
{
	if (!clock)return;
	float _minute = clock->GetCurrentTimeMinute();
	MinuteText->SetText(FText::AsNumber(_minute));
	float _second = clock->GetCurrentTimeSecond();
	SecondText->SetText(FText::AsNumber(FMath::RoundToInt(_second)));
}
void UTP_StatsUI::SetMunitionPlasma(int _plasmaMunValue)
{
	Mun_Plasma_HUD->SetText(FText::AsNumber(_plasmaMunValue));
}
void UTP_StatsUI::SetMinitionNuke(int _nukeMunValue)
{
	Mun_Nuke_HUD->SetText(FText::AsNumber(_nukeMunValue));
}
void UTP_StatsUI::SetVisibleOverHeat()
{
	WarningImage->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle a;
	GetWorld()->GetTimerManager().SetTimer(a,this, &UTP_StatsUI::SetHiddenOverHeat, speedBlinking,false,-1);
}
void UTP_StatsUI::SetHiddenOverHeat()
{
	WarningImage->SetVisibility(ESlateVisibility::Hidden);
	if (ratioOverHeat < 0.75f) {
		ratioWarning = false;
		return;
	}
	FTimerHandle a;
	GetWorld()->GetTimerManager().SetTimer(a,this, &UTP_StatsUI::SetVisibleOverHeat, speedBlinking,false,-1);
}
void UTP_StatsUI::SetAllHidden()
{
	PlayerPanel->SetVisibility(ESlateVisibility::Hidden);
	SequencePanel->SetVisibility(ESlateVisibility::Visible);
}

void UTP_StatsUI::SetAllVisible()
{
	PlayerPanel->SetVisibility(ESlateVisibility::Visible);
	SequencePanel->SetVisibility(ESlateVisibility::Hidden);
}


#pragma region progressBar
void UTP_StatsUI::SetHealth(float _currentHealth, float _maxHealth)
{
	float _ratioHealth = _currentHealth / _maxHealth;
	if (!healthMaterial)return;
	healthMaterial->SetScalarParameterValue("progress", _ratioHealth);
	FLinearColor _color = FMath::Lerp(FLinearColor::White, colorHealthBar, _ratioHealth);
	healthMaterial->SetVectorParameterValue("FinalColor", _color);
}
void UTP_StatsUI::SetShield(float _currentShield, float _maxShield)
{
	float _ratioShield = _currentShield / _maxShield;
	if (!shieldMaterial)return;
	shieldMaterial->SetScalarParameterValue("progress", _ratioShield);
	FLinearColor _color = FMath::Lerp(FLinearColor::White, colorShieldBar, _ratioShield);
	shieldMaterial->SetVectorParameterValue("FinalColor", _color);
}
void UTP_StatsUI::SetGatlingJauge(float _overheatValue, float _overheatMaxValue)
{
	ratioOverHeat = _overheatValue / _overheatMaxValue;
	if (!gatlingMaterial)return;
	gatlingMaterial->SetScalarParameterValue("progress", ratioOverHeat);
	FLinearColor _color = FMath::Lerp(FLinearColor::White, colorGatlingBar, ratioOverHeat * speedColorChangeMinigunBar);
	gatlingMaterial->SetVectorParameterValue("FinalColor", _color);
	if (!ratioWarning && ratioOverHeat >= 0.75f)
	{
		ratioWarning = true;
		SetVisibleOverHeat();
	}
	if (ratioOverHeat >= 1.f)
		OverHeatImage->SetVisibility(ESlateVisibility::Visible);
	if (ratioOverHeat <= 0.f)
		OverHeatImage->SetVisibility(ESlateVisibility::Hidden);
}
void UTP_StatsUI::SetPlasmaJauge(float _plasmaValue, float _plasmaMaxValue)
{
	float _ratioPlasma = _plasmaValue / _plasmaMaxValue;
	if (!plasmaMaterial)return;
	plasmaMaterial->SetScalarParameterValue("progress", _ratioPlasma);
	FLinearColor _color = FMath::Lerp(FLinearColor::White, colorPlasmaBar, _ratioPlasma);
	plasmaMaterial->SetVectorParameterValue("FinalColor", _color);
}
void UTP_StatsUI::SetNukeJauge(float _nukeValue, float _nukeMaxValue)
{
	float _ratioNuke = _nukeValue / _nukeMaxValue;
	if (!nukeMaterial)return;
	nukeMaterial->SetScalarParameterValue("progress", _ratioNuke);
	FLinearColor _color = FMath::Lerp(FLinearColor::White, colorNukeBar, _ratioNuke);
	nukeMaterial->SetVectorParameterValue("FinalColor", _color);
}
#pragma endregion