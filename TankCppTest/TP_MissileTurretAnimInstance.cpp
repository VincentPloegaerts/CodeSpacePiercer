#include "TP_MissileTurretAnimInstance.h"

UTP_MissileTurretAnimInstance::UTP_MissileTurretAnimInstance()
{

}

void UTP_MissileTurretAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_Tower>(GetOwningActor());
	if (!owner)return;
	owner->GetHealthComponent()->onDeathEvent.AddDynamic(this, &UTP_MissileTurretAnimInstance::IsDead);
	owner->OnAttack().AddDynamic(this, &UTP_MissileTurretAnimInstance::IsShooting);
	owner->OnTargetLost().AddDynamic(this, &UTP_MissileTurretAnimInstance::IsNotShooting);
}

void UTP_MissileTurretAnimInstance::IsDead(AActor* _deadActor)
{
	int32 _result = FMath::RandRange(0, 2);
	death = _result;
}

void UTP_MissileTurretAnimInstance::IsShooting()
{
	isShooting = true;
}

void UTP_MissileTurretAnimInstance::IsNotShooting()
{
	isShooting = false;
}
