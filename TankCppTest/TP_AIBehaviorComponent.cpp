#include "TP_AIBehaviorComponent.h"
#include "DrawDebugHelpers.h"
#include "TP_GameMode.h"

UTP_AIBehaviorComponent::UTP_AIBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_AIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<ATP_GameCharacter>(GetOwner());
	target = Cast<ATP_TankPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}
void UTP_AIBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_AIBehaviorComponent::setCurrentDistance()
{
	if (!owner)return;
	FVector _ownerLoc = owner->GetActorLocation();
	if (!target)return;
	FVector _targetLoc = target->GetActorLocation();
	distanceTarget = FVector::Distance(_ownerLoc, _targetLoc);
	if (maxZRange != 0 && FMath::Abs(_targetLoc.Z - _ownerLoc.Z) >= maxZRange)
	{
		distanceTarget = -1;
		if(previousStatus != 3)
			onTargetLost.Broadcast();
		previousStatus = 3;
		return;
	}
	AiBehaviour();
}
void UTP_AIBehaviorComponent::AiBehaviour()
{
	if (distanceTarget <= rangeAttack && previousStatus != 1)
	{
		onTargetReached.Broadcast();
		previousStatus = 1;
	}
	else if (distanceTarget <= rangeAgro && distanceTarget > rangeAttack && previousStatus != 2)
	{
		onTargetNotInReachAnymore.Broadcast();
		previousStatus = 2;
	}
	else if(distanceTarget > rangeAgro && previousStatus != 3)
	{
		onTargetLost.Broadcast();
		previousStatus = 3;
	}
}
void UTP_AIBehaviorComponent::Activation()
{
	float _startDiff = FMath::RandRange(reactionTimeMin, reactionTimeMax);
	GetWorld()->GetTimerManager().SetTimer(timerCheckDistance, this, &UTP_AIBehaviorComponent::setCurrentDistance, reactionTime, true, _startDiff);
}
void UTP_AIBehaviorComponent::DrawDebug()
{
	DrawDebugCircle(GetWorld(), GetOwner()->GetActorLocation(), rangeAgro, 50, FColor::Green, false, -1, 0, 3, FVector(0, 1, 0), FVector(1, 0, 0));
	DrawDebugCircle(GetWorld(), GetOwner()->GetActorLocation(), rangeAttack, 50, FColor::Red, false, -1, 0, 3, FVector(0, 1, 0), FVector(1, 0, 0));
}