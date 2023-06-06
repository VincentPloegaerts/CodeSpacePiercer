#include "TP_ShootPlasmaComponent.h"
#include "TP_TankPlayer.h"
#include "TP_Enemy.h"
#include "TP_AIBehaviorComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TP_BOSS_level3.h"

UTP_ShootPlasmaComponent::UTP_ShootPlasmaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTP_ShootPlasmaComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTP_ShootPlasmaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
}

void UTP_ShootPlasmaComponent::RemoveAmmo()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(GetOwner());
	if (!_player)return;
	ammoData.componentAmmo -= 1;
	ammoData.componentAmmo = ammoData.componentAmmo < 0 ? 0 : ammoData.componentAmmo;
	Super::RemoveAmmo();
}

void UTP_ShootPlasmaComponent::AddAmmo(unsigned int _ammoGive)
{
	ammoData.componentAmmo += _ammoGive;
	ammoData.componentAmmo = ammoData.componentAmmo > ammoData.maxAmmo ? ammoData.maxAmmo : ammoData.componentAmmo;
	Super::AddAmmo(_ammoGive);
}

void UTP_ShootPlasmaComponent::Shoot()
{
	if (!isActivated)return;
	if (ammoData.componentAmmo == 0)
	{
		onAmmoEmpty.Broadcast();
		return;
	}
	RemoveAmmo();
	UWorld* _world = GetWorld();
	if (!_world || !bulletType || !owner)return;
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(owner);
	if (_player)
	{
		_world->GetTimerManager().SetTimer(timerPlasma,this,&UTP_ShootPlasmaComponent::SpawnBullet, timeToShoot,false);
		if (bonesName.Num() <= 0)return;
		UNiagaraFunctionLibrary::SpawnSystemAttached(particleOnShoot, _player->GetSkeletalMesh(), bonesName[0], FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	else
	{
		ATP_GameCharacter* _owner = Cast<ATP_GameCharacter>(owner);
		bulletType.GetDefaultObject()->GetProjectileMovementComponent()->Velocity = _owner->GetSkeletalMesh()->GetForwardVector();
		if (bonesName.Num() <= 0)return;
		_world->SpawnActor<ATP_Bullet>(bulletType, FTransform(owner->GetSkeletalMesh()->GetBoneLocation(bonesName[0])));
	}
	onShootEvent.Broadcast();
	canShoot = false;
}

void UTP_ShootPlasmaComponent::ShootAOEPlasma()
{
	UWorld* _world = GetWorld();
	ATP_GameCharacter* _ownerBoss = Cast<ATP_GameCharacter>(owner);
	if (!_ownerBoss)return;

	bulletType.GetDefaultObject()->GetProjectileMovementComponent()->Velocity = _ownerBoss->GetSkeletalMesh()->GetForwardVector();
	if (bonesName.Num() <= 0)return;
	_world->SpawnActor<ATP_Bullet>(bulletType, FTransform(owner->GetSkeletalMesh()->GetBoneLocation(bonesName[0])));

	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(_world->GetFirstPlayerController()->GetCharacter());
	_player->GetShieldComponent()->TakeDamage(-1);
}

void UTP_ShootPlasmaComponent::SpawnBullet()
{
	ATP_TankPlayer* _player = Cast<ATP_TankPlayer>(owner);
	if (!_player)return;
	bulletType.GetDefaultObject()->GetProjectileMovementComponent()->Velocity = _player->GetCameraComponent()->GetForwardVector();
	if (bonesName.Num())
		GetWorld()->SpawnActor<ATP_Bullet>(bulletType, FTransform(_player->GetSkeletalMesh()->GetBoneLocation(bonesName[0])));
}