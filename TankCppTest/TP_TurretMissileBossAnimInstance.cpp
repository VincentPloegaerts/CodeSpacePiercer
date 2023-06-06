#include "TP_TurretMissileBossAnimInstance.h"

UTP_TurretMissileBossAnimInstance::UTP_TurretMissileBossAnimInstance()
{

}

void UTP_TurretMissileBossAnimInstance::NativeBeginPlay()
{
	owner = Cast<ATP_BOSS_level3>(GetOwningActor());
	if (!owner)return;
	owner->GetTankNukeSystem()->OnShootEvent().AddDynamic(this, &UTP_TurretMissileBossAnimInstance::IsShooting);
	owner->GetTankNukeSystem()->OnStopShootEvent().AddDynamic(this, &UTP_TurretMissileBossAnimInstance::IsNotShooting);
}

void UTP_TurretMissileBossAnimInstance::IsShooting()
{
	isShooting = true;
}

void UTP_TurretMissileBossAnimInstance::IsNotShooting()
{
	isShooting = false;
}

