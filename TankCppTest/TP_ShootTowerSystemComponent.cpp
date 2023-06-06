#include "TP_ShootTowerSystemComponent.h"
#include "TP_GameCharacter.h"
#include "TP_Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "TP_MissileBullet.h"

UTP_ShootTowerSystemComponent::UTP_ShootTowerSystemComponent()
{

}
void UTP_ShootTowerSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!owner)return;
	if (meshShoot)return;
	meshShoot = owner->GetSkeletalMesh();
}
void UTP_ShootTowerSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTP_ShootTowerSystemComponent::Shoot()
{
	if (!isActivated)return;
	onShootEvent.Broadcast();
	if (!isOut)
	{
		GetWorld()->GetTimerManager().SetTimer(shootTimer, this, &UTP_ShootTowerSystemComponent::SpawnBullet, 1.3, false, -1);
		isOut = true;
	}
	else if(firstShoot)
	{
		SpawnBullet();
	}
	canShoot = false;
}
void UTP_ShootTowerSystemComponent::SpawnBullet()
{
	firstShoot = true;
	UWorld* _world = GetWorld();
	if (!_world || !bulletType || !owner)return;
	bulletType.GetDefaultObject()->GetProjectileMovementComponent()->Velocity = meshShoot->GetForwardVector();
	if (bonesName.Num() <= 0)return;
	FVector _location = meshShoot->GetBoneLocation(bonesName[boneCount]);
	ATP_Bullet* _spawnedBullet = _world->SpawnActor<ATP_Bullet>(bulletType, FTransform(_location));
	ATP_MissileBullet* _missile = Cast<ATP_MissileBullet>(_spawnedBullet);
	if (_missile)
	{
		_missile->SetSpeed(_missile->GetSpeed() * boost);
	}
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, particleOnShoot, _location, meshShoot->GetComponentRotation());
	++boneCount;
	if (boneCount >= bonesName.Num())
	{
		boneCount = 0;
		onStopShootEvent.Broadcast();
		return;
	}
	_world->GetTimerManager().SetTimer(shootTimer, this, &UTP_ShootTowerSystemComponent::SpawnBullet, rateBetweenBullet, false);
}

void UTP_ShootTowerSystemComponent::ActivateShoot()
{
	Super::ActivateShoot();
}

void UTP_ShootTowerSystemComponent::DesactivateShoot()
{
	Super::DesactivateShoot();
	firstShoot = false;
	isOut = false;
}
