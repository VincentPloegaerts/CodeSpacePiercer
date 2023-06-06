#include "TP_ShootNukeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TP_TankPlayer.h"
#include "TP_Enemy.h"
#include "TP_NukeBullet.h"
#include "NiagaraFunctionLibrary.h"

UTP_ShootNukeComponent::UTP_ShootNukeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UTP_ShootNukeComponent::BeginPlay()
{
	Super::BeginPlay();
	toIgnore.Add(owner);
}
void UTP_ShootNukeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!isActivated)return;
	if (canShoot && isTriggerShoot)
		Target();
	else
		isTriggerShoot = false;
	CoolDown(DeltaTime);
}

void UTP_ShootNukeComponent::Target()
{
	if (ammoData.componentAmmo == 0)
	{
		onAmmoEmpty.Broadcast();
		FinishTarget();
		isTriggerShoot = false;
		return;
	}
	ATP_TankPlayer* _tempOwner = Cast<ATP_TankPlayer>(owner);
	if (!_tempOwner)return;
	if(time==0)
		onNukesFired.Broadcast();
	if (time >= timeToLock)
	{
		if (targetLock.Num() <= 0)
		{
			isTriggerShoot = false;
			time = 0;
			onNukesLanded.Broadcast();
			FinishTarget();
			AddAmmo(1);
			return;
		}
		Shoot();
		return;
	}
	UWorld* _world = GetWorld();
	time += _world->DeltaTimeSeconds;
	FVector _spawnLocation = owner->GetActorLocation();
	FVector _endLocation = _spawnLocation + _tempOwner->GetCameraComponent()->GetForwardVector() * attackRange;
	FHitResult _result;
	bool _hit = UKismetSystemLibrary::SphereTraceSingleForObjects(_world,
		_spawnLocation,
		_endLocation,
		sizeLock,
		objectsLayer,
		false,
		toIgnore,
		EDrawDebugTrace::None,
		_result,
		true,
		FLinearColor::Red,
		FLinearColor::Green);
	if (!_hit)return;
	ATP_Enemy* _enemy = Cast<ATP_Enemy>(_result.GetActor());
	if (!_enemy)return;
	toIgnore.Add(_enemy);
	if(!_enemy->IsTargetable())return;
	_enemy->GetWidgetComponent()->SetVisibility(true);
	targetLock.Add(_enemy);
	onLockEvent.Broadcast();
}
void UTP_ShootNukeComponent::Shoot()
{
	canShoot = false;
	targetAmount = targetLock.Num();
	GetWorld()->GetTimerManager().SetTimer(timerBullet, this, &UTP_ShootNukeComponent::SpawnBullet, timeBetweenShootOneBullet, true);
}
void UTP_ShootNukeComponent::FinishTarget()
{
	for (size_t i = 0; i < targetLock.Num(); i++)
	{
		if (!targetLock[i] || !targetLock.Contains(targetLock[i]))continue;
		if (!targetLock[i]->GetWidgetComponent())continue;
		targetLock[i]->GetWidgetComponent()->SetVisibility(false);
	}
	targetLock.Empty();
	toIgnore.Empty();
	toIgnore.Add(owner);
	time = 0;
	RemoveAmmo();
	onNukesLanded.Broadcast();
}
void UTP_ShootNukeComponent::SpawnBullet()
{
	UWorld* _world = GetWorld();
	if (!_world || !bulletType)return;
	if (!owner)return;
	if (bulletCount >= targetAmount)
	{
		bulletCount = 0;
		_world->GetTimerManager().ClearTimer(timerBullet);
		FinishTarget();
		return;
	}
	ATP_TankPlayer* _ownerPlayer = Cast<ATP_TankPlayer>(owner);
	if (!_ownerPlayer)return;
	USkeletalMeshComponent* _mesh = _ownerPlayer->GetMeshTankBody();
	if (!_mesh)return;
	if (bonesName.Num() <= 0)return;
	FVector _location = _mesh->GetBoneLocation(bonesName[switchBone]);
	ATP_Bullet* _temp = _world->SpawnActor<ATP_Bullet>(bulletType, FTransform(_location));
	UNiagaraFunctionLibrary::SpawnSystemAttached(particleOnShoot, _mesh, bonesName[switchBone], FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
	++switchBone;
	if (switchBone >= bonesName.Num())
		switchBone = 0;
	ATP_NukeBullet* _nuke = Cast<ATP_NukeBullet>(_temp);
	if (!_nuke)return;
	if (!targetLock.Contains(targetLock[bulletCount]))return;
	_nuke->SetTarget(targetLock[bulletCount]);
	++bulletCount;
}
void UTP_ShootNukeComponent::RemoveAmmo()
{
	ammoData.componentAmmo -= 1;
	ammoData.componentAmmo = ammoData.componentAmmo < 0 ? 0 : ammoData.componentAmmo;
	Super::RemoveAmmo();
}
void UTP_ShootNukeComponent::AddAmmo(unsigned int _ammoGive)
{
	ammoData.componentAmmo += _ammoGive;
	ammoData.componentAmmo = ammoData.componentAmmo > ammoData.maxAmmo ? ammoData.maxAmmo : ammoData.componentAmmo;
	Super::AddAmmo(_ammoGive);
}