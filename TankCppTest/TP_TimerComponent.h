#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TP_TimerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKCPPTEST_API UTP_TimerComponent : public UActorComponent
{
	GENERATED_BODY()
protected:
#pragma region Event
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinishTimer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnFinishTimer onFinishTimer;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerHalfElapsed);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnTimerHalfElapsed onTimerHalfElapsed;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTimer);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnStartTimer onStartTimer;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerChange, float , _timer, float ,_timerMax);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable)
		FOnTimerChange onTimerChange;
#pragma endregion
	UPROPERTY()
		bool stopTimer = false;
	UPROPERTY()
		bool working = false;
	UPROPERTY()
		float timer = 0;
	UPROPERTY()
		float timerMax = 0;
	UPROPERTY(EditAnywhere, Category = "Parameter")
		bool decrease = true;
public:
	UFUNCTION() void SetTimer(float _timer,float _timerMax = 0);
	FORCEINLINE float& GetCurrentTime() { return timer; };
	FORCEINLINE float GetCurrentTimeMinute() { return FMath::TruncToFloat(timer/60.f); };
	FORCEINLINE float GetCurrentTimeSecond() { return (timer - GetCurrentTimeMinute()*60); };
	FORCEINLINE FOnFinishTimer& OnFinishTimer() { return onFinishTimer; };
	FORCEINLINE FOnTimerHalfElapsed& OnTimerHalfElapsed() { return onTimerHalfElapsed; };
	FORCEINLINE FOnStartTimer& OnStartTimer() { return onStartTimer; };
	FORCEINLINE FOnTimerChange& OnTimerChange() { return onTimerChange; };
public:	
	UTP_TimerComponent();
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IncreaseTimer();
	void DecreaseTimer();
};