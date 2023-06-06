#include "TP_TankEnemyAnimInstance.h"
#include "TP_TankEnemy.h"
#include "TP_AITankBehaviorComponent.h"

UTP_TankEnemyAnimInstance::UTP_TankEnemyAnimInstance()
{

}

void UTP_TankEnemyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_TankEnemy>(GetOwningActor());
	if (!owner)return;
	owner->OnTankEnnemyTargetLost().AddDynamic(this, &UTP_TankEnemyAnimInstance::IsIdle);
	owner->GetHealthComponent()->onDeathEvent.AddDynamic(this, &UTP_TankEnemyAnimInstance::IsDead);
	owner->OnHalfLife().AddDynamic(this, &UTP_TankEnemyAnimInstance::ReachedMidHP);
	UTP_AITankBehaviorComponent* _tempo=Cast<UTP_AITankBehaviorComponent>(owner->GetAIBehaviorComponent());
	if (!_tempo)return;
	_tempo->OnNormalShooting().AddDynamic(this, &UTP_TankEnemyAnimInstance::IsShooting);
	_tempo->OnStopNormalShooting().AddDynamic(this, &UTP_TankEnemyAnimInstance::IsNotShooting);
	_tempo->OnShootPlasma().AddDynamic(this, &UTP_TankEnemyAnimInstance::IsShootingPlasma);
	_tempo->OnStopShootingPlasma().AddDynamic(this, &UTP_TankEnemyAnimInstance::IsNotShootingPlasma);
}

void UTP_TankEnemyAnimInstance::IsIdle()
{
	state = 0;
}

void UTP_TankEnemyAnimInstance::IsCharging()
{
	state = 1;
}

void UTP_TankEnemyAnimInstance::ReachedMidHP()
{
	state = 2;
	GetWorld()->GetTimerManager().SetTimer(timer, this, &UTP_TankEnemyAnimInstance::IsIdle, 2.50f, false);
}

void UTP_TankEnemyAnimInstance::IsDead(AActor* _actor)
{
	state = 4;
}

void UTP_TankEnemyAnimInstance::IsShooting()
{
	isShooting = true;
}

void UTP_TankEnemyAnimInstance::IsNotShooting()
{
	isShooting = false;
}

void UTP_TankEnemyAnimInstance::IsShootingPlasma()
{
	isShootingPlasma = true;
}

void UTP_TankEnemyAnimInstance::IsNotShootingPlasma()
{
	isShootingPlasma = false;
}
