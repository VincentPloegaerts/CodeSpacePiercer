#include "TP_TankPlayerAnimInstance.h"

UTP_TankPlayerAnimInstance::UTP_TankPlayerAnimInstance()
{

}

void UTP_TankPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_TankPlayer>(GetOwningActor());
	if (!owner)return;
	owner->GetHealthComponent()->onDeathEvent.AddDynamic(this, &UTP_TankPlayerAnimInstance::IsDead);
	owner->GetShootNukeComponent()->OnNukesFired().AddDynamic(this, &UTP_TankPlayerAnimInstance::FireNuke);
	owner->GetShootNukeComponent()->OnNukesLanded().AddDynamic(this, &UTP_TankPlayerAnimInstance::NukeFired);
	owner->OnMove().AddDynamic(this, &UTP_TankPlayerAnimInstance::UpdateAxisValues);
}

void UTP_TankPlayerAnimInstance::IsDead(AActor* _deadActor)
{
	death = true;
}

void UTP_TankPlayerAnimInstance::FireNuke()
{
	fireNuke=true;
}

void UTP_TankPlayerAnimInstance::NukeFired()
{
	fireNuke = false;
}

void UTP_TankPlayerAnimInstance::UpdateAxisValues()
{
	vertical = owner->GetVerticalAxisValue();
	horizontal = owner->GetHorizontalAxisValue();
}
