#include "TP_PickUpComponent.h"
#include "TP_TankPlayer.h"
#include "TP_PickUp.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* _owner = GetOwner();
	if (!_owner)return;
		_owner->OnActorBeginOverlap.AddDynamic(this,&UTP_PickUpComponent::Pick);
}

void UTP_PickUpComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_PickUpComponent::Pick(AActor* _mine,AActor* _other)
{
	Behavior(_other);
}

//behavior lorsque le pick up overlap avec le player et q'on ets pas full mun ou health
void UTP_PickUpComponent::Behavior(AActor* _targetOverlap)
{
	if (!GetOwner())return;
	ATP_PickUp* _owner = Cast<ATP_PickUp>(GetOwner());
	_owner->DestroySpline();
	_owner->HidePickUp();
	_owner->SetLifeSpan(0.2);
}

