#include "TP_TurretAnimInstance.h"
#include "TP_Tower.h"
#include "TP_ShootTowerSystemComponent.h"

UTP_TurretAnimInstance::UTP_TurretAnimInstance()
{

}

void UTP_TurretAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner=Cast<ATP_Tower>(GetOwningActor());
	if (!owner)return;
	owner->GetHealthComponent()->onDeathEvent.AddDynamic(this, &UTP_TurretAnimInstance::SetDeath);
	owner->GetShootTowerSystemComponent()->OnShootEvent().AddDynamic(this, &UTP_TurretAnimInstance::SetIsShooting);
	owner->GetShootTowerSystemComponent()->OnStopShootEvent().AddDynamic(this, &UTP_TurretAnimInstance::SetNotShooting);
}

void UTP_TurretAnimInstance::SetDeath(AActor* _deadActor)
{
	int32 _result = FMath::RandRange(1, 3);
	death = _result;
}

void UTP_TurretAnimInstance::SetIsShooting()
{
	isShooting = true;
}

void UTP_TurretAnimInstance::SetNotShooting()
{
	isShooting = false;
}
