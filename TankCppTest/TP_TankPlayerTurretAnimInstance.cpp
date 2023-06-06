#include "TP_TankPlayerTurretAnimInstance.h"

UTP_TankPlayerTurretAnimInstance::UTP_TankPlayerTurretAnimInstance()
{

}
void UTP_TankPlayerTurretAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_TankPlayer>(GetOwningActor());
	if (!owner)return;
	owner->GetShootMinigunComponent()->OnStartShootEvent().AddDynamic(this, &UTP_TankPlayerTurretAnimInstance::IsShootingMinigun);
	owner->GetShootMinigunComponent()->OnStopShootEvent().AddDynamic(this, &UTP_TankPlayerTurretAnimInstance::StopShootingMinigun);
	owner->GetShootPlasmaComponent()->OnShootEvent().AddDynamic(this, &UTP_TankPlayerTurretAnimInstance::IsShootingPlasma);
}

void UTP_TankPlayerTurretAnimInstance::IsShootingMinigun()
{
	isShootingMinigun = true;
}
void UTP_TankPlayerTurretAnimInstance::StopShootingMinigun()
{
	isShootingMinigun = false;
}
void UTP_TankPlayerTurretAnimInstance::IsShootingPlasma()
{
	isShootingPlasma = true;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_TankPlayerTurretAnimInstance::IsNotShootingPlasma, 1.2f, false);
}
void UTP_TankPlayerTurretAnimInstance::IsNotShootingPlasma()
{
	isShootingPlasma = false;
}
