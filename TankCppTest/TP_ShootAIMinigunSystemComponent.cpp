#include "TP_ShootAIMinigunSystemComponent.h"
#include "TP_AIBehaviorComponent.h"
#include "TP_Drone.h"

UTP_ShootAIMinigunSystemComponent::UTP_ShootAIMinigunSystemComponent()
{
}
void UTP_ShootAIMinigunSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	ATP_Enemy* _ownerEnemy = Cast<ATP_Enemy>(owner);
	if (!_ownerEnemy)return;
	target = _ownerEnemy->GetAIBehaviorComponent()->GetTarget();
	skeletalMesh = owner->GetSkeletalMesh();
	amountBone = bonesName.Num();
}
void UTP_ShootAIMinigunSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime,TickType,ThisTickFunction);
}

void UTP_ShootAIMinigunSystemComponent::Shoot()
{
	if (!isActivated)return;
	UWorld* _world = GetWorld();
	if (!_world || isOverheated || !owner || !target)return;
	ATP_Drone* _ownerEnemy = Cast<ATP_Drone>(owner);
	if (_ownerEnemy)
	{
		if (_ownerEnemy->GetAiMovement()->GetIsStuned() || _ownerEnemy->isBumped())return;
	}
	FVector actorLocation = owner->GetActorLocation();
	FVector _dir = (target->GetActorLocation() - actorLocation).GetSafeNormal();
	if (!bulletType)return;
	bulletType.GetDefaultObject()->GetProjectileMovementComponent()->Velocity = _dir;
	if (!skeletalMesh)return;
	for (size_t i = 0; i < amountBone; i++)
		_world->SpawnActor<ATP_Bullet>(bulletType,FTransform(skeletalMesh->GetBoneLocation(bonesName[i])));
	onShootEvent.Broadcast();
	canShoot = false;
}