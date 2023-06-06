#include "TP_TankBossAnimInstance.h"
#include "TP_AITankBehaviorComponent.h"
#include "TP_ShootTowerSystemComponent.h"

UTP_TankBossAnimInstance::UTP_TankBossAnimInstance()
{

}

void UTP_TankBossAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	owner = Cast<ATP_BOSS_level3>(GetOwningActor());
	if (!owner)return;
	owner->GetHealthComponent()->OnDeathEvent().AddDynamic(this, &UTP_TankBossAnimInstance::IsDead);
	UTP_AITankBehaviorComponent* _tempo = Cast<UTP_AITankBehaviorComponent>(owner->GetAIBehaviorComponent());
	if (!_tempo)return;
	_tempo->OnNormalShooting().AddDynamic(this, &UTP_TankBossAnimInstance::IsShootingMinigun);
	_tempo->OnStopNormalShooting().AddDynamic(this, &UTP_TankBossAnimInstance::IsNotShootingMinigun);
	_tempo->OnShootPlasma().AddDynamic(this, &UTP_TankBossAnimInstance::IsShootingPlasma);
	_tempo->OnStopShootingPlasma().AddDynamic(this, &UTP_TankBossAnimInstance::IsNotShootingPlasma);
	UTP_ShootTowerSystemComponent* _nukeSystem = Cast<UTP_ShootTowerSystemComponent>(owner->GetTankNukeSystem());
	if (!_nukeSystem)return;
	//_nukeSystem->
}

void UTP_TankBossAnimInstance::IsDead(AActor* _deadActor)
{
	dead = true;
}

void UTP_TankBossAnimInstance::IsShootingPlasma()
{
	isShootingPlasma = true;
}

void UTP_TankBossAnimInstance::IsNotShootingPlasma()
{
	isShootingPlasma = false;
}

void UTP_TankBossAnimInstance::IsShootingMinigun()
{
	isShootingMinigun = true;
}

void UTP_TankBossAnimInstance::IsNotShootingMinigun()
{
	isShootingMinigun = false;
}

void UTP_TankBossAnimInstance::IsShootingNuke()
{
	isShootingNuke = true;
}

void UTP_TankBossAnimInstance::IsNotShootingNuke()
{
	isShootingNuke=false;
}
