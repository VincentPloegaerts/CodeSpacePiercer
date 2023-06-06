#include "TP_DroneAnimInstance.h"
#include "TP_Drone.h"

UTP_DroneAnimInstance::UTP_DroneAnimInstance()
{
}
void UTP_DroneAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_Drone>(GetOwningActor());
	if (!owner)return;
	owner->GetHealthComponent()->OnDeathEvent().AddDynamic(this, &UTP_DroneAnimInstance::SetDeath);
	owner->GetAiMovement()->OnDroneStuned().AddDynamic(this, &UTP_DroneAnimInstance::SetStuned);
	owner->GetAiMovement()->OnDroneUnStuned().AddDynamic(this, &UTP_DroneAnimInstance::SetUnStuned);
	owner->OnDroneAttack().AddDynamic(this, &UTP_DroneAnimInstance::SetIsShooting);
	owner->OnDroneTargetLost().AddDynamic(this, &UTP_DroneAnimInstance::SetLost);
	owner->OnDroneTargetAcquired().AddDynamic(this, &UTP_DroneAnimInstance::SetNotShooting);
	owner->OnDroneTargetAcquired().AddDynamic(this, &UTP_DroneAnimInstance::SetWalkTo);
	owner->OnBumped().AddDynamic(this, &UTP_DroneAnimInstance::IsBumped);
}
void UTP_DroneAnimInstance::SetStuned()
{
	isStuned = true;
}
void UTP_DroneAnimInstance::SetUnStuned()
{
	isStuned = false;
}

void UTP_DroneAnimInstance::SetDeath(AActor* _deadActor)
{
	int32 _result = FMath::RandRange(1, 4);
	death = _result;
}

void UTP_DroneAnimInstance::SetIsShooting()
{
	isShooting = true;
}
void UTP_DroneAnimInstance::SetNotShooting()
{
	isShooting = false;
}

void UTP_DroneAnimInstance::SetWalkTo()
{
	isBumped = false;
	isInRange = true;
}
void UTP_DroneAnimInstance::SetLost()
{
	isInRange = false;
}

void UTP_DroneAnimInstance::IsBumped()
{
	owner->wasBumped = false;
	isStuned = false;
	isBumped = true;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_DroneAnimInstance::IsNotBumped, 2.25, false, -1);
}

void UTP_DroneAnimInstance::IsNotBumped()
{
	isBumped = false;
}
