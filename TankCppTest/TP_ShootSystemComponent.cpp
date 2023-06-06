#include "TP_ShootSystemComponent.h"
#include "TP_Bullet.h"
#include "TP_TankPlayer.h"
#include "GameFramework/ProjectileMovementComponent.h"

UTP_ShootSystemComponent::UTP_ShootSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_ShootSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	owner = Cast<ATP_GameCharacter>(GetOwner());
}
void UTP_ShootSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (canShoot && isTriggerShoot)
		Shoot();
	CoolDown(DeltaTime);
}

void UTP_ShootSystemComponent::ActivateShoot()
{
	if (!isActivated)return;
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(owner);
	if (_player && !_player->IsActive())return;
	isTriggerShoot = true;
	onStartShootEvent.Broadcast();
}
void UTP_ShootSystemComponent::DesactivateShoot()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(owner);
	if (_player && !_player->IsActive())return;
	isTriggerShoot = false;
	onStopShootEvent.Broadcast();
}
void UTP_ShootSystemComponent::SetMeshShoot(USkeletalMeshComponent* _meshShoot)
{
	if (_meshShoot)
		meshShoot = _meshShoot;
}
void UTP_ShootSystemComponent::Shoot()
{
	
}
void UTP_ShootSystemComponent::AddAmmo(unsigned int _ammoGive)
{
	onMunitionChange.Broadcast(ammoData.componentAmmo);
}
void UTP_ShootSystemComponent::RemoveAmmo()
{
	onMunitionChange.Broadcast(ammoData.componentAmmo);
}
void UTP_ShootSystemComponent::CoolDown(float _DeltaTime)
{
	if (canShoot)return;
	shootTime += _DeltaTime;
	onReload.Broadcast(shootTime,shootRate);
	if (shootTime >= shootRate)
	{
		shootTime = 0;
		canShoot = true;
	}
}