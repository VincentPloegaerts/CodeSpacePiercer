#include "TP_TimerComponent.h"

UTP_TimerComponent::UTP_TimerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_TimerComponent::BeginPlay()
{
	Super::BeginPlay();
}
void UTP_TimerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!working)return;
	if (decrease)
		DecreaseTimer();
	else
		IncreaseTimer();
}

void UTP_TimerComponent::SetTimer(float _timer, float _timerMax)
{
	working = true;
	timer = _timer;
	timerMax = _timerMax;
	onTimerChange.Broadcast(timer, timerMax);
}
void UTP_TimerComponent::IncreaseTimer()
{
	if (timer < timerMax)
		timer += GetWorld()->DeltaTimeSeconds;
	onTimerChange.Broadcast(timer, timerMax);
	if (timer >= timerMax)
	{
		onFinishTimer.Broadcast();
		timer = timerMax;
		working = false;
		return;
	}
}
void UTP_TimerComponent::DecreaseTimer()
{
	if (timer > 0)
		timer -= GetWorld()->DeltaTimeSeconds;
	onTimerChange.Broadcast(timer, timerMax);
	if (timer <= 0)
	{
		onFinishTimer.Broadcast();
		timer = 0;
		working = false;
		return;
	}
}